/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance.functions;

import android.car.CarNotConnectedException;
import android.car.hardware.CarPropertyValue;
import android.util.Log;

import com.volvocars.vendorextension.VendorExtensionCallBack;
import com.volvocars.vendorextension.VendorExtensionClient;

import java.util.concurrent.CompletableFuture;

import vendor.volvocars.hardware.vehiclehal.V1_0.PAStatus;

public abstract class VHalDelegate<T> {
    String TAG_COMMON = VHalDelegate.class.getSimpleName();

    protected VendorExtensionClient mVendorExtensionClient;
    protected final static int NO_STATUS = -1;  // -1 = No status registered for this propID
    protected int mVehicleProperty;
    protected int vehiclePropertyStatus = NO_STATUS;
    protected StateChangedListener mListener;

    protected abstract void onUserChangedValue(T value);

    /**
     * Defualt initial state for the buttons
     */
    protected void onSetInitialState() {
        setUiFunctionState(FunctionViewHolder.FunctionState.DISABLED);
        CompletableFuture.runAsync(() -> {
            // Use API to communicate with vehicle and then call setUiState() when ready
            if (mVendorExtensionClient.isSupportedFeature(mVehicleProperty)) {
                Log.d(TAG_COMMON, "Vehicle property " + String.valueOf(mVehicleProperty) + " is available!");

                // Register Property and Status callback
                registerPropCallback();
                registerStatusCallback();

                // Handle default values
                try {
                    if (vehiclePropertyStatus == NO_STATUS) { // no status handling
                        setUiFunctionState(FunctionViewHolder.FunctionState.ENABLED);
                    } else {
                        setUiFunctionState(convertPAStatusToHMIStatus((Integer) mVendorExtensionClient.get(vehiclePropertyStatus)));
                    }
                    setUiValue((T) mVendorExtensionClient.get(mVehicleProperty));
                } catch (VendorExtensionClient.NotSupportedException e) {
                    Log.e(TAG_COMMON, "NotSupported propID: " + mVehicleProperty);
                } catch (CarNotConnectedException e) {
                    Log.e(TAG_COMMON, "CarNotConnectedException, reconnecting", e);
                    mVendorExtensionClient.reconnect();
                }
            }
        });
    }

    protected void setUiValue(T value) {
        mListener.valueChanged(value);
    }

    protected void setUiFunctionState(FunctionViewHolder.FunctionState enabled) {
        mListener.buttonStateChanged(enabled);
    }

    protected void setListener(StateChangedListener listener) {
        this.mListener = listener;
    }

    protected void setDisabledMode(int disabledMode) {
        mListener.disabledModeChanged(disabledMode);
    }

    protected interface StateChangedListener<T> {
        void valueChanged(T value);

        void buttonStateChanged(FunctionViewHolder.FunctionState buttonState);

        default void disabledModeChanged(int disabledMode) {
        }
    }

    /**
     * Register property value callback
     */
    protected void registerPropCallback() {
        mVendorExtensionClient.registerCallback(new VendorExtensionCallBack(mVehicleProperty, 0) {
            @Override
            public void onChangeEvent(CarPropertyValue value) {
                Log.d(TAG_COMMON, "onChangeEvent: Value received: " + value + " Property ID :" + mVehicleProperty);
                setUiValue((T) value.getValue());
                if (vehiclePropertyStatus == NO_STATUS) { // no status handling
                    setUiFunctionState(FunctionViewHolder.FunctionState.ENABLED);
                }
            }

            @Override
            public void onErrorEvent(int propertyId, int zone) {
                Log.d(TAG_COMMON, "onErrorEvent: propertyId = " + propertyId + ", zone = " + zone);
            }
        });
    }

    /**
     * Register status of the property value
     */
    protected void registerStatusCallback() {
        // Register for PA status
        mVendorExtensionClient.registerCallback(new VendorExtensionCallBack(vehiclePropertyStatus, 0) {
            @Override
            public void onChangeEvent(CarPropertyValue value) {
                Log.d(TAG_COMMON, "onChangeEvent: vehiclePropertyStatus: " + value);
                setUiFunctionState(convertPAStatusToHMIStatus((Integer) value.getValue()));
            }

            @Override
            public void onErrorEvent(int propertyId, int zone) {
                Log.d(TAG_COMMON, "onErrorEvent: propertyId = " + propertyId + ", zone = " + zone);
            }
        });
    }

    /**
     * Logic translation between PAStatus to HMI state
     *
     * @param paStatus
     * @return HMI state
     */
    protected FunctionViewHolder.FunctionState convertPAStatusToHMIStatus(int paStatus) {
        switch (paStatus) {
            case PAStatus.Active:
                return FunctionViewHolder.FunctionState.ENABLED;
            case PAStatus.Disabled:
                return FunctionViewHolder.FunctionState.DISABLED;
            case PAStatus.NotAvailable:
                return FunctionViewHolder.FunctionState.INVISIBLE;
            case PAStatus.SystemError:
                return FunctionViewHolder.FunctionState.DISABLED;
            default:
                return FunctionViewHolder.FunctionState.DISABLED;
        }
    }
}
