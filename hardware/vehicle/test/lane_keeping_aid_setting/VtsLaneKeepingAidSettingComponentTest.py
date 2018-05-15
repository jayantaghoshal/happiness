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
import vehiclehalcommon
import threading
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    FctSts2, \
    WarnAndIntv1, \
    WarnTypForLaneChgWarnPen1, \
    SoundHptc1

from generated import datatypes as DE
import logging.config
import logging.handlers


from vehiclehalcommon import VehicleHalCommon, wait_for_signal, PAStatus
from uiautomator import device as device


# Get button Ids for tests.
lka_button_off           = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_off"
lka_button_on            = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_on"
lka_button_mode_both     = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_mode_both"
lka_button_mode_steering = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_mode_steering"
lka_button_mode_sound    = VehicleHalCommon.app_context_vehiclefunctions + "lane_keeping_aid_button_mode_sound"

buttonWaitTimeSeconds = 1.5
populateWaitTimeSeconds = 4

class VtsLaneKeepingAidSettingsComponentTest(base_test.BaseTestClass):

    def setUpClass(self):

        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()
        self.fr = vehiclehalcommon.get_dataelements_connection(self.dut.adb)


    def tearDownClass(self):
        try:
            self.fr.close()
        except:
            pass

    def deviceReboot(self):
        if(self.fr != None):
            self.fr.close()
        time.sleep(1)
        self.dut.shell.one.Execute("reboot")
        self.dut.stopServices()
        self.dut.waitForBootCompletion()
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")
        time.sleep(1)
        self.fr = vehiclehalcommon.get_dataelements_connection(self.dut.adb)
        time.sleep(1)

    # ----------------------------------------------------------------------------------------------------
    # Test Car Config Disabled
    # ----------------------------------------------------------------------------------------------------
    def testLaneKeepingCCDisabled(self):

        print("----------- testLaneKeepingAidCCDisabled -----------")

        self.dut.shell.one.Execute("changecarconfig 150 1")
        self.dut.shell.one.Execute("changecarconfig 23 9")

        # Reboot device after changing carconfig
        self.deviceReboot()

        fr = self.fr

        vehmod = VehModMngtGlbSafe1()
        vehmod.CarModSts1 = DE.CarModSts1.CarModDyno
        vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        vehmod.Chks = 0
        vehmod.Cntr = 0
        vehmod.CarModSubtypWdCarModSubtyp = 0
        vehmod.EgyLvlElecMai = 0
        vehmod.EgyLvlElecSubtyp = 0
        vehmod.PwrLvlElecMai = 0
        vehmod.PwrLvlElecSubtyp = 0
        vehmod.FltEgyCnsWdSts = DE.FltEgyCns1.Flt

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        # UsgModSts Drive
        # LaneKeepAidSts On (not SrvRqrd)
        fr.send_LaneKeepAidSts(DE.FctSts2.On)
        vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(buttonWaitTimeSeconds)


        # Find buttons. They shall be gone when CC is disabled.
        buttonOff = vHalCommon.scrollDownAndFindViewByIdUiAutomator(lka_button_off, device)
        buttonOn = vHalCommon.scrollDownAndFindViewByIdUiAutomator(lka_button_on, device)

        print("buttonOff -> ") + str(buttonOff)
        print("buttonOn -> ") + str(buttonOn)
        asserts.assertEqual(None, buttonOff, "Expect no buttonOff")
        asserts.assertEqual(None, buttonOn, "Expect no buttonOn")

        time.sleep(buttonWaitTimeSeconds)
        fr.stop_LaneKeepAidSts()

    # ----------------------------------------------------------------------------------------------------
    # Test Car Config Enabled
    # ----------------------------------------------------------------------------------------------------
    def testLaneKeepingAidCCEnabledLKAOnOff(self):

        print("----------- testLaneKeepingAidCCEnabledLKAOnOff -----------")

        self.dut.shell.one.Execute("changecarconfig 150 2")
        self.dut.shell.one.Execute("changecarconfig 23 9")

        # Reboot device after changing carconfig
        self.deviceReboot()

        fr = self.fr

        vehmod = VehModMngtGlbSafe1()
        vehmod.CarModSts1 = DE.CarModSts1.CarModDyno
        vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        vehmod.Chks = 0
        vehmod.Cntr = 0
        vehmod.CarModSubtypWdCarModSubtyp = 0
        vehmod.EgyLvlElecMai = 0
        vehmod.EgyLvlElecSubtyp = 0
        vehmod.PwrLvlElecMai = 0
        vehmod.PwrLvlElecSubtyp = 0
        vehmod.FltEgyCnsWdSts = DE.FltEgyCns1.Flt

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        LANE_KEEPING_AID_ON = vHalCommon.get_id('LANE_KEEPING_AID_ON')
        LANE_KEEPING_AID_ON_STATUS = vHalCommon.get_id('LANE_KEEPING_AID_ON_STATUS')
        LANE_KEEPING_AID_MODE = vHalCommon.get_id('LANE_KEEPING_AID_MODE')
        LANE_KEEPING_AID_MODE_STATUS = vHalCommon.get_id('LANE_KEEPING_AID_MODE_STATUS')

        # UsgModSts Drive
        # LaneKeepAidSts On (not SrvRqrd)
        fr.send_LaneKeepAidSts(DE.FctSts2.On)
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(populateWaitTimeSeconds)

        # Get buttons. They shall be gone if CC is disabled
        buttonOff = vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(lka_button_off, device)
        buttonOn = vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(lka_button_on, device)
        print("buttonOff -> ") + str(buttonOff)
        print("buttonOn -> ") + str(buttonOn)

        asserts.assertEqual(buttonOff.enabled, True, "Off button is disabled")
        asserts.assertEqual(buttonOn.enabled, True, "On button is disabled")

        buttonOn.click()
        time.sleep(buttonWaitTimeSeconds)

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------

        print("----------- UsgModSts active -----------")

        time.sleep(buttonWaitTimeSeconds)
        currentLKAMode = vHalCommon.getVhalInt32(LANE_KEEPING_AID_MODE)

        buttonOff.click()
        time.sleep(buttonWaitTimeSeconds)
        # Verify that On/Off is false after Off button click.
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, False)
        asserts.assertEqual(fr.get_LaneKeepAidActv().Sts, 0, "on/off shall be 0")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON_STATUS, PAStatus.Active, "LANE_KEEPING_AID_ON_STATUS (State Active)")

        # Mode should be the unchanged.
        vHalCommon.assertVhalInt32Equal(currentLKAMode, LANE_KEEPING_AID_MODE)
        asserts.assertEqual(fr.get_IntvAndWarnModForLaneKeepAid().Sts, currentLKAMode, "mode shall be unmodified")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_MODE_STATUS, PAStatus.Active, "LANE_KEEPING_AID_MODE_STATUS (State Active)")

        buttonOn.click()
        time.sleep(buttonWaitTimeSeconds)

        # Verify that On/Off is true after On button click.
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, True)
        asserts.assertEqual(fr.get_LaneKeepAidActv().Sts, 1, "on/off shall be 1")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON_STATUS, PAStatus.Active, "LANE_KEEPING_AID_ON_STATUS (State Active)")
        # Mode should be the unchanged.
        vHalCommon.assertVhalInt32Equal(currentLKAMode, LANE_KEEPING_AID_MODE)
        asserts.assertEqual(fr.get_IntvAndWarnModForLaneKeepAid().Sts, currentLKAMode, "mode shall be unmodified")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_MODE_STATUS, PAStatus.Active, "LANE_KEEPING_AID_MODE_STATUS (State Active)")

        # ----------------------------------------------------------------------------------------------------------
        # Test inactive state
        # ----------------------------------------------------------------------------------------------------------

        print("----------- UsgModSts inactive -----------")

        vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(buttonWaitTimeSeconds)

        # On/Off shall be default 0 and mode shall be 0.
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, False)
        asserts.assertEqual(fr.get_LaneKeepAidActv().Sts, 0, "on/off shall be 0")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON_STATUS, PAStatus.Disabled, "LANE_KEEPING_AID_ON_STATUS (State Disabled)")
        vHalCommon.assertVhalInt32Equal(0, LANE_KEEPING_AID_MODE)
        asserts.assertEqual(fr.get_IntvAndWarnModForLaneKeepAid().Sts, WarnAndIntv1.WarnAndIntv, "mode shall be stored value")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_MODE_STATUS, PAStatus.Disabled, "LANE_KEEPING_AID_MODE_STATUS (State Disabled)")

        # Set usage mode
        vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(buttonWaitTimeSeconds)

        # On/Off shall and mode shall be restored.
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, True)
        asserts.assertEqual(fr.get_IntvAndWarnModForLaneKeepAid().Sts, currentLKAMode, "mode shall be the default")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON_STATUS, PAStatus.Active, "LANE_KEEPING_AID_ON_STATUS (State Active)")
        vHalCommon.assertVhalInt32Equal(currentLKAMode, LANE_KEEPING_AID_MODE)
        asserts.assertEqual(fr.get_LaneKeepAidActv().Sts, 1, "on/off shall be 1")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_MODE_STATUS, PAStatus.Active, "LANE_KEEPING_AID_MODE_STATUS (State Active)")

        # ----------------------------------------------------------------------------------------------------------
        # Test System Error
        # ----------------------------------------------------------------------------------------------------------
        print("System Error test")

        fr.send_LaneKeepAidSts(DE.FctSts2.SrvRqrd)

        # UsgModSts ACTIVE
        vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(buttonWaitTimeSeconds)

        # On/Off shall be Off and LKA mode shall be the default (0) on System error.
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON, False)
        vHalCommon.assertVhalInt32Equal(0, LANE_KEEPING_AID_MODE)

        asserts.assertEqual(fr.get_IntvAndWarnModForLaneKeepAid().Sts, WarnAndIntv1.WarnAndIntv, "mode shall be 0")
        asserts.assertEqual(fr.get_WarnTypForLaneKeepAid().Sts, SoundHptc1.Hptc, "Warn type shall be Hptc")
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON_STATUS, PAStatus.SystemError, "LANE_KEEPING_AID_ON_STATUS (State SystemError)")
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_MODE_STATUS, PAStatus.SystemError, "LANE_KEEPING_AID_MODE_STATUS (State SystemError)")


    def testLaneKeepingAidCCEnabledLKAMode(self):
        print("----------- testLaneKeepingAidCCEnabledLKAMode -----------")

        self.dut.shell.one.Execute("changecarconfig 150 2")
        self.dut.shell.one.Execute("changecarconfig 23 9")

        # Reboot device after changing carconfig
        self.deviceReboot()

        fr = self.fr

        vehmod = VehModMngtGlbSafe1()
        vehmod.CarModSts1 = DE.CarModSts1.CarModDyno
        vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        vehmod.Chks = 0
        vehmod.Cntr = 0
        vehmod.CarModSubtypWdCarModSubtyp = 0
        vehmod.EgyLvlElecMai = 0
        vehmod.EgyLvlElecSubtyp = 0
        vehmod.PwrLvlElecMai = 0
        vehmod.PwrLvlElecSubtyp = 0
        vehmod.FltEgyCnsWdSts = DE.FltEgyCns1.Flt

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        LANE_KEEPING_AID_ON = vHalCommon.get_id('LANE_KEEPING_AID_ON')
        LANE_KEEPING_AID_ON_STATUS = vHalCommon.get_id('LANE_KEEPING_AID_ON_STATUS')
        LANE_KEEPING_AID_MODE = vHalCommon.get_id('LANE_KEEPING_AID_MODE')
        LANE_KEEPING_AID_MODE_STATUS = vHalCommon.get_id('LANE_KEEPING_AID_MODE_STATUS')

        # UsgModSts Drive
        # LaneKeepAidSts On (not SrvRqrd)
        fr.send_LaneKeepAidSts(DE.FctSts2.On)
        fr.send_VehModMngtGlbSafe1(vehmod)

        # Extra wait for population views depending on car config.
        time.sleep(populateWaitTimeSeconds)

        # Get buttons. They shall be enabled if CC is enabled
        # Search mode buttons for mode tests.
        buttonBoth = vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(lka_button_mode_both, device)
        buttonSteering = vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(lka_button_mode_steering, device)
        buttonSound = vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(lka_button_mode_sound, device)

        asserts.assertEqual(buttonBoth.enabled, True, "Both button is disabled")
        asserts.assertEqual(buttonSteering.enabled, True, "Steering button is disabled")
        asserts.assertEqual(buttonSound.enabled, True, "Steering button is disabled")


        # ----------------------------------------------------------------------------------------------------------
        # Test LKA Modes
        # ----------------------------------------------------------------------------------------------------------

        # Click buttons to prepare for mode tests.
        print("press sound...")
        buttonSound.click()
        time.sleep(buttonWaitTimeSeconds)
        print("press both...")
        buttonBoth.click()
        time.sleep(buttonWaitTimeSeconds)
        print("asserting mode...")
        asserts.assertEqual(fr.get_IntvAndWarnModForLaneKeepAid().Sts, WarnAndIntv1.WarnAndIntv, "mode shall be 0")
        vHalCommon.assertVhalInt32Equal(0, LANE_KEEPING_AID_MODE)
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON_STATUS, PAStatus.Active, "LANE_KEEPING_AID_ON_STATUS (State Active)")
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_MODE_STATUS, PAStatus.Active, "LANE_KEEPING_AID_MODE_STATUS (State Active)")

        # WarnTypForLaneKeepAidSts.Hptc shall always be sent
        asserts.assertEqual(fr.get_WarnTypForLaneKeepAid().Sts, SoundHptc1.Hptc, "Warn type shall be Hptc")

        buttonSteering.click()
        time.sleep(buttonWaitTimeSeconds)
        asserts.assertEqual(fr.get_IntvAndWarnModForLaneKeepAid().Sts, WarnAndIntv1.Intv, "mode shall be 1")
        vHalCommon.assertVhalInt32Equal(1, LANE_KEEPING_AID_MODE)
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON_STATUS, PAStatus.Active, "LANE_KEEPING_AID_ON_STATUS (State Active)")
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_MODE_STATUS, PAStatus.Active, "LANE_KEEPING_AID_MODE_STATUS (State Active)")

        # WarnTypForLaneKeepAidSts.Hptc shall always be sent
        asserts.assertEqual(fr.get_WarnTypForLaneKeepAid().Sts, SoundHptc1.Hptc, "Warn type shall be Hptc")

        buttonSound.click()
        time.sleep(buttonWaitTimeSeconds)
        asserts.assertEqual(fr.get_IntvAndWarnModForLaneKeepAid().Sts, WarnAndIntv1.Warn, "mode shall be 2")
        vHalCommon.assertVhalInt32Equal(2, LANE_KEEPING_AID_MODE)
        # Check PAStatus
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_ON_STATUS, PAStatus.Active, "LANE_KEEPING_AID_ON_STATUS (State Active)")
        vHalCommon.assert_prop_equals(LANE_KEEPING_AID_MODE_STATUS, PAStatus.Active, "LANE_KEEPING_AID_MODE_STATUS (State Active)")

        # WarnTypForLaneKeepAidSts.Hptc shall always be sent
        asserts.assertEqual(fr.get_WarnTypForLaneKeepAid().Sts, SoundHptc1.Hptc, "Warn type shall be Hptc")


if __name__ == "__main__":
    test_runner.main()
