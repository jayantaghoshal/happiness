/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.powermodingdialogs;

import android.hardware.automotive.vehicle.V2_0.IVehicleCallback;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;

import java.util.ArrayList;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;
import vendor.volvocars.hardware.vehiclehal.V1_0.PowerModeEvent;
import vendor.volvocars.hardware.vehiclehal.V1_0.PowerModeUserFeedback;

/**
 * Callback class used when subscribing to property events in VehicleHal and to trigger
 * system dialogs and retrieve user feedback form the dialogs.
 * */
public class DialogHandler extends IVehicleCallback.Stub {
    private int mCurrentDialog = PowerModeEvent.None;

    @Override
    public void onPropertyEvent(ArrayList<VehiclePropValue> propValues) {
        for (VehiclePropValue propValue : propValues) {
            if(propValue.prop == VehicleProperty.POWERMODE_USER_INTERACTION) {
                int value = propValue.value.int32Values.get(0);
                // pick out bit0-15
                int dialogRequest = value&0xFFFF; // this is a value from the PowerModeEvent enum
                // pick out bit16-31
                int timeCountdown = (value>>16)&0xFFFF;

                handleDialogRequest(dialogRequest, timeCountdown);
            }
        }
    }


    /* TODO Based on the dialogRequest use the SystemDialog-lib/API to present
     * the corresponding popup(system dialog).
     * Also be prepared to receive user feedback from the user and write this to
     * the POWERMODE_USER_INTERACTION property using the
     * values from the PowerModeUserFeedback enum
     */
    private void handleDialogRequest(int dialogRequest, int timeCountdown) {
        if ( mCurrentDialog!=PowerModeEvent.None ) {
            // If we get a new request (including the None-request) having a current dialog
            // already active we remove the current dialog
            if ( mCurrentDialog!=dialogRequest ) {
                removeDialog();
            } else {
                // We got a time-update to the current dialog
                updateDialog(timeCountdown);
            }
        } else {
            createDialog(dialogRequest, timeCountdown);
        }
    }

    // Create a new dialog (unless it is None but it should never be)
    private void createDialog(int dialogRequest, int timeCountdown) {
        if (dialogRequest!=PowerModeEvent.None) {
            // TODO, create a new current dialog
        }
    }

    // Remove the currently visible dialog
    private void removeDialog() {
        // TODO, remove current dialog

        mCurrentDialog = PowerModeEvent.None;
    }

    // Update the time-countdown in the currently visible dialog
    private void updateDialog(int timeCountdown) {
        // TODO, update current dialog with new time value
    }

    @Override
    public void onPropertySet(VehiclePropValue propValue) {}//Not used

    @Override
    public void onPropertySetError(int errorCode, int propId, int areaId) {}// Not used
}
