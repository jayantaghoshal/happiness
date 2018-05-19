/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway.task.response;

import android.util.Log;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

public class NotifyPropertyResponseAsyncTask extends BasePropertyResponseAsyncTask {

    protected NotifyPropertyResponseAsyncTask(String parentLogTag,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService,
            RemoteCtrlPropertyValue propValue) {
        super(parentLogTag, 0 /* dummy */, remoteCtrlPropertyResponseService, propValue);
    }

    public static NotifyPropertyResponseAsyncTask create(String parentLogTag,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse, RemoteCtrlPropertyValue propValue) {
        return new NotifyPropertyResponseAsyncTask(parentLogTag, remoteCtrlPropertyResponse, propValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            RemoteCtrlHalPropertyValue propValue = CarPropertyUtils.toRemoteCtrlHalPropertyValue(mPropValue);

            mRemoteCtrlPropertyResponse.notifyPropertyChanged(propValue);
        } catch (IllegalArgumentException ex) {
            Log.e(getLogTag(), "IllegalArgumentException: ", ex);
        } catch (Exception ex) {
            Log.v(getLogTag(), "RemoteException: " + ex);
        }

        // Necessary but not used
        return null;
    }
}
