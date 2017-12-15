# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import re
import tempfile
from xml.etree.cElementTree import ElementTree as ET
from . import git


class Error(Exception):
    pass


class HashInserter:
    def __init__(self, project_root: str) -> None:
        self._project_root = project_root

        self._revision_regex = re.compile(r"(?P<prefix><project[^>]*?\s+revision\s*=\s*[\"'])"
                                          r"\${(?P<revision>.*?)}"
                                          r"(?P<suffix>[\"'].*?>)", flags=re.DOTALL)
        self._name_regex = re.compile(r"\s+name\s*=\s*[\"'](.*?)[\"']")
        self._path_regex = re.compile(r"\s+path\s*=\s*[\"'](.*?)[\"']")

    def insert(self, template: str) -> str:
        return self._revision_regex.sub(self._replace_match, template)

    def _replace_match(self, match) -> str:
        prefix = match.group('prefix')  # TODO: Is there a better way? Would like to
        suffix = match.group('suffix')  # avoid having groups for prefix and suffix.

        repo = self._repo_from_project_element(match.group())
        rev_hash = repo.rev_parse(match.group('revision'))

        return prefix + rev_hash + suffix

    def _repo_from_project_element(self, xml_element: str) -> git.Repo:
        match = self._path_regex.search(xml_element)
        if not match:
            match = self._name_regex.search(xml_element)
        if not match:
            raise Error('Failed to get repository path from project element: "{}"'.
                        format(xml_element))

        return git.Repo(os.path.join(self._project_root, match.group(1)))


def _safe_replace_file_content(path: str, content: str):
    with tempfile.NamedTemporaryFile(mode='wt', dir=os.path.dirname(path), delete=False) as file:
        file.write(content)
        tmp_path = file.name
    os.replace(tmp_path, path)


def update_file(project_root: str, template_path: str, output_path: str):
    with open(template_path) as file:
        template = file.read()

    manifest = HashInserter(project_root).insert(template)

    if os.path.exists(output_path):
        _safe_replace_file_content(output_path, manifest)
    else:
        with open(output_path, 'w') as file:
            file.write(manifest)


def update_repo(project_root: str,
                template_path_in_project: str,
                manifest_repo_url: str,
                manifest_path_in_repo: str):
    with tempfile.TemporaryDirectory() as tmp_dir:
        repo = git.Repo.clone(manifest_repo_url, tmp_dir, depth=1)

        update_file(project_root,
                    os.path.join(project_root, template_path_in_project),
                    os.path.join(repo.path, manifest_path_in_repo))

        repo.add([manifest_path_in_repo])
        repo.commit('Update manifest')
        repo.push()


def verify_no_floating_branches(manifest_path: str, branch: str):
    def is_sha_hash(revision):
        return re.match(r"[a-f0-9]{40}", revision) is not None

    root = ET()
    parsed_manifest = root.parse(manifest_path)
    projects = parsed_manifest.findall("project")

    for project in projects:
        rev = project.attrib["revision"]
        if not is_sha_hash(rev):
            if rev != "${HEAD}":
                if branch == "master":
                    raise Error("Project %s --- You are not allowed to have floating branches in"
                                " the manifest files on master. All projects must be refered to"
                                " by explicit git hash revision" % project.attrib["name"])
