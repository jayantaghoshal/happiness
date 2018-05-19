/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway;

import android.util.Log;
import android.os.HwBinder;
import android.os.RemoteException;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;


import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyService;
import com.volvocars.remotectrlservices.gateway.task.request.GetPropertyRequestAsyncTask;
import com.volvocars.remotectrlservices.gateway.task.request.SetPropertyRequestAsyncTask;

import java.text.MessageFormat;

/**
 * Communication between HAL -> Gateway -> Service
 */
public class PropertyServiceHandler extends IRemoteCtrlProperty.Stub {
    private String BASE_LOG_TAG;

    private IRemoteCtrlPropertyService mRemoteCtrlPropertyService;
    private IRemoteCtrlPropertyResponse mRemoteCtrlPropertyResponse;

    public PropertyServiceHandler(String parentLogTag) {
        BASE_LOG_TAG = parentLogTag + getClass().getSimpleName().toString();
    }

    public void setRemoteCtrlPropService(IRemoteCtrlPropertyService remoteCtrlPropertyService) {
        this.mRemoteCtrlPropertyService = remoteCtrlPropertyService;
    }

    public void setRemoteCtrlPropertyResponseService(IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService) {
        mRemoteCtrlPropertyResponse = remoteCtrlPropertyResponseService;
    }

    @Override
    public void getProperty(int requestIdentifier, RemoteCtrlHalPropertyValue remoteCtrlPropertyValue) throws RemoteException {
        Log.v(getLogTag(), "getProperty(): [RequestIdentifier = " + requestIdentifier + "], "  + remoteCtrlPropertyValue);
        // TODO: Convert from HAL prop to vanilla prop
        GetPropertyRequestAsyncTask
                .create(getLogTag(), requestIdentifier, mRemoteCtrlPropertyService, mRemoteCtrlPropertyResponse, remoteCtrlPropertyValue)
                .execute();
    }

    @Override
    public void setProperty(int requestIdentifier, RemoteCtrlHalPropertyValue remoteCtrlPropertyValue) throws RemoteException {
        Log.v(getLogTag(), "setProperty(): [RequestIdentifier = " + requestIdentifier + "], "  + remoteCtrlPropertyValue);
        // TODO: Convert from HAL prop to vanilla prop
        SetPropertyRequestAsyncTask
                .create(getLogTag(), requestIdentifier, mRemoteCtrlPropertyService, mRemoteCtrlPropertyResponse, remoteCtrlPropertyValue)
                .execute();
    }

    public void registerRemoteCtrlPropertyHandler(HwBinder.DeathRecipient deathRecipient, int deathRecipientCookie) {
        try {
            Log.v(getLogTag(), "registerRemoteCtrlPropertyHandler()");

            mRemoteCtrlPropertyResponse.registerRemoteCtrlPropertyHandler(this);

            mRemoteCtrlPropertyResponse.linkToDeath(deathRecipient, deathRecipientCookie);
        } catch (RemoteException ex) {
            Log.e(getLogTag(), ": RemoteException: ", ex);
        }
    }

    public void unregisterRemoteCtrlPropertyHandler() {
        try {
            Log.v(getLogTag(), "unregisterRemoteCtrlPropertyHandler()");

            mRemoteCtrlPropertyResponse.unregisterRemoteCtrlPropertyHandler(this);
        } catch (RemoteException ex) {
            Log.e(getLogTag(), "RemoteException: ", ex);
        }
    }

    private String getLogTag() {
        return BASE_LOG_TAG;
    }
}
