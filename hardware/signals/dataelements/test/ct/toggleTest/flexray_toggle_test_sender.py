#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import threading

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.runners.host import const
import sys
import logging
import time

sys.path.append('/usr/local/lib/python2.7/dist-packages')
from generated.pyDataElements import \
    FrSignalInterface, \
    UsgModSts

logger = logging.getLogger(__name__)

def log(s):
    logger.info(s)

NR_OF_TOGGLES = 400
TOGGLE_DT_MS = 100

INCLUDE_DONT = 0
INCLUDE_BEFORE = 1
INCLUDE_AFTER = 2
INCLUDE_BEFORE_SET_ONLY = 3

class ComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

    def setUp(self):
        self.flexray = FrSignalInterface()

    def tearDown(self):
        try:
            self.shell.Execute("pkill toggle_test_receiver")
            self.flexray.close()
        except Exception as e:
            log("ERROR In teardown: %r" % e)


    def sender_thread(self, include_other_signal):
        time.sleep(3)  # hacky wait for target binary to start
        for i in range(NR_OF_TOGGLES):
            if include_other_signal == INCLUDE_BEFORE:
                self.flexray.AdjSpdLimnWarn.send(i % 2)
            if include_other_signal == INCLUDE_BEFORE_SET_ONLY:
                self.flexray.AdjSpdLimnWarn.set(i % 2)
            self.flexray.AdjSpdLimnSts.send(i % 2 + 1)
            if include_other_signal == INCLUDE_AFTER:
                self.flexray.AdjSpdLimnWarn.send(i % 2)
            time.sleep(TOGGLE_DT_MS / 1000.0)

    def runtest(self, include_other_signal):
        sender_thread = threading.Thread(target=self.sender_thread, args=(include_other_signal, ))
        sender_thread.daemon=True
        sender_thread.start()

        timeout_ms = int((TOGGLE_DT_MS * NR_OF_TOGGLES) * 1.5)
        log("timeout ms is : %r" % timeout_ms )
        retval = self.shell.Execute("/data/nativetest64/flexray_toggle_test_receiver %d %d %d" % (
            int(NR_OF_TOGGLES * 0.8),   # *0.8 because we don't really care about number, only interval between toggles
            TOGGLE_DT_MS,
            timeout_ms
        ))
        exit_code = retval[const.EXIT_CODE][0]
        stdout = retval[const.STDOUT]
        asserts.assertEqual(0, exit_code, "Target program fail. View target logcat for details, tag: FlexrayToggleTest, "
                                          "include_other_signal=%d" % include_other_signal)

    def testToggleAdjSpdLimnSts(self):
        self.runtest(INCLUDE_DONT)

    # These are actually testing the Vector interface and not the IHU, we have reported bug to Vector case CS0108726
    #   In short there is a bug when sending 2 fdx messages for same FDX group in very rapid sequence, then the
    #   update bit might not be set properly in the second message
    #def testToggleAdjSpdLimnStsAndAdjSpdLimnWarn(self):
    #    self.runtest(INCLUDE_BEFORE)
    #def testToggleAdjSpdLimnStsAndAdjSpdLimnWarn_setonly(self):
    #    self.runtest(INCLUDE_BEFORE_SET_ONLY)
    #def testToggleAdjSpdLimnStsAndAdjSpdLimnWarn_after(self):
    #    self.runtest(INCLUDE_AFTER)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()
