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
    UsgModSts
from vehiclehalcommon import VehicleHalCommon

ns_per_ms = 1000000

class VtsDAISettingsComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        system_uid = results[const.STDOUT][0].strip()

        self.vHalCommon = VehicleHalCommon(self.dut, system_uid)
        self.flexray = FrSignalInterface()

    def testDriveAwayInformationSetting(self):

        self.vHalCommon.setUpVendorExtension()

        # Get buttons
        _s = 1
        vc, device = self.vHalCommon.getViewClient()
        vc.dump(window=-1)
        dai_off = vc.findViewByIdOrRaise("com.volvocars.halmodulesink:id/buttonSettingOff");
        dai_visual = vc.findViewByIdOrRaise("com.volvocars.halmodulesink:id/buttonSettingVisual");
        dai_visual_sound = vc.findViewByIdOrRaise("com.volvocars.halmodulesink:id/buttonSettingVisualAndSound");
        vc.sleep(_s)

        DAI_SETTING = 557842437 # Hard coded VCC-HAL property ID for DAI_SETTING_VALUE

        current_dai_settings_value = self.vHalCommon.readVhalProperty(DAI_SETTING)
        current_dai_settings_signal_visual = self.flexray.DriveAwayInfoActvReq.get()
        current_dai_settings_signal_visual_sound = self.flexray.DriveAwayInfoSoundWarnActvReq.get()

        # Set UsgModSts to NOT_ACTIVE, setting should not change in NOT_ACTIVE
        self.flexray.UsgModSts.send(UsgModSts.map.UsgModInActv)
        vc.sleep(_s)

        # Set setting to OFF
        dai_off.touch()
        vc.sleep(_s)
        asserts.assertEqual(self.vHalCommon.readVhalProperty(DAI_SETTING), current_dai_settings_value, "Value of prop changed in NOT_ACTIVE")
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)

        # Set setting to VISUAL
        dai_visual.touch()
        vc.sleep(_s)
        asserts.assertEqual(self.vHalCommon.readVhalProperty(DAI_SETTING), current_dai_settings_value, "Value of prop changed in NOT_ACTIVE")
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)

        # Set setting to VISUAL AND SOUND
        dai_visual_sound.touch()
        vc.sleep(_s)
        asserts.assertEqual(self.vHalCommon.readVhalProperty(DAI_SETTING), current_dai_settings_value, "Value of prop changed in NOT_ACTIVE")
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)

        # Set UsgModSts to ACTIVE, setting should change in ACTIVE
        vc.sleep(_s)
        self.flexray.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        vc.sleep(_s)

        # Set setting to OFF
        dai_off.touch()
        vc.sleep(_s)
        settings_value = self.vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(self.vHalCommon.extractValue(settings_value), 0, "Failed to set prop to OFF")
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, self.flexray.DriveAwayInfoActvReq.map.Off)
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, self.flexray.DriveAwayInfoSoundWarnActvReq.map.Off)

        # Set setting to VISUAL
        dai_visual.touch()
        vc.sleep(_s)
        settings_value = self.vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(self.vHalCommon.extractValue(settings_value), 1, "Failed to set prop to VISUAL")
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, self.flexray.DriveAwayInfoActvReq.map.On)
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, self.flexray.DriveAwayInfoSoundWarnActvReq.map.Off)

        # Set setting to VISUAL AND SOUND
        dai_visual_sound.touch()
        vc.sleep(_s)
        settings_value = self.vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(self.vHalCommon.extractValue(settings_value), 2, "Failed to set prop to VISUAL AND SOUND")
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, self.flexray.DriveAwayInfoActvReq.map.On)
        self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, self.flexray.DriveAwayInfoSoundWarnActvReq.map.On)

        # Close applicationa and go back to home screen
        self.dut.adb.shell('input keyevent 3')
        vc.sleep(_s)

if __name__ == "__main__":
    test_runner.main()