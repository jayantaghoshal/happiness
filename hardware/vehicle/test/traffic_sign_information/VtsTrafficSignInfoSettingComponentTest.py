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

import vehiclehalcommon
import threading
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    FctSts2

from generated import datatypes as DE
import logging.config
import logging.handlers

from vehiclehalcommon import VehicleHalCommon, wait_for_signal

from uiautomator import device as device

# Get button Ids for testing.
tsi_rsi_on_id               = VehicleHalCommon.app_context_vehiclefunctions + "tsi_rsi_button_on"
tsi_rsi_off_id              = VehicleHalCommon.app_context_vehiclefunctions + "tsi_rsi_button_off"
tsi_spdCamWarn_on_id        = VehicleHalCommon.app_context_vehiclefunctions + "tsi_spdCamWarn_button_on"
tsi_spdCamWarn_off_id       = VehicleHalCommon.app_context_vehiclefunctions + "tsi_spdCamWarn_button_off"
tsi_spdAlrtVisWarn_on_id    = VehicleHalCommon.app_context_vehiclefunctions + "tsi_spdAlrtVisWarn_button_on"
tsi_spdAlrtVisWarn_off_id   = VehicleHalCommon.app_context_vehiclefunctions + "tsi_spdAlrtVisWarn_button_off"
tsi_spdAlrtSndWarn_on_id    = VehicleHalCommon.app_context_vehiclefunctions + "tsi_spdAlrtSndWarn_button_on"
tsi_spdAlrtSndWarn_off_id   = VehicleHalCommon.app_context_vehiclefunctions + "tsi_spdAlrtSndWarn_button_off"
tsi_offst_plus_id           = VehicleHalCommon.app_context_vehiclefunctions + "tsi_offset_button_plus"
tsi_offst_text_id           = VehicleHalCommon.app_context_vehiclefunctions + "tsi_offset_text"
tsi_offst_minus_id          = VehicleHalCommon.app_context_vehiclefunctions + "tsi_offset_button_minus"
vehicle_function_scrollable_view = VehicleHalCommon.app_context_vehiclefunctions + "recyclerView"

buttonWaitTimeSeconds = 1.5

