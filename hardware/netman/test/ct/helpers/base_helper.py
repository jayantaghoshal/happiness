#!/usr/bin/env python

import logging
import os

from vts.runners.host import const

class BaseHelper:

    def __init__(self, terminal):
        self.terminal = terminal

    def execute_cmd(self, cmd):
        output = self.terminal.Execute(cmd)
        return output
