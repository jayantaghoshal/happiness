# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import glob
import logging
from . import manifest
from . import process_tools
from . import git

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

def on_commit(aosp_root_dir: str, branch: str):
    # Zuul will have already cloned vendor/volvocars

    manifest_repo = git.Repo(os.path.join(aosp_root_dir, ".repo/manifests"))
    volvocars_repo_path = os.path.join(aosp_root_dir, "vendor/volvocars")
    volvocars_repo = git.Repo(volvocars_repo_path)

    repo_init(aosp_root_dir, branch)

    copy_and_apply_templates_to_manifest_repo(aosp_root_dir, volvocars_repo, manifest_repo)
    process_tools.check_output_logged(["repo", "sync",
                                       "--jobs=6",
                                       "--no-clone-bundle",
                                       "--current-branch"], cwd=aosp_root_dir)


def copy_and_apply_templates_to_manifest_repo(aosp_root_dir: str,
                                              volvocars_repo: git.Repo,
                                              manifest_repo: git.Repo,
                                              stage_changes: bool = False):
    vcc_manifest_files = glob.glob(os.path.join(volvocars_repo.path, "manifests") + "/*.xml")

    old_manifest_files_in_manifest_repo = glob.glob(os.path.join(manifest_repo.path, "manifests") + "/*.xml")
    for f in old_manifest_files_in_manifest_repo:
        os.unlink(f)

    for manifest_template_file in vcc_manifest_files:
        dest = os.path.join(manifest_repo.path, os.path.basename(manifest_template_file))
        manifest.update_file(aosp_root_dir, manifest_template_file, dest)
        if stage_changes:
            manifest_repo.add([dest])

    if not manifest_repo.any_changes(staged=stage_changes):
        raise RuntimeError('No manifest changes found. Failed to clone/update repo(s)?')


def post_merge(aosp_root_dir: str,
               branch: str,
               additional_commit_message: str):
    manifest_repo = git.Repo(os.path.join(aosp_root_dir, ".repo/manifests"))
    volvocars_repo_path = os.path.join(aosp_root_dir, "vendor/volvocars")
    volvocars_repo = git.Repo(volvocars_repo_path)

    repo_init(aosp_root_dir, branch)

    copy_and_apply_templates_to_manifest_repo(aosp_root_dir,
                                              volvocars_repo,
                                              manifest_repo,
                                              stage_changes=True)

    # TODO: Include list of changes in commit message and log
    logging.info("Changes found, pushing new manifest")
    manifest_repo.commit("Auto bump\n\n" + additional_commit_message)
    manifest_repo.push(["origin", "HEAD:refs/for/" + branch + "%submit"])
