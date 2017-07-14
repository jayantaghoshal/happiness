from shipit import autobumper
import logging
import logging.config
import json
import os
import sys


def main(args):
    aosp_root_dir = args[0]
    mode = args[1]
    branch = args[2]
    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)
    if mode == "local":
        if len(args) != 3:
            raise SystemExit('Error: Mode local requires 3 args')
        autobumper.on_commit(aosp_root_dir, branch)
    elif mode == "autobump":
        if len(args) != 4:
            raise SystemExit('Error: Mode autobump requires 4 args')
        message = args[3]
        autobumper.post_merge(aosp_root_dir, branch, message)


if __name__ == "__main__":
    main(sys.argv[1:])