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

        #self.dut.start()
        #self.dut.waitForBootCompletion()

        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()

        self.fr = vehiclehalcommon.get_dataelements_connection(self.dut.adb)


    def tearDownClass(self):
        try:
            self.dut.shell.one.Execute("input keyevent 3")
            self.fr.close()
        except:
            pass


    def testFanLevel(self):
        _s = 0.5
        fr = self.fr

        vehmod = VehModMngtGlbSafe1()
        vehmod.CarModSts1 = DE.CarModSts1.CarModDyno
        vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        vehmod.Chks = 0
        vehmod.Cntr = 0
        vehmod.CarModSubtypWdCarModSubtyp = 0
        vehmod.EgyLvlElecMai = 0
        vehmod.EgyLvlElecSubtyp = 0
        vehmod.PwrLvlElecMai = 0
        vehmod.PwrLvlElecSubtyp = 0
        vehmod.FltEgyCnsWdSts = DE.FltEgyCns1.Flt

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)


        vc, device = vHalCommon.getViewClient()

        # Open climate view
        device.touchDip(388.0, 948.0, 0)

        vc.dump()

        #Get fan buttons
        fan_off = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_off")
        fan_level_1 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_one")
        fan_level_2 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_two")
        fan_level_3 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_three")
        fan_level_4 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_four")
        fan_level_5 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_five")
        fan_max = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_max")

        # Set climate to Active
        # TODO: Run negative tests
        vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(vehmod)
        fr.send_ClimaActv(DE.OnOff1.On)
        vc.sleep(_s)

        # Set to OFF
        fan_off.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 0)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Off)

        # Set to Level 1
        fan_level_1.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 1)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinusMinus)

        # Set to Level 2
        fan_level_2.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 2)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutMinus)

        # Set to Level 3
        fan_level_3.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 3)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutoNorm)

        # Set to Level 4
        fan_level_4.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 4)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutPlus)

        # Set to Level 5
        fan_level_5.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 5)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.LvlAutPlusPlus)

        # Set to Max
        fan_max.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1_CENTER, 6)
        asserts.assertEqual(fr.get_HmiHvacFanLvlFrnt(), HmiHvacFanLvl.Max)



if __name__ == "__main__":
    test_runner.main()
