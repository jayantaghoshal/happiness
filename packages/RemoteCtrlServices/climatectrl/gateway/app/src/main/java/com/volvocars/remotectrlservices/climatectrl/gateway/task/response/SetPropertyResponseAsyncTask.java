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

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;
import com.volvocars.remotectrlservices.climatectrl.RemoteCtrlPropertyValue;

public class SetPropertyResponseAsyncTask extends BasePropertyResponseAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.Gateway.RemoteClimateResponseServiceHandler.SetPropertyResponseAsyncTask";

    protected SetPropertyResponseAsyncTask(int requestIdentifier,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl, RemoteCtrlPropertyValue propValue) {
        super(requestIdentifier, nativeRemoteClimateCtrl, propValue);
    }

    public static SetPropertyResponseAsyncTask create(int requestIdentifier,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
                                                      RemoteCtrlPropertyValue requestedPropValue) {
        return new SetPropertyResponseAsyncTask(
                requestIdentifier, nativeRemoteClimateCtrl, requestedPropValue);
    }

    protected Void doInBackground(Void... unused) {
        try {
            RemoteCtrlHalPropertyValue propValue;
            propValue = CarPropertyUtils.toRemoteCtrlHalPropertyValue(mPropValue);

            mNativeRemoteClimateCtrl.sendSetPropertyResp(
                    mRequestIdentifier, propValue);
        } catch (IllegalArgumentException ex) {
            Log.e(TAG, "IllegalArgumentException: ", ex);
        } catch (RemoteException ex) {
            Log.v(TAG, "RemoteException: " + ex);
        }

        // Necessary but not used
        return null;
    }
}
