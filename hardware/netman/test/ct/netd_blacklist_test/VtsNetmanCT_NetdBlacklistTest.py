#!/usr/bin/env python

import logging
import re

from subprocess import call, check_output, CalledProcessError

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.precondition import precondition_utils

class VtsNetmanCT_NetdBlacklistTest(base_test.BaseTestClass):

    def execute_command(self, cmd):
        output = self.terminal.Execute(cmd)
        return output[const.EXIT_CODE][0]

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("test_terminal")
        self.terminal = getattr(self.dut.shell, "test_terminal")

    def tearDown(self):
        # To restore the state when we're done
        self.execute_command("netboyd&")

    def setup_test(self, iface):
        self.execute_command("logcat -c")

        self.execute_command("/data/netd/netd_blacklist_test&")
        logging.info("Test client started")

        logging.info("Setup complete")
        return True

    def run_test(self, iface):
        logging.info("Running test")
        retval = self.execute_command("ifconfig " + iface + " up")

        return retval == 0

    def is_interface_up(self, iface):
        # Checks logcat for the log message of the test client.
        # Returns whether the interface message existed in the log otput or not
        logging.info("Checking logs")
        text = ""
        try:
            text = check_output("adb logcat -d | grep NetdSocketTest", shell=True, universal_newlines=True)
        except CalledProcessError as err:
            text = err.output

        res = re.search('(?<=MESSAGE: ).*', text)
        if res is None:
            return False

        res = re.search(iface, res.group(0))

        return res != None

    def start_test(self, iface, isBlacklisted):
        if not self.setup_test(iface):
            logging.info("Setup failed")
            return False
        if not self.run_test(iface):
            logging.info("Running failed")
            return False
        if not self.is_interface_up(iface):
            # If it should be blacklisted, False is actually considered success here so return True
            return False ^ isBlacklisted
        return True

    def reboot_target(self):
        self.dut.stopServices() #Important for startServices() to work further down
        self.dut.reboot()
        self.dut.waitForBootCompletion()

        self.dut.startServices()
        self.dut.shell.InvokeTerminal("test_terminal")
        self.terminal = getattr(self.dut.shell, "test_terminal")

    def testNetd_NoBlacklisted(self):
        self.execute_command("cp /data/local/tmp/netd_blacklist_config_empty_blacklist.xml /data/netd/netd_config.xml")

        self.reboot_target()
        self.execute_command("pkill -f netboyd")

        retval = self.start_test("eth0", False)
        self.execute_command("ifconfig eth0 down")
        asserts.assertEqual(retval, True)

    def testNetd_Blacklisted(self):
        # Blacklisted, expect no log entry
        self.execute_command("cp /data/local/tmp/netd_blacklist_config_blacklist.xml /data/netd/netd_config.xml")

        self.reboot_target()
        self.execute_command("pkill -f netboyd")

        retval = self.start_test("eth0", True)
        self.execute_command("ifconfig eth0 down")
        asserts.assertEqual(retval, True)

if __name__ == "__main__":
    test_runner.main()
