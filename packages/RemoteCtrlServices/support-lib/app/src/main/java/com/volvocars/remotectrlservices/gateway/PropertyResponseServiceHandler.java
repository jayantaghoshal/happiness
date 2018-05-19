/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway;

import android.car.hardware.CarPropertyValue;
import android.util.Log;
import android.content.Context;
import android.os.RemoteException;
import android.content.pm.PackageManager;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyResponseService;
import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;
import com.volvocars.remotectrlservices.gateway.task.response.GetPropertyResponseAsyncTask;
import com.volvocars.remotectrlservices.gateway.task.response.SetPropertyResponseAsyncTask;
import com.volvocars.remotectrlservices.gateway.task.response.NotifyPropertyResponseAsyncTask;

import java.text.MessageFormat;

/**
 * Communication between Service -> Gateway -> HAL
 */
public class PropertyResponseServiceHandler extends IRemoteCtrlPropertyResponseService.Stub {
    private String BASE_LOG_TAG;

    private IRemoteCtrlPropertyResponse mRemoteCtrlPropertyResponseService = null;

    public PropertyResponseServiceHandler(String parentLogTag) {
        BASE_LOG_TAG = parentLogTag + getClass().getSimpleName().toString();
    }

    public void setRemoteCtrlPropertyResponseService(IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService) {
        mRemoteCtrlPropertyResponseService = remoteCtrlPropertyResponseService;
    }

    @Override
    public void sendSetPropertyResponse(
            int requestIdentifier, RemoteCtrlPropertyValue requestedPropValue) {
        Log.v(getLogTag(), "sendSetPropertyResponse(): [RequestIdentifier = " + requestIdentifier + "], "  + requestedPropValue);

        SetPropertyResponseAsyncTask
                .create(getLogTag(), requestIdentifier, mRemoteCtrlPropertyResponseService, requestedPropValue)
                .execute();
    }

    @Override
    public void sendGetPropertyResponse(
            int requestIdentifier, RemoteCtrlPropertyValue requestedPropValue) {
        Log.v(getLogTag(), "sendGetPropertyResponse(): [RequestIdentifier = " + requestIdentifier + "], "  + requestedPropValue);

        GetPropertyResponseAsyncTask
                .create(getLogTag(), requestIdentifier, mRemoteCtrlPropertyResponseService, requestedPropValue)
                .execute();
    }

    @Override
    public void notifyPropertyChanged(RemoteCtrlPropertyValue propValue) {
        Log.v(getLogTag(), "notifyPropertyChanged(): "  + propValue);

        NotifyPropertyResponseAsyncTask
                .create(getLogTag(), mRemoteCtrlPropertyResponseService, propValue)
                .execute();
    }

    private String getLogTag() {
        return BASE_LOG_TAG;
    }
}
