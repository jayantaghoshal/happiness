/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.carproperty;

import android.car.hardware.CarPropertyValue;
import android.car.hardware.hvac.CarHvacManager;
import android.car.hardware.hvac.CarHvacManager.CarHvacEventCallback;

import android.car.VehicleZone;

import android.hardware.automotive.vehicle.V2_0.VehiclePropertyStatus;

import android.util.Log;

import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

import android.car.CarNotConnectedException;

// TODO: This class's logic will be moved to a class encapsulating the complete logic. Right now
//       this is spread out. /Philip Werner (2018-04-18)
public class CarHvacWrapper {
    private final String TAG = "RemoteCtrl.ClimateCtrl.CarHvacWrapper";

    private CarHvacManager mCarHvacManager = null;

    public CarHvacWrapper(CarHvacManager carHvacManager) {
        mCarHvacManager = carHvacManager;
    }

    public CarPropertyValue<?> setProperty(CarPropertyValue carPropValue)
            throws CarNotConnectedException {
        Log.v(TAG, "setProperty: " + carPropValue);

        int propertyId = carPropValue.getPropertyId();
        int areaId = carPropValue.getAreaId();
        long timestamp = carPropValue.getTimestamp();

        Object o = carPropValue.getValue();
        if (o instanceof Integer) {
            Integer value = (Integer) carPropValue.getValue();
            mCarHvacManager.setIntProperty(propertyId, areaId, value);

            return CarPropertyUtils.createCarPropertyValueWithStatus(
                    carPropValue, VehiclePropertyStatus.AVAILABLE, value);
        } else if (o instanceof Float) {
            Float value = (Float) carPropValue.getValue();
            mCarHvacManager.setFloatProperty(propertyId, areaId, value);

            return CarPropertyUtils.createCarPropertyValueWithStatus(
                    carPropValue, VehiclePropertyStatus.AVAILABLE, value);
        } else if (o instanceof Boolean) {
            Boolean value = (Boolean) carPropValue.getValue();
            mCarHvacManager.setBooleanProperty(propertyId, areaId, value);

            return CarPropertyUtils.createCarPropertyValueWithStatus(
                    carPropValue, VehiclePropertyStatus.AVAILABLE, value);
        }

        throw new IllegalArgumentException("Unsupported value class: " + o.getClass().getName());
    }

    public CarPropertyValue<?> getProperty(CarPropertyValue requestedCarPropValue)
            throws CarNotConnectedException {
        Log.v(TAG, "getProperty: " + requestedCarPropValue);

        int propertyId = requestedCarPropValue.getPropertyId();
        int areaId = requestedCarPropValue.getAreaId();
        long timestamp = requestedCarPropValue.getTimestamp();

        Class<?> clazz = CarPropertyUtils.getJavaClassByManagerPropertyId(propertyId);
        if (Integer.class == clazz) {
            Integer value = mCarHvacManager.getIntProperty(propertyId, areaId);

            return CarPropertyUtils.createCarPropertyValueWithStatus(
                    requestedCarPropValue, VehiclePropertyStatus.AVAILABLE, value);
        } else if (Float.class == clazz) {
            Float value = mCarHvacManager.getFloatProperty(propertyId, areaId);

            return CarPropertyUtils.createCarPropertyValueWithStatus(
                    requestedCarPropValue, VehiclePropertyStatus.AVAILABLE, value);
        } else if (Boolean.class == clazz) {
            Boolean value = mCarHvacManager.getBooleanProperty(propertyId, areaId);

            return CarPropertyUtils.createCarPropertyValueWithStatus(
                    requestedCarPropValue, VehiclePropertyStatus.AVAILABLE, value);
        }

        throw new IllegalArgumentException("Unsupported value class: " + clazz.toString());
    }
}