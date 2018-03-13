#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
import os

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.precondition import precondition_utils
from subprocess import call

sys.path.append('/usr/local/lib/python2.7/dist-packages')

from com.dtmilano.android.viewclient import ViewClient
from fdx import fdx_client
from fdx import fdx_description_file_parser
from generated.pyDataElements import \
    FrSignalInterface, \
    UsgModSts, \
    RoadFricIndcnSts
from vehiclehalcommon import VehicleHalCommon

ns_per_ms = 1000000

app_context = "com.volvocars.halmodulesink:id/"
cs_button_off  = app_context + "buttonConnSafetyOff"
cs_button_on   = app_context + "buttonConnSafetyOn"

class VtsConnSafetySettingsComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()

        self.flexray = FrSignalInterface()

    def deviceReboot(self):
        self.dut.shell.one.Execute("reboot")
        self.dut.stopServices()
        self.dut.waitForBootCompletion()
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons not present
    # ----------------------------------------------------------------------------------------------------------
    def testConnSafetyCCDisabled(self):
        self.dut.shell.one.Execute("changecarconfig 147 1")

        # Rebbot device after changing carconfig
        self.deviceReboot()

        fr = FrSignalInterface()
        vHalCommon = VehicleHalCommon(self.dut, self.system_uid)
        vHalCommon.setUpVendorExtension()

        vc, device = vHalCommon.getViewClient()
        vc.dump(window=-1)

        vHalCommon.assert_ViewNotFoundException(vc, cs_button_off)
        vHalCommon.assert_ViewNotFoundException(vc, cs_button_on)

        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.Off)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons present
    # ----------------------------------------------------------------------------------------------------------
    def testConnSafetyCCEnabled(self):
        self.dut.shell.one.Execute("changecarconfig 147 2")

        # Rebbot device after changing carconfig
        self.deviceReboot()

        fr = FrSignalInterface()
        vHalCommon = VehicleHalCommon(self.dut, self.system_uid)
        vHalCommon.setUpVendorExtension()
        # Get buttons
        _s = 1
        vc, device = vHalCommon.getViewClient()
        vc.dump(window=-1)
        cs_off = vc.findViewByIdOrRaise(cs_button_off);
        cs_on = vc.findViewByIdOrRaise(cs_button_on);

        CONNECTED_SAFETY_ON = vHalCommon.get_id('CONNECTED_SAFETY_ON')

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------

        # UsgModSts ACTIVE
        # FrictionSts ON or OFF
        fr.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        fr.RoadFricIndcnSts.send_repetitive(RoadFricIndcnSts.map.On)
        vc.sleep(_s)

        cs_off.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, 0)
        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.Off)

        cs_on.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, 1)
        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.On)

        fr.RoadFricIndcnSts.stop_send()

        # ----------------------------------------------------------------------------------------------------------
        # Test not_active state
        # ----------------------------------------------------------------------------------------------------------

        # UsgModSts NOT_ACTIVE
        # FrictionSts INVALID
        fr.UsgModSts.send(UsgModSts.map.UsgModInActv)
        fr.RoadFricIndcnSts.send_repetitive(RoadFricIndcnSts.map.NotAvl)
        vc.sleep(_s)

        cs_off.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, 0)
        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.Off)

        cs_on.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, 0)
        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.Off)

        fr.RoadFricIndcnSts.stop_send()

        # ----------------------------------------------------------------------------------------------------------
        # Test not_active state
        # ----------------------------------------------------------------------------------------------------------

        # UsgModSts NOT_ACTIVE
        # FrictionSts ON/OFF
        fr.UsgModSts.send(UsgModSts.map.UsgModInActv)
        fr.RoadFricIndcnSts.send_repetitive(RoadFricIndcnSts.map.On)
        vc.sleep(_s)

        cs_off.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, 0)
        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.Off)

        cs_on.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, 0)
        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.Off)

        fr.RoadFricIndcnSts.stop_send()

        # ----------------------------------------------------------------------------------------------------------
        # Test error state
        # ----------------------------------------------------------------------------------------------------------

        # UsgModSts ACTIVE
        # FrictionSts INVALID
        fr.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        fr.RoadFricIndcnSts.send_repetitive(RoadFricIndcnSts.map.NotAvl)
        vc.sleep(_s)

        cs_off.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, 0)
        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.Off)

        cs_on.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, 0)
        vHalCommon.assert_signal_equals(fr.RoadFricIndcnActvSts, fr.RoadFricIndcnActvSts.map.Off)

        fr.RoadFricIndcnSts.stop_send()

        # ----------------------------------------------------------------------------------------------------------
        # All tests done
        # ----------------------------------------------------------------------------------------------------------
        # Close applicationa and go back to home screen
        self.dut.adb.shell('input keyevent 3')
        vc.sleep(_s)

if __name__ == "__main__":
    test_runner.main()