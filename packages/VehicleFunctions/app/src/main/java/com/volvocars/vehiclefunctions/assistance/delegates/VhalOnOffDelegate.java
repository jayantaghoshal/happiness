/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.delegates;

import android.car.CarNotConnectedException;
import android.car.hardware.CarPropertyValue;
import android.util.Log;

import com.volvocars.vehiclefunctions.assistance.functions.OnOffFunction;
import com.volvocars.vendorextension.VendorExtensionCallBack;
import com.volvocars.vendorextension.VendorExtensionClient;

import java.util.concurrent.CompletableFuture;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

/*
 * VhalOnOffDelegate used for VHAL properties
 * Function Delegates encapsulate the logic to communicate with a vehicle function
 * API and then propagate changes back to the UI by calling setUiState().
 * Dependencies should be injected.
 *
 */
public class VhalOnOffDelegate extends OnOffFunction.Delegate {

    public static final String TAG = VhalOnOffDelegate.class.getSimpleName();
    private final VendorExtensionClient mVendorExtensionClient;
    private final int mVehicleProperty;

    public VhalOnOffDelegate(VendorExtensionClient vendorExtensionClient, int vehicleProperty) {
        mVendorExtensionClient = vendorExtensionClient;
        mVehicleProperty = vehicleProperty;
    }

    @Override
    protected void onUserChangedState(boolean value) {
        // Use API to communicate with vehicle and then call setUiState() when ready
        Log.d(TAG, "OnUserChangedState = " + String.valueOf(value) );
        setUiEnabled(false);

        // Check and correct, state of the HMI
        try {
            if (value == (boolean) mVendorExtensionClient.get(mVehicleProperty)){
                setUiState(value);
                setUiEnabled(true);
                return;
            }
        } catch (VendorExtensionClient.NotSupportedException e) {
            Log.e(TAG, "NotSupportedException: " + mVehicleProperty, e);
            return;
        } catch (CarNotConnectedException e) {
            Log.e(TAG, "CarNotConnectedException", e);
            return;
        }

        try {
            mVendorExtensionClient.set(mVehicleProperty, value);
        } catch (VendorExtensionClient.NotSupportedException e) {
            Log.e(TAG,"Setting is not supported", e);
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
        setUiState(false);
        setUiEnabled(false);
        CompletableFuture.runAsync(() -> {
            // Use API to communicate with vehicle and then call setUiState() when ready
            if (mVendorExtensionClient.isFeatureAvailable(mVehicleProperty)){
                Log.d(TAG, "Vehicle property " + String.valueOf(mVehicleProperty) + " is available!");
                mVendorExtensionClient.registerCallback(new VendorExtensionCallBack(mVehicleProperty, 0) {
                    @Override
                    public void onChangeEvent(CarPropertyValue value) {
                        Log.d(TAG, "onChangeEvent: Value received: " + value);
                        setUiState((Boolean) value.getValue());
                        setUiEnabled(true);
                    }

                    @Override
                    public void onErrorEvent(int propertyId, int zone) {
                        Log.d(TAG, "onErrorEvent: propertyId = " + propertyId + ", zone = " + zone);
                    }
                });
                try {
                    setUiState((boolean)mVendorExtensionClient.get(mVehicleProperty));
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
