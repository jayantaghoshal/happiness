/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.test.lib.vehiclehal;


import static java.lang.Integer.toHexString;

import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.IVehicleCallback;
import android.hardware.automotive.vehicle.V2_0.SubscribeOptions;
import android.hardware.automotive.vehicle.V2_0.VehiclePropConfig;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropertyAccess;
import android.hardware.automotive.vehicle.V2_0.VehiclePropertyChangeMode;
import android.os.HandlerThread;
import android.os.RemoteException;
import android.util.Log;

import java.io.PrintWriter;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;
/**
 * Abstraction for vehicle HAL. This class handles interface with native HAL and do basic parsing
 * of received data (type check). Then each event is sent to corresponding {@link ...}
 * implementation. It is responsibility of {@link ..} to convert data to corresponding
 * CarService for CarManager API.
 */
public class VehicleHal extends IVehicleCallback.Stub {

    private static final boolean DBG = false;

    private static final int NO_AREA = -1;

    private static final String TAG = VehicleHal.class.getSimpleName();

    private final HandlerThread mHandlerThread;

    /** Might be re-assigned if Vehicle HAL is reconnected. */
    private volatile HalClient vehicleHalClient;

    private final HashMap<Integer, SubscribeOptions> mSubscribedProperties = new HashMap<>();
    private volatile HashMap<Integer, VehiclePropConfig> mAllProperties = new HashMap<>();

    private IVehicle vehicle;

    public VehicleHal(IVehicle vehicle) {
        mHandlerThread = new HandlerThread("VEHICLE-HAL");
        mHandlerThread.start();
        this.vehicle = vehicle;
        vehicleHalClient = new HalClient(this.vehicle, mHandlerThread.getLooper(), this /*IVehicleCallback*/);
    }

    public void release() {
        synchronized (this) {
            for (int p : mSubscribedProperties.keySet()) {
                try {
                    vehicleHalClient.unsubscribe(p);
                } catch (RemoteException e) {
                    //  Ignore exceptions on shutdown path.
                    Log.w(TAG, "Failed to unsubscribe", e);
                }
            }
            mSubscribedProperties.clear();
            mAllProperties.clear();
        }
        // keep the looper thread as should be kept for the whole life cycle.
    }

    public Collection<VehiclePropConfig> getAllPropConfigs() {
        return mAllProperties.values();
    }


    /**
     * Use it for direct access for the VehicleHal
     * @return
     */
    public Collection<VehiclePropConfig> getAllPropConfigsDirect() throws RemoteException{
        mAllProperties.clear();
        Set<VehiclePropConfig> properties;
        properties = new HashSet<>(vehicleHalClient.getAllPropConfigs());
        synchronized (this) {
            // Create map of all properties
            for (VehiclePropConfig p : properties) {
                mAllProperties.put(p.prop, p);
            }
        }
        return mAllProperties.values();
    }

    public VehiclePropValue get(int propertyId) throws PropertyTimeoutException {
        return get(propertyId, NO_AREA);
    }

    public VehiclePropValue get(int propertyId, int areaId) throws PropertyTimeoutException {
        if (DBG) {
            Log.i(TAG, "get, property: 0x" + toHexString(propertyId)
                    + ", areaId: 0x" + toHexString(areaId));
        }
        VehiclePropValue propValue = new VehiclePropValue();
        propValue.prop = propertyId;
        propValue.areaId = areaId;
        return vehicleHalClient.getValue(propValue);
    }

    public <T> T get(Class clazz, int propertyId) throws PropertyTimeoutException {
        return get(clazz, createPropValue(propertyId, NO_AREA));
    }

    public <T> T get(Class clazz, int propertyId, int areaId) throws PropertyTimeoutException {
        return get(clazz, createPropValue(propertyId, areaId));
    }

