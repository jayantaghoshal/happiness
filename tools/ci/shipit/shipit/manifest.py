# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import re
import shlex
import tempfile
import logging
import logging.config
import distutils.dir_util
import typing
import xml.etree.ElementTree as ET
from . import git
from . import process_tools


logger = logging.getLogger(__name__)

class Error(Exception):
    pass
#This is used to keep the comments in the manifest files when writing to file
class CommentedTreeBuilder(ET.TreeBuilder):
    def __init__(self, *args, **kwargs):
        super(CommentedTreeBuilder, self).__init__(*args, **kwargs)

    def comment(self, data):
        self.start(ET.Comment, {})
        self.data(data)
        self.end(ET.Comment)

def set_sha_in_template_manifest(project_root: str, template_path: str, repository: str):
    parser = ET.XMLParser(target=CommentedTreeBuilder())
    tree = ET.parse(template_path, parser)
    root = tree.getroot()
    revision = ""

    for project in root.iter('project'):
        current_repo = project.get('name')
        if current_repo == repository:
            revision = git.Repo.ls_remote(repository)
            project.set('revision', revision)

    tree.write(template_path)
    return revision

def update_file(project_root: str, template_path: str, output_path: str, repository: str, using_zuul: bool):
    logger.info("Arguments in update_file: " "project_root: "+ project_root + " template_path: " + template_path + " output_path: " + output_path + " repository: " + repository)
    parser = ET.XMLParser(target=CommentedTreeBuilder())
    tree = ET.parse(template_path, parser)
    root = tree.getroot()

    repos_with_zuul_changes_set = set() # type: typing.Set[str]

    if using_zuul:
        repos_with_zuul_changes_set = get_zuul_repos_in_gate()

    project_to_remove = []
    for project in root.iter('project'):
        current_repo = project.get('name')
        revision = project.get('revision')
        if (revision == "ZUUL_COMMIT_OR_HEAD"):
            logger.info("current_repo = " + current_repo)
            logger.info("ZUUL_COMMIT_OR_HEAD stated in revision field in the manifest")
            # Excecuted in the manifest bump stage, after merge to master
            if not using_zuul:
                revision = use_zuul_commit_or_head(repository, current_repo, using_zuul)
                logger.info("setting revision to: " + revision)
                project.set('revision', revision)
            # Check if the repo should be updated with latest revision in manifest
            # this case if for Zuul repos that is not tested in the gate
            elif current_repo not in repos_with_zuul_changes_set:
                revision = use_zuul_commit_or_head(repository, current_repo, using_zuul)
                logger.info("setting revision to: " + revision)
                project.set('revision', revision)
            # If the zuul repo is tested in the gate, remove it from manifest
            # and use zuul-cloner to get the repository
            else:
                logger.info("Removing " + current_repo + " from manifest")
                project_to_remove.append(project)

    for project in project_to_remove:
        root.remove(project)

    tree.write(output_path)


# This will get a value from the manifest based on the lookup value you send
def get_value_from_manifest_by_git_name(template_path: str, git_repo: str, lookup: str = ""):
    if lookup:
        parser = ET.XMLParser(target=CommentedTreeBuilder())
        tree = ET.parse(template_path, parser)
        root = tree.getroot()
        logger.info("lookup = " + lookup)

        for project in root.findall('project'):
            name = project.get('name')
            return_value =  project.get(lookup)
            if name == git_repo:
                logger.info("return_value = " + return_value)
                return str(return_value)

    return None


def get_zuul_repos_in_gate():
    # The Zuul Changes states the changes in the Gate and the corresponding repos
    zuul_changes = os.environ['ZUUL_CHANGES']
    logger.info("zuul_changes = " + zuul_changes)
    repos_with_zuul_changes = re.findall('(?:^|\^)([^:]+):', zuul_changes)
    repos_with_zuul_changes_set = set(repos_with_zuul_changes)

    return repos_with_zuul_changes_set


def get_zuul_repos_map(template_path: str):
    parser = ET.XMLParser(target=CommentedTreeBuilder())
    tree = ET.parse(template_path, parser)
    root = tree.getroot()
    zuul_repos = {}

    for project in root.findall('project'):
        name = project.get('name')
        path = project.get('path')
        revision = project.get('revision')
        if revision == "ZUUL_COMMIT_OR_HEAD":
            zuul_repos[name] = path
            logger.info("The path = " + path)

    return zuul_repos


def use_zuul_commit_or_head(repo_with_commit: str, current_repo_in_tmp_manifest: str, using_zuul: bool):
    repos_with_zuul_changes_set = set() # type: typing.Set[str]
    if using_zuul:
        repos_with_zuul_changes_set = get_zuul_repos_in_gate()
        base_dir = os.getcwd()
        full_path = os.path.join(base_dir, current_repo_in_tmp_manifest)
    # This will get the revision for the repo that have revision "ZUUL_COMMIT_OR_HEAD" but not currently tested in the gate
    if (current_repo_in_tmp_manifest != repo_with_commit) and (current_repo_in_tmp_manifest not in repos_with_zuul_changes_set):
        logger.info("Checking master on Gerrit server for latest revision")
        revision = git.Repo.ls_remote(current_repo_in_tmp_manifest)
        return revision
    # Used for the bumping of manifest, since it is not using Zuul and triggered by merge to master
    elif (not using_zuul) and (repo_with_commit == current_repo_in_tmp_manifest):
        logger.info("Using GERRIT_NEWREV as revision")
        revision = os.environ['GERRIT_NEWREV']
        return revision
    # Get the revision for the Zuul change that have been downloaded
    elif current_repo_in_tmp_manifest in repos_with_zuul_changes_set:
        logger.info("Using rev-parse in Git repo as revision")
        revision = git.Repo.repo_rev_parse(full_path)
        revision = revision.decode('utf-8').strip()
        return revision


def verify_no_floating_branches(manifest_path: str, branch: str):
    def is_sha_hash(revision):
        return re.match(r"[a-f0-9]{40}", revision) is not None

    parser = ET.XMLParser(target=CommentedTreeBuilder())
    parsed_manifest = ET.parse(manifest_path, parser)
    projects = parsed_manifest.findall("project")

    for project in projects:
        rev = project.attrib["revision"]
        if not is_sha_hash(rev):
            if rev != "ZUUL_COMMIT_OR_HEAD":
                if branch == "master":
                    raise Error("Project %s --- You are not allowed to have floating branches in"
                                " the manifest files on master. All projects must be refered to"
                                " by explicit git hash revision" % project.attrib["name"])
