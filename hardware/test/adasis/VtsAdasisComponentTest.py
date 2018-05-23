#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
import os

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.runners.host import const
from vts.utils.python.controllers import android_device
import logging
import logging.config
sys.path.append('/usr/local/lib/python2.7/dist-packages')

import audio.alsarecorder
import audio.audioutils
import audio.match

import vehiclehalcommon
from generated import datatypes as de_types

class VtsAdasisComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        self.logger = logging.getLogger('Test')
        self.logger.setLevel(logging.DEBUG)
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)
        self.logger.addHandler(ch)

        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")

        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

        self.flexray = vehiclehalcommon.get_dataelements_connection(self.dut.adb)
        self.host_folder = '/tmp'

    def tearDownClass(self):
        try:
            self.flexray.close()
        except Exception:
            pass

    def setUp(self):
        pass

    def tearDown(self):
        pass

    # ----------------------------------------------------------------------------------------------------------
    # Tests
    # ----------------------------------------------------------------------------------------------------------
    def testSkeletonTest(self):
        self.logger.info("Running skeleton test")
        pass


if __name__ == "__main__":
    logging.basicConfig(
        level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()
