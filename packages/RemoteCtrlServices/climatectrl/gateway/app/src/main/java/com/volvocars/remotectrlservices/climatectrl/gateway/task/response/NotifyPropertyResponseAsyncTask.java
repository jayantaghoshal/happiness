/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.response;

import android.util.Log;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;


import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;
import com.volvocars.remotectrlservices.climatectrl.RemoteCtrlPropertyValue;

public class NotifyPropertyResponseAsyncTask extends BasePropertyResponseAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.Gateway.RemoteClimateResponseServiceHandler.NotifyPropertyResponseAsyncTask";

    protected NotifyPropertyResponseAsyncTask(
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService,
            RemoteCtrlPropertyValue propValue) {
        super(0 /* dummy */, remoteCtrlPropertyResponseService, propValue);
    }

    public static NotifyPropertyResponseAsyncTask create(
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl, RemoteCtrlPropertyValue propValue) {
        return new NotifyPropertyResponseAsyncTask(nativeRemoteClimateCtrl, propValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            RemoteCtrlHalPropertyValue propValue;
            propValue = CarPropertyUtils.toRemoteCtrlHalPropertyValue(mPropValue);

            mNativeRemoteClimateCtrl.notifyPropertyChanged(propValue);
        } catch (IllegalArgumentException ex) {
            Log.e(TAG, "IllegalArgumentException: ", ex);
        } catch (Exception ex) {
            Log.v(TAG, "RemoteException: " + ex);
        }

        // Necessary but not used
        return null;
    }
}
