#!/usr/bin/env python
# coding=utf-8

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
from enum import Enum

sys.path.append('/usr/local/lib/python2.7/dist-packages')
sys.path.append('/tmp/ihu/.local/lib/python2.7/site-packages')

from com.dtmilano.android.viewclient import ViewClient
from fdx import fdx_client
from fdx import fdx_description_file_parser
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    HmiCmptmtTSpSpcl

from generated import datatypes as DE
from vehiclehalcommon import \
    VehicleHalCommon, \
    wait_for, \
    wait_for_close_value, \
    get_dataelements_connection

from uiautomator import device as d


temperature_lowest = 17
temperature_highest = 27
wait_time_seconds = 3


# Mirrors the enum defined in types.hal (Vendor extension)
class TemperatureMode(Enum):
    Low = 0
    Normal = 1
    Hi = 2

# For autosar reference:
# HmiCmptmtTSpSpcl
# 0 = Norm
# 1 = Lo
# 2 = Hi

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

        # Open start at home and go to climate view
        self.dut.adb.shell('input keyevent 3')
        time.sleep(1)
        self.dut.adb.shell("input tap 384 988")

        # Get properties and zones
        self.prop_temp = self.vHalCommon.vtypes.VehicleProperty.HVAC_TEMPERATURE_SET
        self.zone_left = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_LEFT
        self.zone_right = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_RIGHT
        self.zone_center = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER
        self.HVAC_TEMPERATURE_MODE = self.vHalCommon.get_id('HVAC_TEMPERATURE_MODE')


    def setUp(self):
        # Setup UI-handles
        self.left_controller = d(resourceId="com.volvocars.launcher:id/temperature_controller_driver")
        self.left_minus_button = self.left_controller.child(resourceId="com.volvocars.launcher:id/minus_button")
        self.left_plus_button = self.left_controller.child(resourceId="com.volvocars.launcher:id/plus_button")
        self.left_temperature_text = self.left_controller.child(resourceId="com.volvocars.launcher:id/temperature_text")

        self.right_controller = d(resourceId="com.volvocars.launcher:id/temperature_controller_passenger")
        self.right_minus_button = self.right_controller.child(resourceId="com.volvocars.launcher:id/minus_button")
        self.right_plus_button = self.right_controller.child(resourceId="com.volvocars.launcher:id/plus_button")
        self.right_temperature_text = self.right_controller.child(resourceId="com.volvocars.launcher:id/temperature_text")

        self.fan_off = d(resourceId="com.volvocars.launcher:id/fan_off")
        self.fan_level_1 = d(resourceId="com.volvocars.launcher:id/fan_one")


        # Setup standard default values
        self.vehmod = self.vHalCommon.GetDefaultInitiated_VehModMngtGlbSafe1()
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        self.fr.send_VehModMngtGlbSafe1(self.vehmod)
        self.fr.send_ClimaActv(DE.OnOff1.Off) # Needed to push signal from error

        # Fan minus minus = 1
        self.vHalCommon.setVhalProperty(self.vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, \
            {"int32Values" : [1]}, self.zone_center)
        # Max Defrost = Off
        self.vHalCommon.setVhalProperty(self.vHalCommon.vtypes.VehicleProperty.HVAC_MAX_DEFROST_ON, \
            {"int32Values" : [0]}, self.zone_center)

        self.vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [23]}, self.zone_left)
        self.vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [23]}, self.zone_right)
        self.vHalCommon.setVhalProperty(self.HVAC_TEMPERATURE_MODE, {"int32Values" : [TemperatureMode.Normal]}, self.zone_left)
        self.vHalCommon.setVhalProperty(self.HVAC_TEMPERATURE_MODE, {"int32Values" : [TemperatureMode.Normal]}, self.zone_right)

        time.sleep(0.5)

    def tearDownClass(self):
        try:
            self.fr.close()
        except:
            pass

    def left_minus_button_click(self):
        self.dut.adb.shell("input tap 150 988")

    def left_plus_button_click(self):
        self.dut.adb.shell("input tap 300 988")

    def right_minus_button_click(self):
        self.dut.adb.shell("input tap 500 988")

    def right_plus_button_click(self):
        self.dut.adb.shell("input tap 650 988")


    def assertTemperatureLeftVhal(self, expected_value):
        wait_for_close_value( \
            lambda : self.vHalCommon.readVhalProperty(self.prop_temp, self.zone_left)['value']['floatValues'][0], \
            expected_value, \
            wait_time_seconds, \
            "Temperature Left Property error.")

    def assertTemperatureHiLoLeftVhal(self, expected_value):
        wait_for( \
            lambda : self.vHalCommon.readVhalProperty(self.HVAC_TEMPERATURE_MODE, self.zone_left)['value']['int32Values'][0], \
            expected_value, \
            wait_time_seconds, \
            "Temperature Left Hi / Lo Property error.")

    def assertTemperatureLeftSignal(self, expected_value):
        wait_for_close_value( \
            lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpForRowFirstLe, \
            expected_value, \
            wait_time_seconds, \
            "Temperature Left Flexray error.")

    def assertTemperatureHiLoLeftSignal(self, expected_value):
        wait_for( \
            lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpSpclForRowFirstLe, \
            expected_value, \
            wait_time_seconds, \
            "Temperature Left Hi / Lo Flexray error.")



    def assertTemperatureRightVhal(self, expected_value):
        wait_for_close_value( \
            lambda : self.vHalCommon.readVhalProperty(self.prop_temp, self.zone_right)['value']['floatValues'][0], \
            expected_value, \
            wait_time_seconds, \
            "Temperature Right Property error.")

    def assertTemperatureHiLoRightVhal(self, expected_value):
        wait_for( \
            lambda : self.vHalCommon.readVhalProperty(self.HVAC_TEMPERATURE_MODE, self.zone_right)['value']['int32Values'][0], \
            expected_value, \
            wait_time_seconds, \
            "Temperature Right Hi / Lo Property error.")

    def assertTemperatureRightSignal(self, expected_value):
        wait_for_close_value( \
            lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpForRowFirstRi, \
            expected_value, \
            wait_time_seconds, \
            "Temperature Right Flexray error.")

    def assertTemperatureHiLoRightSignal(self, expected_value):
        wait_for( \
            lambda : self.fr.get_HmiCmptmtTSp().HmiCmptmtTSpSpclForRowFirstRi, \
            expected_value, \
            wait_time_seconds, \
            "Temperature Right Hi / Lo Flexray error.")


    def assertTemperatureUi(self, ui_control, expected_value, msg=""):
        return#TODO No ui validation works atm, reindroduce when this is fixed
        control_text=ui_control.text
        debug_info = ui_control.info
        read_value = control_text.strip().replace(u"°", "").strip()
        asserts.assertEqual(read_value, expected_value, "Temperature UI error, expected: %r, got %r (%s)\n%r" % (expected_value, control_text, msg, debug_info))


    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment set by VHAL
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentVHALSet(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # First Action
        self.vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [25]}, self.zone_left)
        self.vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [20]}, self.zone_right)

        self.vHalCommon.setVhalProperty(self.HVAC_TEMPERATURE_MODE, {"int32Values" : [TemperatureMode.Normal]}, self.zone_left)
        self.vHalCommon.setVhalProperty(self.HVAC_TEMPERATURE_MODE, {"int32Values" : [TemperatureMode.Normal]}, self.zone_right)
        time.sleep(0.5)

        self.assertTemperatureLeftVhal(25)
        self.assertTemperatureLeftSignal(25)
        self.assertTemperatureHiLoLeftVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.left_temperature_text, "25")

        self.assertTemperatureRightVhal(20)
        self.assertTemperatureRightSignal(20)
        self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.right_temperature_text, "20")

        # Second action for evaluating for Hi/Lo handling
        self.vHalCommon.setVhalProperty(self.HVAC_TEMPERATURE_MODE, {"int32Values" : [TemperatureMode.Low]}, self.zone_left)
        self.vHalCommon.setVhalProperty(self.HVAC_TEMPERATURE_MODE, {"int32Values" : [TemperatureMode.Hi]}, self.zone_right)
        self.vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [20]}, self.zone_right)# A try to confuse the logic

        self.assertTemperatureHiLoLeftVhal(TemperatureMode.Low)
        self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Lo)
        self.assertTemperatureUi(self.left_temperature_text, "Lo")

        self.assertTemperatureHiLoRightVhal(TemperatureMode.Hi)
        self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Hi)
        self.assertTemperatureUi(self.right_temperature_text, "Hi")


    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Left Increase
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentLeftIncrease(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Increase the left temperature to 27 degrees by pressing the plus button
        # This assumes that HMI steps 1 degree for each click
        for temp_value in range(24,29):
            self.left_plus_button_click()
            time.sleep(0.5)

            if(temp_value <= temperature_highest):
                self.assertTemperatureLeftVhal(temp_value)
                self.assertTemperatureLeftSignal(temp_value)
                self.assertTemperatureHiLoLeftVhal(TemperatureMode.Normal)
                self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Norm)
                self.assertTemperatureUi(self.left_temperature_text, str(temp_value))
            else:
                self.assertTemperatureLeftVhal(temperature_highest)
                self.assertTemperatureLeftSignal(temperature_highest)
                self.assertTemperatureHiLoLeftVhal(TemperatureMode.Hi)
                self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Hi)
                self.assertTemperatureUi(self.left_temperature_text, "Hi")


    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Left Decrease
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentLeftDecrease(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Decrease the left temperature to 17 degrees by pressing the minus button
        # This assumes that HMI steps 1 degree for each click
        for temp_value in range(22,15,-1):
            self.left_minus_button_click()
            time.sleep(0.5)

            if(temp_value >= temperature_lowest):
                self.assertTemperatureLeftVhal(temp_value)
                self.assertTemperatureLeftSignal(temp_value)
                self.assertTemperatureHiLoLeftVhal(TemperatureMode.Normal)
                self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Norm)
                self.assertTemperatureUi(self.left_temperature_text, str(temp_value))
            else:
                self.assertTemperatureLeftVhal(temperature_lowest)
                self.assertTemperatureLeftSignal(temperature_lowest)
                self.assertTemperatureHiLoLeftVhal(TemperatureMode.Low)
                self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Lo)
                self.assertTemperatureUi(self.left_temperature_text, "Lo")


    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Right Increase
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentRightIncrease(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Increase the right temperature to 27 degrees by pressing the plus button
        # This assumes that HMI steps 1 degree for each click
        for temp_value in range(24,29):
            self.right_plus_button_click()
            time.sleep(0.5)

            if(temp_value <= temperature_highest):
                self.assertTemperatureRightVhal(temp_value)
                self.assertTemperatureRightSignal(temp_value)
                self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
                self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
                self.assertTemperatureUi(self.right_temperature_text, str(temp_value))
            else:
                self.assertTemperatureRightVhal(temperature_highest)
                self.assertTemperatureRightSignal(temperature_highest)
                self.assertTemperatureHiLoRightVhal(TemperatureMode.Hi)
                self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Hi)
                self.assertTemperatureUi(self.right_temperature_text, "Hi")

    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Right Decrease
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentRightDecrease(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Decrease the right temperature to 17 degrees by pressing the minus button
        # This assumes that HMI steps 1 degree for each click
        for temp_value in range(22,15,-1):
            self.right_minus_button_click()
            time.sleep(0.5)

            if(temp_value >= temperature_lowest):
                self.assertTemperatureRightVhal(temp_value)
                self.assertTemperatureRightSignal(temp_value)
                self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
                self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
                self.assertTemperatureUi(self.right_temperature_text, str(temp_value))
            else:
                self.assertTemperatureRightVhal(temperature_lowest)
                self.assertTemperatureRightSignal(temperature_lowest)
                self.assertTemperatureHiLoRightVhal(TemperatureMode.Low)
                self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Lo)
                self.assertTemperatureUi(self.right_temperature_text, "Lo")

    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Disabled (does not fulfill pre-conditions)
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentDisabled(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set up FlexRay pre-conditions to fail
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(self.vehmod)

        # Press the plus buttons with no effect
        self.left_plus_button_click()
        self.right_plus_button_click()

        self.assertTemperatureLeftVhal(23)
        self.assertTemperatureLeftSignal(23)
        self.assertTemperatureHiLoLeftVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.left_temperature_text, "23")

        self.assertTemperatureRightVhal(23)
        self.assertTemperatureRightSignal(23)
        self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.right_temperature_text, "23")

        # Press the minus buttons with no effect
        self.left_minus_button_click()
        self.right_minus_button_click()

        self.assertTemperatureLeftVhal(23)
        self.assertTemperatureLeftSignal(23)
        self.assertTemperatureHiLoLeftVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.left_temperature_text, "23")

        self.assertTemperatureRightVhal(23)
        self.assertTemperatureRightSignal(23)
        self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.right_temperature_text, "23")


    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Right HalfSteps (assuming that left is the same)
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentRightHalfSteps(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Initialize the right temperature to a degrees outside the standard stepsize
        vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [24.4999]}, self.zone_right)

        #Evaluate that the logic round the value to closest stepsize (currently half-degrees)
        self.assertTemperatureRightVhal(24.5)
        self.assertTemperatureRightSignal(24.5)
        self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
        #Currently UI is displayed as whole degree
        self.assertTemperatureUi(self.right_temperature_text, "24")

        # Increase the right temperature to 27 degrees by pressing the plus button
        # This assumes that HMI steps 1 degree for each click
        for temp_value in range(25,29):
            self.right_plus_button_click()
            time.sleep(0.5)

            if(temp_value <= temperature_highest):
                self.assertTemperatureRightVhal(temp_value)
                self.assertTemperatureRightSignal(temp_value)
                self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
                self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
                self.assertTemperatureUi(self.right_temperature_text, str(temp_value))
            else:
                self.assertTemperatureRightVhal(temperature_highest)
                self.assertTemperatureRightSignal(temperature_highest)
                self.assertTemperatureHiLoRightVhal(TemperatureMode.Hi)
                self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Hi)
                self.assertTemperatureUi(self.right_temperature_text, "Hi")


        # Decrease the right temperature to 17 degrees by pressing the minus button
        # This assumes that HMI steps 1 degree for each click
        for temp_value in range(26,15,-1):
            self.right_minus_button_click()
            time.sleep(0.5)

            if(temp_value >= temperature_lowest):
                self.assertTemperatureRightVhal(temp_value)
                self.assertTemperatureRightSignal(temp_value)
                self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
                self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
                self.assertTemperatureUi(self.right_temperature_text, str(temp_value))
            else:
                self.assertTemperatureRightVhal(temperature_lowest)
                self.assertTemperatureRightSignal(temperature_lowest)
                self.assertTemperatureHiLoRightVhal(TemperatureMode.Low)
                self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Lo)
                self.assertTemperatureUi(self.right_temperature_text, "Lo")


    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment Disabled by Fan
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentDisabledByFan(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Disable temp using Fan Off
        self.vHalCommon.setVhalProperty(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, \
            {"int32Values" : [0]}, self.zone_center)

        time.sleep(0.5)

        self.left_plus_button_click()
        self.right_minus_button_click()
        time.sleep(0.5)

        # Expect startup values due to disabled ui
        self.assertTemperatureLeftVhal(23)
        self.assertTemperatureLeftSignal(23)
        self.assertTemperatureHiLoLeftVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.left_temperature_text, "23")

        self.assertTemperatureRightVhal(23)
        self.assertTemperatureRightSignal(23)
        self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.right_temperature_text, "23")


    # ----------------------------------------------------------------------------------------------------------
    # Test Temperature Adjustment while Max Defrost On/Off
    # ----------------------------------------------------------------------------------------------------------
    def testTemperatureAdjustmentMaxDefrostToogle(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Have right differ from left
        self.vHalCommon.setVhalProperty(self.prop_temp, {"floatValues" : [24]}, self.zone_right)
        time.sleep(0.5)

        # Trigger maxdefrost
        self.vHalCommon.setVhalProperty(self.vHalCommon.vtypes.VehicleProperty.HVAC_MAX_DEFROST_ON, \
            {"int32Values" : [1]}, self.zone_center)
        time.sleep(0.5)

        # Check that both sides goes to high
        self.assertTemperatureHiLoLeftVhal(TemperatureMode.Hi)
        self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Hi)
        self.assertTemperatureUi(self.left_temperature_text, "Hi")

        self.assertTemperatureHiLoRightVhal(TemperatureMode.Hi)
        self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Hi)
        self.assertTemperatureUi(self.right_temperature_text, "Hi")

        # Go from Hi to 27 on left, right stays the same since maxdefrost diables temp sync.
        self.left_minus_button_click()
        time.sleep(5.5)

        self.assertTemperatureLeftVhal(27)
        self.assertTemperatureLeftSignal(27)
        self.assertTemperatureHiLoLeftVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.left_temperature_text, "27")

        # TODO Add this when TempSync exists (make sure its starts on on for this function)
        # self.assertTemperatureHiLoRightVhal(TemperatureMode.Hi)
        # self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Hi)
        # self.assertTemperatureUi(self.right_temperature_text, "Hi")

        # Turn of maxdefrost
        self.vHalCommon.setVhalProperty(self.vHalCommon.vtypes.VehicleProperty.HVAC_MAX_DEFROST_ON, \
            {"int32Values" : [0]}, self.zone_center)
        time.sleep(1.5)

        self.assertTemperatureLeftVhal(23)
        self.assertTemperatureLeftSignal(23)
        self.assertTemperatureHiLoLeftVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoLeftSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.left_temperature_text, "23")

        self.assertTemperatureRightVhal(24)
        self.assertTemperatureRightSignal(24)

        self.assertTemperatureHiLoRightVhal(TemperatureMode.Normal)
        self.assertTemperatureHiLoRightSignal(HmiCmptmtTSpSpcl.Norm)
        self.assertTemperatureUi(self.right_temperature_text, "24")



if __name__ == "__main__":
    test_runner.main()