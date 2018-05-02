/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.delegates;

import android.car.CarNotConnectedException;
import android.util.Log;

import com.volvocars.vehiclefunctions.assistance.functions.VHalDelegate;
import com.volvocars.vehiclefunctions.assistance.functions.FunctionViewHolder;
import com.volvocars.vendorextension.VendorExtensionClient;

import java.util.concurrent.CompletableFuture;

/*
 * VhalThreeStateDelegate used for VHAL properties
 * Function Delegates encapsulate the logic to communicate with a vehicle function
 * API and then propagate changes back to the UI by calling setUiValue().
 * Dependencies should be injected.
 */
public class VhalThreeStateDelegate extends VHalDelegate<Integer> {
    public static final String TAG = VhalThreeStateDelegate.class.getSimpleName();

    private static final int MODE_1_DISABLED = 0b00011011;  // 0001 1011 = 16 + 8 + 2 + 1
    private static final int MODE_2_DISABLED = 0b00010111;  // 0001 0111 = 16 + 4 + 2 + 1
    private static final int MODE_3_DISABLED = 0b00001111;  // 0000 1111 =  8 + 4 + 2 + 1

    private static final int STATE_1 = 1;
    private static final int STATE_2 = 2;
    private static final int STATE_3 = 3;

    private boolean mUseDisabledMode = false;

    public VhalThreeStateDelegate(VendorExtensionClient vendorExtensionClient, int vehicleProperty) {
        mVendorExtensionClient = vendorExtensionClient;
        mVehicleProperty = vehicleProperty;
    }

    public VhalThreeStateDelegate(VendorExtensionClient vendorExtensionClient, int vehicleProperty, int vehiclePropertyStatus, boolean useDisabledMode) {
        mVendorExtensionClient = vendorExtensionClient;
        mVehicleProperty = vehicleProperty;
        mUseDisabledMode = useDisabledMode;
    }

    /*
     * Used for property with status
     */
    public VhalThreeStateDelegate(VendorExtensionClient vendorExtensionClient, int vehicleProperty, int vehiclePropertyStatus) {
        this(vendorExtensionClient, vehicleProperty);
        this.vehiclePropertyStatus = vehiclePropertyStatus;
    }

    @Override
    protected void onUserChangedValue(Integer state) {
        // Use API to communicate with vehicle and then call setUiValue() when ready
        Log.d(TAG, "OnUserChangedState = " + String.valueOf(state));
        setUiFunctionState(FunctionViewHolder.FunctionState.DISABLED);
        // Check and correct, state of the HMI
        try {
            if (state == (int) mVendorExtensionClient.get(mVehicleProperty)) {
                setUiValue(state);
                setUiFunctionState(FunctionViewHolder.FunctionState.ENABLED);
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
            mVendorExtensionClient.set(mVehicleProperty, state);
        } catch (VendorExtensionClient.NotSupportedException e) {
            Log.e(TAG, "Setting is not supported", e);
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
        setUiValue(0);
        setUiFunctionState(FunctionViewHolder.FunctionState.DISABLED);
        CompletableFuture.runAsync(() -> {
            // Use API to communicate with vehicle and then call setUiValue() when ready
            if (mVendorExtensionClient.isSupportedFeature(mVehicleProperty)) {
                Log.d(TAG, "Vehicle property " + String.valueOf(mVehicleProperty) + " is available!");
                // Register Property value callback
                registerPropCallback();

                // Chech if mode is supported
                if (mUseDisabledMode) {
                    setDisabledMode();
                }

                // Since the feature is available, enable it and register PA status handling
                setUiFunctionState(FunctionViewHolder.FunctionState.ENABLED);
                registerStatusCallback();

                try {
                    setUiValue((int) mVendorExtensionClient.get(mVehicleProperty));
                } catch (VendorExtensionClient.NotSupportedException e) {
                    Log.e(TAG, "NotSupported propID: " + mVehicleProperty);
                } catch (CarNotConnectedException e) {
                    Log.e(TAG, "CarNotConnectedException, reconnecting", e);
                    mVendorExtensionClient.reconnect();
                }
            }
        });
    }

    /**
     * Sets the disabled mode for the ThreeStateFunction.
     * Valid disabled modes (1, 2, 3).
     * Default: DisabledMode = 0. In this case, no modes are disabled.
     * TODO: change from using maxValue to configFlags.
     */
    private void setDisabledMode() {
        try {
            int maxValue = (int) mVendorExtensionClient.getCarPropertyConfig(mVehicleProperty).get().getMaxValue();
            if (maxValue == MODE_1_DISABLED) {
                setDisabledMode(STATE_1);
            } else if (maxValue == MODE_2_DISABLED) {
                setDisabledMode(STATE_2);
            } else if (maxValue == MODE_3_DISABLED) {
                setDisabledMode(STATE_3);
            }
        } catch (VendorExtensionClient.NotSupportedException e) {
            Log.e(TAG, "Property not supported: " + e.getMessage());
        } catch (CarNotConnectedException e) {
            Log.e(TAG, "Car is not connected: " + e.getMessage());
        } catch (NullPointerException e){
            Log.e(TAG, "Couldn't find the property config " + e.getMessage());
        }
    }
}
