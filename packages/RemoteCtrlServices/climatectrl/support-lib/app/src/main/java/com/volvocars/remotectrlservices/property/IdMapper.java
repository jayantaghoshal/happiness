/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.property;

import android.car.hardware.hvac.CarHvacManager;

import java.util.HashMap;
import java.util.Map;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty;

public class IdMapper {

    private static final Map<Integer, Integer> propertyIdMap = new HashMap<Integer, Integer>();
    static {
        propertyIdMap.put(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON, CarHvacManager.ID_ZONED_AC_ON);
        propertyIdMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_DIRECTION,
                CarHvacManager.ID_ZONED_FAN_DIRECTION);
        propertyIdMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT);
        propertyIdMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_MAX_DEFROST_ON,
                CarHvacManager.ID_ZONED_MAX_DEFROST_ON);
        propertyIdMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE,
                CarHvacManager.ID_ZONED_TEMP_SETPOINT);
    }

    public static int toCarPropertyId(int propertyId) {
        return propertyIdMap.get(propertyId);
    }

    public static int toRemoteCtrlHalPropertyId(int carPropertyId) {
        for (Map.Entry<Integer, Integer> entry : propertyIdMap.entrySet()) {
            if (entry.getValue() == carPropertyId) {
                return entry.getKey();
            }
        }
        throw new IllegalArgumentException("No remote control hal property id for car property id :" + carPropertyId);
    }
}
