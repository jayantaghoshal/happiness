/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vendorextension;


import android.car.hardware.CarPropertyValue;
import android.car.hardware.CarVendorExtensionManager;
import android.content.Context;
import android.hardware.automotive.vehicle.V2_0.VehicleArea;
import android.support.car.Car;
import android.support.car.CarConnectionCallback;
import android.support.car.CarNotConnectedException;
import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Optional;
import java.util.Vector;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

import vendor.volvocars.hardware.vehiclehal.V1_0.PAStatus;
import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

/**
 * Common API for connecting with Google car service and vendor extension
 * TODO: add security
 * Singleton class
 */
public class VendorExtensionClient {

    private static final String TAG = VendorExtensionClient.class.getSimpleName();
    public static final int WAIT_TIME_MILLISECONDS = 1000;
    public static final int CAR_CONNECT_WAIT_TIME_SECONDS = 5;
    public static final int NO_INNER_AREA = 0;
    static Semaphore semaphore = new Semaphore(1);
    private static VendorExtensionClient sVendorExtensionClient;
    private Context context;
    private Car mCar;
    private CarVendorExtensionManager carVEManager;
    private List<VehiclePropertySupport> supportedFeatures;
    private HashMap<Integer, ArrayList<VendorExtensionCallBack>> vendorCallBacks;


// Init methods

    private void init() {
        vendorCallBacks = new HashMap<>();
        try {
            semaphore.acquire();
        } catch (InterruptedException e) {
            Log.e(TAG, "Thread interrupted", e);
        }
        // Add supported features
        supportedFeatures.add(
                new VehiclePropertySupport(Integer.class, VehicleProperty.DAI_SETTING,
                        NO_INNER_AREA));
        supportedFeatures.add(
                new VehiclePropertySupport(Boolean.class, VehicleProperty.CURVE_SPEED_ADAPTION_ON,
                        NO_INNER_AREA));
        supportedFeatures.add(
                new VehiclePropertySupport(Integer.class, VehicleProperty.CONNECTED_SAFETY_ON,
                        NO_INNER_AREA));
        supportedFeatures.add(
                new VehiclePropertySupport(Integer.class, VehicleProperty.CURVE_SPEED_ADAPTION_STATUS,
                        NO_INNER_AREA));
        supportedFeatures.add(
                new VehiclePropertySupport(Boolean.class, VehicleProperty.LANE_KEEPING_AID_ON,
                        NO_INNER_AREA));
        supportedFeatures.add(
                new VehiclePropertySupport(Integer.class, VehicleProperty.LANE_KEEPING_AID_MODE,
                        NO_INNER_AREA));
 	supportedFeatures.add(
                new VehiclePropertySupport(Integer.class, VehicleProperty.EMERGENCY_LANE_KEEPING_AID_STATUS,
                        NO_INNER_AREA));
        supportedFeatures.add(
                new VehiclePropertySupport(Integer.class, VehicleProperty.EMERGENCY_LANE_KEEPING_AID_ON,
                        NO_INNER_AREA));

        if (mCar == null) {
            mCar = Car.createCar(context, mServiceConnectionCallback);
            Log.d(TAG, "onCreate: Car.connect()");
            try {
                mCar.connect();
            } catch (Exception e) {
                Log.e(TAG, "ERROR: Car.connect() has problems: ", e);
                semaphore.release();
            }
        }

    }

    /**
     * Reconnect with the car service
     * It clears all callbacks
     *
     * @return
     */
    public boolean reconnect() {
        try {
            // Check of it is blocked by other methods
            if (semaphore.availablePermits() == 0) {
                semaphore.release();
            }
            semaphore.acquire();
        } catch (InterruptedException e) {
            Log.e(TAG, "Thread interrupted", e);
        }

        if (mCar.isConnected()) {
            semaphore.release();
            return true;
        } else {
            mCar.connect();
        }

        // Wait for the CarService connection
        try {
            if (semaphore.tryAcquire(CAR_CONNECT_WAIT_TIME_SECONDS, TimeUnit.SECONDS)) {
                return true;
            } else {
                return false;
            }
        } catch (InterruptedException e) {
            Log.e(TAG, "ERROR: Car.connect() has problems: ", e);
            return false;
        }
    }

    /**
     * Talks with the CarVendorExtensionManager
     */
    private final CarConnectionCallback mServiceConnectionCallback =
            new CarConnectionCallback() {
                @Override
                public void onConnected(Car car) {
                    Log.d(TAG, "onConnected()");
                    try {
                        carVEManager = (CarVendorExtensionManager) mCar.getCarManager(
                                android.car.Car.VENDOR_EXTENSION_SERVICE);
                        // Everytime car connect, we need to use new service, therefore clear callbacks
                        vendorCallBacks.clear();
                        registerCallback();
                    } catch (CarNotConnectedException e) {
                        Log.e(TAG, "Car not connected in onConnected", e);
                    }

                    Log.d(TAG, "onConnected: Car.getCarManager()");
                    semaphore.release();
                }

                @Override
                public void onDisconnected(Car car) {
                    Log.d(TAG, "onDisconnected()");
                    // Check if it happened middle of the way of connecting
                    if (semaphore.availablePermits() == 0) {
                        semaphore.release();
                    }
                    // It suggested that we can use the same object of the car
                    reconnect();
                }
            };

