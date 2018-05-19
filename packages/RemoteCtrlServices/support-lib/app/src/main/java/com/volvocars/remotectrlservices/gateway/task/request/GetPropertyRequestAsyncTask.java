/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway.task.request;


import android.os.RemoteException;
import android.util.Log;

import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyService;
import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

public class GetPropertyRequestAsyncTask extends BasePropertyRequestAsyncTask {

    protected GetPropertyRequestAsyncTask(String parentLogTag, int requestIdentifier,
                                          IRemoteCtrlPropertyService remoteCtrlPropertyService,
                                          IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse,
                                          RemoteCtrlHalPropertyValue requestedPropValue) {
        super(parentLogTag, requestIdentifier, remoteCtrlPropertyService, remoteCtrlPropertyResponse, requestedPropValue);
    }

    public static GetPropertyRequestAsyncTask create(String parentLogTag, int requestIdentifier,
                                                     IRemoteCtrlPropertyService remoteCtrlPropertyService,
                                                     IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse,
                                                     RemoteCtrlHalPropertyValue requestedPropValue) {
        return new GetPropertyRequestAsyncTask(parentLogTag, requestIdentifier, remoteCtrlPropertyService,
            remoteCtrlPropertyResponse, requestedPropValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            RemoteCtrlPropertyValue propValue = CarPropertyUtils.toRemoteCtrlPropertyValue(mRequestedPropValue);
            mRemoteCtrlPropertyService.getProperty(mRequestIdentifier, propValue);
        } catch (RemoteException outerEx) {
            Log.e(getLogTag(), "RemoteException: ", outerEx);
            try {
                RemoteCtrlHalPropertyValue response = CarPropertyUtils.createRemoteCtrlHalPropertyValueWithErrorStatus(mRequestedPropValue);

                mRemoteCtrlPropertyResponse.sendGetPropertyResp(mRequestIdentifier, response);
            } catch (RemoteException innerEx) {
                Log.e(getLogTag(), "RemoteException", innerEx);
            }
        } catch (IllegalArgumentException ex) {
            Log.e(getLogTag(), "IllegalArgumentException: ", ex);
        }

        // Necessary but not used
        return null;
    }
}
