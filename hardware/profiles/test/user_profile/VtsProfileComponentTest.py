#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
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

class CtsProfileComponentTest(base_test.BaseTestClass):
    def setUpClass(self):

        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

    def testCreateAndSwitchUser(self):
        self.setUpVccUserSwitch()

        # Get button
        vc, device = self.getViewClient()
        vc.dump(window=-1)
        button_create_user = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/pairUserButtonCreate");

        # Click create User button and wait for the result
        _s_create_user = 5
        button_create_user.touch()
        vc.sleep(_s_create_user)

        # Check if user is created
        vc.dump(window=-1)
        text_result = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/textLog");
        asserts.assertEqual(text_result.getText(), "SUCCESS")

        # Get test user android userID
        button_get_user_id = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/getUserIdButton");
        button_get_user_id.touch()
        vc.sleep(action_time)

        # Save the Ids for later usage
        vc.dump(window=-1)
        user_id_new = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/textLog").getText();
        user_id_owner = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/textOwner").getText();
        edit_text_value = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/editTextUserId");
        edit_text_value.setText(user_id_new);
        button_switch_user = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/pairUserButtonSwitch");
        button_switch_user.touch();

        # Wait till user to switch
        self.waitUntilViewNotAvailable(vc, "com.volvocars.userswitch:id/textOwner", user_switch_timeout)

        # Start the application for the new user
        self.setUpVccUserSwitch()
        vc, device = self.getViewClient()

        # Wait till user setup
        self.waitUntilViewAvailable(vc, "com.volvocars.userswitch:id/textOwner", user_switch_timeout)
        new_user_id_owner = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/textOwner").getText();

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

    def setUpVccUserSwitch(self):
        self.dut.adb.shell('input keyevent 3')
        self.dut.adb.shell('am start -n "com.volvocars.userswitch/com.volvocars.userswitch.MainActivityTest\" -a \"android.intent.action.MAIN\" -c \"android.intent.category.LAUNCHER\"')
        _s = 0.5
        vc, device = self.getViewClient()
        vc.sleep(_s)

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
