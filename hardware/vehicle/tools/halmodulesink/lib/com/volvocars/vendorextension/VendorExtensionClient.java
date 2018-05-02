/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vendorextension;


import android.car.hardware.CarPropertyConfig;
import android.car.hardware.CarPropertyValue;
import android.car.hardware.CarVendorExtensionManager;
import android.content.Context;
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

/**
 * Common API for connecting with Google car service and vendor extension
 * TODO: add security
 * Singleton class
 */
public class VendorExtensionClient {

    private static final String TAG = VendorExtensionClient.class.getSimpleName();
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

                        // Add all supported features
                        List<CarPropertyConfig> properties = carVEManager.getProperties();
                        properties.forEach(property -> {
                            int[] zoneAreas = property.getAreaIds();
                            if (zoneAreas.length != 0) {  // Area zone
                                for (int i = 0; i < zoneAreas.length; i++) {
                                    supportedFeatures.add(new VehiclePropertySupport(property.getPropertyType(), property.getPropertyId(), zoneAreas[i]));
                                }
                            } else { // Global area
                                supportedFeatures.add(new VehiclePropertySupport(property.getPropertyType(), property.getPropertyId(), property.getAreaType()));
                            }
                        });
                    } catch (CarNotConnectedException | android.car.CarNotConnectedException e) {
                        Log.e(TAG, "Car not connected in onConnected", e);
                    }
                    Log.d(TAG, "onConnected: Car.getCarManager()");

                    // Release after car connected
                    semaphore.release();
                    registerCallback();
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
        try {
            semaphore.acquire();
        } catch (InterruptedException e) {
            Log.e(TAG, "Thread interrupted", e);
        }
        CompletableFuture.runAsync(() -> init());
    }


    /**
     * Validation of the permission and connected Car
     * TODO: implement netter exception handling such as throwing a car not connected
     */
    private boolean validateCarConnection() {
        try {
            if (semaphore.tryAcquire(CAR_CONNECT_WAIT_TIME_SECONDS, TimeUnit.SECONDS)) {
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
        validateCarConnection();
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
        validateCarConnection();
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID).findFirst();
        return carVEManager.getGlobalProperty(result.get().type, propID);
    }

    public int getStatus(int propID) throws NotSupportedException, android.car.CarNotConnectedException {
        validateCarConnection();
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
        validateCarConnection();
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID & feature.area == area).findFirst();
        return (Integer) carVEManager.getProperty(result.get().type, propID, area);
    }

    /**
     * Get CarPropertyConfig using ID
     * @param propID
     * @return Optional<CarPropertyConfig>
     * @throws NotSupportedException
     * @throws android.car.CarNotConnectedException
     */
    public Optional<CarPropertyConfig>  getCarPropertyConfig(int propID)
            throws NotSupportedException, android.car.CarNotConnectedException {
        validateCarConnection();
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        List<CarPropertyConfig> properties = carVEManager.getProperties();
        Optional<CarPropertyConfig> result = properties.stream()
                .filter(property -> property.getPropertyId() == propID).findFirst();
        return result;
    }

    /**
     * Get the type information of the PropID
     * @param propID
     * @param area
     * @return Optional<VehiclePropertySupport>
     */
    public Optional<VehiclePropertySupport>  getVehiclePropertySupport(int propID, int area) {
        validateCarConnection();
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID & feature.area == area).findFirst();
        return result;
    }

    /**
     * Set propID value using Global zone ID
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
        validateCarConnection();
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID).findFirst();
        carVEManager.setGlobalProperty(result.get().type,
                propID,
                data);
    }

    /**
     * Set propID value using zone area ID
     * @param propID
     * @param area
     * @param data
     * @throws NotSupportedException
     * @throws android.car.CarNotConnectedException
     */
    public void set(int propID, int area, Object data)
            throws NotSupportedException, android.car.CarNotConnectedException {
        validateCarConnection();
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

    /**
     * Checks if this feature supported by the API
     * @param propId
     * @return
     */
    public boolean isSupportedFeature(int propId) {
        return isSupportedFeature(propId, NO_INNER_AREA);
    }

    /**
     * Checks if this feature supported by the API
     * @param propId
     * @param areaId
     * @return
     */
    public boolean isSupportedFeature(int propId, int areaId) {
        validateCarConnection();
        return supportedFeatures.stream().anyMatch(
                feature -> feature.vehicleProperty == propId & feature.area == areaId);
    }

    /**
     * Checks if this feature supported by the API
     * @return
     */
    public List<Integer> getSupportedFeatures() {
        validateCarConnection();
        return new Vector<Integer>(supportedFeatures.stream()
                .map(vehiclePropertySupport -> vehiclePropertySupport.vehicleProperty)
                .collect(Collectors.toList()));

    }

    /**
     * Register the main call back to the VendorExtension
     */
    private void registerCallback() {
        validateCarConnection();
        try {
            carVEManager.registerCallback(mainCallBack);
        } catch (android.car.CarNotConnectedException e) {
            Log.e(TAG, "Car not connected in VEC");
        }
    }

    /**
     * Register a specific property callback
     * @param callBack
     */
    public void registerCallback(VendorExtensionCallBack callBack) {
        validateCarConnection();
        ArrayList<VendorExtensionCallBack> foundExisting = vendorCallBacks.get(callBack.propId);
        if (foundExisting != null) {
            foundExisting.add(callBack);
        } else {
            ArrayList<VendorExtensionCallBack> newListener = new ArrayList<VendorExtensionCallBack>();
            newListener.add(callBack);
            vendorCallBacks.put(callBack.propId, newListener);
        }
    }

    /**
     * Remove the registered property callback
     * @param callBack
     */
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
        VehiclePropertySupport status;
        boolean available;

        public VehiclePropertySupport(Class type, Integer vehicleProperty, Integer area) {
            this.type = type;
            this.vehicleProperty = vehicleProperty;
            this.area = area;
            this.available = false;
            this.status = null;
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