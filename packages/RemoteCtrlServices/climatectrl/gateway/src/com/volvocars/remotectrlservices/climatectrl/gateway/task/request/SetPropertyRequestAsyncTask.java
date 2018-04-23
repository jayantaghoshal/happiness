/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.request;

import android.os.AsyncTask;
import android.os.RemoteException;
import android.car.hardware.CarPropertyValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.util.Log;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrlservices.climatectrl.*;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

public class SetPropertyRequestAsyncTask extends BasePropertyRequestAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.Gateway.RemoteClimateServiceHandler.SetPropertyAsyncTask";

    public SetPropertyRequestAsyncTask(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
            VehiclePropValue requestedPropValue) {
        super(requestIdentifier, remoteClimateService, nativeRemoteClimateCtrl, requestedPropValue);
    }

    public static SetPropertyRequestAsyncTask create(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
            VehiclePropValue requestedPropValue) {
        return new SetPropertyRequestAsyncTask(requestIdentifier, remoteClimateService,
                nativeRemoteClimateCtrl, requestedPropValue);
    }

    protected Void doInBackground(Void... unused) {
        try {
            CarPropertyValue carPropValue =
                    CarPropertyUtils.toCarPropertyValue(mRequestedPropValue);

            Log.v(TAG, "carPropValue: " + carPropValue);

            mRemoteClimateService.setProperty(mRequestIdentifier, carPropValue);
        } catch (RemoteException outerEx) {
            Log.e(TAG, "RemoteException", outerEx);
            try {
                VehiclePropValue errorValue =
                        CarPropertyUtils.createVehiclePropertyValueWithErrorStatus(
                                mRequestedPropValue);
                mNativeRemoteClimateCtrl.sendSetPropertyResp(
                        (short) mRequestIdentifier, errorValue);
            } catch (RemoteException innerEx) {
                Log.e(TAG, "RemoteException: ", innerEx);
            }
        } catch (IllegalArgumentException ex) {
            Log.e(TAG, "IllegalArgumentException: ", ex);
        }

        // Necessary but not used
        return null;
    }
}
