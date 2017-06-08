from shipit import autobumper
import logging
import logging.config
import json
import os
import sys


def main(args):
    aosp_root_dir = args[0]
    mode = args[1]
    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)
    if mode == "local":
        autobumper.on_commit(aosp_root_dir)
    elif mode == "autobump":
        autobumper.post_merge(aosp_root_dir, args[2])


if __name__ == "__main__":
    main(sys.argv[1:])