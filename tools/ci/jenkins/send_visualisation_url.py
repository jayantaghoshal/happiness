#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import os
import sys
import traceback

sys.path.append(os.path.join(os.environ["REPO_ROOT_DIR"],"vendor/volvocars/tools/"))
from lib.python.gerrit.gerrit_handler import gerrit

def main():
    try:
        gerrit_commit_id = str(os.environ["ZUUL_CHANGE"])
        gerrit_url = "gotsvl1415.got.volvocars.net"
        gerrit_port = "29421"
        gerrit_functional_user = "E9426001"

        gerrit_instance = gerrit(gerrit_url, gerrit_port, gerrit_functional_user)
        query_out = gerrit_instance.query(
            ["--current-patch-set", gerrit_commit_id])[0]

        change_id = query_out["id"]
        commit = query_out["currentPatchSet"]["revision"]

        follow_my_commit_url = "https://ci.volvocars.biz/resolve/?chainId=" + change_id
        message = "Follow_your_commit_here:"
        gerrit_instance.review(
            ["--message", message + follow_my_commit_url, commit])

    except Exception as e:
        print(traceback.format_exc())
        print(e)
        print("Sending VCC-CI's url failed")

if __name__ == '__main__':
    main()
