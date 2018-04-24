# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import re
import tempfile
import unittest
import xml.etree.ElementTree as ET
from unittest.mock import patch, Mock
from unittest.mock import MagicMock
from typing import List

from . import common
from .. import git
from .. import manifest

#The test should use the ZUUL_COMMIT hash
# class ManifestUpdateTest(common.ManifestTestCaseBase):
#     def test_update_file_using_zuul(self):
#         with patch.dict('os.environ', {'GERRIT_NEWREV': '44ff658186e32d408cef7d2bf1b76b6cb776430a', #type: ignore
#                                        'ZUUL_COMMIT'  : '44ff658186e32d408cef7d2bf1b76b6cb776430b',
#                                        'ZUUL_CHANGES' : 'vendor/volvocars:master:refs/changes/84/5684/20'}):

#             template = common.manifest([
#                 '<project name="vendor/foo" path="vendor/foo" revision="ZUUL_COMMIT_OR_HEAD"/>'
#             ])

#             template_path = common.write_file(self.project_root, 'manifest_template.xml', template)
#             output_path = os.path.join(self.project_root, 'manifest.xml')

#             #git.Repo.repo_rev_parse(full_path)
#             with patch.object(git.Repo, 'ls_remote'):
#                 instance = git.Repo("test")
#                 instance.ls_remote.return_value = '44ff658186e32d408cef7d2bf1b76b6cb776430b'  #type: ignore
#                 manifest.update_file(self.project_root, template_path, output_path, "bar", True)

#             with patch.object(git.Repo, 'repo_rev_parse'):
#                 instance = git.Repo("test")
#                 instance.repo_rev_parse.return_value = '44ff658186e32d408cef7d2bf1b76b6cb776430b'  #type: ignore
#                 #manifest.update_file_and_zuul_clone(self.project_root, template_path, output_path, "bar", True)

#             #manifest.update_file_and_zuul_clone(self.project_root, template_path, output_path, "vendor/foo", True)

#             parser = ET.XMLParser()
#             parsed_manifest = ET.parse(output_path, parser)
#             projects = parsed_manifest.findall("project")

#             revision = ""
#             for project in projects:
#                 revision = project.attrib["revision"]
#                 print("revision = " + revision)

#             self.assertEqual(revision, os.environ['ZUUL_COMMIT'])

# #The test should use the GERRIT_NEWREV hash
#     def test_update_file_not_using_zuul(self):
#         with patch.dict('os.environ', {'GERRIT_NEWREV': '44ff658186e32d408cef7d2bf1b76b6cb776430a',  #type: ignore
#                                        'ZUUL_COMMIT'  : '44ff658186e32d408cef7d2bf1b76b6cb776430b',
#                                        'ZUUL_CHANGES' : 'vendor/volvocars:master:refs/changes/84/5684/20'}):

#             template = common.manifest([
#                 '<project name="vendor/foo" path="vendor/foo" revision="ZUUL_COMMIT_OR_HEAD"/>'
#             ])

#             template_path = common.write_file(self.project_root, 'manifest_template.xml', template)
#             output_path = os.path.join(self.project_root, 'manifest.xml')

#             manifest.update_file(self.project_root, template_path, output_path, "vendor/foo", False)

#             parser = ET.XMLParser()
#             parsed_manifest = ET.parse(output_path, parser)
#             projects = parsed_manifest.findall("project")

#             for project in projects:
#                 revision = project.attrib["revision"]

#             self.assertEqual(revision, os.environ['GERRIT_NEWREV'])

# #Test should use the mocked value
#     def test_update_file_repository_without_commit(self):
#         with patch.dict('os.environ', {'GERRIT_NEWREV': '44ff658186e32d408cef7d2bf1b76b6cb776430a', #type: ignore
#                                        'ZUUL_COMMIT'  : '44ff658186e32d408cef7d2bf1b76b6cb776430b',
#                                        'ZUUL_CHANGES' : 'vendor/volvocars:master:refs/changes/84/5684/20'}):

#             template = common.manifest([
#                 '<project name="vendor/foo" path="vendor/foo" revision="ZUUL_COMMIT_OR_HEAD"/>'
#             ])

#             template_path = common.write_file(self.project_root, 'manifest_template.xml', template)
#             output_path = os.path.join(self.project_root, 'manifest.xml')

#             with patch.object(git.Repo, 'ls_remote'):
#                 instance = git.Repo("test")
#                 instance.ls_remote.return_value = '44ff658186e32d408cef7d2bf1b76b6cb776430c'  #type: ignore
#                 manifest.update_file(self.project_root, template_path, output_path, "bar", True)

#             parser = ET.XMLParser()
#             parsed_manifest = ET.parse(output_path, parser)
#             projects = parsed_manifest.findall("project")

#             for project in projects:
#                 revision = project.attrib["revision"]

#             self.assertEqual(revision, '44ff658186e32d408cef7d2bf1b76b6cb776430c')

class VerifyNoFloatingBranchesTest(common.ManifestTestCaseBase):
    def test_manifest_with_branches_on_master(self):
        bar_repo = self._create_test_repo('device/bar')
        branch = "master"

        template = common.manifest([
            '<project name="foo" path="vendor/foo" revision="vcc/aae/googleio_ww02"/>',
            '<project name="foo" path="vendor/foo" revision="research/o"/>'
        ])

        manifest_path = common.write_file(bar_repo.path, 'manifest.xml', template)
        with self.assertRaises(manifest.Error):
            manifest.verify_no_floating_branches(manifest_path, branch)

    def test_manifest_with_branches_on_branch(self):
        bar_repo = self._create_test_repo('device/bar')
        branch = "foo_branch"

        template = common.manifest([
            '<project name="foo" path="vendor/foo" revision="vcc/aae/googleio_ww02"/>',
            '<project name="foo" path="vendor/foo" revision="research/o"/>'
        ])

        manifest_path = common.write_file(bar_repo.path, 'manifest.xml', template)

        try:
            manifest.verify_no_floating_branches(manifest_path, branch)
        except manifest.Error:
            self.fail("verify_no_floating_branches() rasied Exception,"
                      "revision format may not be supported on branch")


    def test_manifest_without_branches_on_master(self):
        bar_repo = self._create_test_repo('device/bar')
        branch = "master"

        template = common.manifest([
            '<project name="bar" path="device/bar" revision="329e29e113cad25d34f4ee1e1100f72632a2730e"/>',
            '<project name="foo" path="vendor/foo" revision="ZUUL_COMMIT_OR_HEAD"/>',
        ])

        manifest_path = common.write_file(bar_repo.path, 'manifest.xml', template)

        try:
            manifest.verify_no_floating_branches(manifest_path, branch)
        except manifest.Error:
            self.fail("verify_no_floating_branches() rasied Exception,"
                      " revision format may not be supported on master")