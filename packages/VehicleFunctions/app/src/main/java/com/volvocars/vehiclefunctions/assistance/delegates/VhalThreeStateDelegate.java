/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.delegates;

import android.car.CarNotConnectedException;
import android.car.hardware.CarPropertyValue;
import android.util.Log;

import com.volvocars.vehiclefunctions.assistance.functions.ThreeStateFunction;
import com.volvocars.vendorextension.VendorExtensionCallBack;
import com.volvocars.vendorextension.VendorExtensionClient;

import java.util.concurrent.CompletableFuture;

/*
 * VhalThreeStateDelegate used for VHAL properties
 * Function Delegates encapsulate the logic to communicate with a vehicle function
 * API and then propagate changes back to the UI by calling setUiState().
 * Dependencies should be injected.
 *
 * This class can be removed when real functionality is implemented.
 */
public class VhalThreeStateDelegate extends ThreeStateFunction.Delegate {

    public static final String TAG = VhalThreeStateDelegate.class.getSimpleName();
    private final VendorExtensionClient mVendorExtensionClient;
    private final int mVehicleProperty;

    public VhalThreeStateDelegate(VendorExtensionClient vendorExtensionClient, int vehicleProperty) {
        mVendorExtensionClient = vendorExtensionClient;
        mVehicleProperty = vehicleProperty;
    }

    @Override
    protected void onUserChangedState(int state) {
        // Use API to communicate with vehicle and then call setUiState() when ready
        Log.d(TAG, "OnUserChangedState = " + String.valueOf(state));
        setUiEnabled(false);
        // Check and correct, state of the HMI
        try {
            if (state == (int) mVendorExtensionClient.get(mVehicleProperty)){
                setUiState(state);
                setUiEnabled(true);
                return;
            }
        } catch (VendorExtensionClient.NotSupportedException e) {
            Log.e(TAG, "NotSupportedException: " + mVehicleProperty ,e);
            return;
        } catch (CarNotConnectedException e) {
            Log.e(TAG, "CarNotConnectedException", e);
            return;
        }


        try {
            mVendorExtensionClient.set(mVehicleProperty, state);
        } catch (VendorExtensionClient.NotSupportedException e) {
            Log.e(TAG,"Setting is not supported", e);
        } catch (CarNotConnectedException e) {
            Log.w(TAG, "Car is not connected.", e);
            CompletableFuture.runAsync(() -> {
                Log.d(TAG, "Car not connected. Trying to reconnect.");
                mVendorExtensionClient.reconnect();
            });

        }
    }

    @Override
    protected void onSetInitialState() {
        setUiState(0);
        setUiEnabled(false);
        CompletableFuture.runAsync(() -> {
            // Use API to communicate with vehicle and then call setUiState() when ready
            if (mVendorExtensionClient.isFeatureAvailable(mVehicleProperty)){
                Log.d(TAG, "Vehicle property " + String.valueOf(mVehicleProperty) + " is available!");
                mVendorExtensionClient.registerCallback(new VendorExtensionCallBack(mVehicleProperty, 0) {
                    @Override
                    public void onChangeEvent(CarPropertyValue value) {
                        Log.d(TAG, "onChangeEvent: Value received: " + value);
                        setUiState((Integer)value.getValue());
                        setUiEnabled(true);
                    }

                    @Override
                    public void onErrorEvent(int propertyId, int zone) {
                        Log.d(TAG, "onErrorEvent: propertyId = " + propertyId + ", zone = " + zone);
                    }
                });
                try {
                    setUiState((int)mVendorExtensionClient.get(mVehicleProperty));
                } catch (VendorExtensionClient.NotSupportedException e) {
                    Log.e(TAG, "NotSupported propID: " + mVehicleProperty);
                } catch (CarNotConnectedException e) {
                    mVendorExtensionClient.reconnect();
                }
                setUiEnabled(true);
            }
        });
    }
}
