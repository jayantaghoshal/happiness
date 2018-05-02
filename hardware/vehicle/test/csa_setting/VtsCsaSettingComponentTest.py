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
    OnOffPen

from generated import datatypes as DE

import logging.config
import logging.handlers

# export VECTOR_FDX_IP=198.18.34.2
from vehiclehalcommon import VehicleHalCommon, wait_for_signal
csa_button_off = VehicleHalCommon.app_context_vehiclefunctions + "curve_speed_adaption_off"
csa_button_on = VehicleHalCommon.app_context_vehiclefunctions + "curve_speed_adaption_on"

number_of_scrolling = 20

class VtsCsaSettingsComponentTest(base_test.BaseTestClass):
    def setUpClass(self):


        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Setup")
        logging.info("----------------------------------------------------------------------------------------------------------")
        self.dut = self.registerController(android_device)[0]
        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()
        self.fr = vehiclehalcommon.get_dataelements_connection(self.dut.adb)
        self.delay = 1


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

        # Setting reset stuff
        # self.dut.adb.shell('stop settingstorage-hidl-server')  # NOLINT
        # time.sleep(1)
        # self.dut.adb.shell('rm /data/vendor/vehiclesettings/vehiclesettings.db*') # NOLINT


    def testFunctionNotPresent(self):
        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function NOT-PRESENT using CarConfig")
        logging.info("----------------------------------------------------------------------------------------------------------")

        # Combined as false
        self.dut.shell.one.Execute("changecarconfig 23 3")#true
        self.dut.shell.one.Execute("changecarconfig 148 2")#false

        self.deviceReboot()
        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()
        vc, device = vHalCommon.getActiveViewClient()
        vc.dump(window=-1)
        vehmod = vHalCommon.GetDefaultInitiated_VehModMngtGlbSafe1()

        self.CURVE_SPEED_ADAPTION_ON = vHalCommon.get_id('CURVE_SPEED_ADAPTION_ON')
        self.CURVE_SPEED_ADAPTION_STATUS = vHalCommon.get_id('CURVE_SPEED_ADAPTION_STATUS')

        logging.info("ACTION (function setting is On by default)")
        self.fr.send_ProfPenSts1(DE.IdPen.ProfUkwn)
        self.fr.send_CrvtSpdWarnSts(DE.FctSts2.On)#true
        self.fr.send_CrvtSpdAdpvSts(DE.OffOnNotAvlSrvRqrd.On)#true

        vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        self.fr.send_VehModMngtGlbSafe1(vehmod)
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        csa_off = vHalCommon.scrollAndFindViewById(csa_button_off, number_of_scrolling)
        csa_on = vHalCommon.scrollAndFindViewById(csa_button_on, number_of_scrolling)

        asserts.assertEqual(None, csa_off, "Expect no csa_off")
        asserts.assertEqual(None, csa_on, "Expect no csa_on")

        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, None, "CURVE_SPEED_ADAPTION_ON (function NOT-PRESENT using CarConfig)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.Off, "AccAdprTurnSpdActv (function NOT-PRESENT using CarConfig)")



    def testFunctionPresent(self):
        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function PRESENT using CarConfig")
        logging.info("----------------------------------------------------------------------------------------------------------")

        # Combined as true
        self.dut.shell.one.Execute("changecarconfig 23 3")#true
        self.dut.shell.one.Execute("changecarconfig 148 3")#true

        self.deviceReboot()
        vHalCommon = vehiclehalcommon.VehicleHalCommon(self.dut, self.system_uid, with_flexray_legacy=False)
        vHalCommon.setUpVehicleFunction()
        vc, device = vHalCommon.getActiveViewClient()
        vc.dump(window=-1)
        vehmod = vHalCommon.GetDefaultInitiated_VehModMngtGlbSafe1()

        self.CURVE_SPEED_ADAPTION_ON = vHalCommon.get_id('CURVE_SPEED_ADAPTION_ON')
        self.CURVE_SPEED_ADAPTION_STATUS = vHalCommon.get_id('CURVE_SPEED_ADAPTION_STATUS')

        logging.info("ACTION (function setting is On by default)")
        self.fr.send_ProfPenSts1(DE.IdPen.ProfUkwn)
        self.fr.send_CrvtSpdWarnSts(DE.FctSts2.On)#true
        self.fr.send_CrvtSpdAdpvSts(DE.OffOnNotAvlSrvRqrd.On)#true

        vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        self.fr.send_VehModMngtGlbSafe1(vehmod)
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        # Get buttons (If buttons are found, assert is deemed as true.)
        csa_off = vHalCommon.scrollAndFindViewByIdOrRaise(csa_button_off, number_of_scrolling)
        csa_on = vHalCommon.scrollAndFindViewByIdOrRaise(csa_button_on, number_of_scrolling)
        csa_off.touch()
        csa_on.touch() #####<- This or some other means to put function in known mode
        vc.sleep(self.delay)

        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 1, "CURVE_SPEED_ADAPTION_ON (function PRESENT using CarConfig)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.On, "AccAdprTurnSpdActv (function PRESENT using CarConfig)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function IN-ACTIVE using UsgMod")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        vehmod.UsgModSts =  DE.UsgModSts1.UsgModAbdnd
        self.fr.send_VehModMngtGlbSafe1(vehmod)

        # csa_on.touch() # Stress the function as this button shouldnt change the output ####################
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 0, "CURVE_SPEED_ADAPTION_ON (IN-ACTIVE using UsgMod)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.Off, "AccAdprTurnSpdActv (IN-ACTIVE using UsgMod)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function ACTIVE using UsgMod")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        vehmod.UsgModSts =  DE.UsgModSts1.UsgModDrvg
        self.fr.send_VehModMngtGlbSafe1(vehmod)
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 1, "CURVE_SPEED_ADAPTION_ON (Active using UsgMod)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.On, "AccAdprTurnSpdActv (ACTIVE using UsgMod)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function SYSTEM-ERROR using CrvtSpdAdpvSts")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        self.fr.send_CrvtSpdAdpvSts(DE.OffOnNotAvlSrvRqrd.SrvRqrd)#false
        vc.sleep(self.delay)
        #csa_on.touch() # Touch this button shouldnt change the output
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 0, "CURVE_SPEED_ADAPTION_ON (function SYSTEM-ERROR using CrvtSpdAdpvSts)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.Off, "AccAdprTurnSpdActv (function SYSTEM-ERROR using CrvtSpdAdpvSts)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function ACTIVE using CrvtSpdAdpvSts")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        self.fr.send_CrvtSpdAdpvSts(DE.OffOnNotAvlSrvRqrd.On)#false
        vc.sleep(self.delay)
        #csa_on.touch() # Touch this button shouldnt change the output
        #vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 1, "CURVE_SPEED_ADAPTION_ON (function ACTIVE using CrvtSpdAdpvSts)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.On, "AccAdprTurnSpdActv (function ACTIVE using CrvtSpdAdpvSts)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function SYSTEM-ERROR using CrvtSpdWarnSts timeout")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        self.fr.stop_CrvtSpdWarnSts()#false
        vc.sleep(self.delay)
        #csa_on.touch() # Touch this button shouldnt change the output
        #vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 0, "CURVE_SPEED_ADAPTION_ON (function SYSTEM-ERROR using CrvtSpdWarnSts timeout)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.Off, "AccAdprTurnSpdActv (function SYSTEM-ERROR using CrvtSpdWarnSts timeout)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function ACTIVE using CrvtSpdWarnSts")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        self.fr.send_CrvtSpdWarnSts(DE.FctSts2.On)#true
        vc.sleep(self.delay)
        # csa_on.touch() # Touch this button shouldnt change the output
        # vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 1, "CURVE_SPEED_ADAPTION_ON (function ACTIVE using CrvtSpdWarnSts)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.On, "AccAdprTurnSpdActv (function ACTIVE using CrvtSpdWarnSts)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function OFF")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        csa_off.touch()
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 0, "CURVE_SPEED_ADAPTION_ON (set function OFF)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.Off, "AccAdprTurnSpdActv (set function OFF)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function ON")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        csa_on.touch()
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 1, "CURVE_SPEED_ADAPTION_ON (set function ON)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.On, "AccAdprTurnSpdActv (set function ON)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set function OFF")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        csa_off.touch()
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 0, "CURVE_SPEED_ADAPTION_ON (set function OFF)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.Off, "AccAdprTurnSpdActv (set function OFF)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set new User for default value")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        self.fr.send_ProfPenSts1(DE.IdPen.Prof3)
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 1, "CURVE_SPEED_ADAPTION_ON (set new User for default value)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.On, "AccAdprTurnSpdActv (set new User for default value)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Pen, DE.IdPen.Prof3, "AccAdprTurnSpdActv (set new User for default value)")



        logging.info("----------------------------------------------------------------------------------------------------------")
        logging.info("Test set previous User for stored value")
        logging.info("----------------------------------------------------------------------------------------------------------")

        logging.info("ACTION")
        self.fr.send_ProfPenSts1(DE.IdPen.ProfUkwn)
        vc.sleep(self.delay)

        logging.info("EVALUATE")
        vHalCommon.assert_prop_equals(self.CURVE_SPEED_ADAPTION_ON, 0, "CURVE_SPEED_ADAPTION_ON (set new User for default value)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Sts, DE.OnOff1.Off, "AccAdprTurnSpdActv (set new User for default value)")
        asserts.assertEqual(self.fr.get_AccAdprTurnSpdActv().Pen, DE.IdPen.ProfUkwn, "AccAdprTurnSpdActv (set new User for default value)")

if __name__ == "__main__":
    test_runner.main()
