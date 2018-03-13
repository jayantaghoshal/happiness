/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.test.lib;


import android.car.hardware.CarPropertyValue;
import android.car.hardware.CarVendorExtensionManager;
import android.content.Context;
import android.hardware.automotive.vehicle.V2_0.VehicleArea;
import android.os.Looper;
import android.support.car.Car;
import android.support.car.CarConnectionCallback;
import android.support.car.CarNotConnectedException;
import android.util.Log;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;


public class ModuleVendorExtension {

    private static final String TAG = ModuleVendorExtension.class.getSimpleName();
    static Semaphore semaphore = new Semaphore(1);
    private static Context mContext;
    /**
     * Example for creating callbacks
     * It's not used for on this example app.
     */
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
    private Car mCar;
    private CarVendorExtensionManager carVEManager;
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
                    Log.d(TAG, "onConnected: notifyAll()");
                    semaphore.release();
                }

                @Override
                public void onDisconnected(android.support.car.Car car) {
                    Log.d(TAG, "onDisconnected()");
                }
            };
    public ModuleVendorExtension(Context context) {
        mContext = context;
        CompletableFuture.runAsync(() -> init());
    }

    // todo: Create an new Exception with good name
    public synchronized CarVendorExtensionManager getCarVEManager() throws Exception {
        //  check if it is running on different thread
        if (Looper.myLooper() == Looper.getMainLooper()) {
            Log.e(TAG, "should run in different thread!");
            throw new Exception("Run on different thread! you are using the main thread");
        }
        if (semaphore.tryAcquire(10, TimeUnit.SECONDS)) {
            semaphore.release();
        } else {
            throw new Exception("No Car.connect()");
        }
        return carVEManager;
    }

    private void init() {
        try {
            semaphore.acquire();
        } catch (InterruptedException e) {
            Log.d(TAG, "Thread interrupted", e);
        }
        if (mCar == null) {
            mCar = Car.createCar(mContext, mServiceConnectionCallback);
            Log.d(TAG, "onCreate: Car.connect()");
            try {
                mCar.connect();
            } catch (Exception e) {
                Log.d(TAG, "ERROR: Car.connect() has problems: ", e);
            }
        }
    }

    public <E> Boolean isFeatureAvailable(Class<E> propertyClass, int propId, int area) {
        try {
            carVEManager.getProperty(propertyClass, propId, area);
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    public void registerCallback() {
        try {
            carVEManager.registerCallback(mHardwareCallback);
        } catch (android.car.CarNotConnectedException e) {
            Log.e(TAG, "Car not connected in VEC");
        }
    }
}