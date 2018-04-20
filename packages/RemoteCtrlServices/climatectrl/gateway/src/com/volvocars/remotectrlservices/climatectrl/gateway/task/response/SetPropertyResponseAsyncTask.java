/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.response;

import android.car.hardware.CarPropertyValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.os.AsyncTask;
import android.os.RemoteException;
import android.util.Log;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrlservices.climatectrl.*;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

public class SetPropertyResponseAsyncTask extends BasePropertyResponseAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.RemoteClimateResponseServiceHandler.SetPropertyResponseAsyncTask";

    protected SetPropertyResponseAsyncTask(int requestIdentifier,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl, CarPropertyValue propValue) {
        super(requestIdentifier, nativeRemoteClimateCtrl, propValue);
    }

    public static SetPropertyResponseAsyncTask create(int requestIdentifier,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
            CarPropertyValue requestedPropValue) {
        return new SetPropertyResponseAsyncTask(
                requestIdentifier, nativeRemoteClimateCtrl, requestedPropValue);
    }

    protected Void doInBackground(Void... unused) {
        try {
            VehiclePropValue vehiclePropValue = CarPropertyUtils.toVehiclePropValue(mPropValue);

            mNativeRemoteClimateCtrl.sendSetPropertyResp(
                    (short) mRequestIdentifier, vehiclePropValue);
        } catch (RemoteException ex) {
            Log.v(TAG, "RemoteException: " + ex);
        }

        // Necessary but not used
        return null;
    }
}

