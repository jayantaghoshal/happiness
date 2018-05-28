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
from vehiclehalcommon import \
    VehicleHalCommon, \
    wait_for, \
    get_dataelements_connection
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    ActrReq, \
    HmiHvacFanLvl
from generated import datatypes as DE
from uiautomator import device as device

wait_time_seconds = 1
wait_for_button_to_be_available = 0.2

class VtsMaxDefrosterComponentTest(base_test.BaseTestClass):
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
        self.dut.adb.shell("input tap 384 988") # Open climate view

        self.fr = get_dataelements_connection(self.dut.adb)
        self.vHalCommon = VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)

        #Get fan buttons
        self.fan_off = device(resourceId=self.vHalCommon.fan_off)
        self.fan_level_2 = device(resourceId=self.vHalCommon.fan_level_2)
        self.fan_level_3 = device(resourceId=self.vHalCommon.fan_level_3)

        #Get Max Defrost button
        self.defrost_button = device(resourceId=self.vHalCommon.defrost_button)

        #Get Air Distribution VENT button
        self.air_dist_middle = device(resourceId=self.vHalCommon.air_dist_middle)

        #get properties and zones
        self.prop_fan = self.vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED
        self.prop_maxdefroster = self.vHalCommon.vtypes.VehicleProperty.HVAC_MAX_DEFROST_ON
        self.zone_center = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER

    def setUp(self):
        #Prepare VehModMngtGlbSafe to be sent
        self.vehmod = self.vHalCommon.GetDefaultInitiated_VehModMngtGlbSafe1()

    def tearDownClass(self):
        try:
            self.dut.shell.one.Execute("input keyevent 3")
            self.fr.close()
        except:
            pass

    # This function is to verify the UI value, VHAL value and Flexray value for Max Defroster.
    def assertMaxDefroster(self,expected_value):
        wait_for(lambda : self.defrost_button.selected, expected_value, wait_time_seconds, "ERROR : Max Defroster UI Value mismatch.")
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_maxdefroster, self.zone_center)['value']['int32Values'], [expected_value], \
             wait_time_seconds, "ERROR : Max Defroster VHAL value.")
        wait_for(lambda : self.fr.get_HmiDefrstMaxReq(), expected_value, wait_time_seconds, "ERROR : Max Defroster FR Value mismatch.")

    # ----------------------------------------------------------------------------------------------------------
    # Test Max Defroster On/Off Cases with Valid Pre-conditions and other functional dependency.
    #
    # Few are already covered in other VTS corresponding to Max Defrost On/Off.
    # For example :
    #        AC and Fan level assert check is already covered in VTS for air conditon
    #        Temp sync asset check is covered in Temp adjustment.
    #        Air Dist assert check is covered in Air Distribution.
    # ----------------------------------------------------------------------------------------------------------

    def testMaxDefrosterOnOff(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)

        #Initialize the max defroster to OFF
        vHalCommon.setVhalProperty(self.prop_maxdefroster, {"int32Values" : [0]}, self.zone_center)
        self.assertMaxDefroster(ActrReq.Off)

        #Set the max defroster to ON
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.On)
        #TODO : Assert Manual Recirec is OFF When User sets the Max defroster to ON.As Manual Recirec is not implemented yet.

        #Set the max defroster to OFF
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.Off)
        #TODO : Assert Manual Recirec is set to prev value When User sets the Max defroster to OFF.As Manual Recirec is not implemented yet.

    # ----------------------------------------------------------------------------------------------------------
    # Test that the previous fan level value 2 is restored after
    # the sequence Max defroster on, change fan level to 3, max defroster off
    # ----------------------------------------------------------------------------------------------------------
    def testMaxDefrosterPrevFanLevelRestoredAfterOff(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)
        time.sleep(wait_for_button_to_be_available)

        self.fan_level_2.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER)['value']['int32Values'], [2], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Max defroster on
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.On)

        self.fan_level_3.click()
        wait_for(lambda : self.vHalCommon.readVhalProperty(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER)['value']['int32Values'], [3], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutoNorm, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

        # Max defroster off
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.Off)

        # Assert that fan level changes back to 2
        wait_for(lambda : self.vHalCommon.readVhalProperty(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER)['value']['int32Values'], [2], \
             wait_time_seconds, "ERROR : Fan Level Property value mismatch.")
        wait_for(lambda : self.fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus, wait_time_seconds, "ERROR : Fan Level FR Value mismatch.")

    # ----------------------------------------------------------------------------------------------------------
    # Test that Max defroster is active according to activation conditions
    # Active = (UsgModDrvg || ClimaActv) && (CarModDyno || CarModNorm)
    # ----------------------------------------------------------------------------------------------------------
    def testMaxDefrosterActiveCondition(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # -- Check UsgModDrvg AND CarModNorm => Active
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModNorm
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        time.sleep(wait_for_button_to_be_available)

        # Max defroster on
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.On)
        # Max defroster off
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.Off)
        # ----

        # -- Check UsgModDrvg AND CarModDyno => Active
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModDyno
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        time.sleep(wait_for_button_to_be_available)

        # Max defroster on
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.On)
        # Max defroster off
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.Off)
        # ----

        # -- Check !UsgModDrvg AND ClimaActv AND CarModNorm => Active
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModAbdnd
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModNorm
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)
        time.sleep(wait_for_button_to_be_available)

        # Max defroster on
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.On)
        # Max defroster off
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.Off)
        # ----

        # -- Check !UsgModDrvg AND ClimaActv AND CarModCrash => Not Active
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModAbdnd
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModCrash
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)
        time.sleep(wait_for_button_to_be_available)

        # Max defroster shall NOT turn on
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.Off)
        # ----

        # -- Check UsgModAbdnd AND !ClimaActv AND CarModNorm => Not Active
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModAbdnd
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModNorm
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.Off)
        time.sleep(wait_for_button_to_be_available)

        # Max defroster shall NOT turn on
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.Off)
        # ----

        # -- Check UsgModDrvg AND CarModNorm => Active (same as first test,
        # this is to check recovery from Not active is possible)
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModNorm
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        time.sleep(wait_for_button_to_be_available)

        # Max defroster on
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.On)
        # Max defroster off
        self.defrost_button.click()
        self.assertMaxDefroster(ActrReq.Off)
        # ----

    # ----------------------------------------------------------------------------------------------------------
    # Test Case When Max Defroster should be set to Off automatically.
    # ----------------------------------------------------------------------------------------------------------

    def testMaxDefrost_AutoSetToOff(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)
        time.sleep(wait_for_button_to_be_available)

        #TODO : When User sets the Auto Climate to ON, Max defroster should set to OFF.
        # As Auto Climate is not implemented yet.

        #Initialize the fan level to 3 and Max Defroster to ON to check against when we set the Fan to Off.
        # Max defroster should set to off automatically.
        self.fan_level_3.click()
        vHalCommon.setVhalProperty(self.prop_maxdefroster, {"int32Values" : [1]}, self.zone_center)
        self.assertMaxDefroster(ActrReq.On)
        self.fan_off.click()
        self.assertMaxDefroster(ActrReq.Off)

        #Again Initialize the Max Defroster to ON to check against Air Distribution
        vHalCommon.setVhalProperty(self.prop_maxdefroster, {"int32Values" : [1]}, self.zone_center)
        self.assertMaxDefroster(ActrReq.On)
        self.air_dist_middle.click()
        self.assertMaxDefroster(ActrReq.Off)

if __name__ == "__main__":
    test_runner.main()
