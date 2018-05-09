/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.request;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

import com.volvocars.remotectrlservices.climatectrl.IRemoteClimateService;
import com.volvocars.remotectrlservices.climatectrl.gateway.BasePropertyAsyncTask;

public abstract class BasePropertyRequestAsyncTask extends BasePropertyAsyncTask {
    protected IRemoteClimateService mRemoteClimateService;
    protected IRemoteCtrlPropertyResponse mNativeRemoteClimateCtrl;
    protected RemoteCtrlHalPropertyValue mRequestedPropValue;

    public BasePropertyRequestAsyncTask(int requestIdentifier,
            IRemoteClimateService remoteClimateService,
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl,
                                        RemoteCtrlHalPropertyValue requestedPropValue) {
        super(requestIdentifier);
        mRemoteClimateService = remoteClimateService;
        mNativeRemoteClimateCtrl = nativeRemoteClimateCtrl;
        mRequestedPropValue = requestedPropValue;
    }
}
