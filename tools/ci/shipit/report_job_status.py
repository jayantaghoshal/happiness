# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
from shipit import nightly_reporter
import os
import json
import logging.config

# Used to report hourly/nightly test status from Jenkins

def main():
    jenkins_job_name = sys.argv[1]
    jenkins_build_number = sys.argv[2]
    status = sys.argv[3].lower() #fail/pass

    if status == "pass":
        nightly_reporter.report_build_success(jenkins_job_name, jenkins_build_number)
    else:
        nightly_reporter.report_build_failed(jenkins_job_name, jenkins_build_number)


if __name__ == "__main__":
    with open(os.path.join(os.path.dirname(__file__), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)
    main()
