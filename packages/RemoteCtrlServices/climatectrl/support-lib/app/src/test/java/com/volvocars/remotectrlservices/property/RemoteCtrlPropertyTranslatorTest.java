/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.property;

import android.car.hardware.CarPropertyValue;
import android.car.hardware.hvac.CarHvacManager;
import android.hardware.automotive.vehicle.V2_0.VehiclePropertyStatus;

import com.volvocars.remotectrlservices.climatectrl.RemoteCtrlPropertyValue;
import com.volvocars.remotectrlservices.property.stubs.RemoteCtrlHalPropertyValueStubs;

import org.junit.Assert;
import org.junit.Test;

import vendor.volvocars.hardware.remotectrl.V1_0.ACState;
import vendor.volvocars.hardware.remotectrl.V1_0.AirFlow;
import vendor.volvocars.hardware.remotectrl.V1_0.FanLevel;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyStatus;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;
import vendor.volvocars.hardware.remotectrl.V1_0.VehicleAreaZone;

public class RemoteCtrlPropertyTranslatorTest {

    /*
    RemoteCtrlHalPropertyValue -> RemoteCtrlPropertyValue
    */
    @Test
    public void toRemoteCtrlPropertyValue_zonedInt32PropertyTest1_shouldTranslateOk() {
        // Arrange
        RemoteCtrlHalPropertyValue halValue =
                RemoteCtrlHalPropertyValueStubs.createGetProperty(
                        RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                        VehicleAreaZone.ROW_1_CENTER,
                        (int)AirFlow.FLOOR);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(halValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), (int)AirFlow.FLOOR);
    }

    @Test
    public void toRemoteCtrlPropertyValue_zonedInt32PropertyTest2_shouldTranslateOk() {
        // Arrange
        RemoteCtrlHalPropertyValue halValue =
                RemoteCtrlHalPropertyValueStubs.createGetProperty(
                        RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                        VehicleAreaZone.ROW_1_CENTER,
                        (int)AirFlow.VENT);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(halValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), (int)AirFlow.VENT);
    }

    @Test
    public void toRemoteCtrlPropertyValue_zonedBooleanPropertyOn_shouldTranslateOk() {
        // Arrange
        RemoteCtrlHalPropertyValue halValue =
                RemoteCtrlHalPropertyValueStubs.createGetProperty(
                        RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON,
                        VehicleAreaZone.ROW_1_CENTER,
                        (int)ACState.ON);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(halValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), (int)ACState.ON);
    }

    @Test
    public void toRemoteCtrlPropertyValue_zonedBooleanPropertyOff_shouldTranslateOk() {
        // Arrange
        RemoteCtrlHalPropertyValue halValue =
                RemoteCtrlHalPropertyValueStubs.createGetProperty(
                        RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON,
                        VehicleAreaZone.ROW_1_CENTER,
                        (int)ACState.OFF);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(halValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), (int)ACState.OFF);
    }

    @Test
    public void toRemoteCtrlPropertyValue_zonedFloatProperty_shouldTranslateOk() {
        // Arrange
        RemoteCtrlHalPropertyValue halValue =
                RemoteCtrlHalPropertyValueStubs.createGetProperty(
                        RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE,
                        VehicleAreaZone.ROW_1_CENTER,
                        22.5f);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(halValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), 22.5f);
    }

    /*
    RemoteCtrlPropertyValue -> RemoteCtrlHalPropertyValue
     */
    @Test
    public void toRemoteCtrlHalPropertyValue_zonedInt32PropertyTest1_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                VehicleAreaZone.ROW_1_CENTER,
                (int)AirFlow.FLOOR);

        // Act
        RemoteCtrlHalPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlHalPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.prop, RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED);
        Assert.assertEquals(value.areaId, VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.status, 0);
        Assert.assertEquals((int)value.value.int32Values.get(0), (int)AirFlow.FLOOR);
    }

    @Test
    public void toRemoteCtrlHalPropertyValue_zonedInt32PropertyTest2_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                VehicleAreaZone.ROW_1_CENTER,
                (int)AirFlow.VENT);

        // Act
        RemoteCtrlHalPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlHalPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.prop, RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED);
        Assert.assertEquals(value.areaId, VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.status, 0);
        Assert.assertEquals((int)value.value.int32Values.get(0), (int)AirFlow.VENT);
    }

    @Test
    public void toRemoteCtrlHalPropertyValue_zonedBooleanPropertyOn_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON,
                VehicleAreaZone.ROW_1_CENTER,
                (int)ACState.ON);

        // Act
        RemoteCtrlHalPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlHalPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.prop, RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON);
        Assert.assertEquals(value.areaId, VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.status, 0);
        Assert.assertEquals((int)value.value.int32Values.get(0), (int)ACState.ON);
    }

    @Test
    public void toRemoteCtrlHalPropertyValue_zonedBooleanPropertyOff_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON,
                VehicleAreaZone.ROW_1_CENTER,
                (int)ACState.OFF);

        // Act
        RemoteCtrlHalPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlHalPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.prop, RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON);
        Assert.assertEquals(value.areaId, VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.status, 0);
        Assert.assertEquals((int)value.value.int32Values.get(0), (int)ACState.OFF);
    }

    @Test
    public void toRemoteCtrlHalPropertyValue_zonedFloatPropertyOff_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE,
                VehicleAreaZone.ROW_1_CENTER,
                (float)22.5);

        // Act
        RemoteCtrlHalPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlHalPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.prop, RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE);
        Assert.assertEquals(value.areaId, VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.status, 0);
        Assert.assertEquals((float)value.value.floatValues.get(0), (float)22.5, 0.01);
    }

    /*
    RemoteCtrlPropertyValue -> CarPropertyValue
     */
    @Test
    public void toCarPropertyValue_zonedInt32PropertyTest1_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                VehicleAreaZone.ROW_1_CENTER,
                (int) FanLevel.FANLEVEL_0);

        // Act
        CarPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toCarPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT);
        Assert.assertEquals(value.getAreaId(), android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), (int) FanLevel.FANLEVEL_0);
    }

    @Test
    public void toCarPropertyValue_zonedInt32PropertyTest2_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                VehicleAreaZone.ROW_1_CENTER,
                (int) FanLevel.FANLEVEL_1);

        // Act
        CarPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toCarPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT);
        Assert.assertEquals(value.getAreaId(), android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), (int) FanLevel.FANLEVEL_1);
    }

    @Test
    public void toCarPropertyValue_zonedBooleanPropertyOn_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON,
                VehicleAreaZone.ROW_1_CENTER,
                (int)ACState.ON);

        // Act
        CarPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toCarPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), CarHvacManager.ID_ZONED_AC_ON);
        Assert.assertEquals(value.getAreaId(), android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), true);
    }


    @Test
    public void toCarPropertyValue_zonedBooleanPropertyOff_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON,
                VehicleAreaZone.ROW_1_CENTER,
                (int)ACState.OFF);

        // Act
        CarPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toCarPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), CarHvacManager.ID_ZONED_AC_ON);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals(value.getValue(), false);
    }

    @Test
    public void toCarPropertyValue_zonedFloatPropertyOff_shouldTranslateOk() {
        // Arrange
        RemoteCtrlPropertyValue propValue = new RemoteCtrlPropertyValue(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE,
                VehicleAreaZone.ROW_1_CENTER,
                (float)22.5);

        // Act
        CarPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toCarPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), CarHvacManager.ID_ZONED_TEMP_SETPOINT);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), 0);
        Assert.assertEquals((float)value.getValue(), 22.5f, 0.01f);
    }

    /*
     CarPropertyValue -> RemoteCtrlPropertyValue
     */
    @Test
    public void fromCarPropertyValueToRemoteCtrlPropertyValue_zonedInt32PropertyTest1_shouldTranslateOk() {
        // Arrange
        CarPropertyValue propValue = new CarPropertyValue(CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT,
                android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER,
                (int)FanLevel.FANLEVEL_0);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), RemoteCtrlHalPropertyStatus.AVAILABLE);
        Assert.assertEquals(value.getValue(), (int) (int) FanLevel.FANLEVEL_0);
    }

    @Test
    public void fromCarPropertyValueToRemoteCtrlPropertyValue_zonedInt32PropertyTest2_shouldTranslateOk() {
        // Arrange
        CarPropertyValue propValue = new CarPropertyValue(CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT,
                android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER,
                (int)FanLevel.FANLEVEL_1);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), RemoteCtrlHalPropertyStatus.AVAILABLE);
        Assert.assertEquals(value.getValue(), (int) (int) FanLevel.FANLEVEL_1);
    }

    @Test
    public void fromCarPropertyValueToRemoteCtrlPropertyValue_zonedBooleanPropertyOn_shouldTranslateOk() {
        // Arrange
        CarPropertyValue propValue = new CarPropertyValue(CarHvacManager.ID_ZONED_AC_ON,
                android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER,
                true);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), RemoteCtrlHalPropertyStatus.AVAILABLE);
        Assert.assertEquals(value.getValue(), 1);
    }


    @Test
    public void fromCarPropertyValueToRemoteCtrlPropertyValue_zonedBooleanPropertyOff_shouldTranslateOk() {
        // Arrange
        CarPropertyValue propValue = new CarPropertyValue(CarHvacManager.ID_ZONED_AC_ON,
                android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER,
                false);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), RemoteCtrlHalPropertyStatus.AVAILABLE);
        Assert.assertEquals(value.getValue(), 0);
    }

    @Test
    public void fromCarPropertyValueToRemoteCtrlPropertyValue_zonedFloatPropertyOff_shouldTranslateOk() {
        // Arrange
        CarPropertyValue propValue = new CarPropertyValue(CarHvacManager.ID_ZONED_TEMP_SETPOINT,
                android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER,
                22.5f);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), RemoteCtrlHalPropertyStatus.AVAILABLE);
        Assert.assertEquals((float)value.getValue(), 22.5f, 0.01f);
    }

    @Test
    public void fromCarPropertyValueToRemoteCtrlPropertyValue_statusAvailable_shouldTranslateOk() {
        // Arrange
        CarPropertyValue propValue = new CarPropertyValue(CarHvacManager.ID_ZONED_TEMP_SETPOINT,
                android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER, VehiclePropertyStatus.AVAILABLE, 0L,
                22.5f);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), RemoteCtrlHalPropertyStatus.AVAILABLE);
        Assert.assertEquals((float)value.getValue(), 22.5f, 0.01f);
    }

    @Test
    public void fromCarPropertyValueToRemoteCtrlPropertyValue_statusUnavailable_shouldTranslateOk() {
        // Arrange
        CarPropertyValue propValue = new CarPropertyValue(CarHvacManager.ID_ZONED_TEMP_SETPOINT,
                android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER, VehiclePropertyStatus.UNAVAILABLE, 0L,
                22.5f);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), RemoteCtrlHalPropertyStatus.USER_INPUT_DISABLED);
        Assert.assertEquals((float)value.getValue(), 22.5f, 0.01f);
    }

    @Test
    public void fromCarPropertyValueToRemoteCtrlPropertyValue_statusError_shouldTranslateOk() {
        // Arrange
        CarPropertyValue propValue = new CarPropertyValue(CarHvacManager.ID_ZONED_TEMP_SETPOINT,
                android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER, VehiclePropertyStatus.ERROR, 0L,
                22.5f);

        // Act
        RemoteCtrlPropertyValue value = com.volvocars.remotectrlservices.property.PropertyTranslator.toRemoteCtrlPropertyValue(propValue);

        // Assert
        Assert.assertEquals(value.getPropertyId(), RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE);
        Assert.assertEquals(value.getAreaId(), VehicleAreaZone.ROW_1_CENTER);
        Assert.assertEquals(value.getStatus(), RemoteCtrlHalPropertyStatus.SYSTEM_ERROR);
        Assert.assertEquals((float)value.getValue(), 22.5f, 0.01f);
    }

}
