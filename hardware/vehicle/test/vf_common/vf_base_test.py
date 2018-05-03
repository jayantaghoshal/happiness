# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import time
import sys
sys.path.append('/usr/local/lib/python2.7/dist-packages')
import typing
from . import signals_common
from . import vehiclehalcommon2
from . import user_common
from . import uiautomator_common
from vts.runners.host import base_test
from vts.runners.host import const
from vts.utils.python.controllers import android_device
from uiautomator import device as device_default
from generated import dataelements_abc, dataelements_fdx

class VFBaseTest(base_test.BaseTestClass):
    def __init__(self, configs, with_flexray=True):
        super(VFBaseTest, self).__init__(configs)
        self.with_flexray = with_flexray
        self.vhal = None  # type: typing.Optional[vehiclehalcommon2.VehicleHalCommon]
        self.users = None  # type: typing.Optional[user_common.UserManagement]
        self.signals = None  # type: typing.Optional[signals_common.Signals]
        self.ui = uiautomator_common.UIAutomatorCommon()
        self.fr = None  # type: typing.Optional[dataelements_abc.DataElements]

    def _setUpClass(self, ):
        super(VFBaseTest, self)._setUpClass()
        self.dut = self.android_devices[0]
        return self.setUpClass()

    def _setUp(self, test_name):
        super(VFBaseTest, self)._setUp(test_name)
        """Proxy function to guarantee the base implementation of setUp
        is called.
        """

        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        results = self.dut.shell.one.Execute("id -u system")
        self.system_uid = results[const.STDOUT][0].strip()
        if self.with_flexray:
            self.fr = signals_common.get_dataelements_connection(self.dut.adb)
            self.signals = signals_common.Signals(self.fr)
        self.vhal = vehiclehalcommon2.VehicleHalCommon(self.dut, self.system_uid)
        self.users = user_common.UserManagement(self.dut)

        return self.setUp()

    def _tearDown(self, test_name):
        if self.with_flexray:
            try:
                self.fr.close()
            except:
                pass
        super(VFBaseTest, self)._tearDown(test_name)
        return self.tearDown()

    def shell(self, command):
        return self.dut.shell.one.Execute(command)

    # Returns true if reboot is required after changing carconfig
    def change_carconfig(self, parameters):
        # type: (typing.List[typing.Tuple[int, int]]) -> bool
        for param, value in parameters:
            self.dut.shell.one.Execute("changecarconfig %d %d" % (param, value))
        return True #TODO: https://icup_android.gerrit.cm.volvocars.biz/#/c/6716/

    def change_carconfig_and_reboot_if_needed(self, parameters):
        # type: (typing.List[typing.Tuple[int, int]]) -> None
        if self.change_carconfig(parameters):
            self.deviceReboot()

    def deviceReboot(self):
        if self. with_flexray and self.fr is not None:
            self.fr.close()
        time.sleep(1)
        self.dut.shell.one.Execute("reboot")
        self.dut.stopServices()
        self.dut.waitForBootCompletion()
        self.dut.startServices()
        self.dut.shell.InvokeTerminal("one")
        time.sleep(1)
        if self.with_flexray:
            self.fr = signals_common.get_dataelements_connection(self.dut.adb)
            self.signals.fr = self.fr   #hmmm, recreate the whole Signals or not, probably not???
        time.sleep(1)
        self.vhal = vehiclehalcommon2.VehicleHalCommon(self.dut, self.system_uid)


    def setUpVehicleFunction(self):
        self.users.waitUntilUserNotOwner()

        self.press_homebutton()
        self.dut.adb.shell('am start -n \"com.volvocars.vehiclefunctions/com.volvocars.vehiclefunctions.VehicleFunctionsActivity\" -a \"android.intent.action.MAIN\" -c \"android.intent.category.LAUNCHER\"')

        self.ui.waitUntilViewAvailable("com.volvocars.vehiclefunctions:id/assistance_item")

        drive_assistant = self.ui.getViewOrRaise("com.volvocars.vehiclefunctions:id/assistance_item")
        drive_assistant.click()

    def press_homebutton(self):
        self.dut.adb.shell('input keyevent 3')