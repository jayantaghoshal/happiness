/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway.task.response;

import android.util.Log;

import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

public class GetPropertyResponseAsyncTask extends BasePropertyResponseAsyncTask {

    protected GetPropertyResponseAsyncTask(String parentLogTag, int requestIdentifier,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService,
                                           RemoteCtrlPropertyValue propValue) {
        super(parentLogTag, requestIdentifier, remoteCtrlPropertyResponseService, propValue);
    }

    public static GetPropertyResponseAsyncTask create(String parentLogTag, int requestIdentifier,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse, RemoteCtrlPropertyValue propValue) {
        return new GetPropertyResponseAsyncTask(parentLogTag,
                requestIdentifier, remoteCtrlPropertyResponse, propValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            RemoteCtrlHalPropertyValue propValue = CarPropertyUtils.toRemoteCtrlHalPropertyValue(mPropValue);

            mRemoteCtrlPropertyResponse.sendGetPropertyResp(mRequestIdentifier, propValue);
        } catch (IllegalArgumentException ex) {
            Log.e(getLogTag(), "IllegalArgumentException: ", ex);
        } catch (Exception ex) {
            Log.v(getLogTag(), "RemoteException: ", ex);
        }

        // Necessary but not used
        return null;
    }
}

