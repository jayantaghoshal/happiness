/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.property;

import android.car.hardware.CarPropertyValue;

import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyType;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

public class PropertyTranslator {

    public static RemoteCtrlPropertyValue toRemoteCtrlPropertyValue(RemoteCtrlHalPropertyValue halValue) throws IllegalArgumentException {

        int propertyType = DataTypeMapper.getPropertyDataType(halValue.prop);
        switch(propertyType) {
            case RemoteCtrlHalPropertyType.BOOLEAN:
                int boolValue = 0;
                if (!halValue.value.int32Values.isEmpty()) {
                    boolValue = halValue.value.int32Values.get(0);
                }
                return new RemoteCtrlPropertyValue(halValue.prop, halValue.areaId, boolValue);
            case RemoteCtrlHalPropertyType.INT32:
                int intValue = 0;
                if (!halValue.value.int32Values.isEmpty()) {
                    intValue = halValue.value.int32Values.get(0);
                }
                return new RemoteCtrlPropertyValue(halValue.prop, halValue.areaId, intValue);
            case RemoteCtrlHalPropertyType.INT32_VEC:
                break;
            case RemoteCtrlHalPropertyType.INT64:
                break;
            case RemoteCtrlHalPropertyType.INT64_VEC:
                break;
            case RemoteCtrlHalPropertyType.FLOAT:
                float floatValue = 0.0f;
                if (!halValue.value.floatValues.isEmpty()) {
                    floatValue = halValue.value.floatValues.get(0);
                }
                return new RemoteCtrlPropertyValue(halValue.prop, halValue.areaId, floatValue);
            case RemoteCtrlHalPropertyType.FLOAT_VEC:
                break;
            case RemoteCtrlHalPropertyType.BYTES:
                break;
        }

        throw new IllegalArgumentException("Unsupported property type: " + propertyType);
    }

    public static RemoteCtrlHalPropertyValue toRemoteCtrlHalPropertyValue(RemoteCtrlPropertyValue propValue) {
        RemoteCtrlHalPropertyValue value = new RemoteCtrlHalPropertyValue();
        value.prop = propValue.getPropertyId();
        value.areaId = propValue.getAreaId();

        int propertyType = DataTypeMapper.getPropertyDataType(propValue.getPropertyId());
        switch(propertyType) {
            case RemoteCtrlHalPropertyType.BOOLEAN:
                value.value.int32Values.add((Integer)propValue.getValue());
                break;
            case RemoteCtrlHalPropertyType.INT32:
                value.value.int32Values.add((Integer)propValue.getValue());
                break;
            case RemoteCtrlHalPropertyType.INT32_VEC:
                break;
            case RemoteCtrlHalPropertyType.INT64:
                break;
            case RemoteCtrlHalPropertyType.INT64_VEC:
                break;
            case RemoteCtrlHalPropertyType.FLOAT:
                value.value.floatValues.add((Float)propValue.getValue());
                break;
            case RemoteCtrlHalPropertyType.FLOAT_VEC:
                break;
            case RemoteCtrlHalPropertyType.BYTES:
                break;
            default:
                throw new IllegalArgumentException("Unsupported property type: " + propertyType);
        }

        return value;
    }

    public static CarPropertyValue toCarPropertyValue(RemoteCtrlPropertyValue propValue) {
        int propertyType = DataTypeMapper.getPropertyDataType(propValue.getPropertyId());
        int carPropertyId = IdMapper.toCarPropertyId(propValue.getPropertyId());
        int carPropertyAreaId = AreaIdMapper.toCarPropertyAreaId(propValue.getAreaId());
        switch(propertyType) {
            case RemoteCtrlHalPropertyType.BOOLEAN:
                return new CarPropertyValue(carPropertyId, carPropertyAreaId, ValueTranslator.toCarPropertyValue(propValue));
            case RemoteCtrlHalPropertyType.INT32:
                return new CarPropertyValue(carPropertyId, carPropertyAreaId, ValueTranslator.toCarPropertyValue(propValue));
            case RemoteCtrlHalPropertyType.INT32_VEC:
                break;
            case RemoteCtrlHalPropertyType.INT64:
                break;
            case RemoteCtrlHalPropertyType.INT64_VEC:
                break;
            case RemoteCtrlHalPropertyType.FLOAT:
                return new CarPropertyValue(carPropertyId, carPropertyAreaId, ValueTranslator.toCarPropertyValue(propValue));
            case RemoteCtrlHalPropertyType.FLOAT_VEC:
                break;
            case RemoteCtrlHalPropertyType.BYTES:
                break;
        }

        throw new IllegalArgumentException("Unsupported property type: " + propertyType);
    }

    public static RemoteCtrlPropertyValue toRemoteCtrlPropertyValue(CarPropertyValue propValue) {
        int propertyType = DataTypeMapper.getPropertyDataType(propValue.getValue());
        int remoteCtrlHalPropertyId = IdMapper.toRemoteCtrlHalPropertyId(propValue.getPropertyId());
        int remoteCtrlHalAreaId = AreaIdMapper.toRemoteCtrlHalAreaId(propValue.getAreaId());
        int remoteCtrlHalStatus = StatusMapper.toRemoteCtrlHalStatus(propValue.getStatus());
        switch(propertyType) {
            case RemoteCtrlHalPropertyType.BOOLEAN:
                return new RemoteCtrlPropertyValue(remoteCtrlHalPropertyId, remoteCtrlHalAreaId, remoteCtrlHalStatus, ValueTranslator.toRemoteCtrlPropertyValue(propValue));
            case RemoteCtrlHalPropertyType.INT32:
                return new RemoteCtrlPropertyValue(remoteCtrlHalPropertyId, remoteCtrlHalAreaId, remoteCtrlHalStatus, ValueTranslator.toRemoteCtrlPropertyValue(propValue));
            case RemoteCtrlHalPropertyType.INT32_VEC:
                break;
            case RemoteCtrlHalPropertyType.INT64:
                break;
            case RemoteCtrlHalPropertyType.INT64_VEC:
                break;
            case RemoteCtrlHalPropertyType.FLOAT:
                return new RemoteCtrlPropertyValue(remoteCtrlHalPropertyId, remoteCtrlHalAreaId, remoteCtrlHalStatus, ValueTranslator.toRemoteCtrlPropertyValue(propValue));
            case RemoteCtrlHalPropertyType.FLOAT_VEC:
                break;
            case RemoteCtrlHalPropertyType.BYTES:
                break;
        }

        throw new IllegalArgumentException("Unsupported property type: " + propertyType);
    }
}
