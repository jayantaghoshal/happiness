/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway.task.response;

import android.car.hardware.CarPropertyValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.os.AsyncTask;
import android.os.RemoteException;
import android.util.Log;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

public class SetPropertyResponseAsyncTask extends BasePropertyResponseAsyncTask {

    protected SetPropertyResponseAsyncTask(String parentLogTag, int requestIdentifier,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse, RemoteCtrlPropertyValue propValue) {
        super(parentLogTag, requestIdentifier, remoteCtrlPropertyResponse, propValue);
    }

    public static SetPropertyResponseAsyncTask create(String parentLogTag, int requestIdentifier,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse,
                                                      RemoteCtrlPropertyValue requestedPropValue) {
        return new SetPropertyResponseAsyncTask(parentLogTag,
                requestIdentifier, remoteCtrlPropertyResponse, requestedPropValue);
    }

    protected Void doInBackground(Void... unused) {
        try {
            RemoteCtrlHalPropertyValue propValue = CarPropertyUtils.toRemoteCtrlHalPropertyValue(mPropValue);

            mRemoteCtrlPropertyResponse.sendSetPropertyResp(mRequestIdentifier, propValue);
        } catch (IllegalArgumentException ex) {
            Log.e(getLogTag(), "IllegalArgumentException: ", ex);
        } catch (RemoteException ex) {
            Log.v(getLogTag(), "RemoteException: " + ex);
        }

        // Necessary but not used
        return null;
    }
}
