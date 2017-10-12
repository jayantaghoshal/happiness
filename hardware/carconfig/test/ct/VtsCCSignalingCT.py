#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.runners.host import const
from vts.utils.python.controllers import android_device
import os
import sys
import re
import logging
import thread, threading
from subprocess import Popen, PIPE
from time import sleep
import time
import csv
sys.path.append('/usr/local/lib/python2.7/dist-packages')
from generated.pyDataElements import \
    FrSignalInterface, \
    UsgModSts

def sendCCParameters(self, event, good=True, sendAll=True):
    VehCfgPrmBlk = self.flexray.VehCfgPrmBlk
    VehCfgPrmVal1 = self.flexray.VehCfgPrmVal1
    VehCfgPrmVal2 = self.flexray.VehCfgPrmVal2
    VehCfgPrmVal3 = self.flexray.VehCfgPrmVal3
    VehCfgPrmVal4 = self.flexray.VehCfgPrmVal4
    VehCfgPrmVal5 = self.flexray.VehCfgPrmVal5
    VehCfgPrmVal6 = self.flexray.VehCfgPrmVal6
    VehCfgPrmVal7 = self.flexray.VehCfgPrmVal7

    for aa in range (1, 600):
        cc_no_of_variables_per_block = 7
        cc_no_of_blocks = 72 #cc_no_of_variables / cc_no_of_variables_per_block

        fp = "/carconfig-default.csv"
        with open(os.path.abspath(os.path.dirname(__file__)) + fp) as csvfile:
            index = 1
            ccfilereader = csv.reader(csvfile, delimiter=',', quotechar='|')
            for row in ccfilereader:

                data = int(row[0])
                block = (index - 1) / 7 + 1
                param = (index - 1) % 7 + 1

                #Hack to inject bad carconfig parameters.
                if good == False:
                    data = 0

                if param == 1:
                    VehCfgPrmVal1.set(data)
                elif param == 2:
                    VehCfgPrmVal2.set(data)
                elif param == 3:
                    VehCfgPrmVal3.set(data)
                elif param == 4:
                    VehCfgPrmVal4.set(data)
                elif param == 5:
                    VehCfgPrmVal5.set(data)
                elif param == 6:
                    VehCfgPrmVal6.set(data)
                else:
                    VehCfgPrmVal7.set(data)
                    VehCfgPrmBlk.send(block)
                    time.sleep(0.1)
                    VehCfgPrmBlk.send(block) #Send twice because sometimes the frames aren't actually sent.
                    time.sleep(0.1)

                    #Used for testing sending incomplete CC parameters.
                    if sendAll == False or event.isSet():
                        return

                index += 1

                if block == cc_no_of_blocks and param == cc_no_of_variables_per_block:
                    return

class ComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one
        self.dut.shell.one.Execute("setenforce 0")  # SELinux pereventmissive mode

    def setUp(self):
        self.dut.stopServices() #Important for startServices() to work further down

        # Starting CANoe simulation might trigger a reboot,
        # Set up required signals for unit to boot and ensure it is booted
        self.flexray = FrSignalInterface()
        self.flexray.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        self.flexray.FRNetworkStatus.send(1)
        self.flexray.VehBattUSysU.send(12)
        self.dut.waitForBootCompletion()

        # After reboot VTS looses the connection to the shell, recreate it
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one

    def testReceivedGoodCarconfig(self):
        # We should have rebooted, check if the carconfig file is in /oem_config/carconfig and that
        # we're in configured state.
        self.RunReceivedCarconfig(True)
        CCfileStatus = self.dut.shell.one.Execute("test -f /oem_config/carconfig/carconfig.csv")['return_codes'][0]
        asserts.assertFalse(CCfileStatus, "Carconfig.csv file not written to /oem_config/carconfig after complete carconfig frames has been received")

        ConfiguredStatus = self.dut.shell.one.Execute("test -f /oem_config/carconfig/CARCONFIG_CONFIGURED")['return_codes'][0]
        asserts.assertFalse(ConfiguredStatus, "Carconfig should be in configured mode but is not")

    def testReceivedBadCarconfig(self):
        # We should have rebooted, check if the carconfig file is in /oem_config/carconfig
        # but we should not be in configured state.
        self.RunReceivedCarconfig(False)
        ConfiguredStatus = self.dut.shell.one.Execute("test -f /oem_config/carconfig/CARCONFIG_CONFIGURED")['return_codes'][0]
        asserts.assertTrue(ConfiguredStatus, "Carconfig should not be in configured mode since we got bad parameters while in BULK state")

    def testReceivedIncompleteCarconfig(self):
        # We should not have rebooted, nor should be store the incomplete carconfig file.
        self.dut.waitForBootCompletion()
        self.dut.shell.one.Execute("rm /oem_config/carconfig/*")
        self.dut.shell.one.Execute("stop carconfig_updater;start carconfig_updater")

        event = threading.Event()
        senderthread= threading.Thread(target=sendCCParameters, args=(self, event, True, False ))
        senderthread.start()
        sleep(30) #Wait for carconfig_updater to time out while waiting for all the cc frames.
        event.set()
        senderthread.join()

        CCfileStatus = self.dut.shell.one.Execute("test -f /oem_config/carconfig/carconfig.csv")['return_codes'][0]
        asserts.assertTrue(CCfileStatus, "Carconfig.csv file not written to /oem_config/carconfig when only received partial carconfig")

    def RunReceivedCarconfig(self, sendOk=True, sendAll=True):
        self.dut.waitForBootCompletion()
        retval = self.dut.adb.devices()
        device = re.findall("(\\S*)\\s*device", retval)[1]
        retval = self.dut.adb.devices()
        device_id = re.findall("(\\S*)\\s*device", retval)[1]

        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("rm /oem_config/carconfig/*")
        self.dut.shell.one.Execute("stop carconfig_updater;start carconfig_updater")

        event = threading.Event()
        senderthread = threading.Thread(target=sendCCParameters, args=(self, event, sendOk, sendAll ))
        senderthread.start()

        end = time.time() + 60 #should be enough time to send parameters and for the ihu to reboot
        self.dut.stopServices()

        while device == device_id:
            asserts.assertTrue(time.time() < end, 'Times up..Not recovered after reboot')
            retval = self.dut.adb.devices()
            try:
                device_id = re.findall("(\\S*)\\s*device", retval)[1]
            except IndexError:
                break
            time.sleep(0.1)

        event.set()
        senderthread.join()

        self.dut.waitForBootCompletion()
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")
        self.shell = self.dut.shell.one

        # Check if the same device as before is rebooted and connected
        retval = self.dut.adb.devices()
        device_id = re.findall("(\\S*)\\s*device", retval)[1]
        while device_id != device:
            retval = self.dut.adb.devices()
            try:
                device_id = re.findall("(\\S*)\\s*device", retval)[1]
            except IndexError:
                device_id = "-1"
            time.sleep(0.1)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()
