#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
import os

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.precondition import precondition_utils
from subprocess import call

sys.path.append('/usr/local/lib/python2.7/dist-packages')

from fdx import fdx_client
from fdx import fdx_description_file_parser


from generated.pyDataElements import \
    FrSignalInterface, \
    DrvrHmiSpdLimAdpnSts, \
    AccAdprSpdLimActvSts

from vehiclehalcommon import wait_for_signal


timeout = 3

class VtsSpeedLimitAdaptationComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        self.flexray = FrSignalInterface()

    def deviceReboot(self):
        self.dut.shell.one.Execute("reboot")
        self.dut.stopServices()
        self.dut.waitForBootCompletion()
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")

    # ----------------------------------------------------------------------------------------------------------
    # Car Config Unavailable
    # ----------------------------------------------------------------------------------------------------------
    def testSpeedLimitAdaptationCCDisabled(self):
        fr = FrSignalInterface()

        # Change Car Config 149 to 2, Car Config 36 to 2 and Car Config 23 to 3
        self.dut.shell.one.Execute("changecarconfig 149 2")
        self.dut.shell.one.Execute("changecarconfig 36 2")
        self.dut.shell.one.Execute("changecarconfig 23 3")
        self.deviceReboot()

        fr.DrvrHmiSpdLimAdpnSts.send(DrvrHmiSpdLimAdpnSts.map.On)
        wait_for_signal(fr,fr.AccAdprSpdLimActvSts, fr.AccAdprSpdLimActvSts.map.Off,timeout)

    # ----------------------------------------------------------------------------------------------------------
    # Car Config Available
    # ----------------------------------------------------------------------------------------------------------
    def testSpeedLimitAdaptationCCEnabled(self):
        fr = FrSignalInterface()

        # Change Car Config 149 to 2, Car Config 36 to 3 and Car Config 23 to 1
        self.dut.shell.one.Execute("changecarconfig 149 2")
        self.dut.shell.one.Execute("changecarconfig 36 3")
        self.dut.shell.one.Execute("changecarconfig 23 1")
        self.deviceReboot()

        # ----------------------------------------------------------------------------------------------------------
        # Test DrvrHmiSpdLimnAdpnSts off
        # ----------------------------------------------------------------------------------------------------------

        fr.DrvrHmiSpdLimAdpnSts.send(DrvrHmiSpdLimAdpnSts.map.Off)
        wait_for_signal(fr,fr.AccAdprSpdLimActvSts, fr.AccAdprSpdLimActvSts.map.Off,timeout)

        # ----------------------------------------------------------------------------------------------------------
        # Test DrvrHmiSpdLimnAdpnSts on
        # ----------------------------------------------------------------------------------------------------------

        fr.DrvrHmiSpdLimAdpnSts.send(DrvrHmiSpdLimAdpnSts.map.On)
        wait_for_signal(fr,fr.AccAdprSpdLimActvSts, fr.AccAdprSpdLimActvSts.map.On,timeout)

if __name__ == "__main__":
    test_runner.main()
