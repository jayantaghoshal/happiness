# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

"""Handle the list command"""
from tabulate import tabulate
from termcolor import colored
from time import strftime
import sys
import json
import re
import xml.etree.ElementTree as ET
from .base import BaseCommand
from ..jenkins_client import JenkinsClient

class NodesCommand(BaseCommand):

    def __init__(self, options):
        super().__init__(options)
        self._options = options
        self._jenkins_client = None

    def run(self):
        if self._options.password is None:
            sys.exit("Jenkins password (API-KEY) is not set")

        self.list_nodes()

    def list_nodes(self):
        nodes = self._jenkins().nodes()
        table = self._create_table(nodes)
        print(tabulate(table,
                       headers=self._headers(),
                       tablefmt="presto"))

    def _create_table(self, nodes):
        table = []
        for node in nodes:
            node_config = self._jenkins().node_config(node["name"])
            if node_config:
                table.append(self._row(node, node_config))
        return table

    def _row(self, node, node_config):
        name = node["name"]
        status = colored("OFFLINE", 'red') if node["offline"] else "ONLINE"
        root = ET.fromstring(node_config)
        hostname = "-"
        hostname = root.find('launcher').find('host').text
        labels = root.find('label').text or "-"
        num_executors = root.find('numExecutors').text
        return [name, hostname, status, num_executors, labels]

    def _headers(self):
        headers = ["Name", "Hostname", "Status", "Executors", "Labels"]
        return headers

    def _jenkins(self):
        if self._jenkins_client is None:
            self._jenkins_client = JenkinsClient(username=self._options.username, password=self._options.password)
        return self._jenkins_client

    def _print_node(self, node):
        print(json.dumps(node,
                         sort_keys=True,
                         indent=4,
                         separators=(',', ': ')))
