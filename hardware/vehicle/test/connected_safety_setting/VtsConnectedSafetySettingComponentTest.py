#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
from vts.runners.host import asserts
from vts.runners.host import test_runner
sys.path.append('/usr/local/lib/python2.7/dist-packages')
from vf_common import vf_base_test
from vf_common import vehiclehalcommon2
from vf_common.signals_common import wait_for_fr_subsignal
from vf_common.vehiclehalcommon import VehicleHalCommon,  PAStatus
from vf_common.vf_base_test import SUBTEST
from generated import datatypes as DE

cs_button_off = VehicleHalCommon.app_context_vehiclefunctions + "connected_safety_button_off"
cs_button_on  = VehicleHalCommon.app_context_vehiclefunctions + "connected_safety_button_on"
CONNECTED_SAFETY_ON = vehiclehalcommon2.Properties.CONNECTED_SAFETY_ON
CONNECTED_SAFETY_ON_STATUS = vehiclehalcommon2.Properties.CONNECTED_SAFETY_ON_STATUS



class VtsConnectedSafetySettingComponentTest(vf_base_test.VFBaseTest):

    def wait_for_RoadFricIndcnActvSts(self, expected_value, msg=""):
        # type: (DE.OnOff1, str) -> None
        wait_for_fr_subsignal(self.fr.get_RoadFricIndcnActv, lambda x: x.Sts, expected_value, message=msg)

    def testCCDisabled_buttonsNotPresentAndFRDefaultSent(self):
        self.change_carconfig_and_reboot_if_needed([(147, 1)])
        self.setUpVehicleFunction()
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv, DE.CarModSts1.CarModDyno)
        self.fr.send_RoadFricIndcnSts(DE.FctSts2.On)     # anything except SrvRqrd
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModDrvg)

        self.ui.scrollAndAssertViewsAreNotFound([cs_button_off, cs_button_on])
        asserts.assertEqual(self.fr.get_RoadFricIndcnActv().Sts, DE.OnOff1.Off, "on/off shall be off")

    def testCCEnabled_ButtonsVisibleAndFlexraySignalsSent(self):
        self.change_carconfig_and_reboot_if_needed([(147,2),
                                                    (23, 9)])
        self.setUpVehicleFunction()
        fr = self.fr

        fr.send_RoadFricIndcnSts(DE.FctSts2.On)   # anything except SrvRqrd
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)

        # Get buttons. They shall be here if CC is enabled.
        buttonOff = self.ui.scrollDownAndFindViewByIdOrRaise(cs_button_off)
        buttonOn = self.ui.scrollDownAndFindViewByIdOrRaise(cs_button_on)
        asserts.assertTrue(buttonOff.enabled, "Off button is disabled")
        asserts.assertTrue(buttonOn.enabled, "On button is disabled")
        self.vhal.wait_for_prop_equals(CONNECTED_SAFETY_ON_STATUS, PAStatus.Active)

        with SUBTEST("UsgModSts Active, off/on buttons work"):
            buttonOff.click()
            self.vhal.wait_for_prop_equals(CONNECTED_SAFETY_ON, False)
            self.wait_for_RoadFricIndcnActvSts(DE.OnOff1.Off)
            buttonOn.click()
            self.vhal.wait_for_prop_equals(CONNECTED_SAFETY_ON, True)
            self.wait_for_RoadFricIndcnActvSts(DE.OnOff1.On)

        with SUBTEST("UsgModSts Inactive, status disabled"):
            self.signals.set_usage_mode(DE.UsgModSts1.UsgModInActv)
            self.vhal.wait_for_prop_equals(CONNECTED_SAFETY_ON, False)
            self.vhal.wait_for_prop_equals(CONNECTED_SAFETY_ON_STATUS, PAStatus.Disabled,
                                           msg="CONNECTED_SAFETY_ON_STATUS (State Disabled)")
            self.wait_for_RoadFricIndcnActvSts(DE.OnOff1.Off)
            asserts.assertFalse(buttonOff.enabled, "Off button is not disabled in usgmod inactv")
            asserts.assertFalse(buttonOn.enabled, "On button is not disabled in usgmod inactv")

        with SUBTEST("UsgModSts Driving, status and on/off restored"):
            self.signals.set_usage_mode(DE.UsgModSts1.UsgModDrvg)
            self.vhal.wait_for_prop_equals(CONNECTED_SAFETY_ON, True)
            self.vhal.wait_for_prop_equals(CONNECTED_SAFETY_ON_STATUS, PAStatus.Active, msg="CONNECTED_SAFETY_ON_STATUS (State Active)")
            self.wait_for_RoadFricIndcnActvSts(DE.OnOff1.On)

        with SUBTEST("System Error test"):
            fr.send_RoadFricIndcnSts(DE.FctSts2.SrvRqrd)
            self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)
            self.wait_for_RoadFricIndcnActvSts(DE.OnOff1.Off)
            self.vhal.assert_prop_equals(CONNECTED_SAFETY_ON, False)
            self.vhal.assert_prop_equals(CONNECTED_SAFETY_ON_STATUS, PAStatus.SystemError, "CONNECTED_SAFETY_ON_STATUS (State SystemError)")

if __name__ == "__main__":
    test_runner.main()
