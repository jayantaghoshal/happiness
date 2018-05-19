/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway.task.request;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyService;
import com.volvocars.remotectrlservices.gateway.task.BasePropertyAsyncTask;

public abstract class BasePropertyRequestAsyncTask extends BasePropertyAsyncTask {
    protected IRemoteCtrlPropertyService mRemoteCtrlPropertyService;
    protected IRemoteCtrlPropertyResponse mRemoteCtrlPropertyResponse;
    protected RemoteCtrlHalPropertyValue mRequestedPropValue;

    public BasePropertyRequestAsyncTask(String parentLogTag, int requestIdentifier,
                                        IRemoteCtrlPropertyService remoteCtrlPropertyService,
                                        IRemoteCtrlPropertyResponse remoteCtrlPropertyResponse,
                                        RemoteCtrlHalPropertyValue requestedPropValue) {
        super(parentLogTag, requestIdentifier);
        mRemoteCtrlPropertyService = remoteCtrlPropertyService;
        mRemoteCtrlPropertyResponse = remoteCtrlPropertyResponse;
        mRequestedPropValue = requestedPropValue;
    }
}
