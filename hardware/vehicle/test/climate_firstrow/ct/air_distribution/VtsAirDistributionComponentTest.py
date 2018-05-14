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
from enum import Enum
from subprocess import call


sys.path.append('/usr/local/lib/python2.7/dist-packages')
from com.dtmilano.android.viewclient import ViewClient
from vehiclehalcommon import \
    VehicleHalCommon, \
    wait_for, \
    get_dataelements_connection
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    HmiCmptmtAirDistbnFrnt, \
    TwliBriSts1
from generated import datatypes as DE
from uiautomator import device as device
wait_time_seconds = 2

# Define Enum for Air distribution property value
class AirDistEnum(Enum):
    FAN_DIRECTION_AUTO = 0
    FAN_DIRECTION_FACE = 1
    FAN_DIRECTION_FLOOR = 2
    FAN_DIRECTION_FACEFLOOR = 3
    FAN_DIRECTION_DEFROST = 4
    FAN_DIRECTION_FACEDEFROST = 5
    FAN_DIRECTION_FLOORDEFROST = 6
    FAN_DIRECTION_VENTFLOORDEFROST = 7

class VtsAirDistributionComponentTest(base_test.BaseTestClass):

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

        #Get Air Defroster button
        self.air_dist_top = device(resourceId=self.vHalCommon.air_dist_top)
        self.air_dist_middle = device(resourceId=self.vHalCommon.air_dist_middle)
        self.air_dist_bottom = device(resourceId=self.vHalCommon.air_dist_bottom)
        self.air_dist_auto = device(resourceId=self.vHalCommon.air_dist_auto)

        #Get Fan button
        self.fan_off = device(resourceId=self.vHalCommon.fan_off)
        self.fan_level_1 = device(resourceId=self.vHalCommon.fan_level_1)
        self.fan_level_5 = device(resourceId=self.vHalCommon.fan_level_5)

        #Get Max Defrost button
        self.defrost_button = device(resourceId=self.vHalCommon.defrost_button)

        #get properties and zones
        self.prop_air_dist = self.vHalCommon.vtypes.VehicleProperty.HVAC_FAN_DIRECTION
        self.zone_center = self.vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER

    def setUp(self):
        #Prepare VehModMngtGlbSafe to be sent
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

    # ----------------------------------------------------------------------------------------------------------
    # VHAL Property value for FAN DIRECTION  ( Reference- types.hal)
    # FAN_DIRECTION_FACE (0x1) -> Vent
    # FAN_DIRECTION_FLOOR (0x2) -> Flr
    # FAN_DIRECTION_FACE | FAN_DIRECTION_FLOOR (0x3)
    # FAN_DIRECTION_DEFROST (0x4) -> Defrst
    # FAN_DIRECTION_FLOOR | FAN_DIRECTION_DEFROST (0x6)
    #     And,
    # FR signal values are : ( Reference- gen_datatypes.h)
    # enum class HmiCmptmtAirDistbnFrnt {
    # Flr=0,
    # Vent=1,
    # Defrst=2,
    # FlrDefrst=3,
    # FlrVent=4,
    # VentDefrst=5,
    # FlrVentDefrst=6,
    # Aut=7,
    # };
    # Hence using three arguments in below function.
    # ----------------------------------------------------------------------------------------------------------

    def assertAirDistribution(self, expected_vhal_value, expected_flexray_value):
        wait_for(lambda : self.vHalCommon.readVhalProperty(self.prop_air_dist, self.zone_center)['value']['int32Values'], [expected_vhal_value], \
            wait_time_seconds, "ERROR : Air Distribution vhal value.")
        wait_for(lambda : self.fr.get_HmiCmptmtAirDistbnFrnt(), expected_flexray_value, wait_time_seconds, "ERROR : Air Distribution FR Value mismatch.")

    #Setting preconditions to true.
    def setting_precondition(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)
        self.fan_level_1.click()

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Distribution - Vent Cases
    # ----------------------------------------------------------------------------------------------------------

    def testairDistribution_Vent(self):

        #Set Pre-conditions to pass
        self.setting_precondition()

        #Initializing the air distribution to AUTO
        self.air_dist_auto.click()

        #Set to Vent
        self.air_dist_middle.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Vent)

        #Reset to Auto from Vent by pressing AUTO
        self.air_dist_auto.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

        #Set to VentDefrost
        self.air_dist_middle.click()
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACEDEFROST,HmiCmptmtAirDistbnFrnt.VentDefrst)

        #Set to Auto from VentDefrost by pressing AUTO
        self.air_dist_auto.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

        #Again Set to VentDefrost to check the toggle case
        self.air_dist_middle.click()
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACEDEFROST,HmiCmptmtAirDistbnFrnt.VentDefrst)

        #Reset to Auto by toggling Defrost and Vent
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Vent)
        self.air_dist_middle.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Distribution - Defrost Cases
    # ----------------------------------------------------------------------------------------------------------

    def testairDistribution_Defrost(self):

        #Set Pre-conditions to pass
        self.setting_precondition()

        #Initializing the air distribution to AUTO
        self.air_dist_auto.click()

        #Set to Defrost
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_DEFROST,HmiCmptmtAirDistbnFrnt.Defrst)

        #Set to Auto from Defrost by pressing AUTO
        self.air_dist_auto.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

        #Set to FloorDefrost
        self.air_dist_bottom.click()
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FLOORDEFROST,HmiCmptmtAirDistbnFrnt.FlrDefrst)

        #Set to Auto from FloorDefrost by pressing AUTO
        self.air_dist_auto.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

        #Again Set to FloorDefrost to check the toggle case
        self.air_dist_bottom.click()
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FLOORDEFROST,HmiCmptmtAirDistbnFrnt.FlrDefrst)

        #Set to Auto by toggling Floor and Defrost
        self.air_dist_bottom.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_DEFROST,HmiCmptmtAirDistbnFrnt.Defrst)
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Distribution - Floor Cases
    # ----------------------------------------------------------------------------------------------------------

    def testairDistribution_Floor(self):

        #Set Pre-conditions to pass
        self.setting_precondition()

        #Initializing the air distribution to AUTO
        self.air_dist_auto.click()

        #Set to Floor
        self.air_dist_bottom.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FLOOR,HmiCmptmtAirDistbnFrnt.Flr)

        #Reset to Auto from Floor by pressing AUTO
        self.air_dist_auto.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

        #Set to FloorVent
        self.air_dist_bottom.click()
        self.air_dist_middle.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACEFLOOR,HmiCmptmtAirDistbnFrnt.FlrVent)

        #Set to Auto from FloorVent by pressing AUTO
        self.air_dist_auto.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

        #Again Set to FloorVent to check the toggle case
        self.air_dist_bottom.click()
        self.air_dist_middle.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACEFLOOR,HmiCmptmtAirDistbnFrnt.FlrVent)

        #Reset to Auto by toggling Vent and Floor
        self.air_dist_middle.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FLOOR,HmiCmptmtAirDistbnFrnt.Flr)
        self.air_dist_bottom.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Distribution - FlrVentDefrst Cases
    # ----------------------------------------------------------------------------------------------------------

    def testairDistribution_FloorVentDefrost(self):

        #Set Pre-conditions to pass
        self.setting_precondition()

        #Initializing the air distribution to AUTO
        self.air_dist_auto.click()

        #Set to FloorVentDefrost
        self.air_dist_bottom.click()
        self.air_dist_middle.click()
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_VENTFLOORDEFROST,HmiCmptmtAirDistbnFrnt.FlrVentDefrst)

        #Reset to Auto from FloorVentDefrost by pressing AUTO
        self.air_dist_auto.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

        #Again set to FloorVentDefrost to check toggle cases
        self.air_dist_bottom.click()
        self.air_dist_middle.click()
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_VENTFLOORDEFROST,HmiCmptmtAirDistbnFrnt.FlrVentDefrst)

        #------------Reset to Auto by toggling FloorVentDefrost------------

        #Toggle Vent - FlrDefrst should remain set.
        self.air_dist_middle.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FLOORDEFROST,HmiCmptmtAirDistbnFrnt.FlrDefrst)

        #set the vent again to make it FloorVentDefrost
        self.air_dist_middle.click()

        #Toggle Defrst - FlrVent should remain set.
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACEFLOOR,HmiCmptmtAirDistbnFrnt.FlrVent)

        #set the Defrst again to make it FloorVentDefrost
        self.air_dist_top.click()

        #Toggle Flr - VentDefrst should remain set.
        self.air_dist_bottom.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACEDEFROST,HmiCmptmtAirDistbnFrnt.VentDefrst)

        #Toggle Vent - Only Defrst should remain set now
        self.air_dist_middle.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_DEFROST,HmiCmptmtAirDistbnFrnt.Defrst)

        #Toggle Defrst - It should set to AUTO now.
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Distribution InActive Cases
    # ----------------------------------------------------------------------------------------------------------

    def testairDistribution_InActive(self):
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)
        self.fan_level_1.click()

        #Initializing the air distribution to Vent
        self.air_dist_middle.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Vent)

        #Set up pre-conditions to fail - (CarMode is Dyno but UserMode Inactive, ClimateActive is Off )
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.Off)

        #Air Distribution should be set to AUTO
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Aut)

        #Try To Set the Air Distribution to other values (Defrst) - Should not work
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Aut)   # It should remain AUTO

        #Again Set Pre-conditions to pass
        self.vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        #Air Distribution should be set to Vent automatically as last state was Vent
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Vent)

        #Again set up the pre-conditions to fail - (This time : UserMode active, ClimateActive is On but CarMode is Trnsp)
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModTrnsp
        fr.send_VehModMngtGlbSafe1(self.vehmod)

        #Air Distribution should be set to AUTO
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Aut)

        #Try To Set the Air Distribution to other values (Floor) - Should not work
        self.air_dist_bottom.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Aut)   # It should remain AUTO

        #Again Set Pre-conditions to pass
        self.vehmod.CarModSts1 = DE.CarModSts1.CarModNorm
        fr.send_VehModMngtGlbSafe1(self.vehmod)

        #Air Distribution should be set to Vent automatically as last state was Vent
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Vent)

        #Set the Fan Level to 0, Air Direction should be set to Auto
        self.fan_off.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Aut)

        #Try To Set the Air Distribution to other values (Defrst) - Should not work
        self.air_dist_top.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Aut)   # It should remain AUTO

        #Again Set the Fan Level to 1, Air Direction should be set to Vent automatically as last state was Vent
        self.fan_off.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FACE,HmiCmptmtAirDistbnFrnt.Aut)

    # ----------------------------------------------------------------------------------------------------------
    # Test Air Distribution - To Check Air Direction value When Max Defroster is set
    # ----------------------------------------------------------------------------------------------------------

    def testairDistribution_WithMaxDefrost(self):

        #Set Pre-conditions to pass
        self.setting_precondition()

        #Initializing the air distribution to AUTO
        self.air_dist_auto.click()

        #Set to Floor
        self.air_dist_bottom.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FLOOR,HmiCmptmtAirDistbnFrnt.Flr)

        #Set the Max Defroster to ON - Air Direction should be set to Auto
        self.defrost_button.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_AUTO,HmiCmptmtAirDistbnFrnt.Aut)

        #Set the Max Defroster to OFF - Air Direction should be set to Floor automatically as last state was Floor
        self.defrost_button.click()
        self.assertAirDistribution(AirDistEnum.FAN_DIRECTION_FLOOR,HmiCmptmtAirDistbnFrnt.Flr)

if __name__ == "__main__":
    test_runner.main()