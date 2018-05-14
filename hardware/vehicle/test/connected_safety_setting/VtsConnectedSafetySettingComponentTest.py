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
import threading
from generated.datatypes import \
    VehModMngtGlbSafe1, \
    FctSts2

from generated import datatypes as DE
import logging.config
import logging.handlers

from vehiclehalcommon import VehicleHalCommon, wait_for_signal

# Get button Ids for testing.
cs_button_off = VehicleHalCommon.app_context_vehiclefunctions + "connected_safety_button_off"
cs_button_on  = VehicleHalCommon.app_context_vehiclefunctions + "connected_safety_button_on"

buttonWaitTimeSeconds = 1.5

class VtsConnectedSafetySettingComponentTest(base_test.BaseTestClass):

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
        time.sleep(5)

    # ----------------------------------------------------------------------------------------------------
    # Test Car Config Disabled
    # ----------------------------------------------------------------------------------------------------
    def testConnectedSafetyCCDisabled(self):

        print("----------- testConnectedSafetyCCDisabled -----------")

        self.dut.shell.one.Execute("changecarconfig 147 1")

        # Reboot device after changing carconfig
        self.deviceReboot()

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

        vc, device = vHalCommon.getActiveViewClient()
        vc.dump(window=-1)

        # UsgModSts Drive
        # RoadFricIndcnSts On (not SrvRqrd)
        fr.send_RoadFricIndcnSts(DE.FctSts2.On)
        vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(buttonWaitTimeSeconds)

        # Find buttons. They shall be disabled when CC is disabled.
        buttonOff = vHalCommon.scrollAndFindViewById(cs_button_off)
        buttonOn = vHalCommon.scrollAndFindViewById(cs_button_on)
        print("buttonOff -> ") + str(buttonOff)
        print("buttonOn -> ") + str(buttonOn)
        asserts.assertEqual(None, buttonOff, "Expect no buttonOff")
        asserts.assertEqual(None, buttonOn, "Expect no buttonOn")

        # Function shall be off on Flexray.
        asserts.assertEqual(fr.get_RoadFricIndcnActv().Sts, 0, "on/off shall be 0")

        time.sleep(buttonWaitTimeSeconds)
        fr.stop_RoadFricIndcnSts()

    # ----------------------------------------------------------------------------------------------------
    # Test Car Config Enabled
    # ----------------------------------------------------------------------------------------------------
    def testConnectedSafetyCCEnabledCSOnOff(self):

        print("----------- testConnectedSafetyCCEnabledCSOnOff -----------")

        self.dut.shell.one.Execute("changecarconfig 147 2")
        self.dut.shell.one.Execute("changecarconfig 23 9")

        # Reboot device after changing carconfig
        self.deviceReboot()

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

        vc, device = vHalCommon.getActiveViewClient()
        vc.dump(window=-1)

        CONNECTED_SAFETY_ON = vHalCommon.get_id('CONNECTED_SAFETY_ON')

        # UsgModSts Drive
        # RoadFricIndcnSts On (not SrvRqrd)
        fr.send_RoadFricIndcnSts(DE.FctSts2.On)
        fr.send_VehModMngtGlbSafe1(vehmod)
        time.sleep(buttonWaitTimeSeconds)

        # Get buttons. They shall be here if CC is enabled.
        buttonOff = vHalCommon.scrollAndFindViewByIdOrRaise(cs_button_off)
        buttonOn = vHalCommon.scrollAndFindViewByIdOrRaise(cs_button_on)
        print("buttonOff -> ") + str(buttonOff)
        print("buttonOn -> ") + str(buttonOn)

        vc.dump(window=-1)
        buttonOff = vc.findViewByIdOrRaise(cs_button_off)
        buttonOn = vc.findViewByIdOrRaise(cs_button_on)

        print("buttonOff enabled -> ") + str(buttonOff.__getattr__('enabled')())
        print("buttonOn  enabled -> ") + str(buttonOn.__getattr__('enabled')())
        asserts.assertEqual(buttonOff.__getattr__('enabled')(),True, "Off button is disabled")
        asserts.assertEqual(buttonOn.__getattr__('enabled')(),True, "On button is disabled")

        buttonOn.touch()
        vc.sleep(buttonWaitTimeSeconds)

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------

        print("----------- UsgModSts Active -----------")

        buttonOff.touch()
        vc.sleep(buttonWaitTimeSeconds)
        # Verify that On/Off is false after Off button click.
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, False)
        asserts.assertEqual(fr.get_RoadFricIndcnActv().Sts, 0, "on/off shall be 0")

        buttonOn.touch()
        vc.sleep(buttonWaitTimeSeconds)

        # Verify that On/Off is true after On button click.
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, True)
        asserts.assertEqual(fr.get_RoadFricIndcnActv().Sts, 1, "on/off shall be 1")

        # ----------------------------------------------------------------------------------------------------------
        # Test inactive state
        # ----------------------------------------------------------------------------------------------------------

        print("----------- UsgModSts Inactive -----------")

        vehmod.UsgModSts = DE.UsgModSts1.UsgModInActv
        fr.send_VehModMngtGlbSafe1(vehmod)
        vc.sleep(buttonWaitTimeSeconds)

        # On/Off shall be default 0.
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, False)
        asserts.assertEqual(fr.get_RoadFricIndcnActv().Sts, 0, "on/off shall be 0")

        # Set usage mode UsgModDrvg.
        vehmod.UsgModSts = DE.UsgModSts1.UsgModDrvg
        fr.send_VehModMngtGlbSafe1(vehmod)
        vc.sleep(buttonWaitTimeSeconds)

        # On/Off shall be restored.
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, True)
        asserts.assertEqual(fr.get_RoadFricIndcnActv().Sts, 1, "on/off shall be 1")

        # # ----------------------------------------------------------------------------------------------------------
        # # Test System Error
        # # ----------------------------------------------------------------------------------------------------------
        print("System Error test")

        # Set Service required.
        fr.send_RoadFricIndcnSts(DE.FctSts2.SrvRqrd)

        # UsgModSts ACTIVE
        vehmod.UsgModSts = DE.UsgModSts1.UsgModActv
        fr.send_VehModMngtGlbSafe1(vehmod)
        vc.sleep(buttonWaitTimeSeconds)

        print("System Error test, pre fr check")
        # Connected Safety shall be Off on Flexray on system error.
        asserts.assertEqual(fr.get_RoadFricIndcnActv().Sts, 0, "on/off shall be 0")
        print("System Error test, pre prop check")
        # Button/Prop On/Off shall be Off on System error.
        vHalCommon.assert_prop_equals(CONNECTED_SAFETY_ON, False)

if __name__ == "__main__":
    test_runner.main()