class VtsTrafficSignInfoSettingComponentTest(base_test.BaseTestClass):

    def setUpClass(self):

        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()
        self.fr = vehiclehalcommon.get_dataelements_connection(self.dut.adb)
        self.vehmod = VehModMngtGlbSafe1()
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModDyno
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        self.vehmod.Chks = 0
        self.vehmod.Cntr = 0
        self.vehmod.CarModSubtypWdCarModSubtyp = 0
        self.vehmod.EgyLvlElecMai = 0
        self.vehmod.EgyLvlElecSubtyp = 0
        self.vehmod.PwrLvlElecMai = 0
        self.vehmod.PwrLvlElecSubtyp = 0
        self.vehmod.FltEgyCnsWdSts = DE.FltEgyCns1.Flt

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
        self.vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid)
        self.vHalCommon.setUpVehicleFunction()

    def testTrafficSignInfoCCDisabledWithoutTSI(self):

        print("============================================================= \
        \n\tTestSuite 1: TSI CarConfig Disabled \
        \n=============================================================\n")

        print("TestCase 1.1: CC Disabled (Without TSI)")
        self.dut.shell.one.Execute("changecarconfig 149 1")
        # Reboot device after changing carconfig
        self.deviceReboot()
        # Setting RoadSgnInfoSts to !SrvRqrd
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.On)
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        # All buttons shall not be available
        self.AssertPropIsNone()
        time.sleep(buttonWaitTimeSeconds)
        self.fr.stop_RoadSgnInfoSts()

        print("TestCase 1.2: CC Disabled (Invalid Region)")
        self.dut.shell.one.Execute("changecarconfig 149 240")
        # Reboot device after changing carconfig
        self.deviceReboot()
        # Setting RoadSgnInfoSts to !SrvRqrd
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.On)
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        # All buttons shall not be available
        self.AssertPropIsNone()
        time.sleep(buttonWaitTimeSeconds)
        self.fr.stop_RoadSgnInfoSts()

    def testTrafficSignInfoCCEnabled_DfltON_NoNVM_V316Above(self):

        print("============================================================= \
        \n\tTestSuite 2: TSI CarConfig Enabled & FactoryReset ON & Without NVM & V316 or above\
        \n=============================================================")
        self.SetCCAndBootDevice(8,5) # V320, IrelandWithSLA

        print("TestCase 2.1: Ensure buttons are Disabled due to usage mode")
        # Ensure all are disabled and prevented from user press
        self.CheckAllBttnsAreEnabled(False)
        time.sleep(buttonWaitTimeSeconds)

        print("TestCase 2.2: Status Enabled by setting UsgMod Active and shall RSI start with True")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        time.sleep(buttonWaitTimeSeconds)
        # Make sure the buttons are enabled because usage mode is Active
        self.CheckAllBttnsAreEnabled(True)
        # Ensure the factory default value for RSI is On if CC149=5
        self.AssertPropAndSignal(1,0,1,0,0)

        print("TestCase 2.3: Turn on Road sign information, Default value for SpdAlrtVis warn is ON")
        self.ScrollAndFindSingleID(1)
        self.rsi_On.click()
        self.AssertPropAndSignal(1,0,1,0,0)

        print("TestCase 2.4: Turn on Speed Camera Warning")
        self.spdCamW_On.click()
        self.AssertPropAndSignal(1,1,1,0,0)

        print("TestCase 2.5: Turn on Speed Alert Sound Warning")
        self.ScrollAndFindSingleID(2)
        self.spdSndW_On.click()
        self.AssertPropAndSignal(1,1,1,1,0)

        print("TestCase 2.6: Set the offset to 5 by pressing plus")
        self.ScrollAndFindSingleID(3)
        self.spdOffst_pls.click()
        self.AssertPropAndSignal(1,1,1,1,5)

        print("TestCase 2.7: Turn off Speed Alert Visual Warning to make sure audio warn goes to OFF")
        self.ScrollAndFindSingleID(2)
        self.spdVisW_Off.click()
        self.AssertPropAndSignal(1,1,0,0,5)

        print("TestCase 2.8: Turn on Speed Alert Visual Warning & make sure everything goes back to ON & offset is 5")
        self.spdVisW_On.click()
        self.AssertPropAndSignal(1,1,1,1,5)

        print("TestCase 2.9: Test cases to ensure max offset dependancy on unit settings")

        print("TestCase 2.10: Turn off just speed camera warning")
        self.ScrollAndFindSingleID(1)
        self.spdCamW_Off.click()
        self.AssertPropAndSignal(1,0,1,1,5)

        print("TestCase 2.11: Turn off road sign info and ensure all off")
        self.rsi_Off.click()
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 2.12: Turn on road sign info and ensure all on except speed camera warning")
        self.rsi_On.click()
        self.AssertPropAndSignal(1,0,1,1,5)

        print("TestCase 2.13: Just make sure the minus button works")
        self.ScrollAndFindSingleID(3)
        self.spdOffst_pls.click()
        self.AssertPropAndSignal(1,0,1,1,10)
        self.spdOffst_mns.click()
        self.AssertPropAndSignal(1,0,1,1,5)

        print("TestCase 2.14: Turn Off roadSgnInfo to ensure that it will not retrieve from NVM")
        self.ScrollAndFindSingleID(1)
        self.rsi_Off.click()
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 2.15: Usage mode to Inactive")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 2.16: Usage mode back to driving to ensure it starts with ON instead of NVM")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.AssertPropAndSignal(1,0,1,1,5)

        print("TestCase 2.17: Set the system error and ensure all are disabled")
        self.ScrollAndFindSingleID(3) #This line helps to find the button again (Getting error without this)
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.SrvRqrd)
        self.CheckAllBttnsAreEnabled(False)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 2.18: Make sure all the values restore after error is cleared")
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.On)
        self.CheckAllBttnsAreEnabled(True)
        self.AssertPropAndSignal(1,0,1,1,5)

    def testTrafficSignInfoCCEnabled_DfltON_NVM_V316Above(self):

        print("============================================================= \
        \n\tTestSuite 3: TSI CarConfig Enabled & FactoryReset ON & With NVM & V316 or above\
        \n=============================================================")
        self.SetCCAndBootDevice(7,19) # V316, CanadaWithSLA

        print("TestCase 3.1: Ensure buttons are Disabled due to usage mode")
        # Ensure all are disabled and prevented from user press
        self.CheckAllBttnsAreEnabled(False)

        print("TestCase 3.2: Status Enabled by setting UsgMod Active and shall RSI and visual warn start with True")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        # Make sure the buttons are enabled because usage mode is Active
        self.CheckAllBttnsAreEnabled(True)
        # Ensure the factory default value for RSI is On if CC149=19
        self.AssertPropAndSignal(1,0,1,0,0)

        print("TestCase 3.3: Turn on Speed Camera Warning")
        self.ScrollAndFindSingleID(1)
        self.spdCamW_On.click()
        self.AssertPropAndSignal(1,1,1,0,0)

        print("TestCase 3.3: Turn off visual warning")
        self.ScrollAndFindSingleID(2)
        self.spdVisW_Off.click()
        self.AssertPropAndSignal(1,1,0,0,0)

        print("TestCase 3.4: Turn off RSI")
        self.ScrollAndFindSingleID(1)
        self.rsi_Off.click()
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 3.5: Change the usage mode to inactive")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 3.6: Change usage mode to driving and ensure is shall retrieve from NVM instead of default ON")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 3.7: Turn on road sign info and ensure the speedcam warning will also turn on")
        self.rsi_On.click()
        self.AssertPropAndSignal(1,1,0,0,0)

    def testTrafficSignInfoCCEnabled_DfltON_NVM_V316Below(self):

        print("============================================================= \
        \n\tTestSuite 4: TSI CarConfig Enabled & FactoryReset ON & With NVM & Below V316\
        \n=============================================================")
        self.SetCCAndBootDevice(6,39) # V541, JapanWithSLA

        print("TestCase 4.1: Ensure buttons are Disabled due to usage mode")
        # Ensure all are disabled and prevented from user press
        self.CheckAllBttnsAreEnabled(False)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 4.2: Set the system error and ensure all are disabled")
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.SrvRqrd)
        self.CheckAllBttnsAreEnabled(False)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 4.3: Make sure the status is still disabed even after the error is cleared")
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.On)
        self.CheckAllBttnsAreEnabled(False)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 4.4: Status Enabled by setting UsgMod Active and shall RSI& VisWarn start with True")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        # Make sure the buttons are enabled because usage mode is Active
        self.CheckAllBttnsAreEnabled(True)
        # Ensure the factory default value for RSI is On if CC149=39
        self.AssertPropAndSignal(1,0,1,0,0)

        print("TestCase 4.5: Increase offset and make sure the maximum is 20 for variants below V316")
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,0,1,0,10)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,0,1,0,20)

    def testTrafficSignInfoCCEnabled_DfltOFF_NVM_V316Above(self):

        print("============================================================= \
        \n\tTestSuite 5: TSI CarConfig Enabled & FactoryReset OFF & With NVM & V316 or above\
        \n=============================================================")
        self.SetCCAndBootDevice(7,20) # V316, SingaporeWithoutSLA

        print("TestCase 5.1: Status Enabled by setting UsgMod Active and shall RSI start with false")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        time.sleep(buttonWaitTimeSeconds)
        # Make sure the buttons are enabled because usage mode is Active
        self.CheckAllBttnsAreEnabled(True)
        # Ensure the factory default value for RSI is Off if CC149=20
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 5.2: Turn on RSI and also VisWarn factory value is ON")
        self.ScrollAndFindSingleID(1)
        self.rsi_On.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,0,1,0,0)

        print("TestCase 5.3: Turn off VisWarn")
        self.ScrollAndFindSingleID(2)
        self.spdVisW_Off.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,0,0,0,0)

        print("TestCase 5.4: Turn on Speed Camera Warning")
        self.ScrollAndFindSingleID(1)
        self.spdCamW_On.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,1,0,0,0)

        print("TestCase 5.5: Turn off RSI")
        self.rsi_Off.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 5.6: Change the usage mode to inactive")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 5.7: Change usage mode to driving and ensure is shall retrieve from NVM instead of default ON")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 5.8: Turn on RSI and ensure speed cam warn turns on")
        self.rsi_On.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,1,0,0,0)

        print("TestCase 5.9: Turn on visual speed warning")
        self.ScrollAndFindSingleID(2)
        self.spdVisW_On.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,1,1,0,0)

        print("TestCase 5.10: Ensure the max offset is only 10")
        self.ScrollAndFindSingleID(3)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,1,1,0,10)

    def testTrafficSignInfoCCEnabled_DfltOFF_NVM_V316Below(self):

        print("============================================================= \
        \n\tTestSuite 6: TSI CarConfig Enabled & FactoryReset OFF & With NVM & Below V316 \
        \n=============================================================")
        self.SetCCAndBootDevice(5,42)#V426, INDIA_TSI_without_SLA

        print("TestCase 6.1: Ensure buttons are Disabled due to usage mode")
        # Ensure all are disabled and prevented from user press
        self.ScrollAndFindSingleID(1)
        self.rsi_On.click()
        self.spdCamW_On.click()
        self.ScrollAndFindSingleID(2)
        self.spdVisW_On.click()
        self.spdSndW_On.click()
        self.ScrollAndFindSingleID(3)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.CheckAllBttnsAreEnabled(False)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 6.2: Status Enabled by setting UsgMod Active and shall RSI start with false")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        # Make sure the buttons are enabled because usage mode is Active
        self.CheckAllBttnsAreEnabled(True)
        # Ensure the factory default value for RSI is On if CC149=42
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 6.3: Turn on RSI")
        self.ScrollAndFindSingleID(1)
        self.rsi_On.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,0,1,0,0)

        print("TestCase 6.3: Turn off VisSpeedWarning")
        self.ScrollAndFindSingleID(2)
        self.spdVisW_Off.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,0,0,0,0)

        print("TestCase 6.4: Turn on Speed Camera Warning")
        self.ScrollAndFindSingleID(1)
        self.spdCamW_On.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,1,0,0,0)

        print("TestCase 6.5: Turn off RSI")
        self.rsi_Off.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 6.6: Change the usage mode to inactive")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 6.7: Change usage mode to driving and ensure is shall retrieve from NVM instead of default ON")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 6.8: Turn on RSI and ensure speed cam warn turns on")
        self.rsi_On.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,1,0,0,0)

        print("TestCase 6.9: Turn on visual speed warning")
        self.ScrollAndFindSingleID(2)
        self.spdVisW_On.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,1,1,0,0)

        print("TestCase 6.10: Ensure the max offset is only 20")
        self.ScrollAndFindSingleID(3)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.spdOffst_pls.click()
        time.sleep(buttonWaitTimeSeconds)
        self.AssertPropAndSignal(1,1,1,0,20)

        print("TestCase 6.11: Set the system error and ensure all are disabled")
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.SrvRqrd)
        self.CheckAllBttnsAreEnabled(False)
        self.AssertPropAndSignal(0,0,0,0,0)

        print("TestCase 6.12: Make sure all the values restore after error is cleared")
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.On)
        self.CheckAllBttnsAreEnabled(True)
        self.AssertPropAndSignal(1,1,1,0,20)

    def SetCCAndBootDevice(self,CC1, CC149):
        # Set carconfigs
        self.dut.shell.one.Execute("changecarconfig 1 "+str(CC1))
        self.dut.shell.one.Execute("changecarconfig 149 "+str(CC149))
        # Clear the setting database
        self.dut.shell.one.Execute("stop settingstorage-hidl-server")
        self.dut.shell.one.Execute("rm /data/vendor/vehiclesettings/vehiclesettings.db*")
        self.dut.shell.one.Execute("start settingstorage-hidl-server")
        # Reboot device after changing carconfig
        self.deviceReboot()
        # set usage mode to inactive
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        self.fr.send_RoadSgnInfoSts(DE.FctSts2.On)
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        time.sleep(buttonWaitTimeSeconds)
        self.ScrollAndFindViewForTSI()

    def AssertPropAndSignal(self, RsiSts, SpdCamWarnSts, SpdAlrtVisWarnSts, SpdAlrtSndWarnSts, SpdAlrtOffst):
        # Get VHAL properties
        TSI_RSI_VHAL                    = self.vHalCommon.get_id('TSI_RSI_ON')
        TSI_SPEEDCAM_AUDIO_WARN_VHAL    = self.vHalCommon.get_id('TSI_SPEEDCAM_AUDIO_WARN_ON')
        TSI_SPEED_VISUAL_WARN_VHAL      = self.vHalCommon.get_id('TSI_SPEED_VISUAL_WARN_ON')
        TSI_SPEED_AUDIO_WARN_VHAL       = self.vHalCommon.get_id('TSI_SPEED_AUDIO_WARN_ON')
        TSI_SPEED_WARN_OFFSET_VHAL      = self.vHalCommon.get_id('TSI_SPEED_WARN_OFFSET')
        # Assert properties and FR signals
        time.sleep(buttonWaitTimeSeconds)
        self.vHalCommon.assert_prop_equals(TSI_RSI_VHAL, RsiSts, "RSI")
        asserts.assertEqual(self.fr.get_RoadSgnInfoActv().Sts, RsiSts, "RoadSignInfo expected is "+str(RsiSts))
        self.vHalCommon.assert_prop_equals(TSI_SPEEDCAM_AUDIO_WARN_VHAL, SpdCamWarnSts, "CAM")
        asserts.assertEqual(self.fr.get_SpdCameraInfoSeldForRoadSgnInfo().Sts, SpdCamWarnSts, "SpdCamWarn expected is "+str(SpdCamWarnSts))
        self.vHalCommon.assert_prop_equals(TSI_SPEED_VISUAL_WARN_VHAL, SpdAlrtVisWarnSts, "VIS")
        asserts.assertEqual(self.fr.get_SpdAlrmActvForRoadSgnInfo().Sts, SpdAlrtVisWarnSts, "SpeedAlert expected is "+str(SpdAlrtVisWarnSts))
        self.vHalCommon.assert_prop_equals(TSI_SPEED_AUDIO_WARN_VHAL, SpdAlrtSndWarnSts, "SND")
        # self.vHalCommon.assertVhalInt32Equal(self.vHalCommon.vtypes.VehicleProperty.TSI_SPEED_WARN_OFFSET_VHAL, SpdAlrtOffst)
        self.vHalCommon.assertVhalInt32Equal(SpdAlrtOffst, TSI_SPEED_WARN_OFFSET_VHAL)
        asserts.assertEqual(self.fr.get_OffsForSpdWarnSetg().Sts, SpdAlrtOffst, "SpeedAlert expected is "+str(SpdAlrtOffst))

    def AssertPropIsNone(self):
        # Get button by ID
        self.rsi_Off     = self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_rsi_off_id, device,5)
        self.rsi_On      = self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_rsi_on_id, device,5)
        self.spdCamW_On  = self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_spdCamWarn_on_id, device,1)
        self.spdCamW_Off = self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_spdCamWarn_off_id, device,1)
        self.spdVisW_On  = self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_spdAlrtVisWarn_on_id, device,1)
        self.spdVisW_Off = self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_spdAlrtVisWarn_off_id, device,1)
        self.spdSndW_On  = self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_spdAlrtSndWarn_on_id,device, 1)
        self.spdSndW_Off = self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_spdAlrtSndWarn_off_id,device, 1)
        self.spdOffst_pls= self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_offst_plus_id, device,1)
        self.spdOffst_txt= self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_offst_text_id,device, 1)
        self.spdOffst_mns= self.vHalCommon.scrollDownAndFindViewByIdUiAutomator(tsi_offst_minus_id,device, 1)
        print("--- RoadSignInfo Off -> ") + str(self.rsi_Off)
        print("--- RoadSignInfo On -> ") + str(self.rsi_On)
        asserts.assertEqual(None, self.rsi_Off, "Expect RSIbuttonOff NotAvailable")
        asserts.assertEqual(None, self.rsi_On, "Expect RSIbuttonOn NotAvailable")
        print("--- SpeedCamWarn Off -> ") + str(self.spdCamW_Off)
        print("--- SpeedCamWarn On -> ") + str(self.spdCamW_On)
        asserts.assertEqual(None, self.spdCamW_Off, "Expect SpdCamWarnOff NotAvailable")
        asserts.assertEqual(None, self.spdCamW_On, "Expect SpdCamWarnOn NotAvailable")
        print("--- SpeedAlertVisWarn Off -> ") + str(self.spdVisW_Off)
        print("--- SpeedAlertVisWarn On -> ") + str(self.spdVisW_On)
        asserts.assertEqual(None, self.spdVisW_Off, "Expect SpdAlrtVisWarnOff NotAvailable")
        asserts.assertEqual(None, self.spdVisW_On, "Expect SpdAlrtVisWarnOn NotAvailable")
        print("--- SpeedAlertSndWarn Off -> ") + str(self.spdSndW_Off)
        print("--- SpeedAlertSndWarn On -> ") + str(self.spdSndW_On)
        asserts.assertEqual(None, self.spdSndW_Off, "Expect SpdAlrtSndWarnOff NotAvailable")
        asserts.assertEqual(None, self.spdSndW_On, "Expect SpdAlrtSndWarnOn NotAvailable")
        print("--- SpeedAlertOffset Plus -> ") + str(self.spdOffst_pls)
        print("--- SpeedAlertOffset Text -> ") + str(self.spdOffst_txt)
        print("--- SpeedAlertOffset Minus -> ") + str(self.spdOffst_mns)
        asserts.assertEqual(None, self.spdOffst_pls, "Expect SpdAlrtOffstPls NotAvailable")
        asserts.assertEqual(None, self.spdOffst_txt, "Expect SpdAlrtOffstTxt NotAvailable")
        asserts.assertEqual(None, self.spdOffst_mns, "Expect SpdAlrtOffstMns NotAvailable")

    def CheckAllBttnsAreEnabled(self,IsEnabled):
        if IsEnabled:
            MsgStr = "not enabled"
        else:
            MsgStr = "not disabled"
        self.ScrollAndFindSingleID(1)
        asserts.assertEqual(self.rsi_Off.enabled, IsEnabled, "RSI Off button is "+MsgStr)
        asserts.assertEqual(self.rsi_On.enabled, IsEnabled, "RSI On button is "+MsgStr)
        asserts.assertEqual(self.spdCamW_Off.enabled, IsEnabled, "SpeedCamWarn Off button is "+MsgStr)
        asserts.assertEqual(self.spdCamW_On.enabled, IsEnabled, "SpeedCamWarn On button is "+MsgStr)
        self.ScrollAndFindSingleID(2)
        asserts.assertEqual(self.spdVisW_Off.enabled, IsEnabled, "SpeedAlertVisWarn Off button is "+MsgStr)
        asserts.assertEqual(self.spdVisW_On.enabled, IsEnabled, "SpeedAlertVisWarn On button is "+MsgStr)
        asserts.assertEqual(self.spdSndW_Off.enabled, IsEnabled, "SpeedAlertSndWarn Off button is "+MsgStr)
        asserts.assertEqual(self.spdSndW_On.enabled, IsEnabled, "SpeedAlertSndWarn On button is "+MsgStr)
        self.ScrollAndFindSingleID(3)
        asserts.assertEqual(self.spdOffst_pls.enabled, IsEnabled, "SpeedAlert offset plus button is "+MsgStr)
        asserts.assertEqual(self.spdOffst_mns.enabled, IsEnabled, "SpeedAlert offset minus button is "+MsgStr)

    def ScrollAndFindViewForTSI(self):
        scrollFlings = 15
        # Get button by ID
        self.rsi_Off     = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_rsi_off_id, device,scrollFlings)
        self.rsi_On      = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_rsi_on_id, device,scrollFlings)
        self.spdCamW_On  = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdCamWarn_on_id, device,scrollFlings)
        self.spdCamW_Off = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdCamWarn_off_id, device,scrollFlings)
        self.spdVisW_On  = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtVisWarn_on_id, device,scrollFlings)
        self.spdVisW_Off = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtVisWarn_off_id, device,scrollFlings)
        self.spdSndW_On  = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtSndWarn_on_id,device, scrollFlings)
        self.spdSndW_Off = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtSndWarn_off_id, device,scrollFlings)
        self.spdOffst_pls= self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_offst_plus_id, device,scrollFlings)
        self.spdOffst_txt= self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_offst_text_id, device,scrollFlings)
        self.spdOffst_mns= self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_offst_minus_id, device,scrollFlings)
        self.prevGrp     = 3

    def ScrollAndFindSingleID(self,HorzGrp):
        if HorzGrp == 1:
            if (self.prevGrp == 2 or self.prevGrp == 3):
                self.rsi_On     = self.vHalCommon.scrollUpAndFindViewByIdOrRaiseUiAutomator(tsi_rsi_on_id, device, 3)
                self.rsi_Off    = self.vHalCommon.scrollUpAndFindViewByIdOrRaiseUiAutomator(tsi_rsi_off_id, device, 3)
                self.spdCamW_On = self.vHalCommon.scrollUpAndFindViewByIdOrRaiseUiAutomator(tsi_spdCamWarn_on_id, device, 3)
                self.spdCamW_Off= self.vHalCommon.scrollUpAndFindViewByIdOrRaiseUiAutomator(tsi_spdCamWarn_off_id,device, 3)
        elif HorzGrp == 2:
            if self.prevGrp == 1:
                self.spdVisW_On  = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtVisWarn_on_id,device, 3)
                self.spdVisW_Off = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtVisWarn_off_id,device, 3)
                self.spdSndW_On  = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtSndWarn_on_id,device, 3)
                self.spdSndW_Off = self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtSndWarn_off_id,device, 3)
            elif self.prevGrp == 3:
                self.spdVisW_On  = self.vHalCommon.scrollUpAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtVisWarn_on_id,device, 3)
                self.spdVisW_Off = self.vHalCommon.scrollUpAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtVisWarn_off_id,device, 3)
                self.spdSndW_On  = self.vHalCommon.scrollUpAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtSndWarn_on_id,device, 3)
                self.spdSndW_Off = self.vHalCommon.scrollUpAndFindViewByIdOrRaiseUiAutomator(tsi_spdAlrtSndWarn_off_id,device, 3)
        elif HorzGrp == 3:
            if (self.prevGrp == 1 or self.prevGrp == 2):
                self.spdOffst_pls= self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_offst_plus_id,device, 3)
                self.spdOffst_txt= self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_offst_text_id, device,3)
                self.spdOffst_mns= self.vHalCommon.scrollDownAndFindViewByIdOrRaiseUiAutomator(tsi_offst_minus_id, device,3)
        self.prevGrp = HorzGrp

if __name__ == "__main__":
    test_runner.main()