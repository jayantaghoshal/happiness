#!/usr/bin/env python

# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
import sys
import os
import typing

from vts.runners.host import asserts
from vts.runners.host import base_test
from vts.runners.host import const
from vts.runners.host import keys
from vts.runners.host import test_runner
from vts.utils.python.controllers import android_device
from vts.utils.python.precondition import precondition_utils
from subprocess import call
from com.dtmilano.android.viewclient import ViewClient, ViewNotFoundException

from generated.pyDataElements import \
    FrSignalInterface, \
    BaseEnumSender, \
    BaseBoolSender, \
    BaseFloatSender, \
    BaseIntegerSender

PydataElementsSenderType = typing.Union[BaseEnumSender, BaseBoolSender, BaseFloatSender, BaseIntegerSender]

property_list = [
    ('CONNECTED_SAFETY_ON', 557842436),
    ('DAI_SETTING', 557842437),
    ('CURVE_SPEED_ADAPTION_ON', 555745286),
]

def wait_for_signal(fr_interface, fdx_signal, expected_value, timeout_sec):
    # type: (FrSignalInterface, PydataElementsSenderType, int, int) -> None

    if fr_interface.connected:
        end = time.time() + timeout_sec
        read_value = fdx_signal.get()
        while time.time() < end:
            read_value = fdx_signal.get()
            if read_value == expected_value:
                break
            time.sleep(0.2)
        asserts.assertEqual(read_value, expected_value,
                            "Expected signal %s to be %d within %d sec, got %d)" %
                            (fdx_signal.fdx_name, expected_value, timeout_sec, fdx_signal.get()))

class VehicleHalCommon():

    def __init__(self, dut, system_uid):

        try:
            dut.hal.InitHidlHal(
                target_type="vehicle",
                target_basepaths=dut.libPaths,
                target_version=2.0,
                target_package="android.hardware.automotive.vehicle",
                target_component_name="IVehicle")
        except:
            e = str(sys.exc_info()[0])
            asserts.assertTrue(False, "VHAL init fail with error: " + str(e) + " Is VHAL up and running?")
            raise

        self.vehicle = dut.hal.vehicle
        self.vehicle.SetCallerUid(system_uid)
        self.vtypes = dut.hal.vehicle.GetHidlTypeInterface("types")
        self.flexray = FrSignalInterface()
        self.dut = dut

    def get_id(self, property):
        for prop in property_list:
            if prop[0] == property:
                return prop[1]
        return 0

    def assert_ViewNotFoundException(self, vc, buttonId):
        exceptionRaised = False
        try:
            vc.findViewByIdOrRaise(buttonId)
        except ViewNotFoundException:
            exceptionRaised = True
        asserts.assertTrue(exceptionRaised, buttonId + " was found when it should be invisible")


    def getViewClient(self):
        kwargs1 = {'ignoreversioncheck': False, 'verbose': True, 'ignoresecuredevice': False, 'serialno': '.*'}
        device, serialno = ViewClient.connectToDeviceOrExit(**kwargs1)
        kwargs2 = {'forceviewserveruse': False, 'useuiautomatorhelper': False, 'ignoreuiautomatorkilled': True,
                   'autodump': False, 'startviewserver': True, 'compresseddump': True}
        return ViewClient(device, serialno, **kwargs2), device

    def setUpVendorExtension(self):
        self.dut.adb.shell('input keyevent 3')
        self.dut.adb.shell('am start -n \"com.volvocars.halmodulesink/com.volvocars.halmodulesink.MainActivity\" -a \"android.intent.action.MAIN\" -c \"android.intent.category.LAUNCHER\"')

        _s = 0.5
        vc, device = self.getViewClient()

        # Open menu drawer button
        # Not possible to press with buttonId, using coordinates instead
        device.touchDip(35.0, 110.0, 0)
        vc.sleep(_s)

        # Open vendor extension
        vc.dump(window=-1)
        vendorExtension = vc.findViewWithTextOrRaise("VendorExtension")

        vendorExtension.touch()
        vc.sleep(_s)

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
            #fail(fullMsg) TODO why does fail() not work any more??
            asserts.assertTrue(False, fullMsg)

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

    def assert_signal_equals(self, fdx_signal, expected_value):
        if self.flexray.connected:
            read_value = fdx_signal.get()
            asserts.assertEqual(read_value, expected_value, "Flexray signal %s Equals to=%d, Expected: %d" % ( fdx_signal.de_name, read_value, expected_value ))

    def assert_prop_equals(self, propId, expected_value):
        value = self.readVhalProperty(propId)
        asserts.assertEqual(self.extractValue(value), expected_value, "Failed: Property value not equal to expected value")

    def assert_prop_area_equals(self, propId, areaId, expected_value):
        value = self.readVhalProperty(propId, areaId)
        asserts.assertEqual(self.extractValue(value), expected_value, "Failed: Property value not equal to expected value")

