# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import re
import tempfile
import pathlib
import shutil
import xml.etree.ElementTree as ET
from . import git

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

def update_file(project_root: str, template_path: str, output_path: str, repository: str, using_zuul: bool):
    print("Arguments in update_file: " "project_root: "+ project_root + " template_path: " + template_path + " output_path: " + output_path + " repository: " + repository)
    parser = ET.XMLParser(target=CommentedTreeBuilder())
    tree = ET.parse(template_path, parser)
    root = tree.getroot()

    for project in root.iter('project'):
        current_repo = project.get('name')
        revision = project.get('revision')
        repo_path = project.get('path')
        if revision == "ZUUL_COMMIT_OR_HEAD":
            print("ZUUL_COMMIT_OR_HEAD stated in revision field in the manifest")
            #There was a problem men manifest path differ from Gerrit repo name
            if repo_path != current_repo:
                align_repo_path(repo_path, current_repo, project_root)

            #check what sha to use, the ZUUL_COMMIT or HEAD from repository
            revision = use_zuul_commit_or_head(repository, current_repo, using_zuul)
            print("current_repo = " + current_repo)
            print("revision = " + revision)
            project.set('revision', revision)

    tree.write(output_path)

def align_repo_path(repo_path: str, repo_name: str, project_root: str):
        print("The repo path and the repo name is not equal in the manifest")
        print(repo_path + " not equal to " + repo_name)
        source_repo_path = os.path.join(project_root, repo_name)
        path_to_create = os.path.join(project_root, repo_path)
        print("Path to create: " + path_to_create)
        pathlib.Path(path_to_create).mkdir(parents=True, exist_ok=True)

        print("Moving folder: " + source_repo_path)
        files = os.listdir(os.path.join(project_root, repo_name))
        for f in files:
            shutil.move(source_repo_path + "/" + f, path_to_create)

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
