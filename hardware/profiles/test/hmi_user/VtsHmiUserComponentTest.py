#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
import time
import re

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

sys.path.append('/usr/local/lib/python2.7/dist-packages')
sys.path.append('/tmp/ihu/.local/lib/python2.7/site-packages')

from vf_common import vehiclehalcommon
from uiautomator import AutomatorDevice

user_creation_timeout = 5;
user_deletion_timeout = 0.1

new_driver_id = "com.volvocars.launcher:id/new_user_profile"
hmi_defined_name = "Sten Olsson"
start_up_user_app ='am start -n \"com.volvocars.launcher/.user.UserProfilesActivity\" -a \"android.intent.action.MAIN\" -c \"android.intent.category.LAUNCHER\"'
hmi_user_profile_list_id = "com.volvocars.launcher:id/user_profile_list"
hmi_user_profile_item_id = "com.volvocars.launcher:id/user_profile_item"
guest_user_test_nr_of_times = 3



class VtsHmiUserComponentTest(base_test.BaseTestClass):

    def setUpClass(self):

        # Init common support library and start up the user application
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

        # Clear the setting database
        self.clearUserProfile()


    def tearDown(self):
        self.clearUserProfile()

    # Reset user profile
    def clearUserProfile(self):
        # Switch to guest for reset
        self.dut.shell.one.Execute("am switch-user 10")

        # Clear the setting database
        self.dut.shell.one.Execute("stop settingstorage-hidl-server")
        self.dut.shell.one.Execute("rm /data/vendor/vehiclesettings/vehiclesettings.db*")
        self.dut.shell.one.Execute("start settingstorage-hidl-server")

        # Remove android users
        result = self.dut.shell.one.Execute("pm list users")
        resultUsers = result['stdouts'][0]
        listUser = re.findall(r'\d+',resultUsers)

        for user in listUser:
            self.dut.shell.one.Execute("pm remove-user " + str(user))
            time.sleep(user_deletion_timeout)


    """ Test Description
    Finds create user button
    Create two users by clicking twice
    Check if two users are created
    Switch to the latest created user
    """
    def testCreateAndSwitchUser(self):
        # Init common support library and start up the user application
        self.deviceReboot()
        self.vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, None, False, True)

        # Setup the environment
        self.vHalCommon.waitUntilUserNotOwner()
        time.sleep(user_creation_timeout)
        self.dut.adb.shell('input keyevent 3')
        self.dut.adb.shell(start_up_user_app)
        time.sleep(1)
        self.vHalCommon.waitUntilViewAvailable(hmi_user_profile_list_id)
        time.sleep(1)

        # Store number of the current users
        list_view_count_first = self.device_def(resourceId=hmi_user_profile_list_id).info['childCount']

        # Get the new driver button and click twice
        new_driver_button = self.device_def(resourceId=new_driver_id)
        new_driver_button.click()
        time.sleep(user_creation_timeout)
        new_driver_button.click()
        time.sleep(user_creation_timeout)
        list_view = self.device_def(resourceId=hmi_user_profile_list_id)
        list_view_count_after = list_view.info['childCount']

        # Compare if a new user is added
        asserts.assertLess(list_view_count_first, list_view_count_after,
                           "Nothing changed after adding a user in the HMI")

        # Check if Pre defined name is exist
        asserts.assertTrue(list_view.child(text=hmi_defined_name).exists, "A new driver not added")

        time.sleep(1)

        # Switch to the defined user
        asserts.assertTrue(list_view.child(text=hmi_defined_name).click(), "User switch click didn't work")

        # Open up the userswitch
        self.vHalCommon.waitUntilViewNotAvailable(hmi_user_profile_list_id)
        self.dut.adb.shell('input keyevent 3')
        time.sleep(1)
        self.dut.adb.shell(start_up_user_app)
        time.sleep(1)
        self.vHalCommon.waitUntilViewAvailable(hmi_user_profile_list_id)
        time.sleep(1)

        # Check If it has switched a user
        list_view = self.device_def(resourceId=hmi_user_profile_list_id)
        user_switched = list_view.child(resourceId=hmi_user_profile_item_id,
                                        instance=0).child(text=hmi_defined_name).exists
        asserts.assertTrue(user_switched, "User is not switched")


    # Check guest user switching
    def testGuestUser(self):

        for _ in range(guest_user_test_nr_of_times):
            # Reset self.device_def
            self.deviceReboot()

            # Init common support library and start up the user application
            self.vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, None, False, True)

            # Check if user is switched
            self.vHalCommon.waitUntilUserNotOwner(True)

    def deviceReboot(self):
        time.sleep(1)
        self.dut.shell.one.Execute("reboot")
        self.dut.stopServices()
        self.dut.waitForBootCompletion()
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")
        self.device_def = AutomatorDevice();

if __name__ == "__main__":
    test_runner.main()
