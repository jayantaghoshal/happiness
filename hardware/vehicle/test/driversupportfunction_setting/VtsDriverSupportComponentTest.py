#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
import time

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device

sys.path.append('/usr/local/lib/python2.7/dist-packages')
sys.path.append('/tmp/ihu/.local/lib/python2.7/site-packages')

from generated import datatypes as DE
from vehiclehalcommon import wait_for
import vehiclehalcommon
from uiautomator import device as device

ns_per_ms = 1000000

app_context = "com.volvocars.vehiclefunctions:id/"
ACC_button_on = app_context + "adaptive_cruise_control_on"
CC_button_on = app_context + "cruise_control_on"
SL_button_on = app_context + "speed_limiter_on"
wait_after_button_click = 0.5


class VtsDriverSupportComponentTest(base_test.BaseTestClass):
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()
        self.flexray = vehiclehalcommon.get_dataelements_connection(self.dut.adb)

    def tearDownClass(self):
        try:
            self.flexray.close()
        except:
            pass

    def deviceReboot(self):
        if self.flexray is not None:
            self.flexray.close()
        time.sleep(1)
        self.dut.shell.one.Execute("reboot")
        self.dut.stopServices()
        self.dut.waitForBootCompletion()
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")
        self.flexray = vehiclehalcommon.get_dataelements_connection(self.dut.adb)
        time.sleep(1)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons not present CC36 <= 1
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportCCDisabled1(self):
        self.dut.shell.one.Execute("changecarconfig 23 1")
        self.dut.shell.one.Execute("changecarconfig 36 1")

        # Reboot device after changing carconfig
        self.deviceReboot()

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        vHalCommon.findAllViewWithIds([ACC_button_on, CC_button_on, SL_button_on], device, assert_views_found=False)
        ACC_button = device(resourceId=ACC_button_on)
        CC_button = device(resourceId=CC_button_on)
        SL_button = device(resourceId=SL_button_on)

        # Compare result
        asserts.assertFalse(ACC_button, "ACC_button is still visible")
        asserts.assertFalse(CC_button, "CC_button is still visible")
        asserts.assertFalse(SL_button, "SL_button is still visible")

        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons not present CC36 > 1
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportCCDisabled2(self):
        self.dut.shell.one.Execute("changecarconfig 23 1")
        self.dut.shell.one.Execute("changecarconfig 36 2")

        # Reboot device after changing carconfig
        self.deviceReboot()

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        vHalCommon.findAllViewWithIds([ACC_button_on, CC_button_on, SL_button_on], device, assert_views_found=False)
        ACC_button = device(resourceId=ACC_button_on)
        CC_button = device(resourceId=CC_button_on)
        SL_button = device(resourceId=SL_button_on)

        # Compare result
        asserts.assertFalse(ACC_button, "ACC_button is still visible")
        asserts.assertFalse(CC_button, "CC_button is still visible")
        asserts.assertFalse(SL_button, "SL_button is still visible")

        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.SL)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons present CC23=2 && CC36>1, 2 Buttons CC && SL
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportCCAndSLEnabled(self):
        self.dut.shell.one.Execute("changecarconfig 23 2")
        self.dut.shell.one.Execute("changecarconfig 36 2")

        # Clear database
        self.dut.shell.one.Execute("stop settingstorage-hidl-server")
        self.dut.shell.one.Execute("rm /data/vendor/vehiclesettings/vehiclesettings.db*")
        self.dut.shell.one.Execute("start settingstorage-hidl-server")

        # Reboot device after changing carconfig
        self.deviceReboot()

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        vehmod = vHalCommon.GetDefaultInitiated_VehModMngtGlbSafe1()

        # Get buttons
        vHalCommon.findAllViewWithIds([CC_button_on, SL_button_on], device, assert_views_found=False)
        ACC_button = device(resourceId=ACC_button_on)
        CC_button = device(resourceId=CC_button_on)
        SL_button = device(resourceId=SL_button_on)

        # Compare result
        asserts.assertFalse(ACC_button, "ACC_button is still visible!")
        asserts.assertTrue(CC_button.enabled, "CC_button is not enabled")
        asserts.assertTrue(SL_button.enabled, "SL_button is not enabled")

        DRIVER_SUPPORT_FUNCTION_ON = vHalCommon.get_id('DRIVER_SUPPORT_FUNCTION_ON')

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------
        # UsgModSts ACTIVE
        self.flexray.send_VehModMngtGlbSafe1(vehmod)
        self.flexray.send_CrsCtrlrSts(DE.CrsCtrlrSts1.Off)
        self.flexray.send_AccSts(DE.AccSts1.Stb)
        self.flexray.send_AdjSpdLimnSts(DE.AdjSpdLimnSts2.Off)

        wait_for(self.flexray.get_AdjSpdLimnSts, DE.AdjSpdLimnSts2.Off, timeout_sec=1)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 0)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

        SL_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 2)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.SL)

        # UsgModSts NOT_ACTIVE
        vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        self.flexray.send_VehModMngtGlbSafe1(vehmod)

        # TODO: Fix when HMI handles Status. Shall expect DE.DrvrSpprtFct.ACC instead of CC.
        CC_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 2)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

        SL_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 2)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

        # UsgModeSts ACTIVE
        vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        self.flexray.send_VehModMngtGlbSafe1(vehmod)
        self.flexray.send_CrsCtrlrSts(DE.CrsCtrlrSts1.Actv)
        self.flexray.send_AccSts(DE.AccSts1.Actv)
        self.flexray.send_AdjSpdLimnSts(DE.AdjSpdLimnSts2.Actv)

        SL_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 0)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons present CC23>2 && CC36<=1, 2 Buttons CC && ACC
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportACCandCCEnabled(self):
        self.dut.shell.one.Execute("changecarconfig 23 3")
        self.dut.shell.one.Execute("changecarconfig 36 1")

        # Clear database
        self.dut.shell.one.Execute("stop settingstorage-hidl-server")
        self.dut.shell.one.Execute("rm /data/vendor/vehiclesettings/vehiclesettings.db*")
        self.dut.shell.one.Execute("start settingstorage-hidl-server")

        # Reboot device after changing carconfig
        self.deviceReboot()

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        vehmod = vHalCommon.GetDefaultInitiated_VehModMngtGlbSafe1()

        # Get buttons
        vHalCommon.findAllViewWithIds([ACC_button_on, CC_button_on], device, assert_views_found=False)
        ACC_button = device(resourceId=ACC_button_on)
        CC_button = device(resourceId=CC_button_on)
        SL_button = device(resourceId=SL_button_on)

        # Compare result
        asserts.assertTrue(ACC_button.enabled, "ACC_button is not enabled")
        asserts.assertTrue(CC_button.enabled, "CC_button is not enabled")
        asserts.assertFalse(SL_button, "SL_button is still visible!")

        DRIVER_SUPPORT_FUNCTION_ON = vHalCommon.get_id('DRIVER_SUPPORT_FUNCTION_ON')

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------
        # UsgModSts ACTIVE
        self.flexray.send_VehModMngtGlbSafe1(vehmod)
        self.flexray.send_CrsCtrlrSts(DE.CrsCtrlrSts1.Off)
        self.flexray.send_AccSts(DE.AccSts1.Stb)
        self.flexray.send_AdjSpdLimnSts(DE.AdjSpdLimnSts2.Off)

        wait_for(self.flexray.get_AdjSpdLimnSts, DE.AdjSpdLimnSts2.Off, timeout_sec=1)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 0)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

        ACC_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 1)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.ACC)

        # UsgModSts NOT_ACTIVE
        vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        self.flexray.send_VehModMngtGlbSafe1(vehmod)

        # TODO: Fix when HMI handles Status. Shall expect DE.DrvrSpprtFct.ACC instead of CC.
        CC_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 1)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

        ACC_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 1)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

        # UsgModeSts ACTIVE
        vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        self.flexray.send_VehModMngtGlbSafe1(vehmod)
        self.flexray.send_CrsCtrlrSts(DE.CrsCtrlrSts1.Actv)
        self.flexray.send_AccSts(DE.AccSts1.Actv)
        self.flexray.send_AdjSpdLimnSts(DE.AdjSpdLimnSts2.Actv)

        ACC_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 0)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons present CC23>2 && CC36>1, All 3 Buttons CC,ACC,SL
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportAllEnabled(self):
        self.dut.shell.one.Execute("changecarconfig 23 3")
        self.dut.shell.one.Execute("changecarconfig 36 2")

        # Clear database
        self.dut.shell.one.Execute("stop settingstorage-hidl-server")
        self.dut.shell.one.Execute("rm /data/vendor/vehiclesettings/vehiclesettings.db*")
        self.dut.shell.one.Execute("start settingstorage-hidl-server")

        # Reboot device after changing carconfig
        self.deviceReboot()

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        vehmod = vHalCommon.GetDefaultInitiated_VehModMngtGlbSafe1()

        # Get buttons
        vHalCommon.findAllViewWithIds([ACC_button_on, CC_button_on, SL_button_on], device)
        ACC_button = device(resourceId=ACC_button_on)
        CC_button = device(resourceId=CC_button_on)
        SL_button = device(resourceId=SL_button_on)

        # Compare result
        asserts.assertTrue(ACC_button.enabled, "ACC_button is not enabled")
        asserts.assertTrue(CC_button.enabled, "CC_button is not enabled")
        asserts.assertTrue(SL_button.enabled, "SL_button is not enabled")

        DRIVER_SUPPORT_FUNCTION_ON = vHalCommon.get_id('DRIVER_SUPPORT_FUNCTION_ON')

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------
        # UsgModSts ACTIVE
        self.flexray.send_VehModMngtGlbSafe1(vehmod)
        self.flexray.send_CrsCtrlrSts(DE.CrsCtrlrSts1.Off)
        self.flexray.send_AccSts(DE.AccSts1.Stb)
        self.flexray.send_AdjSpdLimnSts(DE.AdjSpdLimnSts2.Off)

        wait_for(self.flexray.get_AdjSpdLimnSts, DE.AdjSpdLimnSts2.Off, timeout_sec=1)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 0)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.CC)

        SL_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 2)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.SL)

        ACC_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 1)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.ACC)

        # UsgModSts NOT_ACTIVE
        vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        self.flexray.send_VehModMngtGlbSafe1(vehmod)

        # TODO: Fix when HMI handles Status. Shall expect DE.DrvrSpprtFct.ACC instead of SL.
        SL_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 1)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.SL)

        ACC_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 1)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.SL)

        CC_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 1)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.SL)

        # UsgModeSts ACTIVE
        vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        self.flexray.send_VehModMngtGlbSafe1(vehmod)
        self.flexray.send_CrsCtrlrSts(DE.CrsCtrlrSts1.Actv)
        self.flexray.send_AccSts(DE.AccSts1.Actv)
        self.flexray.send_AdjSpdLimnSts(DE.AdjSpdLimnSts2.Actv)

        SL_button.click()
        time.sleep(wait_after_button_click)
        vHalCommon.assert_prop_equals(DRIVER_SUPPORT_FUNCTION_ON, 0)
        asserts.assertEqual(self.flexray.get_UsrSeldDrvrSpprtFct(), DE.DrvrSpprtFct.SL)

if __name__ == "__main__":
    test_runner.main()
