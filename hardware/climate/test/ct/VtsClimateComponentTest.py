#!/usr/bin/env python

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

class VtsClimateComponentTest(base_test.BaseTestClass):
    """Testing Climate functions"""

    def setUpClass(self):
        """Creates a mirror and init vehicle hal."""
        self.dut = self.registerController(android_device)[0]

        self.dut.shell.InvokeTerminal("one")
        self.dut.shell.one.Execute("setenforce 0")  # SELinux permissive mode
        if not precondition_utils.CanRunHidlHalTest(
            self, self.dut, self.dut.shell.one):
            self._skip_all_testcases = True
            return

        results = self.dut.shell.one.Execute("id -u system")
        system_uid = results[const.STDOUT][0].strip()
        logging.info("system_uid: %s", system_uid)

        if self.coverage.enabled:
            self.coverage.LoadArtifacts()
            self.coverage.InitializeDeviceCoverage(self.dut)

        if self.profiling.enabled:
            self.profiling.EnableVTSProfiling(self.dut.shell.one)

        self.dut.hal.InitHidlHal(
            target_type="vehicle",
            target_basepaths=self.dut.libPaths,
            target_version=2.0,
            target_package="android.hardware.automotive.vehicle",
            target_component_name="IVehicle",
            bits=int(self.abi_bitness))

        self.vehicle = self.dut.hal.vehicle  # shortcut
        self.vehicle.SetCallerUid(system_uid)
        self.vtypes = self.dut.hal.vehicle.GetHidlTypeInterface("types")
        logging.info("vehicle types: %s", self.vtypes)
        asserts.assertEqual(0x00ff0000, self.vtypes.VehiclePropertyType.MASK)
        asserts.assertEqual(0x0f000000, self.vtypes.VehicleArea.MASK)

    def tearDownClass(self):
        """Disables the profiling.

        If profiling is enabled for the test, collect the profiling data
        and disable profiling after the test is done.
        """
        if self._skip_all_testcases:
            return

        if self.profiling.enabled:
            self.profiling.ProcessTraceDataForTestCase(self.dut)
            self.profiling.ProcessAndUploadTraceData()

        if self.coverage.enabled:
            self.coverage.SetCoverageData(dut=self.dut, isGlobal=True)

    def setUp(self):
        self.propToConfig = {}
        for config in self.vehicle.getAllPropConfigs():
            self.propToConfig[config['prop']] = config
        self.configList = self.propToConfig.values()

    def emptyValueProperty(self, propertyId, areaId=0):
        """Creates a property structure for use with the Vehicle HAL.

        Args:
            propertyId: the numeric identifier of the output property.
            areaId: the numeric identifier of the vehicle area of the output
                    property. 0, or omitted, for global.

        Returns:
            a property structure for use with the Vehicle HAL.
        """
        return {
            'prop' : propertyId,
            'timestamp' : 0,
            'areaId' : areaId,
            'value' : {
                'int32Values' : [],
                'floatValues' : [],
                'int64Values' : [],
                'bytes' : [],
                'stringValue' : ""
            }
        }

    def readVhalProperty(self, propertyId, areaId=0):
        """Reads a specified property from Vehicle HAL.

        Args:
            propertyId: the numeric identifier of the property to be read.
            areaId: the numeric identifier of the vehicle area to retrieve the
                    property for. 0, or omitted, for global.

        Returns:
            the value of the property as read from Vehicle HAL, or None
            if it could not read successfully.
        """
        vp = self.vtypes.Py2Pb("VehiclePropValue",
                               self.emptyValueProperty(propertyId, areaId))
        logging.info("0x%x get request: %s", propertyId, vp)
        status, value = self.vehicle.get(vp)
        logging.info("0x%x get response: %s, %s", propertyId, status, value)
        if self.vtypes.StatusCode.OK == status:
            return value
        else:
            logging.warning("attempt to read property 0x%x returned error %d",
                            propertyId, status)

    def setVhalProperty(self, propertyId, value, areaId=0,
                        expectedStatus=0):
        """Sets a specified property in the Vehicle HAL.

        Args:
            propertyId: the numeric identifier of the property to be set.
            value: the value of the property, formatted as per the Vehicle HAL
                   (use emptyValueProperty() as a helper).
            areaId: the numeric identifier of the vehicle area to set the
                    property for. 0, or omitted, for global.
            expectedStatus: the StatusCode expected to be returned from setting
                    the property. 0, or omitted, for OK.
        """
        propValue = self.emptyValueProperty(propertyId, areaId)
        for k in propValue["value"]:
            if k in value:
                if k == "stringValue":
                    propValue["value"][k] += value[k]
                else:
                    propValue["value"][k].extend(value[k])
        vp = self.vtypes.Py2Pb("VehiclePropValue", propValue)
        logging.info("0x%x set request: %s", propertyId, vp)
        status = self.vehicle.set(vp)
        logging.info("0x%x set response: %s", propertyId, status)
        if 0 == expectedStatus:
            expectedStatus = self.vtypes.StatusCode.OK
        asserts.assertEqual(expectedStatus, status, "Prop 0x%x" % propertyId)

    def setAndVerifyIntProperty(self, propertyId, value, areaId=0):
        """Sets a integer property in the Vehicle HAL and reads it back.

        Args:
            propertyId: the numeric identifier of the property to be set.
            value: the int32 value of the property to be set.
            areaId: the numeric identifier of the vehicle area to set the
                    property for. 0, or omitted, for global.
        """
        self.setVhalProperty(propertyId, {"int32Values" : [value]}, areaId=areaId)

        propValue = self.readVhalProperty(propertyId, areaId=areaId)
        asserts.assertEqual(1, len(propValue["value"]["int32Values"]))
        asserts.assertEqual(value, propValue["value"]["int32Values"][0])

  
    def extractZonesAsList(self, supportedAreas):
        """Converts bitwise area flags to list of zones"""
        allZones = [
            self.vtypes.VehicleAreaZone.ROW_1_LEFT,
            self.vtypes.VehicleAreaZone.ROW_1_CENTER,
            self.vtypes.VehicleAreaZone.ROW_1_RIGHT,
            self.vtypes.VehicleAreaZone.ROW_1,
            self.vtypes.VehicleAreaZone.ROW_2_LEFT,
            self.vtypes.VehicleAreaZone.ROW_2_CENTER,
            self.vtypes.VehicleAreaZone.ROW_2_RIGHT,
            self.vtypes.VehicleAreaZone.ROW_2,
            self.vtypes.VehicleAreaZone.ROW_3_LEFT,
            self.vtypes.VehicleAreaZone.ROW_3_CENTER,
            self.vtypes.VehicleAreaZone.ROW_3_RIGHT,
            self.vtypes.VehicleAreaZone.ROW_3,
            self.vtypes.VehicleAreaZone.ROW_4_LEFT,
            self.vtypes.VehicleAreaZone.ROW_4_CENTER,
            self.vtypes.VehicleAreaZone.ROW_4_RIGHT,
            self.vtypes.VehicleAreaZone.ROW_4,
            self.vtypes.VehicleAreaZone.WHOLE_CABIN,
        ]

        extractedZones = []
        for zone in allZones:
            if (zone & supportedAreas == zone):
                extractedZones.append(zone)
        return extractedZones


    def getValueIfPropSupported(self, propertyId):
        """Returns tuple of boolean (indicating value supported or not) and the value itself"""
        if (propertyId in self.propToConfig):
            propValue = self.readVhalProperty(propertyId)
            asserts.assertNotEqual(None, propValue, "expected value, prop: 0x%x" % propertyId)
            asserts.assertEqual(propertyId, propValue['prop'])
            return True, self.extractValue(propValue)
        else:
            return False, None


    def extractValue(self, propValue):
        """Extracts value depending on data type of the property"""
        if propValue == None:
            return None

        # Extract data type
        dataType = propValue['prop'] & self.vtypes.VehiclePropertyType.MASK
        val = propValue['value']
        if self.vtypes.VehiclePropertyType.STRING == dataType:
            asserts.assertNotEqual(None, val['stringValue'])
            return val['stringValue']
        elif self.vtypes.VehiclePropertyType.INT32 == dataType or \
                self.vtypes.VehiclePropertyType.BOOLEAN == dataType:
            asserts.assertEqual(1, len(val["int32Values"]))
            return val["int32Values"][0]
        elif self.vtypes.VehiclePropertyType.INT64 == dataType:
            asserts.assertEqual(1, len(val["int64Values"]))
            return val["int64Values"][0]
        elif self.vtypes.VehiclePropertyType.FLOAT == dataType:
            asserts.assertEqual(1, len(val["floatValues"]))
            return val["floatValues"][0]
        elif self.vtypes.VehiclePropertyType.INT32_VEC == dataType:
            asserts.assertLess(0, len(val["int32Values"]))
            return val["int32Values"]
        elif self.vtypes.VehiclePropertyType.FLOAT_VEC == dataType:
            asserts.assertLess(0, len(val["floatValues"]))
            return val["floatValues"]
        elif self.vtypes.VehiclePropertyType.BYTES == dataType:
            asserts.assertLess(0, len(val["bytes"]))
            return val["bytes"]
        else:
            return val

    def verifyEnumPropIfSupported(self, propertyId, validValues):
        """Verifies that if given property supported it is one of the value in validValues set"""
        supported, val = self.getValueIfPropSupported(propertyId)
        if supported:
            asserts.assertEqual(int, type(val))
            self.assertIntValueInRangeForProp(val, validValues, propertyId)

    def assertLessOrEqual(self, first, second, msg=None):
        """Asserts that first <= second"""
        if second < first:
            fullMsg = "%s is not less or equal to %s" % (first, second)
            if msg:
                fullMsg = "%s %s" % (fullMsg, msg)
            fail(fullMsg)

    def assertIntValueInRangeForProp(self, value, validValues, prop):
        """Asserts that given value is in the validValues range"""
        asserts.assertTrue(value in validValues,
                "Invalid value %d for property: 0x%x, expected one of: %s" % (value, prop, validValues))

    def assertValueInRangeForProp(self, value, rangeBegin, rangeEnd, prop):
        """Asserts that given value is in the range [rangeBegin, rangeEnd]"""
        msg = "Value %s is out of range [%s, %s] for property 0x%x" % (value, rangeBegin, rangeEnd, prop)
        self.assertLessOrEqual(rangeBegin, value, msg)
        self.assertLessOrEqual(value, rangeEnd,  msg)

    def getPropConfig(self, propertyId):
        return self.propToConfig[propertyId]

    def isPropSupported(self, propertyId):
        return self.getPropConfig(propertyId) is not None


    def testFanLevel(self):
        _s = 0.5

        kwargs1 = {'ignoreversioncheck': False, 'verbose': True, 'ignoresecuredevice': False, 'serialno': '.*'}
        device, serialno = ViewClient.connectToDeviceOrExit(**kwargs1)
        kwargs2 = {'forceviewserveruse': False, 'useuiautomatorhelper': False, 'ignoreuiautomatorkilled': True,
                   'autodump': False, 'startviewserver': True, 'compresseddump': True}
        vc = ViewClient(device, serialno, **kwargs2)

        # Open hvac default ui
        device.touchDip(86.0, 1011.0, 0)
        vc.sleep(_s)

        fan_level_OFF_location = (186.0, 643.0, 0)
        fan_level_2_location = (267.0, 634.0, 0)
        fan_level_3_location = (343.0, 635.0, 0)
        fan_level_4_location = (423.0, 636.0, 0)
        fan_level_5_location = (500.0, 635.0, 0)
        fan_level_MAX_location = (589.0, 631.0, 0)
       

        # Click fan level OFF and assert
        device.touchDip(*fan_level_OFF_location)
        val = self.readVhalProperty(self.vtypes.VehicleProperty.HVAC_FAN_SPEED, self.vtypes.VehicleAreaZone.ROW_1)
        asserts.assertEqual(self.extractValue(val), 1)
        vc.sleep(_s)

        # Click fan level 2 and assert
        device.touchDip(*fan_level_2_location)
        val = self.readVhalProperty(self.vtypes.VehicleProperty.HVAC_FAN_SPEED, self.vtypes.VehicleAreaZone.ROW_1)
        asserts.assertEqual(self.extractValue(val), 2)
        vc.sleep(_s)

        # Click fan level 3 and assert
        device.touchDip(*fan_level_3_location)
        val = self.readVhalProperty(self.vtypes.VehicleProperty.HVAC_FAN_SPEED, self.vtypes.VehicleAreaZone.ROW_1)
        asserts.assertEqual(self.extractValue(val), 3)
        vc.sleep(_s)

        # Click fan level 4 and assert
        device.touchDip(*fan_level_4_location)
        val = self.readVhalProperty(self.vtypes.VehicleProperty.HVAC_FAN_SPEED, self.vtypes.VehicleAreaZone.ROW_1)
        asserts.assertEqual(self.extractValue(val), 4)
        vc.sleep(_s)

        # Click fan level 5 and assert
        device.touchDip(*fan_level_5_location)
        val = self.readVhalProperty(self.vtypes.VehicleProperty.HVAC_FAN_SPEED, self.vtypes.VehicleAreaZone.ROW_1)
        asserts.assertEqual(self.extractValue(val), 5)
        vc.sleep(_s)

        # Click fan level MAX and assert
        device.touchDip(*fan_level_MAX_location)
        val = self.readVhalProperty(self.vtypes.VehicleProperty.HVAC_FAN_SPEED, self.vtypes.VehicleAreaZone.ROW_1)
        asserts.assertEqual(self.extractValue(val), 6)
        vc.sleep(_s)

        # Close hvac default ui
        device.touchDip(82.0, 971.0, 0)
        vc.sleep(_s)


if __name__ == "__main__":
    test_runner.main()
