# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import glob
import logging
import re
from xml.etree import ElementTree as ET
from typing import Tuple, List, Dict, Union, Any
from . import manifest
from . import process_tools
from . import git

class Manifest(object):
    def __init__(self, xmlstring: str) -> None:
        try:
            self.root_element = ET.fromstring(xmlstring)
        except Exception:
            self.root_element = None

    def projects(self) -> Dict[str, Dict[str, str]]:
        """
        Collects all the <project...> tags in the manifest and returns a dict with project name as a key and
        all the tag attributes as a dict value.
        """
        project_list = {} # type: Dict[str, Dict[str, str]]
        if self.root_element:
            for project in self.root_element.findall("project"):
                if 'path' not in project.attrib:
                    project.attrib['path'] = project.attrib['name']
                project_list[project.attrib['name']] = project.attrib
        return project_list

    def diff(self, xmlstring: str) -> Dict[str, List[Dict[str, str]]]:
        """
        Diff this manifest with the one supplied through the xmlstring.
        Return a dict with project name as a key and a list consisting of two elements (dict) where element 0 are the
        project attributes from this manifest and element 1 are the attributes of the supplied manifest.
        """
        other = Manifest(xmlstring)
        diff = {} # type: Dict[str, List[Dict[str, str]]]
        for project, details in self.projects().items():
            if project not in other.projects():
                diff[project] = [None, details]
            else:
                if details != other.projects()[project]:
                    diff[project] = [other.projects()[project], details]
        for other_project, other_details in other.projects().items():
            if other_project not in self.projects():
                diff[other_project] = [other_details, None]
        return diff


def check_manifest(aosp_root_dir: str, branch: str):
    # Zuul will have already cloned vendor/volvocars

    volvocars_repo_path = os.path.join(aosp_root_dir, "vendor/volvocars")
    volvocars_repo = git.Repo(volvocars_repo_path)

    vcc_manifest_files = glob.glob(os.path.join(volvocars_repo.path, "manifests") + "/*.xml")
    for manifest_template_file in vcc_manifest_files:
        manifest.verify_no_floating_branches(manifest_template_file, branch)

def repo_init(aosp_root_dir: str, branch: str):
    process_tools.check_output_logged(
        ["repo", "init",
         "-u", "ssh://gotsvl1415.got.volvocars.net:29421/manifest",
         "-b", branch],
        cwd=os.path.abspath(aosp_root_dir))

def on_commit(aosp_root_dir: str, sync: bool, repository: str):
    # Zuul will have already cloned vendor/volvocars

    manifest_repo = git.Repo(os.path.join(aosp_root_dir, ".repo/manifests"))
    head_sha = manifest_repo.rev_parse('HEAD')
    volvocars_repo_path = os.path.join(aosp_root_dir, "vendor/volvocars")
    volvocars_repo = git.Repo(volvocars_repo_path)

    repo_init(aosp_root_dir, head_sha)
    copy_and_apply_templates_to_manifest_repo(aosp_root_dir, volvocars_repo, manifest_repo, repository)

    if sync:
        process_tools.check_output_logged(["repo", "sync",
                                        "--jobs=6",
                                        "--no-clone-bundle",
                                        "--force-sync",
                                        "--detach",
                                        "--current-branch"], cwd=aosp_root_dir)

def copy_and_apply_templates_to_manifest_repo(aosp_root_dir: str,
                                              volvocars_repo: git.Repo,
                                              manifest_repo: git.Repo,
                                              repository: str,
                                              stage_changes: bool = False,
                                              using_zuul: bool = True):
    print("Arguments for copy_and_apply...: stage_changes= %s using_zuul= %s aosp_root_dir: %s volvocars_repo: %s manifest_repo: %s repository: %s" % (stage_changes,
                                                                                                                                                       using_zuul,
                                                                                                                                                       aosp_root_dir,
                                                                                                                                                       volvocars_repo,
                                                                                                                                                       manifest_repo,
                                                                                                                                                       repository))
    vcc_manifest_files = glob.glob(os.path.join(volvocars_repo.path, "manifests") + "/*.xml")
    old_manifest_files_in_manifest_repo = glob.glob(os.path.join(manifest_repo.path, "manifests") + "/*.xml")

    print("Path for vcc_manifest_files " + str(volvocars_repo.path))
    print("Path for old_manifest_files_in_manifest_repo: " + str(manifest_repo.path))

    for manifest_file in old_manifest_files_in_manifest_repo:
        print("Manifests in old_manifest_files_in_manifest_repo: " + manifest_file)

    for manifest_file in vcc_manifest_files:
        print("Manifests in vcc_manifest_files: " + manifest_file)

    for f in old_manifest_files_in_manifest_repo:
        os.unlink(f)

    print("Number of vcc_manifest_files files: " + str(len(vcc_manifest_files)))
    print("Number of old_manifest_files_in_manifest_repo files: " + str(len(old_manifest_files_in_manifest_repo)))

    for manifest_template_file in vcc_manifest_files:
        print("Checking this manifest file: " + str(manifest_template_file))
        dest = os.path.join(manifest_repo.path, os.path.basename(manifest_template_file))
        manifest.update_file(aosp_root_dir, manifest_template_file, dest, repository, using_zuul)
        if stage_changes:
            manifest_repo.add([dest])

    manifest_repo.run_git(["log", "-2", "--pretty=oneline"])
    manifest_repo.run_git(["diff"])

    if not manifest_repo.any_changes(staged=stage_changes):
        manifest_repo.run_git(["log", "-2", "--pretty=oneline"])
        raise RuntimeError('No manifest changes found. Failed to clone/update repo(s)?')

