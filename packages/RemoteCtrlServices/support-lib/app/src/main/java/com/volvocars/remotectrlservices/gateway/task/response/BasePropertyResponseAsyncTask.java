/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway.task.response;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;
import com.volvocars.remotectrlservices.gateway.task.BasePropertyAsyncTask;

public abstract class BasePropertyResponseAsyncTask extends BasePropertyAsyncTask {
    protected IRemoteCtrlPropertyResponse mRemoteCtrlPropertyResponse;
    protected RemoteCtrlPropertyValue mPropValue;

    public BasePropertyResponseAsyncTask(
            String parentLogTag,
            int requestIdentifier,
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse,
            RemoteCtrlPropertyValue propValue) {
        super(parentLogTag, requestIdentifier);
        mRemoteCtrlPropertyResponse = remoteCtrlPropertyResponse;
        mPropValue = propValue;
    }
}
