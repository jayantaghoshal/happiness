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
from vehiclehalcommon import VehicleHalCommon, wait_for, get_dataelements_connection
from generated.datatypes import VehModMngtGlbSafe1, HmiHvacFanLvl, TwliBriSts1, IdPen
from generated import datatypes as DE
from uiautomator import device as device

wait_time_seconds = 1

class VtsFanLevelComponentTest(base_test.BaseTestClass):
    """Testing FanLevel functions"""
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]

        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()

        # Disable the reminder bubble about how to leave full screen in the climate pane
        self.dut.shell.one.Execute("settings put global policy_control \"immersive.preconfirms=*\"")
        self.dut.shell.one.Execute("input keyevent 3") # Home button press
        self.dut.adb.shell("input tap 384 988") # Open climate view

        self.fr = get_dataelements_connection(self.dut.adb)
        self.vHalCommon = VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)

        #Get fan buttons
        self.fan_off = device(resourceId=self.vHalCommon.fan_off)
        self.fan_level_1 = device(resourceId=self.vHalCommon.fan_level_1)
        self.fan_level_2 = device(resourceId=self.vHalCommon.fan_level_2)
        self.fan_level_3 = device(resourceId=self.vHalCommon.fan_level_3)
        self.fan_level_4 = device(resourceId=self.vHalCommon.fan_level_4)
        self.fan_level_5 = device(resourceId=self.vHalCommon.fan_level_5)
        self.fan_max = device(resourceId=self.vHalCommon.fan_max)

        #get properties and zones
        self.prop_fan = self.vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED
        self.zone_center = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER

        # To avoid setting illumination to night mode which will dim the CSD
        self.fr.send_TwliBriSts(TwliBriSts1.Day)
        self.fr.send_ProfPenSts1(IdPen.Prof13)


    def setUp(self):
        self.vehmod = VehModMngtGlbSafe1()
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModNorm
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

    def assertFanLevelUI(self,expected_fan_off = False, expected_fan_one = False, expected_fan_two = False, expected_fan_three = False,\
        expected_fan_four = False, expected_fan_five = False, expected_fan_max = False):
        wait_for(lambda : self.fan_off.selected, expected_fan_off, wait_time_seconds, "ERROR : Fan Level Off UI Value mismatch.")
        wait_for(lambda : self.fan_level_1.selected, expected_fan_one, wait_time_seconds, "ERROR : Fan Level one UI Value mismatch.")
        wait_for(lambda : self.fan_level_2.selected, expected_fan_two, wait_time_seconds, "ERROR : Fan Level two UI Value mismatch.")
        wait_for(lambda : self.fan_level_3.selected, expected_fan_three, wait_time_seconds, "ERROR : Fan Level three UI Value mismatch.")
        wait_for(lambda : self.fan_level_4.selected, expected_fan_four, wait_time_seconds, "ERROR : Fan Level four UI Value mismatch.")
        wait_for(lambda : self.fan_level_5.selected, expected_fan_five, wait_time_seconds, "ERROR : Fan Level five UI Value mismatch.")
        wait_for(lambda : self.fan_max.selected, expected_fan_max, wait_time_seconds, "ERROR : Fan Level max UI Value mismatch.")


    def testFanLevel(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set climate to Active
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        # Set to OFF
        self.fan_off.click()
        self.assertFanLevelUI(True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [0], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Set to Level 1
        self.fan_level_1.click()
        self.assertFanLevelUI(False,True)
        wait_for(lambda : self.fan_off.selected, False, wait_time_seconds, "ERROR : Fan Level UI Value mismatch.")
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [1], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinusMinus, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Set to Level 2
        self.fan_level_2.click()
        self.assertFanLevelUI(False,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [2], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Set to Level 3
        self.fan_level_3.click()
        self.assertFanLevelUI(False,True,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [3], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutoNorm, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Set to Level 4
        self.fan_level_4.click()
        self.assertFanLevelUI(False,True,True,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [4], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutPlus, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Set to Level 5
        self.fan_level_5.click()
        self.assertFanLevelUI(False,True,True,True,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [5], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutPlusPlus, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Set to Max
        self.fan_max.click()
        self.assertFanLevelUI(False,True,True,True,True,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [6], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Max, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

    def testFanLevelAvailability(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set climate to Active
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        # Set to Level 2
        self.fan_level_2.click()
        self.assertFanLevelUI(False,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [2], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Set climate to Inactive
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.Off)

        self.assertFanLevelUI(False,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [7], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Try to set to Level 3 - will not be possible
        self.fan_level_3.click()
        self.assertFanLevelUI(False,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [7], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Set climate to Active - will go back to previous state i.e. 2
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        self.assertFanLevelUI(False,True,True)
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_fan, self.zone_center)['value']['int32Values'], [2], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

if __name__ == "__main__":
    test_runner.main()
