# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import re
import shlex
import tempfile
import distutils.dir_util
import xml.etree.ElementTree as ET
from . import git
from . import process_tools


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

def update_file_and_zuul_clone(project_root: str, template_path: str, output_path: str, repository: str, using_zuul: bool):
    print("Arguments in update_file_and_zuul_clone: " "project_root: "+ project_root + " template_path: " + template_path + " output_path: " + output_path + " repository: " + repository)
    parser = ET.XMLParser(target=CommentedTreeBuilder())
    tree = ET.parse(template_path, parser)
    root = tree.getroot()

    for project in root.iter('project'):
        current_repo = project.get('name')
        print("current_repo = " + current_repo)
        revision = project.get('revision')
        repo_path = project.get('path')
        if revision == "ZUUL_COMMIT_OR_HEAD":
            print("ZUUL_COMMIT_OR_HEAD stated in revision field in the manifest")
            # There was a problem when manifest path differ from Gerrit repo name, zuul cloner need
            # to add the patches to repo stated in the manifest file "path"
            if repo_path != current_repo:
                align_repo_path(repo_path, current_repo, project_root)
            #check what sha to use, the ZUUL_COMMIT or HEAD from repository
            revision = use_zuul_commit_or_head(repository, current_repo, using_zuul)
            print("setting revision to: " + revision)
            project.set('revision', revision)

    tree.write(output_path)

def align_repo_path(repo_path: str, repo_name: str, project_root: str):
        print("The repo path and the repo name is not equal in the manifest")
        print(repo_path + " not equal to " + repo_name)
        source_repo_path = os.path.join(project_root, repo_name)
        destination_repo_path = os.path.join(project_root, repo_path)

        # Only do alignment when the repo is downloaded, for instance in gate_build
        # not in ihu_gate_test or ihu_gate_build
        print("Check if the repo " + source_repo_path + "is downloaded")
        if os.path.isdir(source_repo_path):
            base_folder = project_root
            print("base_folder: " + base_folder)
            print("destination_repo_path: " + destination_repo_path)
            print("the whole path : " + base_folder + destination_repo_path + "/..")
            zuul_url = os.environ['ZUUL_URL']
            zuul_project = os.environ['ZUUL_PROJECT']
            home = os.environ['HOME']
            # since we iterate through the manifest the current repo could be
            # vendor/volvocars but the repo_name coulde be = to foo
            if zuul_project == repo_name:
                print("Using zuul cloner on repo: " + repo_name)
                print("zuul_url: " + zuul_url)
                print("zuul_project: " + zuul_project)
                zuul_wrapper = home + "/zuul_ssh_wrapper.sh"
                print("zuul_wrapper: " + zuul_wrapper)
                os.environ['GIT_SSH'] = zuul_wrapper
                print(os.environ['GIT_SSH'])
                process_tools.check_output_logged(["zuul-cloner", "-v", zuul_url, zuul_project],
                        cwd=os.path.abspath(os.path.join(base_folder, destination_repo_path, "..")))
                os.chdir(project_root)
                print("Current working directory: " + project_root)
            else:
                print("The current ZUUL_PROJECT are not equal to current repo")
                print("ZUUL_PROJECT = " + zuul_project + "repo_name = " + repo_name)
        else:
            print("The repo is not downloaded to workspace, ignoring alignment.")

def use_zuul_commit_or_head(repo_with_commit: str, current_repo_in_tmp_manifest: str, using_zuul: bool):
    if repo_with_commit != current_repo_in_tmp_manifest:
        print("Checking master on Gerrit server for latest revision")
        revision = git.Repo.ls_remote(current_repo_in_tmp_manifest)
    elif (not using_zuul) and (repo_with_commit == current_repo_in_tmp_manifest):
        print("Using GERRIT_NEWREV as revision")
        revision = os.environ['GERRIT_NEWREV']
    else:
        print("Using ZUUL_COMMIT as revision")
        revision = os.environ['ZUUL_COMMIT']

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