def get_changed_projects_from_manifest(manifest_repo: git.Repo) -> Dict[str, Dict[str, List[Dict[str, str]]]]:
    """
    Gets a list of changed manifest files and changes within them. Assumes that the latest manifest in the repository
    has been staged.
    Returns a dict with manifest filenames as keys, and the return value of Manifest().diff() as values.
    """
    manifest_changes = {}
    for manifest_file in manifest_repo.run_git(['diff', '--staged', '--name-only']).splitlines():
        try:
            staged_manifest = manifest_repo.run_git(['show', ':{}'.format(manifest_file)])
        except Exception:
            # Failed to run git show manifest, assume it was removed
            staged_manifest = ""
        try:
            head_manifest = manifest_repo.run_git(['show', 'HEAD:{}'.format(manifest_file)])
        except Exception:
            # Failed to show HEAD of manifest, assume it was added.
            head_manifest = ""
        manifest_changes[manifest_file] = Manifest(staged_manifest).diff(head_manifest)
    return manifest_changes


def git_commit_info(repo: git.Repo,
                    rev: str) -> Dict[str, Any]:
    """
    Returns info about the given commit as a dict.
    """
    info = {'body': []} # type: Dict[str, Any]
    for commit_line in repo.run_git(['show', '-s', rev]).splitlines():
        match = re.search(r"^(\S+?):*\s+(.*)$", commit_line)
        if match:
            info[match.group(1).lower()] = match.group(2)
        else:
            if 'title' not in info:
                if commit_line == "":
                    continue
                info['title'] = commit_line.lstrip()
            else:
                info['body'].append(commit_line)
    return info


def assemble_commit_messages(base_dir: str,
                             manifest_repo: git.Repo) -> Tuple[str, str]:
    """
    Assembles the commit messages for the staged changes in the given manifest repository.
    Returns a tuple consisting of commit title postfix and commit body prefix.
    """
    commit_body_prefix = ""
    commit_title_postfix = ""
    for manifest_name, diff in get_changed_projects_from_manifest(manifest_repo).items():
        for name, details in diff.items():
            if not details[0]:
                commit_body_prefix += " - Added '{}'\n".format(details[1]['path'])
            elif not details[1]:
                commit_body_prefix += " - Removed '{}'\n".format(details[0]['path'])
            else:
                if not os.path.exists(os.path.join(base_dir, details[1]['path'])):
                    commit_body_prefix += " - Updated '{}' to commit {}\n".format(details[0]['name'],
                                                                                  details[0]['revision'])
                    print("Repository {} is MIA (expected it to be at '{}')".format(
                        details[1]['name'],
                        os.path.join(base_dir, details[1]['path'])))
                    continue
                proj_repo = git.Repo(os.path.join(base_dir, details[1]['path']))
                commit_body_prefix_addition = ""
                revlist = [] # type: List[str]
                try:
                    revlist = proj_repo.run_git(['rev-list',
                                                 '^{}'.format(details[0]['revision']),
                                                 details[1]['revision']]).splitlines()
                except Exception:
                    print("ERROR: Failed to execute 'git {}'".format(" ".join(['rev-list',
                                                                               '^{}'.format(details[0]['revision']),
                                                                               details[1]['revision']])))
                for rev in revlist:
                    rev_info = git_commit_info(proj_repo, rev)
                    if rev_info['title'] in commit_title_postfix:
                        # Replace title postfix with real commit message if previous was a merge commit
                        commit_title_postfix = rev_info['title']
                    if commit_title_postfix == "":
                        commit_title_postfix = rev_info['title']
                    jira_tags = re.findall(r"\[\w+-\w+\]", " ".join(rev_info['body']))
                    commit_body_prefix_addition += " - {} {}\n".format(rev_info['title'], " ".join(jira_tags))
                if commit_body_prefix_addition != "":
                    commit_body_prefix += "\n{}:\n".format(details[0]['path']) + commit_body_prefix_addition + "\n"
    return (commit_title_postfix, commit_body_prefix)


def post_merge(aosp_root_dir: str,
               branch: str,
               commit_message: str,
               repo_path_name: str):
    manifest_repo = git.Repo(os.path.join(aosp_root_dir, ".repo/manifests"))
    volvocars_repo_path = os.path.join(aosp_root_dir, "vendor/volvocars")
    volvocars_repo = git.Repo(volvocars_repo_path)

    repo_init(aosp_root_dir, branch)

    copy_and_apply_templates_to_manifest_repo(aosp_root_dir,
                                              volvocars_repo,
                                              manifest_repo,
                                              repo_path_name,
                                              stage_changes=True,
                                              using_zuul=False)

    commit_title = "Auto bump"

    (commit_title_postfix, commit_body_prefix) = assemble_commit_messages(aosp_root_dir, manifest_repo)

    if commit_title_postfix != "":
        commit_title += ": " + commit_title_postfix

    logging.info("Changes found, pushing new manifest")
    manifest_repo.commit(commit_title + "\n\n" + commit_body_prefix + commit_message)
    manifest_repo.push(["origin", "HEAD:refs/for/" + branch + "%submit"])