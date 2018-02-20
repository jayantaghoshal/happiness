#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

sys.path.append('/usr/local/lib/python2.7/dist-packages')

from com.dtmilano.android.viewclient import ViewClient

ns_per_ms = 1000000


class VtsDAISettingsComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode

    def testAddAndPairUser(self):
        self.setUpVccUserSwitch()

        # Get buttons
        _s = 1
        vc, device = self.getViewClient()
        vc.dump(window=-1)

        button_create_user = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/pairUserButtonCreate");
        button_switch_user = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/pairUserButtonSwitch");
        button_dump_user = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/dumpUserButton");
        button_delete_user = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/deleteUserButton");

        vc.sleep(_s)

        # Click create User button and wait for the result
        _s_create_user = 10
        button_create_user.touch()
        vc.sleep(_s_create_user)
        vc.dump(window=-1)

        text_result = vc.findViewByIdOrRaise("com.volvocars.userswitch:id/log");
        asserts.assertEqual(text_result.getText(), "SUCCESS")

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

if __name__ == "__main__":
    test_runner.main()