    /**
     * Init
     *
     * @param context Get context of the application
     */
    private VendorExtensionClient(Context context) {
        this.context = context;
        supportedFeatures = new Vector<>();
        CompletableFuture.runAsync(() -> init());
    }


    /**
     * Validation of the permission and connected Car
     * TODO: implement netter exception handling such as throwing a car not connected
     */
    private boolean validate() {
        try {
            if (semaphore.tryAcquire(WAIT_TIME_MILLISECONDS, TimeUnit.MILLISECONDS)) {
                semaphore.release();
                return true;
            } else {
                return false;
            }
        } catch (InterruptedException e) {
            Log.e(TAG, "Interrupted a while waiting ... ", e);
            return false;
        }
    }

// CarVendorExtension handling methods

    /**
     * Get propID value using Zone Area information (make sure using zoned area information)
     *
     * @param propID
     * @param area
     * @return
     * @throws NotSupportedException
     * @throws android.car.CarNotConnectedException
     */
    public Object get(int propID, int area)
            throws NotSupportedException, android.car.CarNotConnectedException {
        validate();
        if (!isSupportedFeature(propID, area)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID & feature.area == area).findFirst();
        return carVEManager.getProperty(result.get().type, propID, area);
    }

    /**
     * GetStatus propID value using Global zone
     *
     * @param propID
     * @return
     * @throws NotSupportedException
     * @throws android.car.CarNotConnectedException
     */
    public Object get(int propID)
            throws NotSupportedException, android.car.CarNotConnectedException {
        validate();
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID).findFirst();
        return carVEManager.getGlobalProperty(result.get().type, propID);
    }

    public int getStatus(int propID) throws NotSupportedException, android.car.CarNotConnectedException {
        validate();
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID).findFirst();
        return (Integer) carVEManager.getGlobalProperty(result.get().type, propID);
    }

    /**
     * GetStatus propID value using Zone Area information (make sure using zoned area information)
     *
     * @param propID
     * @param area
     * @return
     * @throws NotSupportedException
     * @throws android.car.CarNotConnectedException
     */
    public int getStatus(int propID, int area)
            throws NotSupportedException, android.car.CarNotConnectedException {
        validate();
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID & feature.area == area).findFirst();
        return (Integer) carVEManager.getProperty(result.get().type, propID, area);
    }

    /**
     * Get the type information of the PropID
     *
     * @param propID
     * @param area
     * @return
     */
    public VehiclePropertySupport getVehiclePropertySupport(int propID, int area) {
        validate();
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID & feature.area == area).findFirst();
        return result.get();
    }

    /**
     * Set propID value using Global zone ID
     *
     * @param propID
     * @param data
     * @param <E>
     * @throws NotSupportedException
     * @throws android.car.CarNotConnectedException
     */
    public <E> void set(int propID, Object data) throws NotSupportedException, android.car.CarNotConnectedException {
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        validate();
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID).findFirst();
        carVEManager.setGlobalProperty(result.get().type,
                propID,
                data);
    }

    /**
     * Set propID value using zone area ID
     *
     * @param propID
     * @param area
     * @param data
     * @throws NotSupportedException
     * @throws android.car.CarNotConnectedException
     */
    public void set(int propID, int area, Object data)
            throws NotSupportedException, android.car.CarNotConnectedException {
        validate();
        if (!isSupportedFeature(propID, area)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID
                        & feature.area == area).findFirst();
        carVEManager.setProperty(result.get().type,
                propID,
                area,
                data);
    }

    /**
     * @param contextTemp
     * @return
     */
    public static VendorExtensionClient getVolvoExtension(Context contextTemp) {
        if (sVendorExtensionClient == null) {
            sVendorExtensionClient = new VendorExtensionClient(contextTemp);
        }
        return sVendorExtensionClient;
    }

    // Feature handling methods

    /**
     * TODO: Maybe hide this? can get all information
     *
     * @param propertyClass
     * @param propId
     * @param area
     * @param <E>
     * @return
     */
    private <E> Boolean isFeatureAvailable(Class<E> propertyClass, int propId, int area) {
        validate();
        return isFeatureAvailable(new VehiclePropertySupport(propertyClass, propId, area));
    }

    /**
     * Checks if this feature is available
     *
     * @param propId
     * @param <E>
     * @return
     */
    public <E> Boolean isFeatureAvailable(int propId) {
        validate();
        return isFeatureAvailable(propId, NO_INNER_AREA);
    }

    /**
     * Checks if this feature is available
     *
     * @param propId
     * @param area
     * @param <E>
     * @return
     */
    public <E> Boolean isFeatureAvailable(int propId, int area) {
        validate();
        return isFeatureAvailable(new VehiclePropertySupport(getVehiclePropertySupport(propId, area).type, propId, area));
    }

    /**
     * Checks if this feature is available, Maybe force to use only defined propIds.
     * Right now getProperty can get all the properties even not vendor extension.
     *
     * @param vehiclePropertySupport
     * @param <E>
     * @return
     */
    private <E> Boolean isFeatureAvailable(VehiclePropertySupport vehiclePropertySupport) {
        validate();
        try {
            carVEManager.getProperty(vehiclePropertySupport.type,
                    vehiclePropertySupport.vehicleProperty, vehiclePropertySupport.area);
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * Checks if this feature supported by the API
     *
     * @param propId
     * @return
     */
    public boolean isSupportedFeature(int propId) {
        validate();
        return isSupportedFeature(propId, NO_INNER_AREA);
    }

    /**
     * Checks if this feature supported by the API
     *
     * @param propId
     * @param areaId
     * @return
     */
    public boolean isSupportedFeature(int propId, int areaId) {
        validate();
        return supportedFeatures.stream().anyMatch(
                feature -> feature.vehicleProperty == propId & feature.area == areaId);
    }

    /**
     * Checks if this feature supported by the API
     *
     * @return
     */
    public List<Integer> getSupportedFeatures() {
        validate();
        return new Vector<Integer>(supportedFeatures.stream()
                .filter(this::isFeatureAvailable)
                .map(vehiclePropertySupport -> vehiclePropertySupport.vehicleProperty)
                .collect(Collectors.toList()));

    }

    public List<Integer> getAvailableFeatures() {
        validate();
        return new Vector<Integer>(supportedFeatures.stream()
                .filter(this::isFeatureAvailable)
                .map(vehiclePropertySupport -> vehiclePropertySupport.vehicleProperty)
                .collect(Collectors.toList()));
    }

    /**
     * TODO: decide either supported features synced with vehicle hal and only allow public access "isSupportedFeature"
     *
     * @return
     */
    public List<Integer> getAvailableFeaturesAndSupported() {
        validate();
        supportedFeatures.stream().map(vehiclePropertySupport -> vehiclePropertySupport.available =
                isFeatureAvailable(vehiclePropertySupport));
        return getAvailableFeatures();
    }

    private void registerCallback() {
        validate();
        try {
            carVEManager.registerCallback(mainCallBack);
        } catch (android.car.CarNotConnectedException e) {
            Log.e(TAG, "Car not connected in VEC");
        }
    }

    public void registerCallback(VendorExtensionCallBack callBack) {
        validate();
        ArrayList<VendorExtensionCallBack> foundExisting = vendorCallBacks.get(callBack.propId);
        if (foundExisting != null) {
            foundExisting.add(callBack);
        } else {
            ArrayList<VendorExtensionCallBack> newListener = new ArrayList<VendorExtensionCallBack>();
            newListener.add(callBack);
            vendorCallBacks.put(callBack.propId, newListener);
        }
    }

    public void removeCallback(VendorExtensionCallBack callBack) {
        ArrayList<VendorExtensionCallBack> foundExisting = vendorCallBacks.get(callBack.propId);
        if (foundExisting != null) {
            foundExisting.remove(callBack);
        }
    }

    /**
     * Wrap methods for getting and setting propId
     */
    class VehiclePropertySupport implements Comparable<VehiclePropertySupport> {
        Class type;
        Integer vehicleProperty;
        Integer area;
        boolean available;

        public VehiclePropertySupport(Class type, Integer vehicleProperty, Integer area) {
            this.type = type;
            this.vehicleProperty = vehicleProperty;
            this.area = area;
            this.available = false;
        }

        @Override
        public int compareTo(VehiclePropertySupport o) {
            if (this.type.equals(o.type) &&
                    this.vehicleProperty == o.vehicleProperty &&
                    this.area == o.area) {
                return 0;
            } else {
                return 1;
            }

        }
    }

    /**
     * Callback for logging purpose
     */
    private final CarVendorExtensionManager.CarVendorExtensionCallback mainCallBack =
            new CarVendorExtensionManager.CarVendorExtensionCallback() {
                @Override
                public void onChangeEvent(final CarPropertyValue val) {
                    Log.d(TAG, "mainCallBack onChangeEvent" + val);
                    ArrayList<VendorExtensionCallBack> listeners = vendorCallBacks.get(val.getPropertyId());
                    if (listeners != null && !listeners.isEmpty()) {
                        listeners.stream().filter(callback -> callback.areaId == val.getAreaId())
                                .forEach(vendorExtensionCallBack -> vendorExtensionCallBack.onChangeEvent(val));
                    }
                }


                @Override
                public void onErrorEvent(int propertyId, int zone) {
                    Log.d(TAG, "mainCallBack onErrorEvent" + propertyId);
                    ArrayList<VendorExtensionCallBack> listeners = vendorCallBacks.get(propertyId);
                    if (listeners != null && !listeners.isEmpty()) {
                        listeners.stream().filter(callback -> callback.areaId == zone)
                                .forEach(vendorExtensionCallBack -> vendorExtensionCallBack.onErrorEvent(propertyId, zone));
                    }
                }
            };

    public class NotSupportedException extends Exception {
        public NotSupportedException(String message) {
            super(message);
        }
    }
}