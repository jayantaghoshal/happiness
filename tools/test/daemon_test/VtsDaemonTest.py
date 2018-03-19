#!/usr/bin/env python

# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import test_runner
from vts.utils.python.controllers import adb
from vts.utils.python.controllers import android_device
from vts.utils.python.common import vts_spec_utils
import sys
import time
from time import sleep
sys.path.append('/usr/local/lib/python2.7/dist-packages')
from generated.pyDataElements import \
    FrSignalInterface, \
    UsgModSts
from typing import List

# Service names according to *.rc-file for your component.
# Verifies that the service is running with "init.svc.xxx" property
services_to_test = [
    "dataelements-hidl-server",
    "vehicle-signals-daemon",
    "netboyd",
    "profilemanager-hidl-server",
    "soundnotifications",
    "settingstorage-hidl-server"
]

# Process names to test that they are alive and don't restart
process_names_to_test = [
    "dataelements-hidl-server",
    "vehicle-signals-daemon",
    "netboyd",
    "soundnotifications",
    "vendor.volvocars.hardware.settingsstorage@1.0-service"
]

# Add specific HALs to check with lshal
hals_to_check = [
    "android.hardware.automotive.vehicle@2.0::IVehicle/default",
    "vendor.volvocars.hardware.iplm@1.0::IIplm/default",
    "vendor.volvocars.hardware.signals@1.0::ISignals/default",
    "vendor.volvocars.hardware.vehiclecom@1.0::IVehicleCom/ipcb",
    "vendor.volvocars.hardware.vehiclecom@1.0::IVehicleCom/iplm",
    "vendor.volvocars.hardware.profiles@1.0::ICarProfileManager/default",
    "vendor.volvocars.hardware.settings@1.0::ISettingsStorage/default"
]

# Add specific properties and expected value to check with getprop
expected_properties = [
    ("netboyd.startup_completed", "1"),
    ("netmand.startup_completed", "1"),
    ("ro.boot.swdl.session", "default"),
    ("persist.swdl.EnableAutoFlashing", "1"), # service ensuring VIP is on correct/matching baseline is enabled
]
# fdegroot, caused by kraken, fix offline
#    ("swdl.vip_version_ok", "1"), # and it confirmed that version is ok (it might have been reflashed, but this is final confirmation)


# Add full path of a file to check if it exsist
files_to_exist = [
] # type: List[str]

class ComponentTest(base_test.BaseTestClass):

    def setUpClass(self):
        self.dut = self.registerController(android_device)[0]

    def testServices(self):
        for service in services_to_test:
            retval = self.dut.adb.shell('getprop init.svc.%s' % service)
            asserts.assertEqual(retval.strip(), "running", 'Service {0} is not running'.format(service))


    def testHalsExisting(self):

        missing_hal_list = list()

        for hal in hals_to_check:
            hal_process_name = self.dut.adb.shell('lshal | grep --fixed-strings {0} | cut -f1'.format(hal)).strip()

            if (hal_process_name == ''):
                missing_hal_list.append(' [' + hal + ']')

        if(len(missing_hal_list) > 0):
            asserts.fail('HALs missing: ' + ''.join(missing_hal_list))


    def testHalProcessessNotChanging(self):

        halDictionary = {}
        halErrorDictionary = {}

        # Store away the initial processes for comparison
        for hal in hals_to_check:
            hal_process_name = self.dut.adb.shell('lshal | grep --fixed-strings {0} | cut -f1'.format(hal)).strip()
            hal_process_id = self.dut.adb.shell('lshal | grep --fixed-strings {0} | cut -f2'.format(hal)).strip()
            halDictionary[hal_process_name] = hal_process_id

        # Compare for 10 seconds that the process id stays the same
        end = time.time() + 10
        while time.time() < end:
            for hal in hals_to_check:
                try:
                    hal_process_id = self.dut.adb.shell('lshal | grep --fixed-strings {0} | cut -f2'.format(hal)).strip()

                    if (hal_process_id != halDictionary[hal]):
                          halErrorDictionary[hal] = hal_process_id
                except:
                    pass

            time.sleep(0.1)

        if(halErrorDictionary):
            text = 'HAL processes restarted'
            for key, value in halErrorDictionary.items():
                text += ': ' + key + ' ( ' + halDictionary[key] + ' -> ' + value + ' ) '

            asserts.fail(text)


    def testServicesNotRestart(self):
        process = {}
        for process_name in process_names_to_test:
            try:
                retval = self.dut.adb.shell('pidof %s' % process_name)
                process[process_name] = retval.strip()
            except:
                asserts.fail('No process ID to process name "{0}" found'.format(process_name))
        end = time.time() + 10
        while time.time() < end:
            for process_name in process_names_to_test:
                try:
                    asserts.assertEqual(process.get(process_name),
                        self.dut.adb.shell('pidof %s' % process_name).strip(),
                        'Process ID of service {0} was changed'.format(process_name))
                except:
                    asserts.fail('Could not access process ID for service {0}'.format(process_name))
            time.sleep(0.1)

    def testProperties(self):
        for prop, value in expected_properties:
            retval = self.dut.adb.shell('getprop %s' % prop)
            asserts.assertNotEqual(retval.strip(), '', 'No value to property {0} found'.format(prop))
            asserts.assertEqual(retval.strip(), value, 'Property {0} not set'.format(prop))

    def testFileExist(self):
        for f in files_to_exist:
            try:
                retval = self.dut.adb.shell('ls %s' % f)
            except:
                asserts.fail('File {0} not found'.format(f))

if __name__ == "__main__":
    test_runner.main()
