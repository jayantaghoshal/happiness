# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from datetime import datetime
from jenkins import Jenkins
from urllib.request import urlopen
import json

class JenkinsClient(object):

    def __init__(self, username, password):
        self._server = None
        self._username = username
        self._password = password

    def jobs(self, view=None):
        jobs = self._jenkins().get_jobs(view_name=view)
        return [job['name'] for job in jobs]

    def builds(self, name, max=10):
        job_list = self._jenkins().get_job_info(name)['builds']
        return [self._jenkins().get_build_info(name, job["number"]) for job in job_list[0:max]]

    def build(self, job, build):
        return self._jenkins().get_build_info(job, build)

    def nodes(self):
        return self._jenkins().get_nodes()

    def node_info(self, name):
        if not self._jenkins().node_exists(name):
            return None
        return self._jenkins().get_node_info(name)

    def node_config(self, name):
        if not self._jenkins().node_exists(name):
            return None
        return self._jenkins().get_node_config(name)

    def _jenkins(self):
        if self._server is None:
            self._server = Jenkins('https://icup_android.jenkins.cm.volvocars.biz',
                                   username=self._username,
                                   password=self._password)
        return self._server

    def print_item(self, job):
        print(json.dumps(job,
                         sort_keys=True,
                         indent=4,
                         separators=(',', ': ')))


