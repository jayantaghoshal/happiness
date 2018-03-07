/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vendorextension;


import android.car.hardware.CarPropertyValue;
import android.car.hardware.CarVendorExtensionManager;
import android.content.Context;
import android.support.car.Car;
import android.support.car.CarConnectionCallback;
import android.support.car.CarNotConnectedException;
import android.util.Log;

import java.util.List;
import java.util.Optional;
import java.util.Vector;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Semaphore;
import java.util.stream.Collectors;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

/**
 * Common API for connecting with Google car service and vendor extension
 * TODO: add security
 * Singleton class
 */
public class VendorExtensionClient {

    private static final String TAG = VendorExtensionClient.class.getSimpleName();
    static Semaphore semaphore = new Semaphore(1);
    private static VendorExtensionClient sVendorExtensionClient;
    private final CarVendorExtensionManager.CarVendorExtensionCallback mHardwareCallback =
            new CarVendorExtensionManager.CarVendorExtensionCallback() {
                @Override
                public void onChangeEvent(final CarPropertyValue val) {
                    Log.d(TAG, val.toString());
                }

                @Override
                public void onErrorEvent(final int propertyId, final int zone) {
                    Log.d(TAG, propertyId + " : " + zone);
                }
            };
    private Context context;
    private Car mCar;
    private CarVendorExtensionManager carVEManager;
    public static final int NO_AREA = 0;


// Init methods
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
                }
            };
    private List<VehiclePropertySupport> supportedFeatures;

    /**
     * Init
     *
     * @param context Get context of the application
     */
    private VendorExtensionClient(Context context) {
        this.context = context;
        CompletableFuture.runAsync(() -> init());
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
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID & feature.area == area).findFirst();
        return carVEManager.getProperty(result.get().type, propID, area);
    }

    /**
     * Get propID value using Global zone
     *
     * @param propID
     * @return
     * @throws NotSupportedException
     * @throws android.car.CarNotConnectedException
     */
    public Object get(int propID)
            throws NotSupportedException, android.car.CarNotConnectedException {
        if (!isSupportedFeature(propID)) {
            throw new NotSupportedException("This feature is not available (propID): " + propID);
        }
        Optional<VehiclePropertySupport> result = supportedFeatures.stream()
                .filter(feature -> feature.vehicleProperty == propID).findFirst();
        return carVEManager.getGlobalProperty(result.get().type, propID);
    }

    /**
     * Get the type information of the PropID
     *
     * @param propID
     * @param area
     * @return
     */
    public VehiclePropertySupport getVehiclePropertySupport(int propID, int area) {
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
        if (!isSupportedFeature(propID)) {
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

    private void init() {
        try {
            semaphore.acquire();
        } catch (InterruptedException e) {
            Log.d(TAG, "Thread interrupted", e);
        }
        if (mCar == null) {
            mCar = Car.createCar(context, mServiceConnectionCallback);
            Log.d(TAG, "onCreate: Car.connect()");
            try {
                mCar.connect();
            } catch (Exception e) {
                Log.d(TAG, "ERROR: Car.connect() has problems: ", e);
            }
        }
        // Add supported features
        supportedFeatures.add(
                new VehiclePropertySupport(Integer.class, VehicleProperty.DAI_SETTING,
                        NO_AREA));
        supportedFeatures.add(
                new VehiclePropertySupport(Boolean.class, VehicleProperty.CURVE_SPEED_ADAPTION_ON,
                        NO_AREA));
        supportedFeatures.add(
                new VehiclePropertySupport(Integer.class, VehicleProperty.CONNECTED_SAFETY_ON,
                        NO_AREA));
    }

    // Feature handling methods

    /**
     * TODO: Maybe hide this? can get all information
     * @param propertyClass
     * @param propId
     * @param area
     * @param <E>
     * @return
     */
    private <E> Boolean isFeatureAvailable(Class<E> propertyClass, int propId, int area) {
        return isFeatureAvailable(new VehiclePropertySupport(propertyClass, propId, area));
    }

    /**
     * Checks if this feature is available
     * @param propId
     * @param <E>
     * @return
     */
    public <E> Boolean isFeatureAvailable(int propId) {
        return isFeatureAvailable(propId, NO_AREA);
    }

    /**
     * Checks if this feature is available
     * @param propId
     * @param area
     * @param <E>
     * @return
     */
    public <E> Boolean isFeatureAvailable(int propId, int area) {
        return isFeatureAvailable(new VehiclePropertySupport(getVehiclePropertySupport(propId, area).type, propId, area));
    }

    /**
     * Checks if this feature is available, Maybe force to use only defined propIds.
     * Right now getProperty can get all the properties even not vendor extension.
     * @param vehiclePropertySupport
     * @param <E>
     * @return
     */
    private <E> Boolean isFeatureAvailable(VehiclePropertySupport vehiclePropertySupport) {
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
     * @param propId
     * @return
     */
    public boolean isSupportedFeature(int propId) {
        return isSupportedFeature(propId, NO_AREA);
    }

    /**
     * Checks if this feature supported by the API
     * @param propId
     * @param areaId
     * @return
     */
    public boolean isSupportedFeature(int propId, int areaId) {
        return supportedFeatures.stream().anyMatch(
                feature -> feature.vehicleProperty == propId & feature.area == areaId);
    }

    /**
     * Checks if this feature supported by the API
     * @return
     */
    public List<Integer> getSupportedFeatures() {
        return new Vector<Integer>(supportedFeatures.stream()
                .filter(this::isFeatureAvailable)
                .map(vehiclePropertySupport -> vehiclePropertySupport.vehicleProperty)
                .collect(Collectors.toList()));
    }

    public List<Integer> getAvailableFeatures() {
        return new Vector<Integer>(supportedFeatures.stream()
                .filter(this::isFeatureAvailable)
                .map(vehiclePropertySupport -> vehiclePropertySupport.vehicleProperty)
                .collect(Collectors.toList()));
    }

    /**
     * TODO: decide either supported features synced with vehicle hal and only allow public access "isSupportedFeature"
     * @return
     */
    public List<Integer> getAvailableFeaturesAndSupported() {
        supportedFeatures.stream().map(vehiclePropertySupport -> vehiclePropertySupport.available =
                isFeatureAvailable(vehiclePropertySupport));
        return getAvailableFeatures();
    }

    public void registerCallback() {
        try {
            carVEManager.registerCallback(mHardwareCallback);
        } catch (android.car.CarNotConnectedException e) {
            Log.e(TAG, "Car not connected in VEC");
        }
    }

    public void registerCallback(CarVendorExtensionManager.CarVendorExtensionCallback mHardwareCallback) {
        try {
            carVEManager.registerCallback(mHardwareCallback);
        } catch (android.car.CarNotConnectedException e) {
            Log.e(TAG, "Car not connected in VEC");
        }
    }

    /**
     * Wrap methods for getting and setting propId
     */
    class VehiclePropertySupport implements Comparable <VehiclePropertySupport>{
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
                    this.vehicleProperty==o.vehicleProperty &&
                        this.area == o.area){
                return 0;
            } else {
                return 1;
            }

        }
    }

    public class NotSupportedException extends Exception {
        public NotSupportedException(String message) {
            super(message);
        }
    }
}