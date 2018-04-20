/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.request;

import android.car.hardware.CarPropertyValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.os.AsyncTask;
import android.os.RemoteException;
import android.util.Log;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrlservices.climatectrl.*;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

public class GetPropertyRequestAsyncTask extends BasePropertyRequestAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.RemoteClimateServiceHandler.GetPropertyAsyncTask";

    protected GetPropertyRequestAsyncTask(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
            VehiclePropValue requestedPropValue) {
        super(requestIdentifier, remoteClimateService, nativeRemoteClimateCtrl, requestedPropValue);
    }

    public static GetPropertyRequestAsyncTask create(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
            VehiclePropValue requestedPropValue) {
        return new GetPropertyRequestAsyncTask(requestIdentifier, remoteClimateService,
                nativeRemoteClimateCtrl, requestedPropValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            CarPropertyValue carPropValue =
                    CarPropertyUtils.toCarPropertyValue(mRequestedPropValue);

            mRemoteClimateService.getProperty(mRequestIdentifier, carPropValue);
        } catch (RemoteException outerEx) {
            Log.e(TAG, "RemoteException;", outerEx);
            try {
                VehiclePropValue errorValue =
                        CarPropertyUtils.createVehiclePropertyValueWithErrorStatus(
                                mRequestedPropValue);
                mNativeRemoteClimateCtrl.sendGetPropertyResp(
                        (short) mRequestIdentifier, errorValue);
            } catch (RemoteException innerEx) {
                Log.e(TAG, "RemoteException", innerEx);
            }
        }

        // Necessary but not used
        return null;
    }
}
