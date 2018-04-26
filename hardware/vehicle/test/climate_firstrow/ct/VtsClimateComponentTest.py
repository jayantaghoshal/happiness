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
import vehiclehalcommon
from generated.datatypes import VehModMngtGlbSafe1, HmiHvacFanLvl
from generated import datatypes as DE


class VtsClimateComponentTest(base_test.BaseTestClass):
    """Testing Climate functions"""
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

        self.fr = vehiclehalcommon.get_dataelements_connection(self.dut.adb)
        self.vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        self.vc, self.device = self.vHalCommon.getViewClient()

        # Open climate view and dump the view
        self.device.touchDip(388.0, 948.0, 0)
        self.vc.dump()

        #Get fan buttons
        self.fan_off = self.vc.findViewByIdOrRaise(self.vHalCommon.fan_off)
        self.fan_level_1 = self.vc.findViewByIdOrRaise(self.vHalCommon.fan_level_1)
        self.fan_level_2 = self.vc.findViewByIdOrRaise(self.vHalCommon.fan_level_2)
        self.fan_level_3 = self.vc.findViewByIdOrRaise(self.vHalCommon.fan_level_3)
        self.fan_level_4 = self.vc.findViewByIdOrRaise(self.vHalCommon.fan_level_4)
        self.fan_level_5 = self.vc.findViewByIdOrRaise(self.vHalCommon.fan_level_5)
        self.fan_max = self.vc.findViewByIdOrRaise(self.vHalCommon.fan_max)


    def setUp(self):
        # Open climate view
        self.device.touchDip(388.0, 948.0, 0)

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

    def tearDown(self):
            self.dut.shell.one.Execute("input keyevent 3")


    def tearDownClass(self):
        try:
            self.fr.close()
        except:
            pass


    def testFanLevel(self):
        _s = 0.5
        fr = self.fr
        vHalCommon = self.vHalCommon

        # Set climate to Active
        # TODO: Run negative tests
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)
        self.vc.sleep(_s)

        # Set to OFF
        self.fan_off.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 0)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off)

        # Set to Level 1
        self.fan_level_1.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 1)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinusMinus)

        # Set to Level 2
        self.fan_level_2.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 2)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus)

        # Set to Level 3
        self.fan_level_3.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 3)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutoNorm)

        # Set to Level 4
        self.fan_level_4.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 4)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutPlus)

        # Set to Level 5
        self.fan_level_5.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 5)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutPlusPlus)

        # Set to Max
        self.fan_max.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 6)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Max)

    def testFanLevelAvailability(self):
        _s = 0.5
        fr = self.fr
        vHalCommon = self.vHalCommon


        # Set climate to Active
        print("set to active")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        self.vc.sleep(_s)
        # Set to Level 2
        print("set to Level 2")
        self.fan_level_2.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED,
                                           vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 2)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus)

        # Set climate to Inactive
        print("set to inactive")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.Off)

        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED,
                                           vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 7)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off)

        # Try to set to Level 3 - will not be possible
        print("Try to set to Level 3 - will not work")
        self.fan_level_3.touch()
        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED,
                                           vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 7)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off)

        # Set climate to Active - will go back to previous state i.e. 2
        print("Set climate to Active - will go back to previous state i.e. 2")
        self.vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(self.vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)

        self.vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED,
                                           vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 2)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus)

if __name__ == "__main__":
    test_runner.main()
