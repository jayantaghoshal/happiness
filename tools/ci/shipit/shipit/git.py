# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import subprocess
import shlex

from typing import List, Optional

from . import process_tools


class Error(Exception):
    pass


#
# NOTE: Avoid extending this class too much, prefer using libgit2 instead
#

class Repo:
    def __init__(self, path: str) -> None:
        self.path = os.path.abspath(path)

    @classmethod
    def init(cls, path: str, bare: bool = False):
        repo = cls(path)

        args = ['init']
        if bare:
            args.append('--bare')
        args.append(repo.path)

        repo._run_git(args, run_in_path=False)

        return repo

    @classmethod
    def clone(cls, repo_path_or_url: str, dest_path: str, depth: Optional[int] = None):
        repo = cls(dest_path)

        args = ['clone']
        if depth:
            args.extend(['--depth', str(depth)])
        args.extend([repo_path_or_url, repo.path])

        repo._run_git(args, run_in_path=False)

        return repo

    @classmethod
    def ls_remote(cls, repository: str):
        server = "https://icup_android.gerrit.cm.volvocars.biz/"

        revision =  subprocess.check_output(shlex.split('git ls-remote ' + server + repository + ' refs/heads/master'))
        revision = revision.decode("utf-8")
        revision = revision.split("\t")[0]

        return revision

    @classmethod
    def repo_reset(cls, full_path: str):
        process_tools.check_output_logged(["git", "reset",
                                    "--hard"], cwd=full_path)

    @classmethod
    def repo_clean(cls, full_path: str):
        process_tools.check_output_logged(["git", "clean",
                                "-fdx"], cwd=full_path)

    @classmethod
    def repo_sync_repo(cls, repo_name: str, aosp_root_dir: str):
        process_tools.check_output_logged(["repo", "sync",
                                "--jobs=6",
                                "--no-clone-bundle",
                                "--force-sync",
                                "--detach",
                                "--current-branch",
                                repo_name], cwd=aosp_root_dir)

    @classmethod
    def repo_rev_parse(cls, repo_full_path: str):
        revision = process_tools.check_output_logged(["git",
                                "rev-parse",
                                "HEAD"], cwd=repo_full_path)

        return revision

    @classmethod
    def get_commit_message(cls, repo_full_path: str):
        commit_message = process_tools.check_output_logged(["git",
                                "log",
                                "-1",
                                "--pretty=%B"], cwd=repo_full_path)

        return commit_message

    @classmethod
    def get_commiters_name(cls, repo_full_path: str):
        commit_name = process_tools.check_output_logged(["git",
                                "log",
                                "-1",
                                "--pretty=%an"], cwd=repo_full_path)

        return commit_name

    @classmethod
    def get_commiters_mail(cls, repo_full_path: str):
        commiters_mail = process_tools.check_output_logged(["git",
                                "log",
                                "-1",
                                "--pretty=%ae"], cwd=repo_full_path)

        return commiters_mail

    def add(self, path_specs: List[str]):
        self._run_git(['add', '--'] + path_specs)

    def commit(self, message: str, stage_all=False):
        args = ['commit', '-m', message]
        if stage_all:
            args.append('-a')

        self._run_git(args)

    def log(self) -> str:
        return self._run_git(['log']).stdout_str

    def rev_parse(self, rev: str) -> str:
        return self._run_git(['rev-parse', '--verify', rev]).stdout_str.rstrip()

    def push(self, push_args=[]):
        args = ['push']
        args.extend(push_args)
        self._run_git(args)

    def changed_paths(self, rev_range: str) -> List[str]:
        return self._run_git(['diff', '--name-only', rev_range]).stdout_str.rstrip('\n').split('\n')

    def show(self, git_object: str) -> str:
        return self._run_git(['show', git_object]).stdout_str

    def any_changes(self, staged: bool = False) -> bool:
        args = ['diff', '--quiet']
        if staged:
            args.append('--cached')
        result = self._run_git(args, check_exitcode=False)
        return result.exitcode == 1

    def _run_git(self,
                 git_args: List[str],
                 run_in_path: bool = True,
                 check_exitcode: bool = True) -> process_tools.Result:
        """Run git command with arguments git_args.

        Some commands sets exit code to something other than 0 but only writes to stdout and not
        stderr. E.g. 'commit' does this when invoked and there is nothing to commit. For other
        errors it writes to stderr. Handled by using stdout in error message if stderr is empty.
        """
        args = ['git']

        if run_in_path:
            args.extend(['-C', self.path])

        args.extend(git_args)

        result = process_tools.run(args)

        if check_exitcode and result.exitcode != 0:
            error_msg = result.stderr if result.stderr else result.stdout
            raise Error('Git command "{}" failed. Exit code = {}. Error:\n{}'
                        .format(' '.join(args), result.exitcode, error_msg.decode()))

        return result

    def run_git(self, git_args: List[str]) -> str:
        return self._run_git(git_args).stdout_str
