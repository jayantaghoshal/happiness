import os
import glob
import shutil
from . import manifest
from . import process_tools
from . import git


def on_commit(aosp_root_dir):
    # Zuul will have already cloned vendor/volvocars

    manifest_repo_path = os.path.join(aosp_root_dir, ".repo/manifests")
    volvocars_repo = os.path.join(aosp_root_dir, "vendor/volvocars")

    process_tools.check_output_logged(
        ["repo", "init",
         "-u", "ssh://gotsvl1415.got.volvocars.net:29421/manifest",
         "-b", "master"],
        cwd=os.path.abspath(aosp_root_dir))

    copy_and_apply_templates_to_manifest_repo(aosp_root_dir, volvocars_repo, manifest_repo_path)
    process_tools.check_output_logged(["repo", "sync",
                                       "--jobs=6",
                                       "--no-clone-bundle",
                                       "--current-branch"], cwd=aosp_root_dir)


def copy_and_apply_templates_to_manifest_repo(aosp_root_dir, volvocars_repo, manifest_repo_path):
    vcc_manifest_files = glob.glob(os.path.join(volvocars_repo, "manifests") + "/*.xml")

    old_manifest_files_in_manifest_repo = glob.glob(os.path.join(manifest_repo_path, "manifests") + "/*.xml")
    for f in old_manifest_files_in_manifest_repo:
        os.unlink(f)

    for manifest_template_file in vcc_manifest_files:
        dest = os.path.join(manifest_repo_path, os.path.basename(manifest_template_file))
        manifest.update_file(aosp_root_dir, manifest_template_file, dest)


def post_merge(aosp_root_dir):
    manifest_repo_path = os.path.join(aosp_root_dir, ".repo/manifests")
    volvocars_repo = os.path.join(aosp_root_dir, "vendor/volvocars")

    process_tools.check_output_logged(
        ["repo", "init",
         "-u", "ssh://gotsvl1415.got.volvocars.net:29421/manifest",
         "-b", "master"],
        cwd=os.path.abspath(aosp_root_dir))

    copy_and_apply_templates_to_manifest_repo(aosp_root_dir, volvocars_repo, manifest_repo_path)
    manifest_repo = git.Repo(manifest_repo_path)
    manifest_repo.commit("Auto bump", True)
    manifest_repo.push()

