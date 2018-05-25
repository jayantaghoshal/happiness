#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging

import re

from vts.runners.host import const

from volvocars.hardware.netman.test.ct.helpers.execution_context import (ExecutionContext,
                                                                         NamespaceExecutionContext)

class PingTest(object):

    def __init__(self, execution_context):
        self.execution_context = execution_context
        self.is_destination_reachable = False

    def ping(self, destination):
        cmd_line = self._build_cmd_line(destination)
        output = self.execution_context.execute(cmd_line)
        self._parse_result(output[const.STDOUT])

    def _build_cmd_line(self, target):
        return " ".join(["ping", "-c 5", "-W 1", target])

    def _parse_result(self, output):
        reg_exp = r"time=([0-9.]+) ms"
        groups = re.search(reg_exp, str(output))
        if groups:
            logging.info("REACHABLE")
            self.is_destination_reachable = True
        else:
            logging.info("NOT REACHABLE")
            self.is_destination_reachable = False
