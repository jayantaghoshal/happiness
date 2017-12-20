#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import os

from vts.runners.host import const

class BaseHelper:

    def __init__(self, terminal):
        self.terminal = terminal

    def execute_cmd(self, cmd):
        output = self.terminal.Execute(cmd)
        return output
