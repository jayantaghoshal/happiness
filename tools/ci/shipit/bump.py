from shipit import autobumper
import logging
import logging.config
import json
import os
import sys


def main(aosp_root_dir):
    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)
    autobumper.on_commit(aosp_root_dir)


if __name__ == "__main__":
    main(sys.argv[1])