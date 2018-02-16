/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.test.lib.vehiclehal;

import android.hardware.automotive.vehicle.V2_0.*;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

public class VehicleHalUtil {

    public static String getPropertyName(int propId){
        if ((propId & VehiclePropertyGroup.MASK) == VehiclePropertyGroup.VENDOR) {
            return VehicleProperty.toString(propId);  // VENDOR
        } else {
            return android.hardware.automotive.vehicle.V2_0.VehicleProperty.toString(propId);  // SYSTEM
        }
    }

    public static String getVehicleZoneNameByVehicleAreaAndAreaId(int vehicleArea, int areaId){
        String retVal = "";
        switch (vehicleArea) {
            case VehicleArea.GLOBAL:
            retVal = "GLOBAL";
                break;

            case VehicleArea.WINDOW:
            retVal = VehicleAreaWindow.toString(areaId);
                break;

            case VehicleArea.MIRROR:
            retVal = VehicleAreaMirror.toString(areaId);
                break;

            case VehicleArea.SEAT:
            retVal = VehicleAreaSeat.toString(areaId);
                break;

            case VehicleArea.ZONE:
            retVal = VehicleAreaZone.toString(areaId);
                break;

            case VehicleArea.DOOR:
            retVal = VehicleAreaDoor.toString(areaId);
                break;

            case VehicleArea.MASK:;

            default:
                retVal = "No valid Area" + getPropertyName(vehicleArea);
        }
        return retVal;
    }
}