    @SuppressWarnings("unchecked")
    public <T> T get(Class clazz, VehiclePropValue requestedPropValue)
            throws PropertyTimeoutException {
        VehiclePropValue propValue;
        propValue = vehicleHalClient.getValue(requestedPropValue);

        if (clazz == Integer.class || clazz == int.class) {
            return (T) propValue.value.int32Values.get(0);
        } else if (clazz == Boolean.class || clazz == boolean.class) {
            return (T) Boolean.valueOf(propValue.value.int32Values.get(0) == 1);
        } else if (clazz == Float.class || clazz == float.class) {
            return (T) propValue.value.floatValues.get(0);
        } else if (clazz == Integer[].class) {
            Integer[] intArray = new Integer[propValue.value.int32Values.size()];
            return (T) propValue.value.int32Values.toArray(intArray);
        } else if (clazz == Float[].class) {
            Float[] floatArray = new Float[propValue.value.floatValues.size()];
            return (T) propValue.value.floatValues.toArray(floatArray);
        } else if (clazz == int[].class) {
            return (T) HalServiceUtils.toIntArray(propValue.value.int32Values);
        } else if (clazz == float[].class) {
            return (T) HalServiceUtils.toFloatArray(propValue.value.floatValues);
        } else if (clazz == byte[].class) {
            return (T) HalServiceUtils.toByteArray(propValue.value.bytes);
        } else if (clazz == String.class) {
            return (T) propValue.value.stringValue;
        } else {
            throw new IllegalArgumentException("Unexpected type: " + clazz);
        }
    }

    public VehiclePropValue get(VehiclePropValue requestedPropValue)
            throws PropertyTimeoutException {
        return vehicleHalClient.getValue(requestedPropValue);
    }

    public void set(VehiclePropValue propValue) throws PropertyTimeoutException {
        vehicleHalClient.setValue(propValue);
    }

    VehiclePropValueSetter set(int propId) {
        return new VehiclePropValueSetter(vehicleHalClient, propId, NO_AREA);
    }


    VehiclePropValueSetter set(int propId, int areaId) {
        return new VehiclePropValueSetter(vehicleHalClient, propId, areaId);
    }

    static boolean isPropertySubscribable(VehiclePropConfig config) {
        if ((config.access & VehiclePropertyAccess.READ) == 0 ||
                (config.changeMode == VehiclePropertyChangeMode.STATIC)) {
            return false;
        }
        return true;
    }

    static void dumpProperties(PrintWriter writer, Collection<VehiclePropConfig> configs) {
        for (VehiclePropConfig config : configs) {
            writer.println(String.format("property 0x%x", config.prop));
        }
    }

    @Override
    public void onPropertyEvent(ArrayList<VehiclePropValue> propValues) {
//        synchronized (this) {
//            for (VehiclePropValue v : propValues) {
//                HalServiceBase service = mPropertyHandlers.get(v.prop);
//                if(service == null) {
//                    Log.e(TAG, "HalService not found for prop: 0x"
//                            + toHexString(v.prop));
//                    continue;
//                }
//                service.getDispatchList().add(v);
//                mServicesToDispatch.add(service);
//                VehiclePropertyEventInfo info = mEventLog.get(v.prop);
//                if (info == null) {
//                    info = new VehiclePropertyEventInfo(v);
//                    mEventLog.put(v.prop, info);
//                } else {
//                    info.addNewEvent(v);
//                }
//            }
//        }
//        for (HalServiceBase s : mServicesToDispatch) {
//            s.handleHalEvents(s.getDispatchList());
//            s.getDispatchList().clear();
//        }
//        mServicesToDispatch.clear();
    }

    @Override
    public void onPropertySet(VehiclePropValue value) {
        // No need to handle on-property-set events in HAL service yet.
    }

