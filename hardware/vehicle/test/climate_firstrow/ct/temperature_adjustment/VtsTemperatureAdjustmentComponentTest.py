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

# from com.dtmilano.android.viewclient import ViewClient
from fdx import fdx_client
from fdx import fdx_description_file_parser
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    HmiCmptmtTSpSpcl
from generated import datatypes as DE
from vehiclehalcommon import \
    VehicleHalCommon, \
    wait_for, \
    get_dataelements_connection

temperature_low = 16.5
temperature_high = 27.5
wait_time_seconds = 3



class VtsTemperatureAdjustmentComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()

        self.fr = get_dataelements_connection(self.dut.adb)
        self.vHalCommon = VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        self.vc, self.device = self.vHalCommon.getViewClient()

        # Open climate view
        self.device.touchDip(384.0, 988.0, 0)
        self.vc.dump()

        #Get plus and minus buttons
        left_controller = self.vc.findViewByIdOrRaise(self.vHalCommon.temperature_controller_left)
        self.left_plus_button = self.vc.findViewByIdOrRaise(self.vHalCommon.temperature_plus_button, left_controller)
        self.left_minus_button = self.vc.findViewByIdOrRaise(self.vHalCommon.temperature_minus_button, left_controller)
        right_controller = self.vc.findViewByIdOrRaise(self.vHalCommon.temperature_controller_right)
        self.right_plus_button = self.vc.findViewByIdOrRaise(self.vHalCommon.temperature_plus_button,right_controller)
        self.right_minus_button = self.vc.findViewByIdOrRaise(self.vHalCommon.temperature_minus_button,right_controller)

        #get properties and zones
        self.prop_temp = self.vHalCommon.vtypes.VehicleProperty.HVAC_TEMPERATURE_SET
        self.zone_left = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_LEFT
        self.zone_right = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_RIGHT


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

    def assertTemperatureLeft(self, expected_value):
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_temp, self.zone_left)['value']['floatValues'], [expected_value], \
             wait_time_seconds, "Temperature Left Property error.")
        wait_for(lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpForRowFirstLe, expected_value, wait_time_seconds, "Temperature Left Flexray error.")

        #If the temperature is high or low, we should send a special flexray signal.
        if(expected_value == temperature_high):
            wait_for(lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpSpclForRowFirstLe, HmiCmptmtTSpSpcl.Hi, wait_time_seconds, "Temperature Left Flexray High/Low error")
        if(expected_value == temperature_low):
            wait_for(lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpSpclForRowFirstLe, HmiCmptmtTSpSpcl.Lo, wait_time_seconds, "Temperature Left Flexray High/Low error")

    def assertTemperatureRight(self, expected_value):
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_temp, self.zone_right)['value']['floatValues'], [expected_value], \
            wait_time_seconds, "Temperature Right Property error.")
        wait_for(lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpForRowFirstRi, expected_value, wait_time_seconds, "Temperature Right Flexray error.")

        #If the temperature is high or low, we should send a special flexray signal.
        if(expected_value == temperature_high):
            wait_for(lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpSpclForRowFirstRi, HmiCmptmtTSpSpcl.Hi, wait_time_seconds, "Temperature Right Flexray High/Low error")
        if(expected_value == temperature_low):
            wait_for(lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpSpclForRowFirstRi, HmiCmptmtTSpSpcl.Lo, wait_time_seconds, "Temperature Right Flexray High/Low error")

    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Left Increase
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentLeftIncrease(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        #Set up FlexRay pre-conditions to pass
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Initialize the left temperature to 23 degrees
        vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [23]}, self.zone_left)
        self.assertTemperatureLeft(23)

        #Increase the left temperature to 27 degrees by pressing the plus button
        for temp_value in range(24,28):
            self.left_plus_button.touch()
            self.assertTemperatureLeft(temp_value)

        #Increase the left temperature to HIGH by pressing the plus button
        self.left_plus_button.touch()
        self.assertTemperatureLeft(27.5)

    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Left Decrease
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentLeftDecrease(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        #Set up FlexRay pre-conditions to pass
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Reset the left temperature to 23 degrees
        vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [23]}, self.zone_left)
        self.assertTemperatureLeft(23)

        #Decrease the left temperature to 17 degrees by pressing the minus button
        for temp_value in range(22,16,-1):
            self.left_minus_button.touch()
        self.assertTemperatureLeft(temp_value)

        #Decrease the left temperature to LOW by pressing the minus button
        self.left_minus_button.touch()
        self.assertTemperatureLeft(16.5)

    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Right Increase
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentRightIncrease(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        #Set up FlexRay pre-conditions to pass
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Initialize the right temperature to 23 degrees
        vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [23]}, self.zone_right)
        self.assertTemperatureRight(23)

        #Increase the right temperature to 27 degrees by pressing the plus button
        for temp_value in range(24,28):
            self.right_plus_button.touch()
            self.assertTemperatureRight(temp_value)

        #Increase the right temperature to HIGH by pressing the plus button
        self.right_plus_button.touch()
        self.assertTemperatureRight(27.5)

    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Right Decrease
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentRightDecrease(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        #Set up FlexRay pre-conditions to pass
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Reset the right temperature to 23 degrees
        vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [23]}, self.zone_right)
        self.assertTemperatureRight(23)

        #Decrease the right temperature to 17 degrees by pressing the minus button
        for temp_value in range(22,16,-1):
            self.right_minus_button.touch()
            self.assertTemperatureRight(temp_value)

        #Decrease the right temperature to LOW by pressing the plus button
        self.right_minus_button.touch()
        self.assertTemperatureRight(16.5)

    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Disabled (does not fulfill pre-conditions)
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentDisabled(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        #Reset the temperatures to 23 degrees
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [23]}, self.zone_left)
        vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [23]}, self.zone_right)

        self.assertTemperatureLeft(23)
        self.assertTemperatureRight(23)

        #Set up FlexRay pre-conditions to fail
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.Off)

        #Press the plus buttons with no effect
        self.left_plus_button.touch()
        self.right_plus_button.touch()
        self.assertTemperatureLeft(23)
        self.assertTemperatureRight(23)

        #Press the minus buttons with no effect
        self.left_minus_button.touch()
        self.right_minus_button.touch()
        self.assertTemperatureLeft(23)
        self.assertTemperatureRight(23)


if __name__ == "__main__":
    test_runner.main()