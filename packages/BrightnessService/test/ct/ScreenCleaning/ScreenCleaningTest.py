#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
sys.path.append("/usr/local/lib/python2.7/dist-packages")
import logging
import re
from time import sleep, time
import serial #type: ignore
from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.runners.host import const

class ComponentTest(base_test.BaseTestClass):

    def setUp(self):

        self.logger = logging.getLogger("TestScreenCleaningMode")
        self.vip_serial = serial.Serial("/dev/ttyUSB0",115200, timeout=1000)
        self.vip_serial.write("\r\n") #clean console.

        if not self.verify_serial_is_vip_app():
            self.logger.info("swap tty")
            self.vip_serial.close()
            self.vip_serial = serial.Serial("/dev/ttyUSB1",115200, timeout=1000)
            self.vip_serial.write("\r\n") #clean console.
            asserts.assertEqual(self.verify_serial_is_vip_app(),True,"This should be vip_tty")

        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")

    def verify_serial_is_vip_app(self, timeout_sec=5):
        self.vip_serial.write("version \r\n")
        stop_time = time() + timeout_sec
        while time() < stop_time:
            line = self.vip_serial.readline()
            self.logger.info(line)
            if not line:
                continue
            if re.match(r"\s*Project_ID.*VCC_IHU.*VIP", line) is not None:
                return True
            if re.match(r".*/sh: version: not found", line) is not None:
                return False
        return False


    def tearDown(self):
        self.vip_serial.close()

    def pressHomeButton(self):
        press = "hisip msgh MP 0x01 0x02 0x04 0x05 0x04 0x02 0x0 0x0 0x0 0x17 0x81 0xff \r\n" #PressButton
        press += "hisip msgh MP 0x01 0x02 0x04 0x05 0x04 0x02 0x0 0x0 0x0 0x17 0x01 0xff \r\n" #clearBit
        self.vip_serial.write(press)

    def releaseHomeButton(self):
        release = "hisip msgh MP 0x01 0x02 0x04 0x05 0x04 0x02 0x0 0x0 0x0 0x17 0x41 0xff \r\n" #releaseButton
        release += "hisip msgh MP 0x01 0x02 0x04 0x05 0x04 0x02 0x0 0x0 0x0 0x17 0x01 0xff \r\n" #clearBit
        self.vip_serial.write(release)

    def testScreenCleaningMode(self):

        getactiveappcmd = 'dumpsys window windows | grep "Window #0"'
        inputswipe = "input touchscreen swipe 0 0 500 500" #can take the top statusbar down.
        inputtap = "input touchscreen tap 384 1000" #can launch climate
        brightnesswindow = "com.volvocars.brightnessservice" #service that runs screencleaning overlay
        launcherwindow = "com.volvocars.launcher"

        #check active app
        logData = self.dut.shell.one.Execute(getactiveappcmd)[const.STDOUT][0]
        self.logger.info(logData)
        asserts.assertEqual(logData.find(brightnesswindow),-1,"Overlay should not be active.")
        asserts.assertNotEqual(logData.find(launcherwindow),-1,"Launcher should be active")

        #Short press should not trigger screen cleaning
        self.pressHomeButton()
        sleep(0.3)
        self.releaseHomeButton()
        asserts.assertEqual(logData.find(brightnesswindow),-1,"Overlay should not be active.")
        asserts.assertNotEqual(logData.find(launcherwindow),-1,"Launcher should be active")

        self.pressHomeButton()
        sleep(3.5)
        self.releaseHomeButton()
        logData = self.dut.shell.one.Execute(getactiveappcmd)[const.STDOUT][0]
        asserts.assertNotEqual(logData.find(brightnesswindow),-1,"Overlay should be active")

        self.dut.shell.one.Execute(inputtap)[const.STDOUT][0]
        sleep(0.5)
        logData = self.dut.shell.one.Execute(getactiveappcmd)[const.STDOUT][0]
        asserts.assertNotEqual(logData.find(brightnesswindow),-1,"Overlay should still be active")

        self.dut.shell.one.Execute(inputswipe)[const.STDOUT][0]
        sleep(0.5)
        logData = self.dut.shell.one.Execute(getactiveappcmd)[const.STDOUT][0]
        asserts.assertNotEqual(logData.find(brightnesswindow),-1,"Overlay should still be active")

        #Short press should not exit screen cleaning
        self.pressHomeButton()
        sleep(0.3)
        self.releaseHomeButton()
        logData = self.dut.shell.one.Execute(getactiveappcmd)[const.STDOUT][0]
        asserts.assertNotEqual(logData.find(brightnesswindow),-1,"Overlay should still be active")

        self.pressHomeButton()
        sleep(3.5)
        self.releaseHomeButton()
        logData = self.dut.shell.one.Execute(getactiveappcmd)[const.STDOUT][0]
        asserts.assertEqual(logData.find(launcherwindow),31,"Launcher should be active")


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG, format='%(asctime)s %(levelname)s %(name)s   %(message)s')
    test_runner.main()