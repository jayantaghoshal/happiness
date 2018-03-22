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
from fdx import fdx_client
from fdx import fdx_description_file_parser
from generated.pyDataElements import \
    FrSignalInterface, \
    UsgModSts, \
    ClimaActv
from vehiclehalcommon import VehicleHalCommon

ns_per_ms = 1000000

class VtsClimateComponentTest(base_test.BaseTestClass):
    """Testing Climate functions"""
    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()

    def testFanLevel(self):
        _s = 0.5

        vHalCommon = VehicleHalCommon(self.dut, self.system_uid)
        fr = FrSignalInterface()
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
        fr.UsgModSts.send(UsgModSts.map.UsgModDrvg)
        fr.ClimaActv.send_repetitive(ClimaActv.map.On)
        vc.sleep(_s)

        # Set to OFF
        fan_off.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1, 0)
        vHalCommon.assert_signal_equals(fr.HmiHvacFanLvlFrnt, fr.HmiHvacFanLvlFrnt.map.Off)

        # Set to Level 1
        fan_level_1.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1, 1)
        vHalCommon.assert_signal_equals(fr.HmiHvacFanLvlFrnt, fr.HmiHvacFanLvlFrnt.map.LvlAutMinusMinus)

        # Set to Level 2
        fan_level_2.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1, 2)
        vHalCommon.assert_signal_equals(fr.HmiHvacFanLvlFrnt, fr.HmiHvacFanLvlFrnt.map.LvlAutMinus)

        # Set to Level 3
        fan_level_3.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1, 3)
        vHalCommon.assert_signal_equals(fr.HmiHvacFanLvlFrnt, fr.HmiHvacFanLvlFrnt.map.LvlAutoNorm)

        # Set to Level 4
        fan_level_4.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1, 4)
        vHalCommon.assert_signal_equals(fr.HmiHvacFanLvlFrnt, fr.HmiHvacFanLvlFrnt.map.LvlAutPlus)

        # Set to Level 5
        fan_level_5.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1, 5)
        vHalCommon.assert_signal_equals(fr.HmiHvacFanLvlFrnt, fr.HmiHvacFanLvlFrnt.map.LvlAutPlusPlus)

        # Set to Max
        fan_max.touch()
        vc.sleep(_s)
        vHalCommon.assert_prop_area_equals(vHalCommon.vtypes.VehicleProperty.HVAC_FAN_SPEED, vHalCommon.vtypes.VehicleAreaZone.ROW_1, 6)
        vHalCommon.assert_signal_equals(fr.HmiHvacFanLvlFrnt, fr.HmiHvacFanLvlFrnt.map.Max)

        # Close climate view
        self.dut.shell.one.Execute("input keyevent 3")  # SELinux permissive mode
        fr.ClimaActv.stop_send()

if __name__ == "__main__":
    test_runner.main()
