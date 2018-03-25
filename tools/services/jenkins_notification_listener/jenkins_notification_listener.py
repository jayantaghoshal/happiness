# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from flask import Flask, json, request
from jenkins import Jenkins
from http.client import HTTPConnection
import redis
import os
import requests
import datetime
import logging
import re

class RedisClient(object):

    def __init__(self):
        self._redis = None

    def set(self, key, value):
        return self.redis().set(key, value)

    def get(self, key):
        value = self.redis().get(key)
        logging.info("Redis GET %s -> %s" % (key, value))
        return value

    def redis(self):
        if self._redis is None:
            self._redis = redis.StrictRedis(host='gotsvl1416.got.volvocars.net',
                                            port=6379,
                                            db=0,
                                            decode_responses=True)
        return self._redis

class JenkinsClient(object):

    def __init__(self, username, password):
        self._server = None
        self._username = username
        self._password = password

    def build(self, job, build):
        return self._jenkins().get_build_info(job, build)

    def _jenkins(self):
        if self._server is None:
            self._server = Jenkins('https://icup_android.jenkins.cm.volvocars.biz',
                                   username=self._username,
                                   password=self._password)
        return self._server

class VCCCIProxy(object):
    VCC_CI_API_URL="http://cimb.volvocars.biz/api/1.6.0"
    TEST_REPORT_URL="http://10.251.165.178:3000"

    SUPPORTED_JOBS = [
        "icup_android_manifest_bump",
        "ihu_gate_build",
        "ihu_gate_test",
        "ihu_gate_test-generic",
        "ihu_gate_test-flexray",
        "ihu_gate_test-apix",
        "ihu_gate_test-audio",
        "ihu_gate_test-cem",
        "ihu_hourly",
        "ihu_hourly_build-eng",
        "ihu_hourly_test-generic",
        "ihu_hourly_test-flexray",
        "ihu_hourly_test-apix",
        "ihu_hourly_test-audio",
        "ihu_hourly_test-cem",
        "ihu_daily",
        "ihu_daily_build_vcc_eng",
        "ihu_daily_build_vcc_userdebug",
        "ihu_daily_build_vcc_user",
        "ihu_daily_test-generic",
        "ihu_daily_test-flexray",
        "ihu_daily_test-apix",
        "ihu_daily_test-audio",
        "ihu_daily_test-cem",
        "ihu_daily_vts1-generic",
        "ihu_daily_vts2-generic",
        "ihu_daily_vts3-generic",
        "ihu_staging",
        "ihu_staging_test-generic",
        "ihu_staging_test-flexray",
        "ihu_staging_test-apix",
        "ihu_staging_test-audio",
        "ihu_staging_test-cem"
    ]

    SUB_JOBS = [
        "ihu_hourly_build-eng",
        "ihu_hourly_test-generic",
        "ihu_hourly_test-flexray",
        "ihu_hourly_test-apix",
        "ihu_hourly_test-audio",
        "ihu_daily_build_vcc_eng",
        "ihu_daily_build_vcc_userdebug",
        "ihu_daily_build_vcc_user",
        "ihu_daily_test-generic",
        "ihu_daily_test-flexray",
        "ihu_daily_test-apix",
        "ihu_daily_test-audio",
        "ihu_daily_vts1-generic",
        "ihu_daily_vts2-generic",
        "ihu_daily_vts3-generic",
        "ihu_staging_test-generic",
        "ihu_staging_test-flexray",
        "ihu_staging_test-apix",
        "ihu_staging_test-audio",
        "ihu_staging_test-cem"
    ]

    def __init__(self, jenkins):
        self._jenkins = JenkinsClient
        self._notification = {}

    def process_notification(self, notification):
        self._notification = notification

        if self.name() in self.SUPPORTED_JOBS:
            logging.info("Received supported notification from Jenkins build %s/%s", self.name(), self.number())
            self.print_json(self._notification)

            if self.phase() == "STARTED":
                logging.info("Jenkins build %s/%d started...", self.name(), self.number())
                if self.name() in self.SUB_JOBS:
                    self.testsuite_started()
                else:
                    if self.name() != "ihu_hourly" and self.name() != "ihu_daily" and self.name() != "ihu_staging":
                        self.activity_started()

            if self.phase() == "COMPLETED":
                logging.info("Jenkins build %s/%d finished!", self.name(), self.number())
                if self.name() in self.SUB_JOBS:
                    self.testsuite_finished()
                else:
                    self.activity_finished()

    def activity_started(self):
        data = {
            "chain_id": self.chain_id(),
            "name": self.name(),
            "url": self.url(),
            "logs":self.logs(),
            "target": self.target()
        }
        url = self.VCC_CI_API_URL + "/ActivityStarted"
        logging.info("POST request to VCC-CI - %s", url)
        self.print_json(data)
        if self.send_data():
            r = requests.post(url, data=json.dumps(data), headers=self.headers())
            logging.debug(r.status_code)
            logging.debug(r.text)

    def activity_finished(self):
        data = {
            "chain_id": self.chain_id(),
            "name": self.name(),
            "url": self.url(),
            "logs":self.logs(),
            "target": self.target(),
            "status": self.status().lower()
        }
        url = self.VCC_CI_API_URL + "/ActivityFinished"
        logging.info("POST request to VCC-CI - %s", url)
        self.print_json(data)
        if self.send_data():
            r = requests.post(url, data=json.dumps(data), headers=self.headers())
            logging.debug(r.status_code)
            logging.debug(r.text)

    def testsuite_started(self):
        data = {
            "chain_id": self.chain_id(),
            "name": self.name(),
            "activity": self.activity(),
            "url": self.url(),
            "logs":self.logs(),
            "target": self.target()
        }
        url = self.VCC_CI_API_URL + "/TestsuiteStarted"
        logging.info("POST request to VCC-CI - %s", url)
        self.print_json(data)
        if self.send_data():
            r = requests.post(url, data=json.dumps(data), headers=self.headers())
            logging.debug(r.status_code)
            logging.debug(r.text)

    def testsuite_finished(self):
        data = {
            "chain_id": self.chain_id(),
            "name": self.name(),
            "activity": self.activity(),
            "url": self.url(),
            "logs":self.logs(),
            "target": self.target(),
            "status": self.status().lower()
        }
        url = self.VCC_CI_API_URL + "/TestsuiteFinished"
        logging.info("POST request to VCC-CI - %s", url)
        self.print_json(data)
        if self.send_data():
            r = requests.post(url, data=json.dumps(data), headers=self.headers())
            logging.debug(r.status_code)
            logging.debug(r.text)

    def send_data(self):
        if "DEBUG" in os.environ:
            if os.environ['DEBUG'] == "1":
                logging.debug("Note! - No data sent to VCC-CI Vis in debug mode")
                return False
        return True

    def headers(self):
        return {
            'User-Agent': 'jenkins_notification_server.py (Magnus Larsson <magnus.larsson.5@volvocars.com>)',
            'Content-Type': 'application/json'
        }

    def phase(self):
        return self._notification["build"]["phase"]

    def status(self):
        return self._notification["build"]["status"]

    def name(self):
        return self._notification["name"]

    def activity(self):
        return re.compile(r'(ihu_hourly|ihu_daily|ihu_staging)').search(self.name()).group(1)

    def number(self):
        return self._notification["build"]["number"]

    def url(self):
        return self._notification["build"]["full_url"]

    def target(self):
        return "VCC-CI"

    def logs(self):
        logs = []
        logs.append(self.url() + "consoleFull")
        if self.phase() == "COMPLETED":
            logs.append("%s/tests?top_job_name=%s&build_number=%s" % (
                self.TEST_REPORT_URL,
                self.name(),
                self.number()))
        return logs

    def jenkins_build(self, name, number):
        build = self._jenkins.build(name, number)
        self.print_json(build)
        if "changeSet" in build:
            if "items" in build["changeSet"]:
                if len(build["changeSet"]["items"]) == 0:
                    return None
                item = build["changeSet"]["items"][-1]
                if "id" in item:
                    key = "icup_android.gerrit.commit_id.%s.change_id" % item["id"]
                    return RedisClient().get(key)

    def chain_id(self):
        if self.name() == "ihu_hourly" or self.name() == "ihu_daily":
            logging.info("Get build from Jenkins - %s/%s", self.name(), self.number())
            chain_id = self.jenkins_build(self.name(), self.number())
            if chain_id is not None:
                return chain_id

        if "parameters" in self._notification["build"]:
            parameters = self._notification["build"]["parameters"]

            if "GERRIT_CHANGE_ID" in parameters:
                return parameters["GERRIT_CHANGE_ID"]

            if "ZUUL_CHANGE" in parameters:
                key = "icup_android.gerrit.change_number.%s.change_id" % parameters["ZUUL_CHANGE"]
                return RedisClient().get(key)

            if "UPSTREAM_JOB_GIT_REVISION" in parameters:
                key = "icup_android.gerrit.commit_id.%s.change_id" % parameters["UPSTREAM_JOB_GIT_REVISION"]
                return RedisClient().get(key)

        if "commit" in self._notification["build"]["scm"]:
            commit = self._notification["build"]["scm"]["commit"]
            key = "icup_android.gerrit.commit_id.%s.change_id" % commit
            return RedisClient().get(key)

    def print_json(self, data):
        logging.info(json.dumps(data,
                                sort_keys=True,
                                indent=4,
                                separators=(',', ': ')))

def create_logger():
    level = logging.DEBUG
    logging.basicConfig(format='%(asctime)s %(levelname)s %(message)s',
                        level=level)
    requests_log = logging.getLogger("requests.packages.urllib3")
    requests_log.setLevel(logging.DEBUG)
    requests_log.propagate = True

# Main App
create_logger()
app = Flask(__name__)

@app.route("/events", methods=['POST', 'GET'])
def events():
    jenkins_client = JenkinsClient(
        username=os.environ['USER'],
        password=os.environ['JENKINS_API_KEY'])

    if request.method == 'POST':
        try:
            logging.debug("Notification received for Jenkins build %s...", request.get_json()["build"]["url"])
            vccci = VCCCIProxy(jenkins_client)
            vccci.process_notification(request.get_json())
        except TypeError as e:
            logging.debug("Unsupported message - \"%s\"" % e)
            return json.dumps({"error": "Unsupported message"}), 400, {'content-type': 'application/json'}

    return json.dumps({}), 200, {'content-type':'application/json'}

