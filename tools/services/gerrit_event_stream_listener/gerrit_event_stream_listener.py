#!/usr/bin/env python3

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from jenkins import Jenkins
import paramiko
import gerritlib.gerrit
import redis
import requests
import traceback
import os
import sys
import json
import logging
import datetime
import urllib.parse
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


class GerritStream(object):

    def __init__(self, user, host, thread=True, port=29418, keyfile=None):
        self.gerrit = gerritlib.gerrit.Gerrit(host, user, port)
        if thread:
            self.gerrit.startWatching()

    def get_event(self):
        return self.gerrit.getEvent()


class VCCCIProxy(object):
    VCC_CI_API_URL="http://cimb.volvocars.biz/api/1.6.0"

    def __init__(self, jenkins):
        self.jenkins = jenkins

    def process_event(self, event):
        self._event = event
        try:
            logging.info("Event %s for project %s received from Gerrit",
                self.event_type(),
                self.project())

            if self.supported_event():
                if(self.event_type() == "patchset-created"):
                    self.print_json(event)
                    self.chain_id_mapping()
                    self.source_created()
                elif(self.event_type() == "change-abandoned"):
                    self.print_json(event)
                    self.source_abandoned()
                elif(self.event_type() == "change-merged"):
                    self.print_json(event)
                    self.chain_id_mapping()
                    if self.project() == "manifest":
                        self.baseline_created()
                    else:
                        self.source_published()
        except Exception:
                exc_type, exc_value, exc_traceback = sys.exc_info()
                logging.error(repr(traceback.format_exception(exc_type,
                                                          exc_value,
                                                          exc_traceback)))

    def supported_event(self):
        supported_projects = [
            "manifest",
            "vendor/volvocars"
        ]
        if self.project() in supported_projects:
            if self.branch() == "master":
                return True
        return False

    def source_created(self):
        data = {
            "chain_id": self.change_id(),
            "url": self.url(),
            "author_id": self.author(),
            "repository": "ihu-%s" % urllib.parse.quote_plus(self.project()),
            "commit_id": self.commit_id(),
            "commit_msg": self.commit_message(),
            "target": "VCC-CI"
        }
        url = self.VCC_CI_API_URL + "/SourceCreated"
        logging.info("POST request to %s", url)
        self.print_json(data)
        if self.send_data():
            r = requests.post(url, data=json.dumps(data), headers=self.headers())
            logging.info(r.status_code)
            logging.info(r.text)

    def source_abandoned(self):
        data = {
            "chain_id": self.change_id(),
            "repository": "ihu-%s" % urllib.parse.quote_plus(self.project()),
            "target": "VCC-CI"
        }
        url = self.VCC_CI_API_URL + "/SourceAbandoned"
        logging.info("POST request to %s", url)
        self.print_json(data)
        if self.send_data():
            r = requests.post(url, data=json.dumps(data), headers=self.headers())
            logging.info(r.status_code)
            logging.info(r.text)

    def source_published(self):
        data = {
            "chain_id": self.change_id(),
            "url": self.url(),
            "author_id": self.author(),
            "repository": "ihu-%s" % urllib.parse.quote_plus(self.project()),
            "commit_id": self.commit_id(),
            "commit_msg": self.commit_message(),
            "merge_time": self.merge_time(),
            "target": "VCC-CI"
        }
        url = self.VCC_CI_API_URL + "/SourcePublished"
        logging.info("POST request to %s", url)
        self.print_json(data)
        if self.send_data():
            r = requests.post(url, data=json.dumps(data), headers=self.headers())
            logging.info(r.status_code)
            logging.info(r.text)

    def baseline_created(self):
        try:
            logging.info("Extract data from manifest-bump build in Jenkins")
            p = re.compile(r'/job/(icup_android_manifest_bump)/(\d+)/')
            m = p.search(self.commit_message())
            build = jenkins_client.build(m.group(1), int(m.group(2)))
            upstream_chain_id = ""

            for action in build["actions"]:
                if "parameters" in action:
                    for parameter in action["parameters"]:
                        if (parameter["name"] == "GERRIT_CHANGE_ID"):
                            upstream_chain_id = parameter["value"]

            data = {
                "chain_id": self.change_id(),
                "url": self.url(),
                "input_chain_ids": [upstream_chain_id],
                "author_id": self.author(),
                "baseline_name": "ihu-manifest",
                "type": "module",
                "baseline": "ihu",
                "revision": self.commit_id(),
                "project": "ihu-manifest" ,
                "target": "VCC-CI"
            }
            url = self.VCC_CI_API_URL + "/BaselineCreated"
            logging.info("POST request to %s", url)
            self.print_json(data)
            if self.send_data():
                r = requests.post(url, data=json.dumps(data), headers=self.headers())
                logging.info(r.status_code)
                logging.info(r.text)
        except Exception as err:
            logging.error(err)

    def project(self):
        if "project" in self._event:
            return self._event["project"]
        if "refUpdate" in self._event:
            if "project" in self._event["refUpdate"]:
                return self._event["refUpdate"]["project"]
        return ""

    def branch(self):
        return self._event["change"]["branch"]

    def event_type(self):
        return self._event["type"]

    def commit_message(self):
        return self._event["change"]["commitMessage"]

    def commit_id(self):
        if "newRev" in self._event:
            return self._event["newRev"]
        else:
            return self._event["patchSet"]["revision"]

    def change_id(self):
        return self._event["changeKey"]["id"]

    def change_number(self):
        return self._event["change"]["number"]

    def email(self):
        return self._event["change"]["owner"]["email"]

    def username(self):
        return self._event["change"]["owner"]["username"]

    def author(self):
        return self.username()

    def url(self):
        return self._event["change"]["url"]

    def merge_time(self):
        return datetime.datetime.fromtimestamp(self._event["eventCreatedOn"]).strftime("%Y-%m-%dT%H:%M:%SZ")

    def chain_id_mapping(self):
        key = "icup_android.gerrit.commit_id.%s.change_id" % self.commit_id()
        value = self.change_id()
        logging.info("Redis: %s = %s", key, value)
        if not RedisClient().set(key, value):
            logging.error("ERROR: Failed to SET %s=%s", key, value)

        key = "icup_android.gerrit.change_number.%s.change_id" % self.change_number()
        logging.info("Redis: %s = %s", key, value)
        if not RedisClient().set(key, value):
            logging.error("ERROR: Failed to SET %s=%s", key, value)

    def headers(self):
        return {
            'User-Agent': 'gerrit_event_stream_listener.py (Magnus Larsson <magnus.larsson.5@volvocars.com>)',
            'Content-Type': 'application/json'
        }

    def send_data(self):
        if ("DEBUG" in os.environ) and (os.environ['DEBUG'] == "1"):
            logging.debug("Note! - No data sent to VCC-CI Vis in debug mode")
            return False
        return True

    def print_json(self, data):
        logging.info(json.dumps(data,
                                sort_keys=True,
                                indent=4,
                                separators=(',', ': ')))

def create_logger():
    level=logging.INFO
    if ("DEBUG" in os.environ) and (os.environ['DEBUG'] == "1"):
        level = logging.DEBUG

    logging.basicConfig(format='%(asctime)s %(levelname)s %(message)s',
                        level=level)
    requests_log = logging.getLogger("requests.packages.urllib3")
    requests_log.setLevel(level)
    requests_log.propagate = True

    gerrit_log = logging.getLogger("gerrit.GerritWatcher")
    gerrit_log.setLevel(level)
    gerrit_log.propagate = True

# Main App

create_logger()

gerrit = GerritStream(host="gotsvl1415.got.volvocars.net",
                      user=os.environ['GERRIT_USER'],
                      port=29421,
                      keyfile=None)

jenkins_client = JenkinsClient(
        username=os.environ['GERRIT_USER'],
        password=os.environ['JENKINS_API_KEY'])

vccci = VCCCIProxy(jenkins_client)

while True:
    vccci.process_event(gerrit.get_event())
