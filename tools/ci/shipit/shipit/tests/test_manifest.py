import os
import re
import tempfile
import unittest

from typing import List

from .. import git
from .. import manifest


def _write_file(dir_path: str, file_name: str, content: str):
    path = os.path.join(dir_path, file_name)
    with open(path, 'w') as file:
        file.write(content)
    return path


def _manifest(project_elements: List[str], regex: bool = False) -> str:
    head = ('<?xml version="1.0" encoding="UTF-8"?>\n'
            '<manifest>\n'
            '  <include name="i_must_not_be_modified.xml"/>\n')
    tail = '\n</manifest>\n'

    ret = '^' + re.sub(r"\?", '\\?', head) if regex else head
    ret += '\n'.join(['  ' + e for e in project_elements])
    ret += tail + '$' if regex else tail

    return ret


class ManifestTestCase(unittest.TestCase):
    def setUp(self):
        self.tmp_dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp_dir.cleanup)
        self.project_root = self.tmp_dir.name

    def _create_test_repo(self, path_in_project: str, bare: bool = False) -> git.Repo:
        path = os.path.join(self.project_root, path_in_project)
        os.makedirs(path)

        repo = git.Repo.init(path, bare=bare)

        if not bare:
            repo.add([_write_file(repo.path, 'README.txt', 'Testing\n123\n')])
            repo.commit('Initial commit in ' + repo.path)  # Use of path => HEAD is "unique".

        return repo


class HashInserterTest(ManifestTestCase):
    def test_basic(self):
        bar_repo = self._create_test_repo('device/bar')
        self._create_test_repo('device/baz')
        foo_repo = self._create_test_repo('vendor/foo')

        template = _manifest([
            '<project name="bar" path="device/bar" revision="${master}"/>',
            '<project name="baz" path="device/baz" revision="cant_touch_this"/>',
            '<project name="foo" path="vendor/foo" revision="${HEAD}"/>'
        ])
        pattern = _manifest([
            '<project name="bar" path="device/bar" revision="[0-9a-f]{40}"/>',
            '<project name="baz" path="device/baz" revision="cant_touch_this"/>',
            '<project name="foo" path="vendor/foo" revision="[0-9a-f]{40}"/>'
        ], regex=True)

        manifest1 = manifest.HashInserter(self.project_root).insert(template)
        bar_hash1 = bar_repo.rev_parse('master')
        foo_hash1 = foo_repo.rev_parse('master')
        self.assertRegex(manifest1, pattern)
        self.assertTrue(bar_hash1 in manifest1)
        self.assertTrue(foo_hash1 in manifest1)

        foo_repo.add([_write_file(foo_repo.path, 'dsflk.txt', 'sdkfjk')])
        foo_repo.commit('Changed something in foo')

        manifest2 = manifest.HashInserter(self.project_root).insert(template)
        foo_hash2 = foo_repo.rev_parse('master')
        self.assertRegex(manifest2, pattern)
        self.assertNotEqual(manifest1, manifest2)
        self.assertTrue(bar_hash1 in manifest2)
        self.assertFalse(foo_hash1 in manifest2)
        self.assertTrue(foo_hash2 in manifest2)

        bar_repo.add([_write_file(bar_repo.path, 'sdfjdkj.txt', 'afkji')])
        bar_repo.commit('Changed something in bar')
        foo_repo.add([_write_file(foo_repo.path, '128398.txt', 'asdkfjd')])
        foo_repo.commit('Changed something in foo again')

        manifest3 = manifest.HashInserter(self.project_root).insert(template)
        bar_hash3 = bar_repo.rev_parse('master')
        foo_hash3 = foo_repo.rev_parse('master')
        self.assertRegex(manifest3, pattern)
        self.assertNotEqual(manifest2, manifest3)
        self.assertFalse(bar_hash1 in manifest3)
        self.assertTrue(bar_hash3 in manifest3)
        self.assertFalse(foo_hash2 in manifest3)
        self.assertTrue(foo_hash3 in manifest3)

    def test_name_only(self):
        self._create_test_repo('device/bar')
        self._create_test_repo('vendor/foo')

        template = _manifest([
            '<project name="device/bar" revision="${master}"/>',
            '<project name="vendor/foo" revision="${HEAD}"/>'
        ])
        pattern = _manifest([
            '<project name="device/bar" revision="[0-9a-f]{40}"/>',
            '<project name="vendor/foo" revision="[0-9a-f]{40}"/>'
        ], regex=True)

        self.assertRegex(manifest.HashInserter(self.project_root).insert(template), pattern)

    def test_missing_name_and_path(self):
        with self.assertRaises(Exception):
            manifest.HashInserter(self.project_root).insert(_manifest([
                '<project revision="${master}"/>'
            ]))

    def test_repo_does_not_exist(self):
        with self.assertRaises(Exception):
            manifest.HashInserter(self.project_root).insert(_manifest([
                '<project name="foo" path="vendor/foo" revision="${master}"/>'
            ]))

    def test_rev_does_not_exist(self):
        self._create_test_repo('vendor/foo')

        with self.assertRaises(git.Error):
            manifest.HashInserter(self.project_root).insert(_manifest([
                '<project name="foo" path="vendor/foo" revision="${nonexistent_branch_or_tag}"/>'
            ]))

    def test_space(self):
        self._create_test_repo('vendor/foo')
        template = _manifest([
            ('<project \n'
             '          name    \n'
             '    =      "foo"      path=    "vendor/foo"   \n'
             '                 revision      =    "${master}"    \n'
             '       />')
        ])
        pattern = _manifest([
            ('<project \n'
             '          name    \n'
             '    =      "foo"      path=    "vendor/foo"   \n'
             '                 revision      =    "[0-9a-f]{40}"    \n'
             '       />')
        ], regex=True)

        self.assertRegex(manifest.HashInserter(self.project_root).insert(template), pattern)


