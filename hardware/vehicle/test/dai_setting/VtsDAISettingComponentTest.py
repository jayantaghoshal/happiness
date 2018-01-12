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
        # if not precondition_utils.CanRunHidlHalTest(
        #     self, self.dut, self.dut.shell.one):
        #     self._skip_all_testcases = True
        #     return

        results = self.dut.shell.one.Execute("id -u system")
        system_uid = results[const.STDOUT][0].strip()
        # logging.info("system_uid: %s", system_uid)

        if self.coverage.enabled:
            self.coverage.LoadArtifacts()
            self.coverage.InitializeDeviceCoverage(self.dut)

        if self.profiling.enabled:
            self.profiling.EnableVTSProfiling(self.dut.shell.one)

        try:
            self.dut.hal.InitHidlHal(
                target_type="vehicle",
                target_basepaths=self.dut.libPaths,
                target_version=2.0,
                target_package="android.hardware.automotive.vehicle",
                target_component_name="IVehicle",
                bits=int(self.abi_bitness))
        except:
            e = str(sys.exc_info()[0])
            asserts.assertTrue(False, "VHAL init fail with error: " + str(e) + " Is VHAL up and running?")
            raise

        self.vehicle = self.dut.hal.vehicle  # shortcut
        self.vehicle.SetCallerUid(system_uid)
        self.vtypes = self.dut.hal.vehicle.GetHidlTypeInterface("types")
        # logging.info("vehicle types: %s", self.vtypes)
        # asserts.assertEqual(0x00ff0000, self.vtypes.VehiclePropertyType.MASK)
        # asserts.assertEqual(0x0f000000, self.vtypes.VehicleArea.MASK)

        self.flexray = FrSignalInterface()
        self.vHalCommon = VehicleHalCommon(self.dut, system_uid)

    def tearDownClass(self):
            """Disables the profiling.

            If profiling is enabled for the test, collect the profiling data
            and disable profiling after the test is done.
            """
            if self._skip_all_testcases:
                return

            if self.profiling.enabled:
                self.profiling.ProcessTraceDataForTestCase(self.dut)
                self.profiling.ProcessAndUploadTraceData()

            if self.coverage.enabled:
                self.coverage.SetCoverageData(dut=self.dut, isGlobal=True)

    def testDriveAwayInformationSetting(self):
        self.dut.adb.shell('input keyevent 3')
        self.dut.adb.shell('am start -n \"com.volvocars.halmodulesink/com.volvocars.halmodulesink.MainActivity\" -a \"android.intent.action.MAIN\" -c \"android.intent.category.LAUNCHER\"')

        _s = 0.5
        kwargs1 = {'ignoreversioncheck': False, 'verbose': True, 'ignoresecuredevice': False, 'serialno': '.*'}
        device, serialno = ViewClient.connectToDeviceOrExit(**kwargs1)
        kwargs2 = {'forceviewserveruse': False, 'useuiautomatorhelper': False, 'ignoreuiautomatorkilled': True,
                   'autodump': False, 'startviewserver': True, 'compresseddump': True}
        vc = ViewClient(device, serialno, **kwargs2)

        # Open menu drawer button
        # Not possible to press with buttonId, using coordinates instead
        device.touchDip(35.0, 110.0, 0)
        vc.sleep(_s)

        # Open vendor extension
        vc.dump(window=-1)
        vendorExtension = vc.findViewWithTextOrRaise("VendorExtension")
        # vendorExtension = vc.findViewByIdOrRaise("com.volvocars.halmodulesink:id/design_menu_item_text");

        vendorExtension.touch()
        vc.sleep(_s)

        # Get buttons
        vc.dump(window=-1)
        dai_off = vc.findViewByIdOrRaise("com.volvocars.halmodulesink:id/buttonSettingOff");
        dai_visual = vc.findViewByIdOrRaise("com.volvocars.halmodulesink:id/buttonSettingVisual");
        dai_visual_sound = vc.findViewByIdOrRaise("com.volvocars.halmodulesink:id/buttonSettingVisualAndSound");
        vc.sleep(_s)

        DAI_SETTING = 557842437 # Hard coded VCC-HAL property ID for DAI_SETTING_VALUE

        # -------------------------------------------------------------------------------------
        # ---------- TODO: Remove comments when the new signaldatabase is integrated ----------
        # -------------------------------------------------------------------------------------

        current_dai_settings_value = self.vHalCommon.readVhalProperty(DAI_SETTING)
        # current_dai_settings_signal_visual = self.flexray.DriveAwayInfoActvReq.get()
        # current_dai_settings_signal_visual_sound = self.flexray.DriveAwayInfoSoundWarnActvReq.get()

        # Set UsgModSts to NOT_ACTIVE, setting should not change in NOT_ACTIVE
        self.flexray.UsgModSts.send(UsgModSts.map.UsgModInActv)
        vc.sleep(_s)

        # Set setting to OFF
        dai_off.touch()
        vc.sleep(_s)
        asserts.assertEqual(self.vHalCommon.readVhalProperty(DAI_SETTING), current_dai_settings_value, "Value of prop changed in NOT_ACTIVE")
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)

        # Set setting to VISUAL
        dai_visual.touch()
        vc.sleep(_s)
        asserts.assertEqual(self.vHalCommon.readVhalProperty(DAI_SETTING), current_dai_settings_value, "Value of prop changed in NOT_ACTIVE")
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)

        # Set setting to VISUAL AND SOUND
        dai_visual_sound.touch()
        vc.sleep(_s)
        asserts.assertEqual(self.vHalCommon.readVhalProperty(DAI_SETTING), current_dai_settings_value, "Value of prop changed in NOT_ACTIVE")
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, current_dai_settings_signal_visual)
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, current_dai_settings_signal_visual_sound)

        # Set UsgModSts to ACTIVE, setting should change in ACTIVE
        vc.sleep(_s)
        self.flexray.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        vc.sleep(_s)

        # Set setting to OFF
        dai_off.touch()
        vc.sleep(_s)
        settings_value = self.vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(self.vHalCommon.extractValue(settings_value), 0, "Failed to set prop to OFF")
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, self.flexray.DriveAwayInfoActvReq.map.Off)
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, self.flexray.DriveAwayInfoSoundWarnActvReq.map.Off)

        # Set setting to VISUAL
        dai_visual.touch()
        vc.sleep(_s)
        settings_value = self.vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(self.vHalCommon.extractValue(settings_value), 1, "Failed to set prop to VISUAL")
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, self.flexray.DriveAwayInfoActvReq.map.On)
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, self.flexray.DriveAwayInfoSoundWarnActvReq.map.Off)

        # Set setting to VISUAL AND SOUND
        dai_visual_sound.touch()
        vc.sleep(_s)
        settings_value = self.vHalCommon.readVhalProperty(DAI_SETTING)
        asserts.assertEqual(self.vHalCommon.extractValue(settings_value), 2, "Failed to set prop to VISUAL AND SOUND")
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoActvReq, self.flexray.DriveAwayInfoActvReq.map.On)
        # self.vHalCommon.assert_signal_equals(self.flexray.DriveAwayInfoSoundWarnActvReq, self.flexray.DriveAwayInfoSoundWarnActvReq.map.On)

        # Close applicationa and go back to home screen
        self.dut.adb.shell('input keyevent 3')
        vc.sleep(_s)

if __name__ == "__main__":
    test_runner.main()