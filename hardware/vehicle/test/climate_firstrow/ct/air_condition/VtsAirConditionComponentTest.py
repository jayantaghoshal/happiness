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
from com.dtmilano.android.viewclient import ViewClient, adbclient
from vehiclehalcommon import \
    VehicleHalCommon, \
    wait_for, \
    get_dataelements_connection
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    HmiHvacFanLvl, \
    HmiCmptmtCoolgReq, \
    TwliBriSts1, \
    IdPen
from generated import datatypes as DE
from uiautomator import device as device
wait_time_seconds = 1

class VtsAirConditionComponentTest(base_test.BaseTestClass):
    """Testing Climate functions"""
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()

        #To move to Main Screen
        self.dut.shell.one.Execute("input keyevent 3")

        # Open climate view
        self.dut.adb.shell("input tap 384 988")

        self.fr = get_dataelements_connection(self.dut.adb)
        self.vHalCommon = VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)

        # To avoid setting illumination to night mode which will dim the CSD
        self.fr.send_TwliBriSts(TwliBriSts1.Day)

        #Get fan buttons
        self.fan_off = device(resourceId=self.vHalCommon.fan_off)
        self.fan_level_2 = device(resourceId=self.vHalCommon.fan_level_2)

        #Get AC button
        self.ac_button = device(resourceId=self.vHalCommon.ac_button)

        #Get Max Defrost button
        self.defrost_button = device(resourceId=self.vHalCommon.defrost_button)

        #get properties and zones
        self.prop_fan = self.vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED
        self.prop_ac = self.vHalCommon.vtypes.VehicleProperty.HVAC_AC_ON
        self.zone_center = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER

    def setUp(self):
        #Prepare VehModMngtGlbSafe to be sent
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
            self.dut.shell.one.Execute("input keyevent 3")
            self.fr.close()
        except:
            pass

    # This function is to verify the UI value, property value and Flexray value for Air Condition.
    def assertAirCondition(self,expected_value):
        wait_for(lambda : self.ac_button.selected, expected_value, wait_time_seconds, "ERROR : Air Condition UI Value mismatch.")
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_ac, self.zone_center)['value']['int32Values'], [expected_value], \
             wait_time_seconds, "ERROR : Air Condition Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiCmptmtCoolgReq(), expected_value, wait_time_seconds, "ERROR : Air Condition FR Value mismatch.")

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Condition Active Cases - When AC is OFF previously
    # ----------------------------------------------------------------------------------------------------------

    def testairConditionActive_OFF(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Initialize the AC to OFF
        vHalCommon.setVhalProperty(self.prop_ac, {"int32Values" : [0]}, self.zone_center)
        self.assertAirCondition(0)

        #Set fan to OFF - AC must be OFF
        self.fan_off.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [0], \
            wait_time_seconds, "ERROR : Fan Property value.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off, wait_time_seconds, "ERROR : Fan Level Value mismatch.")
        self.assertAirCondition(0)

        #Set fan to Level 2 - AC remains to OFF
        self.fan_level_2.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [2], \
            wait_time_seconds, "ERROR : Fan Property value.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level Value mismatch.")
        self.assertAirCondition(0)

        # Set max defrost to ON - AC must be ON with FAN Level-5
        self.defrost_button.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [5], \
            wait_time_seconds, "ERROR : Fan Property value.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutPlusPlus, wait_time_seconds, "ERROR : Fan Level Value mismatch.")
        self.assertAirCondition(1)

        #Set max defrost to OFF - Reset AC to prev state
        self.defrost_button.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [2], \
            wait_time_seconds, "ERROR : Fan Property value.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level Value mismatch.")
        self.assertAirCondition(0)

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Condition Active Cases - When AC is ON previously
    # ----------------------------------------------------------------------------------------------------------
    def testairConditionActive_ON(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Initialize the AC to ON
        vHalCommon.setVhalProperty(self.prop_ac, {"int32Values" : [1]}, self.zone_center)
        self.assertAirCondition(1)

        # Set ac to OFF
        self.ac_button.click()
        self.assertAirCondition(0)

        # Again Set ac to ON (Auto)
        self.ac_button.click()
        self.assertAirCondition(1)

        #Set fan to OFF - AC must be OFF
        self.fan_off.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [0], \
            wait_time_seconds, "ERROR : Fan Property value.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off, wait_time_seconds, "ERROR : Fan Level Value mismatch.")
        self.assertAirCondition(0)

        # Set fan to Level 2 - AC should set to last state
        self.fan_level_2.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [2], \
            wait_time_seconds, "ERROR : Fan Property value.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level Value mismatch.")
        self.assertAirCondition(1)

        # Set max defrost to ON - AC must be ON with FAN Level-5
        self.defrost_button.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [5], \
            wait_time_seconds, "ERROR : Fan Property value.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutPlusPlus, wait_time_seconds, "ERROR : Fan Level Value mismatch.")
        self.assertAirCondition(1)

        #Set max defrost to OFF - Reset AC to prev state
        self.defrost_button.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [2], \
            wait_time_seconds, "ERROR : Fan Property value.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level Value mismatch.")
        self.assertAirCondition(1)

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Condition InActive Cases - When pre-conditions are not full-filled and Last state of AC is OFF
    # ----------------------------------------------------------------------------------------------------------

    def testairConditionInActive_OFF(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Initialize the AC to OFF
        vHalCommon.setVhalProperty(self.prop_ac, {"int32Values" : [0]}, self.zone_center)
        self.assertAirCondition(0)

        #Set up pre-conditions to fail
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.Off)

        #Try To Set the AC ON - Should not work
        self.ac_button.click()
        self.assertAirCondition(0)

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Condition InActive Cases - When pre-conditions are not full-filled and Last state of AC is ON
    # ----------------------------------------------------------------------------------------------------------

    def testairConditionInActive_ON(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Initialize the AC to ON
        vHalCommon.setVhalProperty(self.prop_ac, {"int32Values" : [1]}, self.zone_center)
        self.assertAirCondition(1)

        #Set up pre-conditions to fail
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.Off)

        #AC should be OFF
        self.assertAirCondition(0)

        #Again Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #AC should be ON automatically as last state was ON
        self.assertAirCondition(1)

if __name__ == "__main__":
    test_runner.main()
