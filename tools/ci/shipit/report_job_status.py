# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
from shipit import reporter
import os
import json
import logging.config

# Used to report hourly/daily test status from Jenkins

po_emails = {
    'shwan.ciyako@volvocars.com',
    'anna.arasa@volvocars.com',
    'ahmed.kachroudi@volvocars.com',
    'anas.al-daghestani@volvocars.com',
    'pontius.nokrach@volvocars.com',
    'hans.bleckert@volvocars.com',
}


def main():
    with open(os.path.join(os.path.dirname(__file__), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    jenkins_job_name = sys.argv[1]
    jenkins_build_number = sys.argv[2]
    status = sys.argv[3].lower() #fail/pass
    git_hash = sys.argv[4]

    try:
        int(jenkins_build_number)
    except ValueError as e:
        raise Exception('Unknown value for jenkins build number')

    if jenkins_job_name == 'ihu_hourly':
        if status == 'pass':
            #reporter.set_gerrit_confidence_level(jenkins_job_name, jenkins_build_nr, status) #Not implemented
            pass
        elif status == 'fail':
            reporter.email_committers_since_last(jenkins_job_name, jenkins_build_number, git_hash)
        else:
            raise Exception('Unknown value for status parameter')
    elif jenkins_job_name == 'ihu_daily':
        if status == 'pass':
            #reporter.set_gerrit_confidence_level(jenkins_job_name, jenkins_build_nr, status) #Not implemented
            pass
        elif status == 'fail':
            pass #Skipped as daily still ain't stable enough
            #reporter.email_list_of_users(po_emails, jenkins_job_name, jenkins_build_number, git_hash)
        else:
            raise Exception('Unknown value for status parameter')
    else:
        raise Exception('Unknown value for jenkins job name parameter')

    reporter.update_manifest_branch(jenkins_job_name, status, git_hash)


if __name__ == "__main__":
    main()
