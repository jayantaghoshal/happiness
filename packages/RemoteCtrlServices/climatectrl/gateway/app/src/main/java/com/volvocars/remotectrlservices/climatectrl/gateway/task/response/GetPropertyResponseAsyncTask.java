/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.response;

import android.util.Log;

import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;
import com.volvocars.remotectrlservices.climatectrl.RemoteCtrlPropertyValue;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

public class GetPropertyResponseAsyncTask extends BasePropertyResponseAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.Gateway.RemoteClimateResponseServiceHandler.GetPropertyResponseAsyncTask";

    protected GetPropertyResponseAsyncTask(int requestIdentifier,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService,
                                           RemoteCtrlPropertyValue propValue) {
        super(requestIdentifier, remoteCtrlPropertyResponseService, propValue);
    }

    public static GetPropertyResponseAsyncTask create(int requestIdentifier,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl, RemoteCtrlPropertyValue propValue) {
        return new GetPropertyResponseAsyncTask(
                requestIdentifier, nativeRemoteClimateCtrl, propValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            Log.v(TAG, "GetPropertyResponseAsyncTask");

            RemoteCtrlHalPropertyValue propValue;
            propValue = CarPropertyUtils.toRemoteCtrlHalPropertyValue(mPropValue);

            Log.v(TAG, "RemoteCtrlPropValue: " + propValue);

            mNativeRemoteClimateCtrl.sendGetPropertyResp(
                    mRequestIdentifier, propValue);
        } catch (IllegalArgumentException ex) {
            Log.e(TAG, "IllegalArgumentException: ", ex);
        } catch (Exception ex) {
            Log.v(TAG, "RemoteException: ", ex);
        }

        // Necessary but not used
        return null;
    }
}

