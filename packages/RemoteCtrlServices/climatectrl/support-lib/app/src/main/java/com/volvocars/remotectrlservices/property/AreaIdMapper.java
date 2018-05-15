/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.property;

import java.util.HashMap;
import java.util.Map;

import vendor.volvocars.hardware.remotectrl.V1_0.VehicleAreaZone;

public class AreaIdMapper {

    private static final Map<Integer, Integer> areaIdMap = new HashMap<Integer, Integer>();
    static {
        areaIdMap.put(
                VehicleAreaZone.ROW_1_CENTER, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_CENTER);
        areaIdMap.put(
                VehicleAreaZone.ROW_1_LEFT, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_LEFT);
        areaIdMap.put(
                VehicleAreaZone.ROW_1_RIGHT, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_1_RIGHT);

        areaIdMap.put(
                VehicleAreaZone.ROW_2_CENTER, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_2_CENTER);
        areaIdMap.put(
                VehicleAreaZone.ROW_2_LEFT, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_2_LEFT);
        areaIdMap.put(
                VehicleAreaZone.ROW_2_RIGHT, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_2_RIGHT);

        areaIdMap.put(
                VehicleAreaZone.ROW_3_CENTER, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_3_CENTER);
        areaIdMap.put(
                VehicleAreaZone.ROW_3_LEFT, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_3_LEFT);
        areaIdMap.put(
                VehicleAreaZone.ROW_3_RIGHT, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_3_RIGHT);

        areaIdMap.put(
                VehicleAreaZone.ROW_4_CENTER, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_4_CENTER);
        areaIdMap.put(
                VehicleAreaZone.ROW_4_LEFT, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_4_LEFT);
        areaIdMap.put(
                VehicleAreaZone.ROW_4_RIGHT, android.hardware.automotive.vehicle.V2_0.VehicleAreaZone.ROW_4_RIGHT);
    }

    public static int toCarPropertyAreaId(int areaId) {
        return areaIdMap.get(areaId);
    }

    public static int toRemoteCtrlHalAreaId(int carPropertyAreaId) {
        for (Map.Entry<Integer, Integer> entry : areaIdMap.entrySet()) {
            if (entry.getValue() == carPropertyAreaId) {
                return entry.getKey();
            }
        }
        throw new IllegalArgumentException("No remote control hal area id for car property area id :" + carPropertyAreaId);
    }
}
