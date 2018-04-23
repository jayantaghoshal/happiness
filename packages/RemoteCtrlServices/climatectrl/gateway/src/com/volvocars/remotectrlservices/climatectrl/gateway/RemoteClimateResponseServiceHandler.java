/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway;

import android.car.hardware.CarPropertyValue;
import android.util.Log;
import android.content.Context;
import android.os.RemoteException;
import android.content.pm.PackageManager;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.StatusCode;

import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

import com.volvocars.remotectrlservices.climatectrl.gateway.task.response.GetPropertyResponseAsyncTask;
import com.volvocars.remotectrlservices.climatectrl.gateway.task.response.SetPropertyResponseAsyncTask;
import com.volvocars.remotectrlservices.climatectrl.gateway.task.response.NotifyPropertyResponseAsyncTask;

import com.volvocars.remotectrlservices.climatectrl.*;

/**
 * Communication between Service -> Gateway -> HAL
 */
public class RemoteClimateResponseServiceHandler extends IRemoteClimateResponseService.Stub {
    private static final String TAG = "RemoteCtrl.ClimateCtrl.RemoteClimateResponseServiceHandler";

    private static final String PERMISSION_REMOTE_CTRL = "android.volvocars.permission.REMOTE_CTRL";

    private IRemoteCtrlPropertyResponse mRemoteCtrlPropertyResponseService = null;

    public RemoteClimateResponseServiceHandler() {}

    public RemoteClimateResponseServiceHandler(
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService) {
        mRemoteCtrlPropertyResponseService = remoteCtrlPropertyResponseService;
    }

    public void setRemoteCtrlPropertyResponseService(
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService) {
        mRemoteCtrlPropertyResponseService = remoteCtrlPropertyResponseService;
    }

    @Override
    public void sendSetPropertyResponse(
            int requestIdentifier, CarPropertyValue requestedPropValue) {
        SetPropertyResponseAsyncTask
                .create(requestIdentifier, mRemoteCtrlPropertyResponseService, requestedPropValue)
                .execute();
    }

    @Override
    public void sendGetPropertyResponse(
            int requestIdentifier, CarPropertyValue requestedPropValue) {
        GetPropertyResponseAsyncTask
                .create(requestIdentifier, mRemoteCtrlPropertyResponseService, requestedPropValue)
                .execute();
    }

    @Override
    public void notifyPropertyChanged(CarPropertyValue propValue) {
        NotifyPropertyResponseAsyncTask.create(mRemoteCtrlPropertyResponseService, propValue)
                .execute();
    }
}
