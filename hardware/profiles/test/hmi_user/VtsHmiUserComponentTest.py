#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
import time

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

sys.path.append('/usr/local/lib/python2.7/dist-packages')
sys.path.append('/tmp/ihu/.local/lib/python2.7/site-packages')

from vehiclehalcommon import *
import vehiclehalcommon
from uiautomator import device as device

user_creation_timeout = 5;

new_driver_id = "com.volvocars.launcher:id/new_user_profile"
hmi_defined_name = "Sten Olsson"
start_up_user_app ='am start -n \"com.volvocars.launcher/.user.UserProfilesActivity\" -a \"android.intent.action.MAIN\" -c \"android.intent.category.LAUNCHER\"'
hmi_user_profile_list_id = "com.volvocars.launcher:id/user_profile_list"
hmi_user_profile_item_id = "com.volvocars.launcher:id/user_profile_item"


class VtsHmiUserComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        # Init common support library and start up the user application
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        self.vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, None, False, True)
        self.vHalCommon.waitUntilUserNotOwner()
        self.dut.adb.shell('input keyevent 3')
        self.dut.adb.shell(start_up_user_app)
        self.vHalCommon.waitUntilViewAvailable(hmi_user_profile_list_id)

    """ Test Description
    Finds create user button
    Create two users by clicking twice
    Check if two users are created
    Switch to the latest created user
    """
    def testCreateAndSwitchUser(self):
        # Store number of the current users
        list_view_count_first = device(resourceId=hmi_user_profile_list_id).info['childCount']

        # Get the new driver button and click twice
        new_driver_button = device(resourceId=new_driver_id)
        new_driver_button.click()
        time.sleep(user_creation_timeout)
        new_driver_button.click()
        time.sleep(user_creation_timeout)
        list_view = device(resourceId=hmi_user_profile_list_id)
        list_view_count_after = list_view.info['childCount']

        # Compare if a new user is added
        asserts.assertLess(list_view_count_first, list_view_count_after,
                           "Nothing changed after adding a user in the HMI")

        # Check if Pre defined name is exist
        asserts.assertTrue(list_view.child(text=hmi_defined_name).exists, "A new driver not added")

        # Switch to the defined user
        list_view.child(text=hmi_defined_name).click()

        # Open up the userswitch
        self.vHalCommon.waitUntilViewNotAvailable(hmi_user_profile_list_id)
        self.dut.adb.shell('input keyevent 3')
        self.dut.adb.shell(start_up_user_app)
        self.vHalCommon.waitUntilViewAvailable(hmi_user_profile_list_id)

        # Check If it has switched a user
        list_view = device(resourceId=hmi_user_profile_list_id)
        user_switched = list_view.child(resourceId=hmi_user_profile_item_id,
                                        instance=0).child(text=hmi_defined_name).exists
        asserts.assertTrue(user_switched, "User is not switched")


if __name__ == "__main__":
    test_runner.main()
