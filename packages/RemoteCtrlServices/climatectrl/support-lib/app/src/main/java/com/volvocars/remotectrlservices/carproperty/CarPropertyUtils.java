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

import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyType;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyStatus;

import com.volvocars.remotectrlservices.climatectrl.RemoteCtrlPropertyValue;

import android.car.hardware.hvac.CarHvacManager;

import android.util.Log;

import java.util.Collections;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Arrays;

/**
 * Utility functions to work with {@link CarPropertyConfig} and {@link CarPropertyValue}
 */
public final class CarPropertyUtils {
    private static final String TAG = "RemoteCtrl.ClimateCtrl.CarPropertyUtils";

    // TODO: Add tests for these (Philip Werner 2018-04-20)
    private static final Map<Integer, Integer> mMgrHalPropIdMap = new HashMap<Integer, Integer>();
    static {
        mMgrHalPropIdMap.put(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON, CarHvacManager.ID_ZONED_AC_ON);
        mMgrHalPropIdMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_DIRECTION,
                CarHvacManager.ID_ZONED_FAN_DIRECTION);
        mMgrHalPropIdMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT);
        mMgrHalPropIdMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_MAX_DEFROST_ON,
                CarHvacManager.ID_ZONED_MAX_DEFROST_ON);
        mMgrHalPropIdMap.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE,
                CarHvacManager.ID_ZONED_TEMP_SETPOINT);
    }

    private static final Map<Integer, Integer> mTypeMapping = new HashMap<Integer, Integer>();
    static {
        mTypeMapping.put(
                RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_AC_ON, RemoteCtrlHalPropertyType.BOOLEAN);
        mTypeMapping.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_DIRECTION,
                RemoteCtrlHalPropertyType.INT32);
        mTypeMapping.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_FAN_SPEED,
                RemoteCtrlHalPropertyType.INT32);
        mTypeMapping.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_MAX_DEFROST_ON,
                RemoteCtrlHalPropertyType.BOOLEAN);
        mTypeMapping.put(RemoteCtrlHalProperty.REMOTECTRLHAL_HVAC_TEMPERATURE,
                RemoteCtrlHalPropertyType.FLOAT);
    }

    private static final Map<Integer, Integer> mStatusMapping = new HashMap<Integer, Integer>();
    static {
        mStatusMapping.put(
        RemoteCtrlHalPropertyStatus.SYSTEM_ERROR, VehiclePropertyStatus.ERROR);
        //mTypeMapping.put(RemoteCtrlHalPropertyStatus.NOT_PRESENT, VehiclePropertyStatus.INT32);
        mStatusMapping.put(RemoteCtrlHalPropertyStatus.USER_INPUT_DISABLED, VehiclePropertyStatus.UNAVAILABLE);
        mStatusMapping.put(RemoteCtrlHalPropertyStatus.AVAILABLE, VehiclePropertyStatus.AVAILABLE);
    }

    /* Utility class has no public constructor */
    private CarPropertyUtils() {}

    public static RemoteCtrlPropertyValue toRemoteCtrlPropertyValue(
            RemoteCtrlHalPropertyValue propValue) {
        Class<?> clazz = getJavaClass(propValue.prop & VehiclePropertyType.MASK);
        int areaId = propValue.areaId;
        RemoteCtrlHalPropertyValue.RawValue v = propValue.value;

        if (Boolean.class == clazz) {
            boolean value = v.int32Values.isEmpty() ? false : v.int32Values.get(0) == 1;
            return new RemoteCtrlPropertyValue<>(propValue.prop, areaId, value);
        } else if (String.class == clazz) {
            return new RemoteCtrlPropertyValue<>(propValue.prop, areaId, v.stringValue);
        } else if (Long.class == clazz) {
            return new RemoteCtrlPropertyValue<>(propValue.prop, areaId, v.int64Values.get(0));
        } else if (byte[].class == clazz) {
            byte[] halData = toByteArray(v.bytes);
            return new RemoteCtrlPropertyValue<>(propValue.prop, areaId, halData);
        } else /* All list properties */ {
            Object[] values = getRawValueList(clazz, v).toArray();
            return new RemoteCtrlPropertyValue<>(
                    propValue.prop, areaId, values.length == 1 ? values[0] : values);
        }
    }

    public static RemoteCtrlHalPropertyValue toRemoteCtrlHalPropertyValue(
            RemoteCtrlPropertyValue propValue) {
        RemoteCtrlHalPropertyValue retValPropValue = new RemoteCtrlHalPropertyValue();
        retValPropValue.prop = propValue.getPropertyId();
        retValPropValue.areaId = propValue.getAreaId();
        retValPropValue.status = propValue.getStatus();
        RemoteCtrlHalPropertyValue.RawValue v = retValPropValue.value;

        Object o = propValue.getValue();

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
            throw new IllegalArgumentException("Unexpected type in: " + propValue);
        }

        return retValPropValue;
    }

    public static CarPropertyValue<?> toCarPropertyValue(VehiclePropValue halValue) {
        int managerPropId = CarPropertyUtils.halToManagerPropId(halValue.prop);
        return toCarPropertyValue(halValue, managerPropId);
    }



    /** Converts {@link VehiclePropValue} to {@link CarPropertyValue} */
    public static CarPropertyValue<?> toCarPropertyValue(
            RemoteCtrlPropertyValue remoteCtrlPropertyValue) {
        int propertyId =
                CarPropertyUtils.halToManagerPropId(remoteCtrlPropertyValue.getPropertyId());
        int areaId = remoteCtrlPropertyValue.getAreaId();
        int status = remoteCtrlPropertyValue.getStatus();
        Object o = remoteCtrlPropertyValue.getValue();

        if (mTypeMapping.get(remoteCtrlPropertyValue.getPropertyId()) == RemoteCtrlHalPropertyType.BOOLEAN) {
            Log.v(TAG, "BOOLEAN TYPE");
            if (o instanceof Object[]) {
                Log.v(TAG, "OBJECT ARRAY TYPE");
                if (((Object [])o).length == 0) {
                    Log.v(TAG, "OBJECT ARRAY TYPE SIZE 0");
                    return new CarPropertyValue<>(propertyId, areaId, Boolean.TRUE);
                } else {
                    Log.v(TAG, "INTEGER ARRAY TYPE");
                    Integer[] integerArray = Arrays.copyOf((Object [])o, ((Object [])o).length, Integer[].class);
                    return new CarPropertyValue<>(propertyId, areaId, (Boolean)(integerArray[0] == 1));
                }
            } else if (o instanceof Integer) {
                Log.v(TAG, "INTEGER TYPE");
                return new CarPropertyValue<>(propertyId, areaId, (Boolean)((Integer)o == 1));
            }
        } else if (mTypeMapping.get(remoteCtrlPropertyValue.getPropertyId()) == RemoteCtrlHalPropertyType.INT32) {
            Log.v(TAG, "INT32 TYPE");
            if (o instanceof Object[]) {
                // TODO: Get raw values
                if (((Object [])o).length == 0) {
                    return new CarPropertyValue<>(propertyId, areaId, 0);
                } else {
                    Integer[] integerArray = Arrays.copyOf((Object [])o, ((Object [])o).length, Integer[].class);
                    return new CarPropertyValue<>(propertyId, areaId, integerArray[0]);
                }
            } else if (o instanceof Integer) {
                return new CarPropertyValue<>(propertyId, areaId, (Integer)o);
            }
        } else if (mTypeMapping.get(remoteCtrlPropertyValue.getPropertyId()) == RemoteCtrlHalPropertyType.FLOAT) {
            Log.v(TAG, "FLOAT TYPE");
            if (o instanceof Object[]) {
                // TODO: Get raw values
                if (((Object [])o).length == 0) {
                    return new CarPropertyValue<>(propertyId, areaId, 0.0f);
                } else {
                    Float[] floatArray = Arrays.copyOf((Object [])o, ((Object [])o).length, Float[].class);
                    return new CarPropertyValue<>(propertyId, areaId, floatArray[0]);
                }
            } else if (o instanceof Float) {
                return new CarPropertyValue<>(propertyId, areaId, (Float)o);
            }
        }

        throw new IllegalArgumentException("Unexpected type in: " + remoteCtrlPropertyValue);
    }

    /** Converts {@link VehiclePropValue} to {@link CarPropertyValue} */
    public static RemoteCtrlPropertyValue<?> toRemoteCtrlPropertyValue(
            CarPropertyValue carPropertyValue) {
        int propertyId = CarPropertyUtils.managerToHalPropId(carPropertyValue.getPropertyId());
        int areaId = carPropertyValue.getAreaId();
        int status = managerToHalStatus(carPropertyValue.getStatus());
        Object o = carPropertyValue.getValue();

        if (o instanceof Boolean) {
            Integer value = ((Boolean) o) ? 1 : 0;
            return new RemoteCtrlPropertyValue<>(propertyId, areaId, status, value);
        } else if (o instanceof String) {
            return new RemoteCtrlPropertyValue<>(propertyId, areaId, status, (String) o);
        } else if (o instanceof Integer) {
            return new RemoteCtrlPropertyValue<>(propertyId, areaId, status, (Integer) o);
        } else if (o instanceof Float) {
            return new RemoteCtrlPropertyValue<>(propertyId, areaId, status, (Float) o);
        } else {
            throw new IllegalArgumentException("Unexpected type in: " + carPropertyValue);
        }
    }

    public static VehiclePropValue toVehiclePropValue(CarPropertyValue carProp) {
        int halPropId = CarPropertyUtils.managerToHalPropId(carProp.getPropertyId());
        return toVehiclePropValue(carProp, halPropId);
    }

    /** Converts {@link VehiclePropValue} to {@link CarPropertyValue} */
    public static CarPropertyValue<?> toCarPropertyValue(
            VehiclePropValue halValue, int propertyId) {
        Class<?> clazz = getJavaClass(halValue.prop & VehiclePropertyType.MASK);
        int areaId = halValue.areaId;
        VehiclePropValue.RawValue v = halValue.value;

        if (Boolean.class == clazz) {
            boolean value = v.int32Values.isEmpty() ? false : v.int32Values.get(0) == 1;
            return new CarPropertyValue<>(propertyId, areaId, value);
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

    public static RemoteCtrlPropertyValue<?> createRemoteCtrlPropertyValueWithErrorStatus(
        RemoteCtrlPropertyValue propValue) {
        Object o = propValue.getValue();
        int propertyId = propValue.getPropertyId();
        int areaId = propValue.getAreaId();
        if (o instanceof Integer) {
            return new RemoteCtrlPropertyValue<Integer>(propertyId, areaId,
                    RemoteCtrlHalPropertyStatus.SYSTEM_ERROR, (Integer) propValue.getValue());
        } else if (o instanceof Float) {
            return new RemoteCtrlPropertyValue<Float>(propertyId, areaId,
                    RemoteCtrlHalPropertyStatus.SYSTEM_ERROR, (Float) propValue.getValue());
        } else if (o instanceof Boolean) {
            return new RemoteCtrlPropertyValue<Boolean>(propertyId, areaId,
                    RemoteCtrlHalPropertyStatus.SYSTEM_ERROR, (Boolean) propValue.getValue());
        }

        throw new IllegalArgumentException("Unsupported value type: " + o.getClass().getName());
    }

    public static Class<?> getJavaClassByManagerPropertyId(int managerPropId) {
        int halPropId = managerToHalPropId(managerPropId);
        return getJavaClass(managerToHalPropId(managerPropId) & VehiclePropertyType.MASK);
    }

    public static RemoteCtrlHalPropertyValue createRemoteCtrlHalPropertyValueWithErrorStatus(
            RemoteCtrlHalPropertyValue value) {
        value.status = RemoteCtrlHalPropertyStatus.SYSTEM_ERROR;
        return value;
    }

    public static VehiclePropValue createVehiclePropertyValueWithErrorStatus(
            VehiclePropValue propValue) {
        propValue.status = VehiclePropertyStatus.ERROR;
        return propValue;
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

    public static int halToManagerStatus(int status) {
        return mStatusMapping.get(status);
    }

    public static int managerToHalStatus(int status) {
        for (Map.Entry<Integer, Integer> entry : mStatusMapping.entrySet()) {
            if (entry.getValue() == status) {
                return entry.getKey();
            }
        }
        throw new IllegalArgumentException("No manager status (" + status
                + ") found for Hvac status id (" + status + ")");
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

    private static List getRawValueList(Class<?> clazz, RemoteCtrlHalPropertyValue.RawValue value) {
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
