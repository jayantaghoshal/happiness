/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.property.stubs;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

public class RemoteCtrlHalPropertyValueStubs {

    public static <T> RemoteCtrlHalPropertyValue createGetProperty(int propertyId, int areaId, T value) {
        RemoteCtrlHalPropertyValue propValue = new RemoteCtrlHalPropertyValue();
        propValue.prop = propertyId;
        propValue.areaId = areaId;
        if (value instanceof Integer) {
            propValue.value.int32Values.add((Integer)value);
        } else if (value instanceof Float) {
            propValue.value.floatValues.add((Float)value);
        }
        return propValue;
    }
}
