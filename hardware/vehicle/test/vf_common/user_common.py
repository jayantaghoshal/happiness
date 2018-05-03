# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import re
import time
import logging
from vts.runners.host import const
from vts.runners.host import asserts


class UserManagement:
    def __init__(self, dut):
        self.dut = dut


    def waitUntilUserNotOwner(self, timeout_seconds=30):
        logging.info("Waiting until user not owner")

        start_time = time.time()
        while time.time() - start_time <= timeout_seconds:
            result = self.dut.shell.one.Execute("dumpsys activity | grep mUserLru")
            lastUserStr = result[const.STDOUT][0]
            lastUser = re.findall(r'\d+', lastUserStr)[-1]
            if lastUser != '0':
                return
            logging.info("Waiting...%d", (time.time() - start_time))
            time.sleep(2)
        asserts.assertTrue(False, "Time out! User didn't switch")
