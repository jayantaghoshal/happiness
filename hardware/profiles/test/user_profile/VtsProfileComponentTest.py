#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
import logging

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

sys.path.append('/usr/local/lib/python2.7/dist-packages')

from com.dtmilano.android.viewclient import ViewClient
from com.dtmilano.android.viewclient import ViewNotFoundException

action_time = 1;
user_switch_timeout = 10;
app_context = "com.volvocars.halmodulesink:id/"
us_pair_user_button_id  = app_context + "pairUserButtonCreate"
us_text_log_result_id = app_context + "textLog"
us_get_user_id = app_context + "getUserIdButton"
us_edit_view_id = app_context + "editTextUserId"
us_text_owner_id = app_context + "textOwner"
us_switch_user_button_id = app_context + "pairUserButtonSwitch"
user_id = "VCC"

class CtsProfileComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

    # ----------------------------------------------------------------------------------------------------------
    # Test
    # Create a user
    # Switch to the created user
    # ----------------------------------------------------------------------------------------------------------
    def testCreateAndSwitchUser(self):
        self.setUpUserSwitch()
        vc, device = self.getViewClient()
        vc.dump(window=-1)

        # Write user name on the screen to create
        edit_text_value = vc.findViewByIdOrRaise(us_edit_view_id);
        edit_text_value.setText(user_id);

        # Get create user button
        button_create_user = vc.findViewByIdOrRaise(us_pair_user_button_id);

        # Click create User button and wait for the result
        _s_create_user = 5
        button_create_user.touch()
        vc.sleep(_s_create_user)

        # Check if user is created
        vc.dump(window=-1)
        text_result = vc.findViewByIdOrRaise(us_text_log_result_id);
        asserts.assertEqual(text_result.getText(), "SUCCESS")

        # Get test user android userID
        button_get_user_id = vc.findViewByIdOrRaise(us_get_user_id);
        button_get_user_id.touch()
        vc.sleep(action_time)

        # Save the Ids for later usage
        vc.dump(window=-1)
        user_id_new = vc.findViewByIdOrRaise(us_text_log_result_id).getText();
        user_id_owner = vc.findViewByIdOrRaise(us_text_owner_id).getText();
        edit_text_value = vc.findViewByIdOrRaise(us_edit_view_id);
        edit_text_value.setText(user_id_new);
        button_switch_user = vc.findViewByIdOrRaise(us_switch_user_button_id);
        button_switch_user.touch();

        # Wait till user to switch
        self.waitUntilViewNotAvailable(vc, us_text_owner_id, user_switch_timeout)

        # Start the application for the new user
        self.setUpUserSwitch()
        vc, device = self.getViewClient()
        vc.dump(window=-1)

        # Wait till user setup
        self.waitUntilViewAvailable(vc, us_text_owner_id, user_switch_timeout)
        new_user_id_owner = vc.findViewByIdOrRaise(us_text_owner_id).getText();

        # Compare user IDs
        asserts.assertEqual(user_id_new, new_user_id_owner)
        asserts.assertNotEqual(user_id_new, user_id_owner)

        # TODO: switch back to owner. Cannot do it right now since, owner doesn't exist in the CEM!!


    def getViewClient(self):
        kwargs1 = {'ignoreversioncheck': False, 'verbose': True, 'ignoresecuredevice': False,
                   'serialno': '.*'}
        device, serialno = ViewClient.connectToDeviceOrExit(**kwargs1)
        kwargs2 = {'forceviewserveruse': False, 'useuiautomatorhelper': False,
                   'ignoreuiautomatorkilled': True,
                   'autodump': False, 'startviewserver': True, 'compresseddump': True}
        return ViewClient(device, serialno, **kwargs2), device

    def waitUntilViewNotAvailable(self, vc, view_name, timeout_seconds):
        found = False;
        timer = 0;
        logging.info("Waiting until view " + view_name + " no available")
        while not found:
            timer = timer + 1
            vc.dump(window=-1)
            try:
                vc.findViewByIdOrRaise(view_name);
                logging.info("waiting: " + str(timer))
            except ViewNotFoundException:
                logging.info("View is not available anymore")
                return
            if timer >= timeout_seconds:
                logging.error("Timeout")
                asserts.assertTrue(False, "Time out! View is still visible: " + view_name)
            vc.sleep(1);

    def setUpUserSwitch(self):
        self.dut.adb.shell('input keyevent 3')
        self.dut.adb.shell('am start -n \"com.volvocars.halmodulesink/com.volvocars.halmodulesink.MainActivity\" -a \"android.intent.action.MAIN\" -c \"android.intent.category.LAUNCHER\"')

        _s = 0.5
        vc, device = self.getViewClient()

        # Open menu drawer button
        # Not possible to press with buttonId, using coordinates instead
        device.touchDip(10, 132, 0)
        vc.sleep(_s)

        # Get create user button
        # test = vc.findViewByIdOrRaise("com.volvocars.halmodulesink:id/toolbar");
        #
        # # Click create User button and wait for the result
        # _s_create_user = 5
        # test.touch()

        # Open vendor extension
        vc.dump(window=-1)
        userSwitch = vc.findViewWithTextOrRaise("User Switch")

        userSwitch.touch()
        vc.sleep(_s)

    def waitUntilViewAvailable(self, vc, view_name, timeout_seconds):
        found = False;
        timer = 0;
        logging.info("Waiting until view " + view_name + " available")
        while not found:
            timer = timer + 1
            vc.dump(window=-1)
            try:
                vc.findViewByIdOrRaise(view_name);
                logging.info("View is available")
                return
            except ViewNotFoundException:
                logging.info("waiting: " + str(timer))
            if timer >= timeout_seconds:
                logging.error("Timeout")
                asserts.assertTrue(False, "Time out! View is no still visible: " + view_name)
            vc.sleep(1);

if __name__ == "__main__":
    test_runner.main()
