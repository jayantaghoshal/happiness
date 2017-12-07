# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#!/usr/bin/env python

import logging

from os.path import join

from vts.runners.host import const

import re

class ExecutionContext(object):

    def __init__(self, terminal):
        self.terminal = terminal

    def execute(self, cmd):
        logging.info("execute cmd: " + cmd)
        return self.terminal.Execute(cmd)

class NamespaceExecutionContext(ExecutionContext):

    def __init__(self, namespace, terminal):
        ExecutionContext.__init__(self, terminal)
        self.namespace = namespace

    def execute(self, cmd):
        formatted_cmd = self._format_cmd_line(cmd)
        logging.info("formatted cmd: " + formatted_cmd)
        return self.terminal.Execute(formatted_cmd)

    def _format_cmd_line(self, cmd):
        return " ".join(["ip", "netns", "exec"] + [self.namespace] + [cmd])
