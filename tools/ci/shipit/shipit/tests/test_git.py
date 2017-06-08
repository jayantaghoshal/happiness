import os
import re
import tempfile
import unittest

from .. import git


def _write_file(dir_path: str, file_name: str, content: str) -> str:
    path = os.path.join(dir_path, file_name)
    with open(path, 'w') as file:
        file.write(content)
    return path


def _append_to_file(path: str, content: str):
    with open(path, 'a') as file:
        file.write(content)


def _search_log(repo: git.Repo, pattern: str):
    return re.search(pattern, repo.log(), flags=re.DOTALL | re.MULTILINE)


class RepoTest(unittest.TestCase):
    def setUp(self):
        self.tmp_dir = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp_dir.cleanup)
        self.tmp_path = self.tmp_dir.name

    def _assert_in_log(self, repo: git.Repo, pattern: str):
        if not _search_log(repo, pattern):
            self.fail('Pattern \"{}\" not found in git log'.format(pattern))

    def _assert_not_in_log(self, repo: git.Repo, pattern: str):
        if _search_log(repo, pattern):
            self.fail('Pattern \"{}\" found in git log'.format(pattern))

    def test_init(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'test_init'))
        self.assertTrue(os.path.exists(os.path.join(repo.path, '.git')))

    def test_init_bare(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'test_init_bare.git'), bare=True)
        self.assertTrue(os.path.exists(os.path.join(repo.path, 'HEAD')))

    def test_add_and_commit(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'test_add_commit'))

        with self.assertRaises(git.Error):
            repo.add(['does_not_exist.txt'])

        exists_path1 = _write_file(repo.path, 'exists1.txt', '123')
        repo.add([exists_path1])
        repo.commit('Foo bar')
        self._assert_in_log(repo, r"^\s*Foo bar$")

        exists_path2 = _write_file(repo.path, 'exists2.txt', '456')
        repo.add([exists_path2])
        repo.commit('Baz qux')
        self._assert_in_log(repo, r"^\s*Baz qux$.*^\s*Foo bar$")

        with self.assertRaises(git.Error):
            repo.commit('Nothing to commit, will fail')

    def test_add_relative_path(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'test_add_relative_path'))

        relative_path = 'exists3.txt'
        relative_msg = 'Added relative file without changing work dir when running git'

        _write_file(repo.path, relative_path, 'abc')

        repo.add([relative_path])
        repo.commit(relative_msg)

        self._assert_in_log(repo, relative_msg)

    def test_commit_stage_all(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'stage_all'))

        path = _write_file(repo.path, 'foo.txt', 'foo')
        repo.add([path])
        repo.commit('Foo bar')

        with self.assertRaises(git.Error):
            repo.commit('Noting modified, will fail', stage_all=True)

        _append_to_file(path, 'bar')
        repo.commit('Modified foo.txt', stage_all=True)
        self._assert_in_log(repo, r"^\s*Modified foo.txt$")

    def test_commit_multiline_message(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'multiline_message'))

        path = _write_file(repo.path, 'foo.txt', 'foo')
        repo.add([path])

        lines = ['Short summary',
                 '',
                 'The quick brown fox jumps over the lazy dog.',
                 'Pack my box with five dozen liquor jugs.']
        msg = '\n'.join(lines)
        msg_regex = r"^\s*" + r"\n\s*".join(lines) + r"$"

        repo.commit(msg)
        self._assert_in_log(repo, msg_regex)

    def test_rev_parse(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'stage_all'))

        path = _write_file(repo.path, 'foo.txt', 'foo')
        repo.add([path])
        repo.commit('Foo bar')

        rev_hash = repo.rev_parse('HEAD')
        self.assertEqual(rev_hash, repo.rev_parse('master^{commit}'))
        self.assertIsNotNone(re.match('^[0-9a-f]{40}$', rev_hash))
        self._assert_in_log(repo, rev_hash)

        # Ifc. needs changing if we are to support multiple hashes. Make sure we error out instead.
        with self.assertRaises(git.Error):
            repo.rev_parse('master..')

    def test_clone(self):
        repo_to_clone = git.Repo.init(os.path.join(self.tmp_path, 'repo_to_clone'))

        path = _write_file(repo_to_clone.path, 'foo.txt', 'foo')
        repo_to_clone.add([path])
        repo_to_clone.commit('First commit')

        _append_to_file(path, '\n123')
        repo_to_clone.commit('Second commit', stage_all=True)

        repo = git.Repo.clone(repo_to_clone.path, os.path.join(self.tmp_path, 'cloned_repo'))
        self._assert_in_log(repo, 'Second commit\n.+First commit')

        # --depth only relevant for URLs => prepend file://
        repo_depth_1 = git.Repo.clone('file://' + repo_to_clone.path,
                                      os.path.join(self.tmp_path, 'cloned_repo_depth_1'),
                                      depth=1)
        self._assert_in_log(repo_depth_1, 'Second commit')
        self._assert_not_in_log(repo_depth_1, 'First commit')

    def test_clone_repo_does_not_exist(self):
        path = os.path.join(self.tmp_path, 'does_not_exist')
        dest_path = os.path.join(self.tmp_path, 'must_not_exist')

        with self.assertRaises(git.Error):
            git.Repo.clone(path, dest_path)

        self.assertFalse(os.path.exists(dest_path))

    def test_push(self):
        origin_repo = git.Repo.init(os.path.join(self.tmp_path, 'origin_repo.git'), bare=True)
        repo = git.Repo.clone(origin_repo.path, os.path.splitext(origin_repo.path)[0])

        path = _write_file(repo.path, 'foo.txt', 'foo')
        repo.add([path])
        repo.commit('Foo bar')

        repo.push()

        self._assert_in_log(origin_repo, 'Foo bar')

    def test_changed_paths(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'changed_paths'))

        dir_path = os.path.join(repo.path, 'more_stuff')
        os.mkdir(dir_path)

        paths = sorted([_write_file(repo.path, 'foo.txt', '123\n'),
                        _write_file(dir_path, 'file_in_subdir', '456\n'),
                        _write_file(repo.path, 'zzz_last_file_with_space_at_end ', '789\n')])
        repo.add(paths)
        repo.commit('Initial commit')

        _append_to_file(paths[0], 'abc\n')
        repo.commit('One file changed', stage_all=True)

        _append_to_file(paths[1], 'def\n')
        _append_to_file(paths[2], 'ghi\n')
        repo.commit('Two files changed', stage_all=True)

        rel_paths = [os.path.relpath(path, start=repo.path) for path in paths]

        self.assertEqual(rel_paths[:1], repo.changed_paths('HEAD~2..HEAD~1'))
        self.assertEqual(rel_paths[1:], repo.changed_paths('HEAD^'))
        self.assertEqual(rel_paths, repo.changed_paths('master~2'))

    def test_show(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'show'))

        path = _write_file(repo.path, 'foo.txt', '123\n')
        repo.add([path])
        repo.commit('Foo')

        _append_to_file(path, 'abc\n')
        repo.commit('Bar', stage_all=True)

        self.assertTrue('Foo' in repo.show('master^'))
        self.assertEqual('123\n', repo.show('master^:foo.txt'))
        self.assertTrue('Bar' in repo.show('master'))
        self.assertEqual('123\nabc\n', repo.show('HEAD:foo.txt'))

        with self.assertRaises(git.Error):
            repo.show('does_not_exist')

        with self.assertRaises(git.Error):
            repo.show('master:does_not_exist.txt')

    def test_any_changes_staged(self):
        repo = git.Repo.init(os.path.join(self.tmp_path, 'any_changes_staged'))

        self.assertFalse(repo.any_changes_staged())

        path = _write_file(repo.path, 'foo.txt', '123\n')
        repo.add([path])
        self.assertTrue(repo.any_changes_staged())

        repo.commit('Foo')
        self.assertFalse(repo.any_changes_staged())

        _append_to_file(path, 'abc\n')
        self.assertFalse(repo.any_changes_staged())

        repo.add([path])
        self.assertTrue(repo.any_changes_staged())

        repo.commit('Foo2')
        self.assertFalse(repo.any_changes_staged())

        path2 = _write_file(repo.path, 'bar.txt', '456\n')
        self.assertFalse(repo.any_changes_staged())

        repo.add([path2])
        self.assertTrue(repo.any_changes_staged())

        repo.commit('Bar')
        self.assertFalse(repo.any_changes_staged())
