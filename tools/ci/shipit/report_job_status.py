# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
from shipit import reporter
import os
import json
import logging.config
from jenkins import Jenkins

# Used to report hourly/daily test status from Jenkins

po_emails = {
    'shwan.ciyako@volvocars.com',
    'anna.arasa@volvocars.com',
    'ahmed.kachroudi@volvocars.com',
    'anas.al-daghestani@volvocars.com',
    'pontius.nokrach@volvocars.com',
    'hans.bleckert@volvocars.com',
}


def get_job_status(jenkins_job_name: str, jenkins_build_number: str):
    jenkins = Jenkins('https://icup_android.jenkins.cm.volvocars.biz',
            username=os.environ['JENKINS_USER'],
            password=os.environ['JENKINS_API_PASSWORD'])
    build_info = jenkins.get_build_info(jenkins_job_name, int(jenkins_build_number), depth=0)
    status = 'pass'
    for build in build_info['subBuilds']:
        if not build['result'] == 'SUCCESS':
            status = 'fail'
    return status


def main():
    with open(os.path.join(os.path.dirname(__file__), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    jenkins_job_name = sys.argv[1]
    jenkins_build_number = sys.argv[2]
    git_hash = sys.argv[3]

    try:
        int(jenkins_build_number)
    except ValueError as e:
        raise Exception('Unknown value for jenkins build number')

    status = get_job_status(jenkins_job_name, jenkins_build_number)

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
