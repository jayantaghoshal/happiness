/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnessservice;

import android.util.Log;
import android.os.RemoteException;
import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.VehicleArea;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;

public final class VehicleHalUtils {
    public static final String TAG = "BrightnessService";
    /**
    * Helper function for doing a get against Vehicle hal.
    * @param requestedPropValue is VehiclePropValue containing the wanted VehicleProperty
    * */
    public static ValueResult GetVehiclePropValue(IVehicle vehiclehal, VehiclePropValue requestedPropValue) {
        final ValueResult result = new ValueResult();
        try {
            vehiclehal.get(requestedPropValue,
                    (status, propValue) -> {
                        result.status = status;
                        result.propValue = propValue;
                    });
        } catch (RemoteException e) {
            Log.e(TAG, "Failed to get value from vehicle HAL", e);
            result.status = 0;
        }
        return result;
    }
    /** Helper function for setting property in Vehicle hal .
     * @param propertyInt is the VehicleProperty id.
     * @param valueToSet is the wanted value for the property,
     * this function only sets the int32value!
     * */
    public static void SetVehiclePropValue(IVehicle vehiclehal, int propertyInt, int valueToSet) {
        try {
            VehiclePropValue setprop = new VehiclePropValue();
            setprop.prop = propertyInt;
            setprop.value.int32Values.add(valueToSet);
            setprop.areaId = VehicleArea.GLOBAL;
            int setResult = vehiclehal.set(setprop);
            Log.d(TAG, "set result " + setResult);
        } catch (RemoteException ex) {
            Log.e(TAG, ex.getMessage());
        }
    }
    /**
     * Helper class for lambda callback in getVehiclePropValue
     *  StatusCode and VehiclePropValue
     * */
    public static final class ValueResult {
        int status;
        VehiclePropValue propValue;
    }
}