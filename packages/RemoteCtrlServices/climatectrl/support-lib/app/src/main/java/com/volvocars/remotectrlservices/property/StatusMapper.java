/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.property;

import android.hardware.automotive.vehicle.V2_0.VehiclePropertyStatus;

import java.util.HashMap;
import java.util.Map;

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyStatus;

class StatusMapper {
    private static final Map<Integer, Integer> statusMap = new HashMap<Integer, Integer>();
    static {
        statusMap.put(VehiclePropertyStatus.ERROR, RemoteCtrlHalPropertyStatus.SYSTEM_ERROR);
        statusMap.put(VehiclePropertyStatus.UNAVAILABLE, RemoteCtrlHalPropertyStatus.USER_INPUT_DISABLED);
        statusMap.put(VehiclePropertyStatus.AVAILABLE, RemoteCtrlHalPropertyStatus.AVAILABLE);
        // TODO: VehiclePropertyStatus is not available in Android. Discussion ongoing with Google about this. /Philip Werner 2018-05-16
        //statusMap.put(VehiclePropertyStatus.NOT_AVAILABLE, RemoteCtrlHalPropertyStatus.NOT_AVAILABLE);
    }

    public static int toRemoteCtrlHalStatus(int carPropertyStatus) {
        return statusMap.get(carPropertyStatus);
    }
}
