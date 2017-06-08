import os
import subprocess

from typing import List, Optional

from . import process_tools


class Error(Exception):
    pass


class Repo:
    def __init__(self, path: str):
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

    def add(self, path_specs: List[str]):
        self._run_git(['add', '--'] + path_specs)

    def commit(self, message: str, stage_all=False):
        args = ['commit', '-m', message]
        if stage_all:
            args.append('-a')

        self._run_git(args)

    def log(self) -> str:
        return self._run_git(['log'])

    def rev_parse(self, rev: str) -> str:
        return self._run_git(['rev-parse', '--verify', rev]).rstrip()

    def push(self, push_args=[]):
        args = ['push']
        args.extend(push_args)
        self._run_git(args)

    def changed_paths(self, rev_range: str) -> List[str]:
        return self._run_git(['diff', '--name-only', rev_range]).rstrip('\n').split('\n')

    def show(self, git_object: str) -> str:
        return self._run_git(['show', git_object])

    def _run_git(self, git_args: List[str], run_in_path: bool = True) -> str:
        """Run git command with arguments git_args.

        Some commands sets exit code to something other than 0 but only writes to stdout and not
        stderr. E.g. 'commit' does this when invoked and there is nothing to commit. For other
        errors it writes to stderr. Handled by using stdout in error message if stderr is empty.
        """
        args = ['git']

        if run_in_path:
            args.extend(['-C', self.path])

        args.extend(git_args)

        try:
            return process_tools.check_output_logged(args).decode()
        except subprocess.CalledProcessError as error:
            error_msg = error.stderr if error.stderr else error.stdout
            raise Error('{}\n{}'.format(error, error_msg.decode()))
