#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
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

from fdx import fdx_client
from fdx import fdx_description_file_parser
from generated.pyDataElements import \
    FrSignalInterface, \
    UsgModSts, \
    LaneKeepAidActvSts, \
    IntvAndWarnModForLaneKeepAidSts, \
    WarnTypForLaneKeepAidSts, \
    LaneKeepAidSts
from vehiclehalcommon import VehicleHalCommon, wait_for_signal

# Get button Ids for testing.
lka_button_off           = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_off"
lka_button_on            = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_on"
lka_button_mode_both     = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_mode_both"
lka_button_mode_steering = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_mode_steering"
lka_button_mode_sound    = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_mode_sound"

waitTimeSeconds = 1

class VtsLaneKeepingAidSettingsComponentTest(base_test.BaseTestClass):
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

    # ----------------------------------------------------------------------------------------------------
    # Test Car Config Disabled
    # ----------------------------------------------------------------------------------------------------
    def testLaneKeepingCCDisabled(self):

        print("----------- testLaneKeepingAidCCDisabled -----------")

        self.dut.shell.one.Execute("changecarconfig 150 1")
        self.dut.shell.one.Execute("changecarconfig 23 9")

        # Reboot device after changing carconfig
        self.deviceReboot()

        vHalCommon = VehicleHalCommon(self.dut, self.system_uid)
        vHalCommon.setUpVehicleFunction()

        vc, device = vHalCommon.getViewClient()
        vc.dump(window=-1)

        # Find buttons. They shall be disabled when CC is disabled.
        buttonOff = vHalCommon.scrollAndFindViewByIdOrRaise(lka_button_off, 5)
        buttonOn = vHalCommon.scrollAndFindViewByIdOrRaise(lka_button_on, 5)
        print("buttonOff -> ") + str(buttonOff)
        print("buttonOn -> ") + str(buttonOn)
        asserts.assertEqual(buttonOff.__getattr__('enabled')(),False, "Off button info button is not disabled")
        asserts.assertEqual(buttonOn.__getattr__('enabled')(),False, "On button info button is not disabled")

    # ----------------------------------------------------------------------------------------------------
    # Test Car Config Enabled
    # ----------------------------------------------------------------------------------------------------
    def testLaneKeepingAidCCEnabled(self):

        print("----------- testLaneKeepingAidCCEnabled -----------")

        self.dut.shell.one.Execute("changecarconfig 150 2")
        self.dut.shell.one.Execute("changecarconfig 23 9")

        # Reboot device after changing carconfig
        self.deviceReboot()

        fr = FrSignalInterface()
        vHalCommon = VehicleHalCommon(self.dut, self.system_uid)
        vHalCommon.setUpVehicleFunction()

        vc, device = vHalCommon.getViewClient()
        vc.dump(window=-1)

        LANE_KEEPING_AID_ON = vHalCommon.get_id('LANE_KEEPING_AID_ON')
        LANE_KEEPING_AID_MODE = vHalCommon.get_id('LANE_KEEPING_AID_MODE')

        # Get buttons. They shall be enabled if CC is enabled
        buttonOff = vHalCommon.scrollAndFindViewByIdOrRaise(lka_button_off, 5)
        buttonOn = vHalCommon.scrollAndFindViewByIdOrRaise(lka_button_on, 5)
        print("buttonOff -> ") + str(buttonOff)
        print("buttonOn -> ") + str(buttonOn)

        asserts.assertNotEqual(None, buttonOff)
        asserts.assertNotEqual(None, buttonOn)

        print("buttonOff enabled -> ") + str(buttonOff.__getattr__('enabled')())
        print("buttonOn  enabled -> ") + str(buttonOn.__getattr__('enabled')())
        asserts.assertEqual(buttonOff.__getattr__('enabled')(),True, "Off button is disabled")
        asserts.assertEqual(buttonOn.__getattr__('enabled')(),True, "On button is disabled")

        buttonOn.touch()

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------

        print("----------- PRE UsgModSts ACTIVE -----------")

        # UsgModSts ACTIVE
        # LaneKeepAidActvSts ON or OFF
        fr.LaneKeepAidSts.send_repetitive(LaneKeepAidSts.map.On)

        fr.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        vc.sleep(waitTimeSeconds)
        currentLKAMode = vHalCommon.getVhalInt32(LANE_KEEPING_AID_MODE)

        buttonOff.touch()
        vc.sleep(waitTimeSeconds)
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, False)
        # Mode should be the default.
        vHalCommon.assertVhalInt32Equal(currentLKAMode, LANE_KEEPING_AID_MODE)
        wait_for_signal(fr, fr.LaneKeepAidActvSts, fr.LaneKeepAidActvSts.map.Off, 3)

        buttonOn.touch()
        vc.sleep(waitTimeSeconds)
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, True)
        vHalCommon.assertVhalInt32Equal(currentLKAMode, LANE_KEEPING_AID_MODE)
        wait_for_signal(fr,fr.LaneKeepAidActvSts, fr.LaneKeepAidActvSts.map.On, 3)

        # ----------------------------------------------------------------------------------------------------------
        # Test not_active state
        # ----------------------------------------------------------------------------------------------------------

        print("----------- PRE UsgModSts NOT_ACTIVE -----------")

        # UsgModSts NOT_ACTIVE
        # LaneKeepAidActvSts ON/OFF
        fr.UsgModSts.send(UsgModSts.map.UsgModInActv)
        vc.sleep(waitTimeSeconds)

        buttonOff.touch()
        vc.sleep(waitTimeSeconds)

        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, 0)
        wait_for_signal(fr,fr.LaneKeepAidActvSts, fr.LaneKeepAidActvSts.map.Off, 3)

        # ----------------------------------------------------------------------------------------------------------
        # Test LKA Modes
        # ----------------------------------------------------------------------------------------------------------

        fr.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        vc.sleep(waitTimeSeconds)

        buttonOn.touch()
        vc.sleep(waitTimeSeconds)

        buttonBoth = vHalCommon.scrollAndFindViewByIdOrRaise(lka_button_mode_both, 5)
        buttonSteering = vHalCommon.scrollAndFindViewByIdOrRaise(lka_button_mode_steering, 5)
        buttonSound = vHalCommon.scrollAndFindViewByIdOrRaise(lka_button_mode_sound, 5)
        print("buttonBoth -> ") + str(buttonBoth)
        print("buttonSteering -> ") + str(buttonSteering)
        print("buttonSound -> ") + str(buttonSound)

        buttonBoth.touch()
        vc.sleep(waitTimeSeconds)
        vHalCommon.assertVhalInt32Equal(0, LANE_KEEPING_AID_MODE)

        buttonSteering.touch()
        vc.sleep(waitTimeSeconds)
        vHalCommon.assertVhalInt32Equal(1, LANE_KEEPING_AID_MODE)

        wait_for_signal(fr,fr.IntvAndWarnModForLaneKeepAidSts, fr.IntvAndWarnModForLaneKeepAidSts.map.Intv, 5)

        buttonSound.touch()
        vc.sleep(waitTimeSeconds)
        vHalCommon.assertVhalInt32Equal(2, LANE_KEEPING_AID_MODE)

        wait_for_signal(fr,fr.IntvAndWarnModForLaneKeepAidSts, fr.IntvAndWarnModForLaneKeepAidSts.map.Warn, 5)

        # WarnTypForLaneKeepAidSts.Hptc shall always be sent
        wait_for_signal(fr,fr.WarnTypForLaneKeepAidSts, fr.WarnTypForLaneKeepAidSts.map.Hptc, 3)


        fr.LaneKeepAidSts.stop_send()


        # ----------------------------------------------------------------------------------------------------------
        # Test System Error
        # ----------------------------------------------------------------------------------------------------------
        print("UsgModSts ACTIVE and system error")

        # UsgModSts ACTIVE
        # LaneKeepAidActvSts ON or OFF
        fr.LaneKeepAidSts.send_repetitive(LaneKeepAidSts.map.SrvRqrd)
        fr.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        vc.sleep(waitTimeSeconds)

        # On/Off shall be Off and LKA mode shall be the default (0) on System error.
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, False)
        vHalCommon.assertVhalInt32Equal(0, LANE_KEEPING_AID_MODE)

        fr.LaneKeepAidSts.stop_send()

        # ----------------------------------------------------------------------------------------------------------
        # All tests done
        # ----------------------------------------------------------------------------------------------------------
        # Close applicationa and go back to home screen
        self.dut.adb.shell('input keyevent 3')
        vc.sleep(waitTimeSeconds)

if __name__ == "__main__":
    test_runner.main()
