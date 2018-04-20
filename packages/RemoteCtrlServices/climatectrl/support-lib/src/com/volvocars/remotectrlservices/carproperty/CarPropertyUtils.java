/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.carproperty;

import static java.lang.Integer.toHexString;

import android.car.VehicleAreaType;
import android.car.hardware.CarPropertyConfig;
import android.car.hardware.CarPropertyValue;
import android.hardware.automotive.vehicle.V2_0.VehicleArea;
import android.hardware.automotive.vehicle.V2_0.VehicleAreaConfig;
import android.hardware.automotive.vehicle.V2_0.VehiclePropConfig;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropertyType;
import android.hardware.automotive.vehicle.V2_0.VehicleProperty;
import android.hardware.automotive.vehicle.V2_0.VehiclePropertyStatus;

import android.car.hardware.hvac.CarHvacManager;

import android.util.Log;

import java.util.Collections;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

/**
 * Utility functions to work with {@link CarPropertyConfig} and {@link CarPropertyValue}
 */
public final class CarPropertyUtils {
    private static final String TAG = "RemoteCtrl.ClimateCtrl.CarPropertyUtils";

    // TODO: Add tests for these (Philip Werner 2018-04-20)
    private static final Map<Integer, Integer> mMgrHalPropIdMap = new HashMap<Integer, Integer>();
    static {
        mMgrHalPropIdMap.put(
                VehicleProperty.HVAC_FAN_SPEED, CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT);
        mMgrHalPropIdMap.put(
                VehicleProperty.HVAC_FAN_DIRECTION, CarHvacManager.ID_ZONED_FAN_DIRECTION);
        mMgrHalPropIdMap.put(VehicleProperty.HVAC_AC_ON, CarHvacManager.ID_ZONED_AC_ON);
        mMgrHalPropIdMap.put(
                VehicleProperty.HVAC_MAX_DEFROST_ON, CarHvacManager.ID_ZONED_MAX_DEFROST_ON);
        mMgrHalPropIdMap.put(
                VehicleProperty.HVAC_TEMPERATURE_SET, CarHvacManager.ID_ZONED_TEMP_SETPOINT);
        mMgrHalPropIdMap.put(VehicleProperty.HVAC_DEFROSTER, CarHvacManager.ID_WINDOW_DEFROSTER_ON);
    }

    /* Utility class has no public constructor */
    private CarPropertyUtils() {}

    public static int halToManagerPropId(int halPropId) {
        return mMgrHalPropIdMap.get(halPropId);
    }

    public static int managerToHalPropId(int managerPropId) {
        for (Map.Entry<Integer, Integer> entry : mMgrHalPropIdMap.entrySet()) {
            if (entry.getValue() == managerPropId) {
                return entry.getKey();
            }
        }
        throw new IllegalArgumentException("No manager property id (" + managerPropId
                + ") found for Hvac property id (" + managerPropId + ")");
    }

    public static CarPropertyValue<?> createCarPropertyValueWithStatus(
            CarPropertyValue propValue, int status, Integer value) {
        return new CarPropertyValue<Integer>(propValue.getPropertyId(), propValue.getAreaId(),
                status, propValue.getTimestamp(), value);
    }

    public static CarPropertyValue<?> createCarPropertyValueWithStatus(
            CarPropertyValue propValue, int status, Boolean value) {
        return new CarPropertyValue<Boolean>(propValue.getPropertyId(), propValue.getAreaId(),
                status, propValue.getTimestamp(), value);
    }

    public static CarPropertyValue<?> createCarPropertyValueWithStatus(
            CarPropertyValue propValue, int status, Float value) {
        return new CarPropertyValue<Float>(propValue.getPropertyId(), propValue.getAreaId(), status,
                propValue.getTimestamp(), value);
    }

    public static CarPropertyValue<?> createCarPropertyValueWithErrorStatus(
            CarPropertyValue propValue) {
        Object o = propValue.getValue();
        if (o instanceof Integer) {
            return createCarPropertyValueWithStatus(
                    propValue, VehiclePropertyStatus.ERROR, (Integer) propValue.getValue());
        } else if (o instanceof Float) {
            return createCarPropertyValueWithStatus(
                    propValue, VehiclePropertyStatus.ERROR, (Float) propValue.getValue());
        } else if (o instanceof Boolean) {
            return createCarPropertyValueWithStatus(
                    propValue, VehiclePropertyStatus.ERROR, (Boolean) propValue.getValue());
        }

        throw new IllegalArgumentException("Unsupported value type: " + o.getClass().getName());
    }

    public static Class<?> getJavaClassByManagerPropertyId(int managerPropId) {
        int halPropId = managerToHalPropId(managerPropId);
        return getJavaClass(managerToHalPropId(managerPropId) & VehiclePropertyType.MASK);
    }

    public static VehiclePropValue createVehiclePropertyValueWithErrorStatus(
            VehiclePropValue propValue) {
        propValue.status = VehiclePropertyStatus.ERROR;
        return propValue;
    }

    public static CarPropertyValue<?> toCarPropertyValue(VehiclePropValue halValue) {
        int managerPropId = CarPropertyUtils.halToManagerPropId(halValue.prop);
        return toCarPropertyValue(halValue, managerPropId);
    }

