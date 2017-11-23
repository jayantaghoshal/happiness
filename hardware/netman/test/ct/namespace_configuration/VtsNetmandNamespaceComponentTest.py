#!/usr/bin/env python

import logging
import time
import re
import sys
import os

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.precondition import precondition_utils

from volvocars.hardware.netman.test.ct.helpers import netman_helper as nh

class VtsNetmandNamespaceComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()
        self.target = nh.NetmanHelper(self.terminal)

    def test_Vcc_Namespace_Exists(self):
        # Assert
        asserts.assertTrue(self.target.namespace_exists("vcc"), "Namespace vcc does not exist")

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
