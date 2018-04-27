/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.delegates;

import android.car.CarNotConnectedException;
import android.util.Log;

import com.volvocars.vehiclefunctions.assistance.functions.VHalDelegate;
import com.volvocars.vendorextension.VendorExtensionClient;

import java.util.concurrent.CompletableFuture;

/*
 * IntegerFunction used for VHAL properties
 * Function Delegates encapsulate the logic to communicate with a vehicle function
 * API and then propagate changes back to the UI by calling setUiValue().
 * Dependencies should be injected.
 *
 */
public class VhalIntegerDelegate extends VHalDelegate<Integer> {

    public static final String TAG = VhalIntegerDelegate.class.getSimpleName();


    public VhalIntegerDelegate(VendorExtensionClient vendorExtensionClient, int vehicleProperty) {
        mVendorExtensionClient = vendorExtensionClient;
        mVehicleProperty = vehicleProperty;
    }

    /*
     * Used for property with status
     */
    public VhalIntegerDelegate(VendorExtensionClient vendorExtensionClient, int vehicleProperty, int vehiclePropertyStatus) {
        this(vendorExtensionClient, vehicleProperty);
        this.vehiclePropertyStatus = vehiclePropertyStatus;
    }

    @Override
    protected void onUserChangedValue(Integer value) {
        Log.d(TAG, "onUserChangedValue = " + value);
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
}
