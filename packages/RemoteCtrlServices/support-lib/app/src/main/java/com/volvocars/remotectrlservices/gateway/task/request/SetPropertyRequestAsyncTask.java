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

public class SetPropertyRequestAsyncTask extends BasePropertyRequestAsyncTask {

    public SetPropertyRequestAsyncTask(
            String parentLogTag,
            int requestIdentifier,
            IRemoteCtrlPropertyService remoteCtrlPropertyService,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse,
            RemoteCtrlHalPropertyValue requestedPropValue) {
        super(parentLogTag, requestIdentifier, remoteCtrlPropertyService, remoteCtrlPropertyResponse, requestedPropValue);
    }

    public static SetPropertyRequestAsyncTask create(
            String parentLogTag,
            int requestIdentifier,
            IRemoteCtrlPropertyService remoteCtrlPropertyService,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse,
            RemoteCtrlHalPropertyValue requestedPropValue) {
        return new SetPropertyRequestAsyncTask(parentLogTag, requestIdentifier, remoteCtrlPropertyService,
        remoteCtrlPropertyResponse, requestedPropValue);
    }

    protected Void doInBackground(Void... unused) {
        try {
            RemoteCtrlPropertyValue propValue = CarPropertyUtils.toRemoteCtrlPropertyValue(mRequestedPropValue);
            mRemoteCtrlPropertyService.setProperty(mRequestIdentifier, propValue);
        } catch (RemoteException outerEx) {
            Log.e(getLogTag(), "RemoteException", outerEx);
            try {
                RemoteCtrlHalPropertyValue response = CarPropertyUtils.createRemoteCtrlHalPropertyValueWithErrorStatus(mRequestedPropValue);

                mRemoteCtrlPropertyResponse.sendSetPropertyResp(mRequestIdentifier, response);
            } catch (RemoteException innerEx) {
                Log.e(getLogTag(), "RemoteException: ", innerEx);
            }
        } catch (IllegalArgumentException ex) {
            Log.e(getLogTag(), "IllegalArgumentException: ", ex);
        }

        // Necessary but not used
        return null;
    }
}
