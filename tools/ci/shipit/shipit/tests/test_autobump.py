# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import re
import tempfile
import unittest

from typing import List

import xml.etree.ElementTree as ET
from unittest.mock import patch, Mock
from unittest.mock import MagicMock
from . import common
from .. import git
from .. import manifest
from .. import autobumper

class BasicAutobumpTest(common.ManifestTestCaseBase):
    def test_update_file_not_using_zuul_for_autobump(self):
        manifest_repo = self._create_test_repo("manifest")
        self._create_test_repo("vendor/foo")
        self._create_test_repo("vendor/bar")

        template = common.manifest([
            '<project name="foo" path="vendor/foo" revision="${master}"/>'
        ])

        template_path = common.write_file(manifest_repo.path, 'manifest_template.xml', template)
        manifest_repo.add([template_path])
        manifest_repo.commit("initial")

        template = common.manifest([
            '<project name="foo" path="vendor/foo" revision="${master}"/>',
            '<project name="bar" path="vendor/bar" revision="${master}"/>'
        ])

        template_path = common.write_file(manifest_repo.path, 'manifest_template.xml', template)
        manifest_repo.add([template_path])

        title, body = autobumper.assemble_commit_messages(self.tmp_dir.name, manifest_repo)

        self.assertTrue("Added 'vendor/bar'" in body)

    def test_assemble_commit_messages_deleted_items(self):
        manifest_repo = self._create_test_repo("manifest")
        self._create_test_repo("vendor/foo")
        self._create_test_repo("vendor/bar")

        template = common.manifest([
            '<project name="foo" path="vendor/foo" revision="${master}"/>'
            '<project name="bar" path="vendor/bar" revision="${master}"/>'
        ])

        template_path = common.write_file(manifest_repo.path, 'manifest_template.xml', template)
        manifest_repo.add([template_path])
        manifest_repo.commit("initial")

        template = common.manifest([
            '<project name="foo" path="vendor/foo" revision="${master}"/>'
        ])

        template_path = common.write_file(manifest_repo.path, 'manifest_template.xml', template)
        manifest_repo.add([template_path])

        title, body = autobumper.assemble_commit_messages(self.tmp_dir.name, manifest_repo)

        self.assertTrue("Removed 'vendor/bar'" in body)

    def test_git_commit_info(self):
        foo = self._create_test_repo("vendor/foo")

        foo.add([common.write_file(foo.path, "README_TOO.txt", "Testing, testing")])
        foo.commit("commit-message 1\n\ncommit-body line 1\ncommit-body line 2")

        info = autobumper.git_commit_info(foo, "master")

        self.assertFalse("merge" in info)
        self.assertTrue("commit-message 1" in info['title'])
        self.assertTrue("commit-body line 1" in " ".join(info['body']))
        self.assertTrue("commit-body line 2" in " ".join(info['body']))

    def test_git_commit_info_empty_body(self):
        foo = self._create_test_repo("vendor/foo")

        foo.add([common.write_file(foo.path, "README_TOO.txt", "Testing, testing")])
        foo.commit("commit-message 1")

        info = autobumper.git_commit_info(foo, "master")

        self.assertFalse("merge" in info)
        self.assertTrue("commit-message 1" in info['title'])

"""
    def test_assemble_commit_messages_changed_items(self):
        manifest_repo = self._create_test_repo("manifest")
        foo = self._create_test_repo("vendor/foo")
        self._create_test_repo("vendor/bar")

        template = common.manifest([
            '<project name="foo" path="vendor/foo" revision="master"/>',
            '<project name="bar" path="vendor/bar" revision="master"/>'
        ])

        manifest_repo.add([common.write_file(manifest_repo.path, 'manifest_template.xml', template)])
        manifest_repo.commit("initial")

        foo.run_git(["checkout", "-b", "branch1"])
        foo.add([common.write_file(foo.path, "README_TOO.txt", "Testing, testing")])
        foo.commit("from branch1\n\n[JIRA-1287623]")

        foo.run_git(["checkout", "master"])
        foo.add([common.write_file(foo.path, "README_ALSO.txt", "inania inepta saturabuntur")])
        foo.commit("Nonsense wonsense\n\n[JIRASOMETHING-31337]\n[OTHERJIRA-000]")

        foo.run_git(["merge", "--commit", "-m", "Merge 'from branch1'", "branch1"])
        foo.run_git(["rev-parse", "master"])

        manifest_repo.add([common.write_file(manifest_repo.path, 'manifest_template.xml', template)])

        title, body = autobumper.assemble_commit_messages(self.tmp_dir.name, manifest_repo)

        print("title: " + title)
        print("body: " + body)

        self.assertTrue("from branch1" in title)
        self.assertTrue("\nvendor/foo:\n - Merge 'from branch1' \n - Nonsense wonsense [JIRASOMETHING-31337] [OTHERJIRA-000]\n - from branch1 [JIRA-1287623]" in body)


    def test_git_commit_info_empty_body(self):
        foo = self._create_test_repo("vendor/foo")

        foo.add([common.write_file(foo.path, "README_TOO.txt", "Testing, testing")])
        foo.commit("commit-message 1")

        info = autobumper.git_commit_info(foo, "master")

        self.assertFalse("merge" in info)
        self.assertTrue("commit-message 1" in info['title'])

        foo.run_git(["merge", "--commit", "-m", "Merge 'from branch1'", "branch1"])

        manifest_repo.add([common.write_file(manifest_repo.path, "default.xml", manifest.HashInserter(self.project_root).insert(template))])

        title, body = autobumper.assemble_commit_messages(self.tmp_dir.name, manifest_repo)

        self.assertTrue("from branch1" in title)
        self.assertTrue("\nvendor/foo:\n - Merge 'from branch1' \n - Nonsense wonsense [JIRASOMETHING-31337] [OTHERJIRA-000]\n - from branch1 [JIRA-1287623]" in body)


    def test_assemble_commit_messages_merge_commit(self):
        manifest_repo = self._create_test_repo("manifest")

        foo = self._create_test_repo("vendor/foo")

        self._create_test_repo("vendor/bar")

        template = common.manifest([
            '<project name="foo" path="vendor/foo" revision="${master}"/>',
            '<project name="bar" path="vendor/bar" revision="${master}"/>'
        ])

        manifest_repo.add([common.write_file(manifest_repo.path, "default.xml", manifest.HashInserter(self.project_root).insert(template))])
        manifest_repo.commit("initial")

        foo.run_git(["checkout", "-b", "branch1"])
        foo.add([common.write_file(foo.path, "README_TOO.txt", "Testing, testing")])
        foo.commit("from branch1\n\n[JIRA-1287623]")

        foo.run_git(["checkout", "master"])

        foo.run_git(["merge", "--commit", "-m", "Merge some nonsense branch", "branch1"])

        manifest_repo.add([common.write_file(manifest_repo.path, "default.xml", manifest.HashInserter(self.project_root).insert(template))])

        title, body = autobumper.assemble_commit_messages(self.tmp_dir.name, manifest_repo)

        self.assertTrue("from branch1" in title)
        self.assertTrue("\nvendor/foo:\n - from branch1 [JIRA-1287623]" in body)
"""