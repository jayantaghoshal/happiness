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
import vehiclehalcommon
import threading
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    FctSts2


from generated import datatypes as DE
import logging.config
import logging.handlers

from vehiclehalcommon import VehicleHalCommon, wait_for_signal

# Get button Ids for testing.
elka_button_off           = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_off"
elka_button_on            = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_on"


buttonWaitTimeSeconds = 1.5

class VtsELaneKeepingAidSettingsComponentTest(base_test.BaseTestClass):

    def setUpClass(self):

        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()
        self.fr = vehiclehalcommon.get_dataelements_connection(self.dut.adb)


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




    # ----------------------------------------------------------------------------------------------------
    # Test 1
    # Test Condition : Disable Car Config
    # Expected Result : ELKA buttons should not be visible
    # ----------------------------------------------------------------------------------------------------
    def testELaneKeepingCCDisabled(self):

        print("----------- testELaneKeepingAidCCDisabled -----------")

        self.dut.shell.one.Execute("changecarconfig 150 1")
        self.dut.shell.one.Execute("changecarconfig 316 0")

        # Reboot device after changing carconfig
        self.deviceReboot()

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

        vc, device = vHalCommon.getActiveViewClient()
        vc.dump(window=-1)

        # Find buttons. They shall be disabled when CC is disabled.
        elka_button_off = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_off"
        elka_button_on = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_on"
        buttonOff = vHalCommon.scrollAndFindViewById(elka_button_off)
        buttonOn = vHalCommon.scrollAndFindViewById(elka_button_on)
        print("buttonOff -> ") + str(buttonOff)
        print("buttonOn -> ") + str(buttonOn)
        asserts.assertEqual(None, buttonOff, "Expect no buttonOff")
        asserts.assertEqual(None, buttonOn, "Expect no buttonOn")

        time.sleep(buttonWaitTimeSeconds)

    # ----------------------------------------------------------------------------------------------------
    # Test 2
    # Test Condition : Enabled Car Config
    # Steps: a) Enable Car Config and Reboot.
    #        b) Introduce Error Condition
    #        c) Check the button status [expected: Disabled]
    #        d) Remove Error Condition
    #        e) Check the button status [expected: Enabled]
    #        f) Set UsgMod to InActive
    #        g) Check the button status [expected: Disabled]
    #        h) Set UsgMod to Active
    #        i) Check the button status [expected: Enabled]
    #        j) touch On Button
    #        k) Check OnOff value [OnOff value true]
    #        l) touch Off Button
    #        m) Check OnOff value [OnOff value false]
    # ----------------------------------------------------------------------------------------------------
    def testELaneKeepingCCEnabled(self):

        print("----------- testELaneKeepingAidCCEnabled -----------")

        self.dut.shell.one.Execute("changecarconfig 150 2")
        self.dut.shell.one.Execute("changecarconfig 316 2")

        # Reboot device after changing carconfig
        self.deviceReboot()

        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()

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
        vHalCommon.setUpVehicleFunction()

        EMERGENCY_LANE_KEEPING_AID_ON = vHalCommon.get_id('EMERGENCY_LANE_KEEPING_AID_ON')

        vc, device = vHalCommon.getActiveViewClient()
        vc.dump(window=-1)

        # Introducing Error conditions
        fr.send_LaneKeepAidSts(DE.FctSts2.SrvRqrd)
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(buttonWaitTimeSeconds)

        # Find buttons. They shall be Visible when CC is enabled.
        elka_button_off = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_off"
        elka_button_on = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_on"
        buttonOff = vHalCommon.scrollAndFindViewById(elka_button_off)
        buttonOn = vHalCommon.scrollAndFindViewById(elka_button_on)

        print("buttonOff enabled -> ") + str(buttonOff.__getattr__('enabled')())
        print("buttonOn  enabled -> ") + str(buttonOn.__getattr__('enabled')())

        # Find buttons. but it should be disabled.
        asserts.assertEqual(buttonOff.__getattr__('enabled')(),False, "Off button is enabled")
        asserts.assertEqual(buttonOn.__getattr__('enabled')(),False, "On button is enabled")

        vHalCommon.assert_prop_equals(EMERGENCY_LANE_KEEPING_AID_ON, False)
        asserts.assertEqual(fr.get_LaneKeepAidRoadEdgeActv().Sts, 0, "on/off shall be 0")

        time.sleep(buttonWaitTimeSeconds)

        # Removing Error conditions
        fr.send_LaneKeepAidSts(DE.FctSts2.On)
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(buttonWaitTimeSeconds)

        vc.dump(window=-1)

        # Find buttons. They shall be Visible when CC is enabled.
        elka_button_off = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_off"
        elka_button_on = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_on"
        buttonOff = vHalCommon.scrollAndFindViewById(elka_button_off)
        buttonOn = vHalCommon.scrollAndFindViewById(elka_button_on)

        print("buttonOff enabled -> ") + str(buttonOff.__getattr__('enabled')())
        print("buttonOn  enabled -> ") + str(buttonOn.__getattr__('enabled')())

         # Find buttons. Buttons should be enabled.
        asserts.assertEqual(buttonOff.__getattr__('enabled')(),True, "Off button is disabled")
        asserts.assertEqual(buttonOn.__getattr__('enabled')(),True, "On button is disabled")

        vHalCommon.assert_prop_equals(EMERGENCY_LANE_KEEPING_AID_ON, True)
        asserts.assertEqual(fr.get_LaneKeepAidRoadEdgeActv().Sts, 1, "on/off shall be 0")

        time.sleep(buttonWaitTimeSeconds)

        vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(vehmod)

        time.sleep(buttonWaitTimeSeconds)

        # Find buttons. They shall be visible but disabled.

        vc.dump(window=-1)

        # Find buttons. They shall be Visible when CC is enabled
        elka_button_off = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_off"
        elka_button_on = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_on"
        buttonOff = vHalCommon.scrollAndFindViewById(elka_button_off)
        buttonOn = vHalCommon.scrollAndFindViewById(elka_button_on)

        print("buttonOff enabled -> ") + str(buttonOff.__getattr__('enabled')())
        print("buttonOn  enabled -> ") + str(buttonOn.__getattr__('enabled')())
        asserts.assertEqual(buttonOff.__getattr__('enabled')(),False, "Off button is enabled")
        asserts.assertEqual(buttonOn.__getattr__('enabled')(),False, "On button is enabled")

        vHalCommon.assert_prop_equals(EMERGENCY_LANE_KEEPING_AID_ON, False)
        asserts.assertEqual(fr.get_LaneKeepAidRoadEdgeActv().Sts, 0, "on/off shall be 0")

        vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        fr.send_VehModMngtGlbSafe1(vehmod)

        time.sleep(buttonWaitTimeSeconds)
        vc.dump(window=-1)

        # Find buttons. They shall be Visible when CC is enabled
        elka_button_off = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_off"
        elka_button_on = VehicleHalCommon.app_context_vehiclefunctions + "emergency_lane_keeping_aid_button_on"
        buttonOff = vHalCommon.scrollAndFindViewById(elka_button_off)
        buttonOn = vHalCommon.scrollAndFindViewById(elka_button_on)

        print("buttonOff enabled -> ") + str(buttonOff.__getattr__('enabled')())
        print("buttonOn  enabled -> ") + str(buttonOn.__getattr__('enabled')())
        asserts.assertEqual(buttonOff.__getattr__('enabled')(),True, "Off button is disabled")
        asserts.assertEqual(buttonOn.__getattr__('enabled')(),True, "On button is disabled")

        buttonOff.touch()
        vc.sleep(buttonWaitTimeSeconds)

        # Verify that On/Off is false after Off button click.
        vHalCommon.assert_prop_equals(EMERGENCY_LANE_KEEPING_AID_ON, False)
        asserts.assertEqual(fr.get_LaneKeepAidRoadEdgeActv().Sts, 0, "on/off shall be 0")

        buttonOn.touch()
        vc.sleep(buttonWaitTimeSeconds)

        #Verify that On/Off is true after On button click.
        vHalCommon.assert_prop_equals(EMERGENCY_LANE_KEEPING_AID_ON, True)
        asserts.assertEqual(fr.get_LaneKeepAidRoadEdgeActv().Sts, 1, "on/off shall be 1")

        fr.stop_LaneKeepAidSts()


if __name__ == "__main__":
    test_runner.main()
