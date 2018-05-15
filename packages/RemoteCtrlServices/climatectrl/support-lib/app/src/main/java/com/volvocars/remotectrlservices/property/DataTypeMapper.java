/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.property;

import java.util.HashMap;
import java.util.Map;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyType;

class DataTypeMapper {

    private static final Map<Integer, Integer> typeMap = new HashMap <Integer, Integer>();
    static {
        typeMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON, RemoteCtrlHalPropertyType.INT32);
        typeMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_DIRECTION, RemoteCtrlHalPropertyType.INT32);
        typeMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED, RemoteCtrlHalPropertyType.INT32);
        typeMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_MAX_DEFROST_ON, RemoteCtrlHalPropertyType.INT32);
        typeMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE, RemoteCtrlHalPropertyType.FLOAT);
    }

    public static int getPropertyDataType(int halProperty) {
        return halProperty & RemoteCtrlHalPropertyType.MASK;
    }

    public static int getPropertyDataType(Object halValue) {
        if (halValue instanceof Boolean) {
            return RemoteCtrlHalPropertyType.BOOLEAN;
        } else if (halValue instanceof Integer) {
            return RemoteCtrlHalPropertyType.INT32;
        } else if (halValue instanceof Integer[]) {
            return RemoteCtrlHalPropertyType.INT32_VEC;
        } else if (halValue instanceof Float) {
            return RemoteCtrlHalPropertyType.FLOAT;
        } else if (halValue instanceof Float[]) {
            return RemoteCtrlHalPropertyType.FLOAT_VEC;
        } else if (halValue instanceof String) {
            return RemoteCtrlHalPropertyType.STRING;
        } else if (halValue instanceof Byte[]) {
            return RemoteCtrlHalPropertyType.BYTES;
        }

        throw new IllegalArgumentException("Unsupported data type for object: " + halValue.getClass().toString());
    }
}