    /** Converts {@link VehiclePropValue} to {@link CarPropertyValue} */
    public static CarPropertyValue<?> toCarPropertyValue(
            VehiclePropValue halValue, int propertyId) {
        Class<?> clazz = getJavaClass(halValue.prop & VehiclePropertyType.MASK);
        int areaId = halValue.areaId;
        VehiclePropValue.RawValue v = halValue.value;

        if (Boolean.class == clazz) {
            return new CarPropertyValue<>(propertyId, areaId, v.int32Values.get(0) == 1);
        } else if (String.class == clazz) {
            return new CarPropertyValue<>(propertyId, areaId, v.stringValue);
        } else if (Long.class == clazz) {
            return new CarPropertyValue<>(propertyId, areaId, v.int64Values.get(0));
        } else if (byte[].class == clazz) {
            byte[] halData = toByteArray(v.bytes);
            return new CarPropertyValue<>(propertyId, areaId, halData);
        } else /* All list properties */ {
            Object[] values = getRawValueList(clazz, v).toArray();
            return new CarPropertyValue<>(
                    propertyId, areaId, values.length == 1 ? values[0] : values);
        }
    }

    public static VehiclePropValue toVehiclePropValue(CarPropertyValue carProp) {
        int halPropId = CarPropertyUtils.managerToHalPropId(carProp.getPropertyId());
        return toVehiclePropValue(carProp, halPropId);
    }

    /** Converts {@link CarPropertyValue} to {@link VehiclePropValue} */
    public static VehiclePropValue toVehiclePropValue(CarPropertyValue carProp, int halPropId) {
        VehiclePropValue vehicleProp = new VehiclePropValue();
        vehicleProp.prop = halPropId;
        vehicleProp.areaId = carProp.getAreaId();
        vehicleProp.status = carProp.getStatus();
        VehiclePropValue.RawValue v = vehicleProp.value;

        Object o = carProp.getValue();

        if (o instanceof Boolean) {
            v.int32Values.add(((Boolean) o) ? 1 : 0);
        } else if (o instanceof Integer) {
            v.int32Values.add((Integer) o);
        } else if (o instanceof Float) {
            v.floatValues.add((Float) o);
        } else if (o instanceof Integer[]) {
            Collections.addAll(v.int32Values, (Integer[]) o);
        } else if (o instanceof Float[]) {
            Collections.addAll(v.floatValues, (Float[]) o);
        } else if (o instanceof String) {
            v.stringValue = (String) o;
        } else if (o instanceof byte[]) {
            for (byte b : (byte[]) o) {
                v.bytes.add(b);
            }
        } else {
            throw new IllegalArgumentException("Unexpected type in: " + carProp);
        }

        return vehicleProp;
    }

    private static @VehicleAreaType.VehicleAreaTypeValue int getVehicleAreaType(int halArea) {
        switch (halArea) {
            case VehicleArea.GLOBAL:
                return VehicleAreaType.VEHICLE_AREA_TYPE_NONE;
            case VehicleArea.ZONE:
                return VehicleAreaType.VEHICLE_AREA_TYPE_ZONE;
            case VehicleArea.SEAT:
                return VehicleAreaType.VEHICLE_AREA_TYPE_SEAT;
            case VehicleArea.DOOR:
                return VehicleAreaType.VEHICLE_AREA_TYPE_DOOR;
            case VehicleArea.WINDOW:
                return VehicleAreaType.VEHICLE_AREA_TYPE_WINDOW;
            case VehicleArea.MIRROR:
                return VehicleAreaType.VEHICLE_AREA_TYPE_MIRROR;
            default:
                throw new RuntimeException("Unsupported area type " + halArea);
        }
    }

    private static Class<?> getJavaClass(int halType) {
        switch (halType) {
            case VehiclePropertyType.BOOLEAN:
                return Boolean.class;
            case VehiclePropertyType.FLOAT:
                return Float.class;
            case VehiclePropertyType.INT32:
                return Integer.class;
            case VehiclePropertyType.INT32_VEC:
                return Integer[].class;
            case VehiclePropertyType.FLOAT_VEC:
                return Float[].class;
            case VehiclePropertyType.STRING:
                return String.class;
            case VehiclePropertyType.BYTES:
                return byte[].class;
            // case VehiclePropertyType.COMPLEX:
            //     return Object.class;
            default:
                throw new IllegalArgumentException("Unexpected type: " + toHexString(halType));
        }
    }

    private static List getRawValueList(Class<?> clazz, VehiclePropValue.RawValue value) {
        if (classMatched(Float.class, clazz)) {
            return value.floatValues;
        } else if (classMatched(Integer.class, clazz)) {
            return value.int32Values;
        } else {
            throw new IllegalArgumentException("Unexpected type: " + clazz);
        }
    }

    private static boolean classMatched(Class<?> class1, Class<?> class2) {
        return class1 == class2 || class1.getComponentType() == class2;
    }

    private static float[] toFloatArray(List<Float> list) {
        final int size = list.size();
        final float[] array = new float[size];
        for (int i = 0; i < size; ++i) {
            array[i] = list.get(i);
        }
        return array;
    }

    private static int[] toIntArray(List<Integer> list) {
        final int size = list.size();
        final int[] array = new int[size];
        for (int i = 0; i < size; ++i) {
            array[i] = list.get(i);
        }
        return array;
    }

    private static byte[] toByteArray(List<Byte> list) {
        final int size = list.size();
        final byte[] array = new byte[size];
        for (int i = 0; i < size; ++i) {
            array[i] = list.get(i);
        }
        return array;
    }
}

