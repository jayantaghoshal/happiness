/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.request;


import android.os.RemoteException;
import android.util.Log;

import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;
import com.volvocars.remotectrlservices.climatectrl.IRemoteClimateService;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

public class GetPropertyRequestAsyncTask extends BasePropertyRequestAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.Gateway.RemoteClimateServiceHandler.GetPropertyAsyncTask";

    protected GetPropertyRequestAsyncTask(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
                                          RemoteCtrlHalPropertyValue requestedPropValue) {
        super(requestIdentifier, remoteClimateService, nativeRemoteClimateCtrl, requestedPropValue);
    }

    public static GetPropertyRequestAsyncTask create(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
                                                     RemoteCtrlHalPropertyValue requestedPropValue) {
        return new GetPropertyRequestAsyncTask(requestIdentifier, remoteClimateService,
                nativeRemoteClimateCtrl, requestedPropValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            com.volvocars.remotectrlservices.climatectrl.RemoteCtrlPropertyValue propValue;
            propValue = CarPropertyUtils.toRemoteCtrlPropertyValue(mRequestedPropValue);
            Log.v(TAG, "RemoteCtrlHalPropValue: " + mRequestedPropValue);
            Log.v(TAG, "RemoteCtrlPropValue: " + propValue);
            mRemoteClimateService.getProperty(mRequestIdentifier, propValue);
        } catch (RemoteException outerEx) {
            Log.e(TAG, "RemoteException: ", outerEx);
            try {
                RemoteCtrlHalPropertyValue response = CarPropertyUtils.createRemoteCtrlHalPropertyValueWithErrorStatus(mRequestedPropValue);

                mNativeRemoteClimateCtrl.sendGetPropertyResp(
                        (short) mRequestIdentifier, mRequestedPropValue);
            } catch (RemoteException innerEx) {
                Log.e(TAG, "RemoteException", innerEx);
            }
        } catch (IllegalArgumentException ex) {
            Log.e(TAG, "IllegalArgumentException: ", ex);
        }

        // Necessary but not used
        return null;
    }
}
