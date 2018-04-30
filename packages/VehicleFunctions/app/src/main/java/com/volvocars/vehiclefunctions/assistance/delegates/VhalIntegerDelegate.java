/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.delegates;

import android.car.CarNotConnectedException;
import android.car.hardware.CarPropertyValue;
import android.util.Log;

import com.volvocars.vehiclefunctions.assistance.functions.IntegerFunction;
import com.volvocars.vendorextension.VendorExtensionCallBack;
import com.volvocars.vendorextension.VendorExtensionClient;

import java.util.concurrent.CompletableFuture;


import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

/*
 * IntegerFunction used for VHAL properties
 * Function Delegates encapsulate the logic to communicate with a vehicle function
 * API and then propagate changes back to the UI by calling setUiValue().
 * Dependencies should be injected.
 *
 */
public class VhalIntegerDelegate extends IntegerFunction.Delegate {

    public static final String TAG = VhalIntegerDelegate.class.getSimpleName();
    private final VendorExtensionClient mVendorExtensionClient;
    private final int mVehicleProperty;
    private int vhalValue;

    public VhalIntegerDelegate(VendorExtensionClient vendorExtensionClient, int vehicleProperty) {
        mVendorExtensionClient = vendorExtensionClient;
        mVehicleProperty = vehicleProperty;
    }

    @Override
    protected void onUserChangedValue(int value) {
        Log.d(TAG, "onUserChangedValue = " + value);
        setUiEnabled(false);
        try {
            mVendorExtensionClient.set(mVehicleProperty, value);
        } catch (VendorExtensionClient.NotSupportedException e) {
            Log.e(TAG, "Setting is not supported", e);
        } catch (CarNotConnectedException e) {
            Log.e(TAG, "Car is not connected", e);
            CompletableFuture.runAsync(() -> {
                Log.d(TAG, "Car not connected. Trying to reconnect.");
                mVendorExtensionClient.reconnect();
            });
        }
    }

    @Override
    protected void onSetInitialState() {
        setUiEnabled(false);
        CompletableFuture.runAsync(() -> {
            // Use API to communicate with vehicle and then call setUiState() when ready
            if (mVendorExtensionClient.isFeatureAvailable(mVehicleProperty)){
                Log.d(TAG, "Vehicle property " + String.valueOf(mVehicleProperty) + " is available!");
                mVendorExtensionClient.registerCallback(new VendorExtensionCallBack(mVehicleProperty, 0) {
                    @Override
                    public void onChangeEvent(CarPropertyValue value) {
                        Log.d(TAG, "onChangeEvent: Value received: " + value +" Property ID :" + mVehicleProperty);
                        setUiValue((Integer)value.getValue());
                        setUiEnabled(true);
                    }

                    @Override
                    public void onErrorEvent(int propertyId, int zone) {
                        Log.d(TAG, "onErrorEvent: propertyId = " + propertyId + ", zone = " + zone);
                    }
                });
                try {
                    setUiValue((int)mVendorExtensionClient.get(mVehicleProperty));
                } catch (VendorExtensionClient.NotSupportedException e) {
                    Log.e(TAG, "NotSupported propID: " + mVehicleProperty);
                } catch (CarNotConnectedException e) {
                    Log.e(TAG, "CarNotConnectedException, reconnecting", e);
                    mVendorExtensionClient.reconnect();
                }
                setUiEnabled(true);
            }
        });
    }
}
