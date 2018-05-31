# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import logging.config
import json
import os
import sys
from shipit import autobumper


def main(args):
    if len(args) < 2:
        raise SystemExit('Error: bump.py requires 2 args')

    aosp_root_dir = args[0]
    mode = args[1]

    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    if mode == "check":
        branch = args[2]
        autobumper.check_manifest(aosp_root_dir, branch)
    elif mode == "local":
        syncMode = args[2]
        if syncMode == "no_sync":
            sync = False
        elif syncMode == "sync":
            sync = True
        repository = args[3]
        autobumper.on_commit(aosp_root_dir, sync, repository)
    elif mode == "sync_repo_based_on_manifest":
        repository = args[2]
        autobumper.sync_repo(aosp_root_dir, repository)
    elif mode == "update_manifest_for_progression":
        repository = args[2]
        branch = args[3]
        autobumper.progression_manifest(aosp_root_dir, repository, branch)
    elif mode == "autobump":
        if len(args) != 5:
            raise SystemExit('Error: Mode autobump requires 4 args')
        branch = args[2]
        message = args[3]
        repo_path_name = args[4]
        autobumper.post_merge(aosp_root_dir, branch, message, repo_path_name)


if __name__ == "__main__":
    main(sys.argv[1:])
