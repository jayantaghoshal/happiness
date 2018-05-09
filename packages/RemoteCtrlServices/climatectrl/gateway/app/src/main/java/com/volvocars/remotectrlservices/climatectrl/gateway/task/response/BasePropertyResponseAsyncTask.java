/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.response;

import android.car.hardware.CarPropertyValue;
import android.os.AsyncTask;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrlservices.climatectrl.gateway.BasePropertyAsyncTask;
import com.volvocars.remotectrlservices.climatectrl.*;

public abstract class BasePropertyResponseAsyncTask extends BasePropertyAsyncTask {
    protected IRemoteCtrlPropertyResponse mNativeRemoteClimateCtrl;
    protected RemoteCtrlPropertyValue mPropValue;

    public BasePropertyResponseAsyncTask(int requestIdentifier,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl, RemoteCtrlPropertyValue propValue) {
        super(requestIdentifier);
        mNativeRemoteClimateCtrl = nativeRemoteClimateCtrl;
        mPropValue = propValue;
    }
}
