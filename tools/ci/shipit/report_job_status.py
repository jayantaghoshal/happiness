# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
from shipit import reporter
import os
import json
import logging.config
import logging
from jenkins import Jenkins
import argparse

logger = logging.getLogger(__name__)

# Used to report hourly/daily test status from Jenkins

po_emails = {
    'shwan.ciyako@volvocars.com',
    'anna.arasa@volvocars.com',
    'ahmed.kachroudi@volvocars.com',
    'anas.al-daghestani@volvocars.com',
    'pontius.nokrach@volvocars.com',
    'hans.bleckert@volvocars.com',
}


class JenkinsClient():

    def __init__(self):
        os.environ['HTTP_PROXY'] = ''
        os.environ['HTTPS_PROXY'] = ''
        os.environ['http_proxy'] = ''
        os.environ['https_proxy'] = ''
        os.environ['NO_PROXY'] = ''
        self._jenkins = Jenkins('https://icup_android.jenkins.cm.volvocars.biz',
                username=os.environ['JENKINS_USER'],
                password=os.environ['JENKINS_API_PASSWORD'])

    def get_job_status(self, jenkins_job_name: str, jenkins_build_number: int):
        build_info = self._jenkins.get_build_info(jenkins_job_name, jenkins_build_number, depth=0)
        status = 'pass'
        for build in build_info['subBuilds']:
            if not build['result'] == 'SUCCESS':
                status = 'fail'
        return status


def report(job_name, job_number, git_hash, status, last_status):
    if job_name == 'ihu_hourly':
        if status == 'pass':
            #reporter.set_gerrit_confidence_level(job_name, job_number, status) #Not implemented
            pass
        elif status == 'fail':
            if last_status == 'pass':
                logger.info("Emailing committers since last")
                reporter.email_committers_since_last(job_name, str(job_number), git_hash)
            else:
                pass
        else:
            raise Exception('Unknown value for status parameter')

    elif job_name == 'ihu_daily':
        if status == 'pass':
            #reporter.set_gerrit_confidence_level(job_name, str(job_number), status) #Not implemented
            pass
        elif status == 'fail':
            pass #Skipped as daily still ain't stable enough
            #reporter.email_list_of_users(po_emails, job_name, str(job_number), git_hash)
        else:
            raise Exception('Unknown value for status parameter')

    elif job_name == 'ihu_hourly_build-eng':
        # Only update the manifest branches
        pass

    elif job_name == 'ihu_daily_build_vcc_eng':
        # Only update the manifest branches
        pass

    else:
        raise Exception('Unknown value for job name parameter')

    reporter.update_manifest_branches(job_name, status, git_hash)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-j", "--job_name", type=str, required=True,
                        help="The job name for which results should be reported")
    parser.add_argument("-n", "--job_number", type=int, required=False,
                        help="The jenkins job number, used for retrieving job status")
    parser.add_argument("-g", "--git_hash", type=str, required=True,
                        help="The git hash for the results")
    parser.add_argument("-s", "--status", type=str, default=None,
                        help="The job status to be reported")
    parser.add_argument("-l", "--last_status", type=str, default=None,
                        help="The previous job status")
    return parser.parse_args()


def main():
    with open(os.path.join(os.path.dirname(__file__), "logging.json"), "rt") as f:
        log_config = json.load(f)
    logging.config.dictConfig(log_config)

    args = parse_args()

    if args.status:
        status = args.status
    else:
        jenkins_client = JenkinsClient()
        status = jenkins_client.get_job_status(args.job_name, args.job_number)
    logger.info("Job status: {}".format(status))

    if args.last_status:
        last_status = args.last_status
    elif args.job_number > 0:
        last_status = jenkins_client.get_job_status(args.job_name, args.job_number - 1)
    else:
        last_status = 'none'
    logger.info("Last job status: {}".format(last_status))

    report(args.job_name, args.job_number, args.git_hash, status, last_status)


if __name__ == "__main__":
    main()
