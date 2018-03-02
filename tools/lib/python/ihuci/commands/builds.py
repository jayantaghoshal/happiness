# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""Handle the list command"""
from tabulate import tabulate
from termcolor import colored
from time import strftime
import sys
import json
import re
from .base import BaseCommand
from ..jenkins_client import JenkinsClient
from ..jenkins_build import JenkinsBuild

class BuildsCommand(BaseCommand):

    def __init__(self, options):
        super().__init__(options)
        self._options = options
        self._jenkins_client = None

    def run(self):
        if self._options.password is None:
            sys.exit("Jenkins password (API-KEY) is not set")

        if self._options.job is None:
            ##
            # List Jenkins jobs
            #
            self.list_jobs()
        else:
            if self._options.build is None:
                ##
                # List Jenkins builds
                #
                self.list_builds()
            else:
                ##
                # Show build info
                #
                self.print_build()

    def list_jobs(self):
        print("Listing jobs in view \"IHU Android CI\"")
        for job in sorted(self._jenkins().jobs("IHU Android CI")):
            print(job)

    def list_builds(self):
        builds = self._jenkins().builds(self._options.job, max=self._options.max)
        table = self._create_table(builds)
        print(tabulate(table,
                       headers=self._headers(),
                       tablefmt="presto"))

    def print_build(self):
        build = self._jenkins().build(self._options.job, self._options.build)
        self._print_build(build)

    def _create_table(self, builds):
        table = []
        for _build in builds:
            build = JenkinsBuild.create(_build)
            if build.result == "FAILURE":
                build.result = colored(build.result, 'red')
            elif build.result == "ABORTED":
                build.result = colored(build.result, 'yellow')
            elif build.result == "PENDING":
                build.result = colored(build.result, attrs=['bold'])
            table.append(self._row(build))
        return table

    def _row(self, build):
        row = []
        if re.match("icup_android_manifest_bump", self._options.job) is not None:
            # Manifest Bump
            row = [build.number,
                   build.time,
                   build.result,
                   build.repo,
                   build.branch,
                   build.commit_id,
                   build.author,
                   build.message]
        elif re.match("ihu_commit_check", self._options.job) is not None:
            # Commit Check jobs
            row = [build.number,
                   build.time,
                   build.result,
                   build.host,
                   build.duration,
                   build.repo,
                   build.branch,
                   build.commit_id,
                   build.zuul_change_ids]
        elif re.match("ihu_gate_\w+", self._options.job) is not None:
            # Commit Gate jobs
            row = [build.number,
                   build.time,
                   build.result,
                   build.host,
                   build.duration,
                   build.repo,
                   build.branch,
                   build.commit_id,
                   build.zuul_change_ids]
        elif re.match("ihu_\w+_test", self._options.job) is not None:
            # Hourly, Daily sub jobs
            row = [build.number,
                   build.time,
                   build.result,
                   build.host,
                   build.duration]
        else:
            # Hourly, Daily top jobs
            row = [build.number,
                   build.time,
                   build.result,
                   build.duration,
                   build.commit_id,
                   build.message]
        return row

    def _headers(self):
        headers = []
        if re.match("icup_android_manifest_bump.*", self._options.job) is not None:
            # Manifest Bump
            headers = ["ID", "Time", "Result", "Repo", "Branch", "Commit ID", "Author", "Message"]
        elif re.match("ihu_commit_check", self._options.job) is not None:
            # Commit Check jobs
            headers = ["ID", "Time", "Result", "Host", "Duration", "Repo", "Branch", "Zuul Commit ID", "ZUUL Change IDs"]
        elif re.match("ihu_gate_\w+", self._options.job) is not None:
            # Commit Gate jobs
            headers = ["ID", "Time", "Result", "Host", "Duration", "Repo", "Branch", "Zuul Commit ID", "ZUUL Change IDs"]
        elif re.match("ihu_\w+_test", self._options.job) is not None:
            # Hourly, Daily sub jobs
            headers = ["ID", "Time", "Result", "Host", "Duration"]
        else:
            # Hourly, Daily top jobs
            headers = ["ID", "Time", "Result", "Duration", "Commit ID", "Commit Message"]
        return headers

    def _jenkins(self):
        if self._jenkins_client is None:
            self._jenkins_client = JenkinsClient(username=self._options.username, password=self._options.password)
        return self._jenkins_client

    def _print_build(self, job):
        print(json.dumps(job,
                         sort_keys=True,
                         indent=4,
                         separators=(',', ': ')))