class ManifestUpdateTest(ManifestTestCase):
    def test_update_file(self):
        bar_repo = self._create_test_repo('device/bar')
        foo_repo = self._create_test_repo('vendor/foo')

        template = _manifest([
            '<project name="bar" path="device/bar" revision="${master}"/>',
            '<project name="foo" path="vendor/foo" revision="${HEAD}"/>'
        ])
        pattern = _manifest([
            '<project name="bar" path="device/bar" revision="[0-9a-f]{40}"/>',
            '<project name="foo" path="vendor/foo" revision="[0-9a-f]{40}"/>'
        ], regex=True)

        template_path = _write_file(self.project_root, 'manifest_template.xml', template)
        output_path = os.path.join(self.project_root, 'manifest.xml')

        manifest.update_file(self.project_root, template_path, output_path)

        with open(output_path) as file:
            output1 = file.read()

        bar_repo.add([_write_file(bar_repo.path, 'bar.txt', '123')])
        bar_repo.commit('Changed something in bar')

        foo_repo.add([_write_file(foo_repo.path, 'foo.txt', 'abc')])
        foo_repo.commit('Changed something in foo')

        manifest.update_file(self.project_root, template_path, output_path)

        with open(output_path) as file:
            output2 = file.read()

        self.assertRegex(output1, pattern)
        self.assertRegex(output2, pattern)
        self.assertNotEqual(output1, output2)

    def test_update_repo(self):
        bar_repo = self._create_test_repo('device/bar')
        foo_repo = self._create_test_repo('vendor/foo')

        manifest_repo = self._create_test_repo('manifests.git', bare=True)
        manifest_repo_url = 'file://' + manifest_repo.path

        template = _manifest([
            '<project name="bar" path="device/bar" revision="${master}"/>',
            '<project name="foo" path="vendor/foo" revision="${HEAD}"/>'
        ])
        pattern = _manifest([
            '<project name="bar" path="device/bar" revision="[0-9a-f]{40}"/>',
            '<project name="foo" path="vendor/foo" revision="[0-9a-f]{40}"/>'
        ], regex=True)

        template_path_in_foo = 'manifest_template.xml'
        template_path_in_project = os.path.join('vendor/foo', template_path_in_foo)
        foo_repo.add([_write_file(foo_repo.path, template_path_in_foo, template)])
        foo_repo.commit('Add manifest template')

        manifest.update_repo(self.project_root,
                             template_path_in_project,
                             manifest_repo_url,
                             'manifest.xml')

        manifest1 = manifest_repo.show('HEAD:manifest.xml')

        bar_repo.add([_write_file(bar_repo.path, 'data.dat', 'dfjkkj')])
        bar_repo.commit('Changed something')

        manifest.update_repo(self.project_root,
                             template_path_in_project,
                             manifest_repo_url,
                             'manifest.xml')

        manifest2 = manifest_repo.show('HEAD:manifest.xml')

        self.assertRegex(manifest1, pattern)
        self.assertRegex(manifest2, pattern)
        self.assertNotEqual(manifest1, manifest2)


class VerifyNoFloatingBranchesTest(ManifestTestCase):
    def test_manifest_with_branches_on_master(self):
        bar_repo = self._create_test_repo('device/bar')
        branch = "master"

        template = _manifest([
            '<project name="foo" path="vendor/foo" revision="vcc/aae/googleio_ww02"/>',
            '<project name="foo" path="vendor/foo" revision="research/o"/>'
        ])

        manifest_path = _write_file(bar_repo.path, 'manifest.xml', template)
        with self.assertRaises(manifest.Error):
            manifest.verify_no_floating_branches(manifest_path, branch)

    def test_manifest_with_branches_on_branch(self):
        bar_repo = self._create_test_repo('device/bar')
        branch = "foo_branch"

        template = _manifest([
            '<project name="foo" path="vendor/foo" revision="vcc/aae/googleio_ww02"/>',
            '<project name="foo" path="vendor/foo" revision="research/o"/>'
        ])

        manifest_path = _write_file(bar_repo.path, 'manifest.xml', template)

        try:
            manifest.verify_no_floating_branches(manifest_path, branch)
        except manifest.Error:
            self.fail("verify_no_floating_branches() rasied Exception,"
                      "revision format may not be supported on branch")


    def test_manifest_without_branches_on_master(self):
        bar_repo = self._create_test_repo('device/bar')
        branch = "master"

        template = _manifest([
            '<project name="bar" path="device/bar" revision="329e29e113cad25d34f4ee1e1100f72632a2730e"/>',
            '<project name="foo" path="vendor/foo" revision="${HEAD}"/>',
        ])

        manifest_path = _write_file(bar_repo.path, 'manifest.xml', template)

        try:
            manifest.verify_no_floating_branches(manifest_path, branch)
        except manifest.Error:
            self.fail("verify_no_floating_branches() rasied Exception,"
                      " revision format may not be supported on master")