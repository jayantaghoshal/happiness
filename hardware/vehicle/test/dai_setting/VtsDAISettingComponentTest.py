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
from com.dtmilano.android.viewclient import ViewClient
from fdx import fdx_client
from fdx import fdx_description_file_parser
import vehiclehalcommon
import threading
from generated.datatypes import \
    VehModMngtGlbSafe1

from generated import datatypes as DE
import logging.config
import logging.handlers

from vehiclehalcommon import VehicleHalCommon, wait_for_signal
from com.dtmilano.android.viewclient import ViewNotFoundException

# Button ids
dai_off_id  = VehicleHalCommon.app_context_vehiclefunctions + "dai_state_1_no_info_button"
dai_visual_id   = VehicleHalCommon.app_context_vehiclefunctions + "dai_state_2_visual_only_button"
dai_visual_sound_id   = VehicleHalCommon.app_context_vehiclefunctions + "dai_state_3_visual_and_audio_button"

class VtsDAISettingsComponentTest(base_test.BaseTestClass):
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

    def testDriveAwayInfoCCDisabled(self):

        print("------------------testDriveAwayInfoCCDisabled ---------------")

        self.dut.shell.one.Execute("changecarconfig 315 1")
        self.deviceReboot()

        fr=self.fr

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

        vc, device = vHalCommon.getActiveViewClient()

        vc.dump(window=-1)
        dai_off = vc.findViewById(dai_off_id)
        dai_visual = vc.findViewById(dai_visual_id)
        dai_visual_sound = vc.findViewById(dai_visual_sound_id)

        # Check if buttons are invisible
        asserts.assertEqual(dai_off, None, "Button is visible:" + dai_off_id)
        asserts.assertEqual(dai_visual, None, "Button is visible:" + dai_visual_id)
        asserts.assertEqual(dai_visual_sound, None, "Button is visible:" + dai_visual_sound_id)

        # Check default values for daisetting
        asserts.assertEqual(fr.get_DriveAwayInfoActvReq(), 0, "on/off shall be 0")
        asserts.assertEqual(fr.get_DriveAwayInfoSoundWarnActvReq(), 0, "on/off shall be 0")

    def testDriveAwayInfoCCEnabled(self):

        print("------------------testDriveAwayInfoCCEnabled----------------------")

        self.dut.shell.one.Execute("changecarconfig 315 3")
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

        vHalCommon = VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vc, device = vHalCommon.getActiveViewClient()
        vHalCommon.setUpVehicleFunction()

        # Get buttons
        _s = 1
        # Wait for the buttons to appear, in case of the slow population
        vHalCommon.waitUntilViewAvailable(vc, dai_off_id, timeout_seconds=5)
        vc.dump(window=-1)
        dai_off = vc.findViewByIdOrRaise(dai_off_id);
        dai_visual = vc.findViewByIdOrRaise(dai_visual_id);
        dai_visual_sound = vc.findViewByIdOrRaise(dai_visual_sound_id);
        DAI_SETTING = vHalCommon.get_id('DAI_SETTING')

        # Set UsgModSts to ACTIVE, setting should change in ACTIVE
        fr.send_VehModMngtGlbSafe1(vehmod)

        # Set setting to OFF
        dai_off.touch()
        vc.sleep(_s)
        settings_value = vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(vHalCommon.extractValue(settings_value), 0, "Failed to set prop to OFF")

        asserts.assertEqual(fr.get_DriveAwayInfoActvReq(), 0, "on/off shall be 0")
        asserts.assertEqual(fr.get_DriveAwayInfoSoundWarnActvReq(), 0, "on/off shall be 0")

        # Set setting to VISUAL
        dai_visual.touch()
        vc.sleep(_s)
        settings_value = vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(vHalCommon.extractValue(settings_value), 1, "Failed to set prop to VISUAL")
        asserts.assertEqual(fr.get_DriveAwayInfoActvReq(), 1, "on/off shall be 1")
        asserts.assertEqual(fr.get_DriveAwayInfoSoundWarnActvReq(), 0, "on/off shall be 0")

        # Set setting to VISUAL AND SOUND
        dai_visual_sound.touch()
        vc.sleep(_s)
        settings_value = vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(vHalCommon.extractValue(settings_value), 2, "Failed to set prop to VISUAL AND SOUND")
        asserts.assertEqual(fr.get_DriveAwayInfoActvReq(), 1, "on/off shall be 1")
        asserts.assertEqual(fr.get_DriveAwayInfoSoundWarnActvReq(), 1, "on/off shall be 1")

        # Set UsgModSts to NOT_ACTIVE, setting should not change in NOT_ACTIVE
        current_dai_settings_value = vHalCommon.getVhalInt32(DAI_SETTING)
        current_dai_settings_signal_visual = fr.get_DriveAwayInfoActvReq()
        current_dai_settings_signal_visual_sound = fr.get_DriveAwayInfoSoundWarnActvReq()

        vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(vehmod)

        # Set setting to OFF
        dai_off.touch()
        vc.sleep(_s)
        vHalCommon.assertVhalInt32Equal(current_dai_settings_value, DAI_SETTING)
        asserts.assertEqual(fr.get_DriveAwayInfoActvReq(), current_dai_settings_signal_visual)
        asserts.assertEqual(fr.get_DriveAwayInfoSoundWarnActvReq(), current_dai_settings_signal_visual_sound)

        # Set setting to VISUAL
        dai_visual.touch()
        vc.sleep(_s)
        vHalCommon.assertVhalInt32Equal(current_dai_settings_value, DAI_SETTING)
        asserts.assertEqual(fr.get_DriveAwayInfoActvReq(), current_dai_settings_signal_visual)
        asserts.assertEqual(fr.get_DriveAwayInfoSoundWarnActvReq(), current_dai_settings_signal_visual_sound)

        # Set setting to VISUAL AND SOUND
        dai_visual_sound.touch()
        vc.sleep(_s)
        vHalCommon.assertVhalInt32Equal(current_dai_settings_value, DAI_SETTING)
        asserts.assertEqual(fr.get_DriveAwayInfoActvReq(), current_dai_settings_signal_visual)
        asserts.assertEqual(fr.get_DriveAwayInfoSoundWarnActvReq(), current_dai_settings_signal_visual_sound)

        # Close applicationa and go back to home screen
        self.dut.adb.shell('input keyevent 3')
        vc.sleep(_s)

    def deviceReboot(self):
        if self.fr is not None:
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

if __name__ == "__main__":
    test_runner.main()