    @Override
    public void onPropertySetError(int errorCode, int propId, int areaId) {
//        Log.e(TAG, String.format("onPropertySetError, errorCode: %d, prop: 0x%x, "
//                + "area: 0x%x", errorCode, propId, areaId));
//        if (propId != VehicleProperty.INVALID) {
//            HalServiceBase service = mPropertyHandlers.get(propId);
//            if (service != null) {
//                service.handlePropertySetError(propId, areaId);
//            }
//        }
    }

//
//    /**
//     * Inject a fake boolean HAL event - for testing purposes.
//     * @param propId - VehicleProperty ID
//     * @param areaId - Vehicle Area ID
//     * @param value - true/false to inject
//     */
//    public void injectBooleanEvent(int propId, int areaId, boolean value) {
//        VehiclePropValue v = createPropValue(propId, areaId);
//        v.value.int32Values.add(value? 1 : 0);
//        onPropertyEvent(Lists.newArrayList(v));
//    }
//
//    /**
//     * Inject a fake Integer HAL event - for testing purposes.
//     * @param propId - VehicleProperty ID
//     * @param value - Integer value to inject
//     */
//    public void injectIntegerEvent(int propId, int value) {
//        VehiclePropValue v = createPropValue(propId, 0);
//        v.value.int32Values.add(value);
//        v.timestamp = SystemClock.elapsedRealtimeNanos();
//        onPropertyEvent(Lists.newArrayList(v));
//    }

    private static class VehiclePropertyEventInfo {
        private int eventCount;
        private VehiclePropValue lastEvent;

        private VehiclePropertyEventInfo(VehiclePropValue event) {
            eventCount = 1;
            lastEvent = event;
        }

        private void addNewEvent(VehiclePropValue event) {
            eventCount++;
            lastEvent = event;
        }
    }

    final class VehiclePropValueSetter {
        final WeakReference<HalClient> mClient;
        final VehiclePropValue mPropValue;

        private VehiclePropValueSetter(HalClient client, int propId, int areaId) {
            mClient = new WeakReference<>(client);
            mPropValue = new VehiclePropValue();
            mPropValue.prop = propId;
            mPropValue.areaId = areaId;
        }

        void to(boolean value) throws PropertyTimeoutException {
            to(value ? 1 : 0);
        }

        void to(int value) throws PropertyTimeoutException {
            mPropValue.value.int32Values.add(value);
            submit();
        }

        void to(int[] values) throws PropertyTimeoutException {
            for (int value : values) {
                mPropValue.value.int32Values.add(value);
            }
            submit();
        }

        void to(Collection<Integer> values) throws PropertyTimeoutException {
            mPropValue.value.int32Values.addAll(values);
            submit();
        }

        void submit() throws PropertyTimeoutException {
            HalClient client =  mClient.get();
            if (client != null) {
                if (DBG) {
                    Log.i(TAG, "set, property: 0x" + toHexString(mPropValue.prop)
                            + ", areaId: 0x" + toHexString(mPropValue.areaId));
                }
                client.setValue(mPropValue);
            }
        }
    }

    private static String dumpVehiclePropValue(VehiclePropValue value) {
        final int MAX_BYTE_SIZE = 20;

        StringBuilder sb = new StringBuilder()
                .append("Property:0x").append(toHexString(value.prop))
                .append(",timestamp:").append(value.timestamp)
                .append(",zone:0x").append(toHexString(value.areaId))
                .append(",floatValues: ").append(Arrays.toString(value.value.floatValues.toArray()))
                .append(",int32Values: ").append(Arrays.toString(value.value.int32Values.toArray()))
                .append(",int64Values: ")
                .append(Arrays.toString(value.value.int64Values.toArray()));

        if (value.value.bytes.size() > MAX_BYTE_SIZE) {
            Object[] bytes = Arrays.copyOf(value.value.bytes.toArray(), MAX_BYTE_SIZE);
            sb.append(",bytes: ").append(Arrays.toString(bytes));
        } else {
            sb.append(",bytes: ").append(Arrays.toString(value.value.bytes.toArray()));
        }
        sb.append(",string: ").append(value.value.stringValue);

        return sb.toString();
    }

    private static VehiclePropValue createPropValue(int propId, int areaId) {
        VehiclePropValue propValue = new VehiclePropValue();
        propValue.prop = propId;
        propValue.areaId = areaId;
        return propValue;
    }
}
