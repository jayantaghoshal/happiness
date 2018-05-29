#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import sys
import time

from vts.runners.host import test_runner

sys.path.append('/usr/local/lib/python2.7/dist-packages')

from generated import datatypes as DE
from vf_common import vf_base_test
from vf_common import uiautomator_common
from vf_common.signals_common import wait_for_fr_signal
from vf_common.vf_vts_common import wait_for

app_context = "com.volvocars.vehiclefunctions:id/"
acc_button_on = app_context + "adaptive_cruise_control_on"
cc_button_on = app_context + "cruise_control_on"
sl_button_on = app_context + "speed_limiter_on"

DRIVER_SUPPORT_FUNCTION_MODE = 557842459  # Hard coded VCC-HAL property ID for DRIVER_SUPPORT_FUNCTION
CRUISE_CONTROL = 0
ADAPTIVE_CRUISE_CONTROL = 1
SPEED_LIMITER = 2


class VtsDriverSupportComponentTest(vf_base_test.VFBaseTest):

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons not present CC36 <= 1
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportCCDisabled1(self):
        self.change_carconfig_and_reboot_if_needed([(23, 1), (36, 1)])
        self.setUpVehicleFunction()

        self.ui.scrollAndAssertViewsAreNotFound([
            acc_button_on,
            cc_button_on,
            sl_button_on])

        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons not present CC36 > 1
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportCCDisabled2(self):
        self.change_carconfig_and_reboot_if_needed([(23, 1), (36, 2)])
        self.setUpVehicleFunction()

        self.ui.scrollAndAssertViewsAreNotFound([
            acc_button_on,
            cc_button_on,
            sl_button_on])

        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.SL)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons present CC23=2 && CC36>1, 2 Buttons CC && SL
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportCCAndSLEnabled(self):
        self.change_carconfig_and_reboot_if_needed([(23, 2), (36, 2)])

        # Clear database
        self.dut.shell.one.Execute("stop settingstorage-hidl-server")
        self.dut.shell.one.Execute("rm /data/vendor/vehiclesettings/vehiclesettings.db*")
        self.dut.shell.one.Execute("start settingstorage-hidl-server")

        self.setUpVehicleFunction()

        self.ui.scrollAndAssertViewsAreNotFound([acc_button_on])
        cc_button = self.ui.scrollDownAndFindViewById(cc_button_on)
        sl_button = self.ui.scrollDownAndFindViewById(sl_button_on)

        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)

        sl_button.click()
        self.vhal.wait_for_Int32_equals(SPEED_LIMITER, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.SL)

        # UsgModSts NOT_ACTIVE
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModInActv)

        cc_button.click()
        self.vhal.wait_for_Int32_equals(SPEED_LIMITER, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        sl_button.click()
        self.vhal.wait_for_Int32_equals(SPEED_LIMITER, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        # UsgModSts ACTIVE
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)

        cc_button.click()
        self.vhal.wait_for_Int32_equals(CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        sl_button.click()
        self.vhal.wait_for_Int32_equals(SPEED_LIMITER, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.SL)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons present CC23>2 && CC36<=1, 2 Buttons CC && ACC
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportACCandCCEnabled(self):
        self.change_carconfig_and_reboot_if_needed([(23, 3), (36, 1)])

        # Clear database
        self.dut.shell.one.Execute("stop settingstorage-hidl-server")
        self.dut.shell.one.Execute("rm /data/vendor/vehiclesettings/vehiclesettings.db*")
        self.dut.shell.one.Execute("start settingstorage-hidl-server")

        self.setUpVehicleFunction()

        self.ui.scrollAndAssertViewsAreNotFound([sl_button_on])
        acc_button = self.ui.scrollDownAndFindViewById(acc_button_on)
        cc_button = self.ui.scrollDownAndFindViewById(cc_button_on)

        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)

        acc_button.click()
        self.vhal.wait_for_Int32_equals(ADAPTIVE_CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.ACC)

        # UsgModSts NOT_ACTIVE
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModInActv)

        cc_button.click()
        self.vhal.wait_for_Int32_equals(ADAPTIVE_CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        acc_button.click()
        self.vhal.wait_for_Int32_equals(ADAPTIVE_CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        # UsgModSts ACTIVE
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)

        cc_button.click()
        self.vhal.wait_for_Int32_equals(CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        acc_button.click()
        self.vhal.wait_for_Int32_equals(ADAPTIVE_CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.ACC)

    # ----------------------------------------------------------------------------------------------------------
    # Test buttons present CC23>2 && CC36>1, All 3 Buttons CC,ACC,SL
    # ----------------------------------------------------------------------------------------------------------
    def testDriverSupportAllEnabled(self):
        self.change_carconfig_and_reboot_if_needed([(23, 3), (36, 2)])

        # Clear database
        self.dut.shell.one.Execute("stop settingstorage-hidl-server")
        self.dut.shell.one.Execute("rm /data/vendor/vehiclesettings/vehiclesettings.db*")
        self.dut.shell.one.Execute("start settingstorage-hidl-server")

        self.setUpVehicleFunction()

        acc_button = self.ui.scrollDownAndFindViewById(acc_button_on)
        cc_button = self.ui.scrollDownAndFindViewById(cc_button_on)
        sl_button = self.ui.scrollDownAndFindViewById(sl_button_on)

        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.CC)

        # ----------------------------------------------------------------------------------------------------------
        # Test active state
        # ----------------------------------------------------------------------------------------------------------
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)

        sl_button.click()
        self.vhal.wait_for_Int32_equals(SPEED_LIMITER, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.SL)

        acc_button.click()
        self.vhal.wait_for_Int32_equals(ADAPTIVE_CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.ACC)

        # UsgModSts NOT_ACTIVE
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModInActv)

        sl_button.click()
        self.vhal.wait_for_Int32_equals(ADAPTIVE_CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.SL)

        acc_button.click()
        self.vhal.wait_for_Int32_equals(ADAPTIVE_CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.SL)

        cc_button.click()
        self.vhal.wait_for_Int32_equals(ADAPTIVE_CRUISE_CONTROL, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.SL)

        # UsgModSts ACTIVE
        self.signals.set_usage_mode(DE.UsgModSts1.UsgModActv)

        sl_button.click()
        self.vhal.wait_for_Int32_equals(SPEED_LIMITER, DRIVER_SUPPORT_FUNCTION_MODE)
        wait_for_fr_signal(self.fr.get_UsrSeldDrvrSpprtFct, DE.DrvrSpprtFct.SL)


if __name__ == "__main__":
    test_runner.main()

