import os
import re
import tempfile
import unittest

from typing import List

from .. import git
from .. import autobumper

def write_file(dir_path: str, file_name: str, content: str):
    path = os.path.join(dir_path, file_name)
    with open(path, 'w') as file:
        file.write(content)
    return path

def manifest(project_elements: List[str], regex: bool = False) -> str:
    head = ('<?xml version="1.0" encoding="UTF-8"?>\n'
            '<manifest>\n')
    tail = '\n</manifest>\n'

    ret = '^' + re.sub(r"\?", '\\?', head) if regex else head
    ret += '\n'.join(['  ' + e for e in project_elements])
    ret += tail + '$' if regex else tail

    return ret


class ManifestTestCaseBase(unittest.TestCase):
    def setUp(self):
        self.tmp_dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp_dir.cleanup)
        self.project_root = self.tmp_dir.name

    def _create_test_repo(self, path_in_project: str, bare: bool = False, initial_commit: bool = True) -> git.Repo:
        path = os.path.join(self.project_root, path_in_project)
        os.makedirs(path)

        repo = git.Repo.init(path, bare=bare)

        if not bare and initial_commit:
            repo.add([write_file(repo.path, 'README.txt', 'Testing\n123\n')])
            repo.commit('Initial commit in ' + repo.path)  # Use of path => HEAD is "unique".

        return repo


