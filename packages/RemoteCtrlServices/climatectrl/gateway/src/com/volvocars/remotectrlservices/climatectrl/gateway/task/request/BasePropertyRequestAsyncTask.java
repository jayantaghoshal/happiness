/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.request;

import android.car.hardware.CarPropertyValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.os.AsyncTask;
import android.os.RemoteException;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrlservices.climatectrl.gateway.BasePropertyAsyncTask;
import com.volvocars.remotectrlservices.climatectrl.*;

public abstract class BasePropertyRequestAsyncTask extends BasePropertyAsyncTask {
    protected IRemoteClimateService mRemoteClimateService;
    protected IRemoteCtrlPropertyResponse mNativeRemoteClimateCtrl;
    protected VehiclePropValue mRequestedPropValue;

    public BasePropertyRequestAsyncTask(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
            VehiclePropValue requestedPropValue) {
        super(requestIdentifier);
        mRemoteClimateService = remoteClimateService;
        mNativeRemoteClimateCtrl = nativeRemoteClimateCtrl;
        mRequestedPropValue = requestedPropValue;
    }
}
