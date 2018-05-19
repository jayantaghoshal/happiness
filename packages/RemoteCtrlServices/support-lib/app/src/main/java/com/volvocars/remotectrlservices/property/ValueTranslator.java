/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.property;

import android.car.hardware.CarPropertyValue;
import android.car.hardware.hvac.CarHvacManager;

import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;

import java.util.HashMap;
import java.util.Map;

import vendor.volvocars.hardware.remotectrl.V1_0.AirFlow;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty;

public class ValueTranslator {
    private static final Map<Integer, Integer> typeMap = new HashMap<Integer, Integer>();
    static {
        typeMap.put((int)AirFlow.FLOOR, android.hardware.automotive.vehicle.V2_0.VehicleHvacFanDirection.FLOOR);
    }

    public static Object toCarPropertyValue(RemoteCtrlPropertyValue propValue) {
        switch(propValue.getPropertyId())
        {
            case RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED:
                return propValue.getValue();
            case RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON:
                return (int)propValue.getValue() == 1;
            case RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE:
                return (float)propValue.getValue();
        }

        throw new IllegalArgumentException("Unsupported property: " + propValue.getPropertyId());
    }

    public static Object toRemoteCtrlPropertyValue(CarPropertyValue propValue) {
        switch(propValue.getPropertyId())
        {
            case CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT:
                return propValue.getValue();
            case CarHvacManager.ID_ZONED_AC_ON:
                return (Boolean)propValue.getValue() ? 1 : 0;
            case CarHvacManager.ID_ZONED_TEMP_SETPOINT:
                return (float)propValue.getValue();
        }

        throw new IllegalArgumentException("Unsupported property: " + propValue.getPropertyId());
    }
}
