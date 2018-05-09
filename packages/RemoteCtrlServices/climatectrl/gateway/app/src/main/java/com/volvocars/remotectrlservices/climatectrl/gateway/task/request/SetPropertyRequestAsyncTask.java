/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.request;

import android.os.RemoteException;
import android.util.Log;

import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;
import com.volvocars.remotectrlservices.climatectrl.IRemoteClimateService;
import com.volvocars.remotectrlservices.climatectrl.RemoteCtrlPropertyValue;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

public class SetPropertyRequestAsyncTask extends BasePropertyRequestAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.Gateway.RemoteClimateServiceHandler.SetPropertyAsyncTask";

    public SetPropertyRequestAsyncTask(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
            RemoteCtrlHalPropertyValue requestedPropValue) {
        super(requestIdentifier, remoteClimateService, nativeRemoteClimateCtrl, requestedPropValue);
    }

    public static SetPropertyRequestAsyncTask create(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
                                                     RemoteCtrlHalPropertyValue requestedPropValue) {
        return new SetPropertyRequestAsyncTask(requestIdentifier, remoteClimateService,
                nativeRemoteClimateCtrl, requestedPropValue);
    }

    protected Void doInBackground(Void... unused) {
        try {
            RemoteCtrlPropertyValue propValue = CarPropertyUtils.toRemoteCtrlPropertyValue(mRequestedPropValue);

            Log.v(TAG, "RemoteCtrlPropValue: " + mRequestedPropValue);

            mRemoteClimateService.setProperty(mRequestIdentifier, propValue);
        } catch (RemoteException outerEx) {
            Log.e(TAG, "RemoteException", outerEx);
            try {
                RemoteCtrlHalPropertyValue response = CarPropertyUtils.createRemoteCtrlHalPropertyValueWithErrorStatus(mRequestedPropValue);

                mNativeRemoteClimateCtrl.sendSetPropertyResp(
                        (short) mRequestIdentifier, response);
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
