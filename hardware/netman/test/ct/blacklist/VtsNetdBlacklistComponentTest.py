#!/usr/bin/env python

import logging
import os
import time
import errno

from vts.runners.host import asserts
from vts.runners.host import utils
from vts.runners.host import base_test
from vts.runners.host import errors
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.controllers import adb
from vts.utils.python.precondition import precondition_utils
from subprocess import check_output

import blacklist_helpers as bh

    ## -------------------------------------------------
    ## --/ Android interfaces access blacklist Tests /--
    ## -------------------------------------------------


class VtsNetdBlacklistComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.helper = bh.BlacklistHelper()
        # Attempt to clear logcat, remove and reinstall app to guarentee updated version
        logging.info(check_output("adb logcat -c", shell=True))
        self.helper.silentAppUninstall("com.volvocars.interfacetest")
        logging.info(check_output(["adb", "install", "DATA/ihuInterfaceTestAPK.apk"]))

    def setUp(self):
        self.terminal = None
        self.terminal = self._invoke_terminal()

    def test_Blacklist_Disabled(self):
        # Arrange
        logFilePath = "/tmp/logcatoutput1%d.txt" %os.getpid()
        logcatFilter = "BlacklistD"
        # Each method requires unique logcat filter since we cannot trust that logcat is cleared
        self.helper.startAdbLogcat(logFilePath, logcatFilter)
        # terminate app if already running and clear old data
        logging.info(check_output('adb shell pm clear com.volvocars.interfacetest', shell=True))

        # Act
        logging.info(check_output('adb shell am start -n com.volvocars.interfacete'
                     'st/.MainActivity --es \"caller\" \"%s\"'%logcatFilter, shell=True))
        # Not very elegant but app invokation returns immidiately with result. The App requires
        # some runtime to query and log visible interfaces to logcat
        time.sleep(5)
        interfaces = self.helper.readTestResult(logFilePath)
        # self.helper.printInterfaces(interfaces)
        self.helper.cleanUp(logFilePath)

        #Assert
        #IHU should without any blacklist have 4 interfaces (TODO: add future ifaces!)
        asserts.assertTrue('eth0' in interfaces, 'eth0 interface not in ifaceList!')
        asserts.assertTrue('eth1' in interfaces, 'eth1 interface not in ifaceList!')
        asserts.assertTrue('lo' in interfaces, 'lo interface not in ifaceList!')
        asserts.assertTrue('sit0' in interfaces, 'sit0 interface not in ifaceList!')
        asserts.assertEqual(4,len(interfaces))

    def test_Blacklist_Active(self):
        # Arrange
        # TODO:Activate blacklist functionality on target
        logFilePath = "/tmp/logcatoutput2%d.txt"%os.getpid()
        logcatFilter = "BlacklistE"
        # Each method requires unique logcat filter since we cannot trust that logcat is cleared
        self.helper.startAdbLogcat(logFilePath, logcatFilter)
        # terminate app if already running and clear old data
        logging.info(check_output('adb shell pm clear com.volvocars.interfacetest', shell=True))

        # Act
        logging.info(check_output('adb shell am start -n com.volvocars.interfacete'
                     'st/.MainActivity --es \"caller\" \"%s\"'%logcatFilter, shell=True))
        # Not very elegant but app invokation returns immidiately with result. The App requires
        # some runtime to query and log visible interfaces to logcat
        time.sleep(5)
        interfaces = self.helper.readTestResult(logFilePath)
        # self.helper.printInterfaces(interfaces)
        self.helper.cleanUp(logFilePath)

        # Assert
        # IHU should with active Blacklisting not expose eth0/eth1 but lo/sit0 should remain.
        asserts.assertFalse('eth0' in interfaces, 'eth0 should not be in ifaceList!')
        asserts.assertFalse('eth1' in interfaces, 'eth1 should not be in ifaceList!')
        asserts.assertTrue('lo' in interfaces, 'lo interface not in ifaceList!')
        asserts.assertTrue('sit0' in interfaces, 'sit0 interface not in ifaceList!')

    def _invoke_terminal(self, name="test_terminal"):
        self.dut.shell.InvokeTerminal(name)
        return getattr(self.dut.shell, name)

if __name__ == "__main__":
    test_runner.main()
