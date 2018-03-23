#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import requests
import logging
import json
import redis
import os

VCC_CI_API_URL = "http://cimb.volvocars.biz/api/1.6.0"
redis_con = redis.Redis("gotsvl1416.got.volvocars.net")


class VCCCIProxy(object):

    def __init__(self, change_id: str) -> None:
        if "TOP_JOB_NUMBER" in os.environ and "TOP_JOB_JOBNAME" in os.environ and os.environ["TOP_JOB_NUMBER"] and os.environ["TOP_JOB_JOBNAME"]:
            self.top_test_job_build_number = int(os.environ["TOP_JOB_NUMBER"])
            self.top_test_job_name = os.environ["TOP_JOB_JOBNAME"]
        else:
            self.top_test_job_name = os.environ["JOB_NAME"]
            self.top_test_job_build_number = int(os.environ["BUILD_NUMBER"])

        self.testsuite_name = os.environ["JOB_NAME"]
        self.test_job_build_number = int(os.environ["BUILD_NUMBER"])

        self.chain_id = change_id
        self.url_template = "https://icup_android.jenkins.cm.volvocars.biz/job/"
        self.target = "VCC-CI"

    def getLogURL(self, module_name: str):
        log = "http://10.251.165.178:3000/detailed_view?module_name=%s&job_name=%s&build_number=%s" % (
            module_name,
            self.testsuite_name,
            self.test_job_build_number)
        return log

    def getUrl(self):
        return "https://icup_android.jenkins.cm.volvocars.biz/job/%s/%s" % (self.testsuite_name, self.test_job_build_number)

    def print_json(self, data):
        logging.info(json.dumps(data,
                                sort_keys=True,
                                indent=4,
                                separators=(',', ': ')))

    def headers(self):

        return {
            'User-Agent': 'IHU CI',
            'Content-Type': 'application/json'
        }

    def testcase_started(self, module_name: str):

        data = {

            "chain_id": self.chain_id,
            "name": module_name,
            "activity": self.top_test_job_name,
            "testsuite_name": self.testsuite_name,
            "url": self.url_template + self.testsuite_name,
            "logs": [self.getLogURL(module_name)],
            "target": self.target

        }

        url = VCC_CI_API_URL + "/TestcaseStarted"
        logging.info("POST request to VCC-CI - %s", url)
        self.print_json(data)
        r = requests.post(url, json=json.dumps(data), headers=self.headers())
        logging.debug(str(r.status_code))
        logging.debug(r.text)

    def testcase_finished(self, module_name: str, status: bool):

        data = {
            "chain_id": self.chain_id,
            "name": module_name,
            "activity": self.top_test_job_name,
            "testsuite_name": self.testsuite_name,
            "url": self.url_template + self.testsuite_name,
            "logs": [self.getLogURL(module_name)],
            "target": self.target,
            "status": "success" if status else "failure"
        }

        url = VCC_CI_API_URL + "/TestcaseFinished"
        logging.info("POST request to VCC-CI - %s", url)
        self.print_json(data)
        r = requests.post(url, json=json.dumps(data), headers=self.headers())
        logging.debug(str(r.status_code))
        logging.debug(r.text)
