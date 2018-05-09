/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway;

import android.util.Log;
import android.content.Intent;
import android.content.Context;
import android.os.HwBinder;
import android.os.IBinder;
import android.os.RemoteException;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.car.hardware.CarPropertyValue;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.RemoteCtrlHalPropertyValue;

import com.volvocars.remotectrlservices.climatectrl.IRemoteClimateService;

import com.volvocars.remotectrlservices.climatectrl.gateway.task.request.GetPropertyRequestAsyncTask;
import com.volvocars.remotectrlservices.climatectrl.gateway.task.request.SetPropertyRequestAsyncTask;

/**
 * Communication between HAL -> Gateway -> Service
 */
public class RemoteClimateServiceHandler extends IRemoteCtrlProperty.Stub {
    private static final String TAG = "RemoteCtrl.ClimateCtrl.RemoteClimateServiceHandler";

    private IRemoteClimateService mRemoteClimateService;
    private IRemoteCtrlPropertyResponse mNativeRemoteClimateCtrl;

    public RemoteClimateServiceHandler() {}

    public RemoteClimateServiceHandler(IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl) {
        mNativeRemoteClimateCtrl = nativeRemoteClimateCtrl;
    }

    public void setRemoteClimateService(IRemoteClimateService remoteClimateService) {
        mRemoteClimateService = remoteClimateService;
    }

    public void setRemoteCtrlPropertyResponseService(
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService) {
        mNativeRemoteClimateCtrl = remoteCtrlPropertyResponseService;
    }

    @Override
    public void getProperty(int requestIdentifier, RemoteCtrlHalPropertyValue remoteCtrlPropertyValue) throws RemoteException {
        Log.v(TAG, "getProperty: " + remoteCtrlPropertyValue.toString());
        // TODO: Convert from HAL prop to vanilla prop
        GetPropertyRequestAsyncTask
                .create(requestIdentifier, mRemoteClimateService, mNativeRemoteClimateCtrl,
                        remoteCtrlPropertyValue)
                .execute();
    }

    @Override
    public void setProperty(int requestIdentifier, RemoteCtrlHalPropertyValue remoteCtrlPropertyValue) throws RemoteException {
        Log.v(TAG, "setProperty: " + remoteCtrlPropertyValue.toString());
        // TODO: Convert from HAL prop to vanilla prop
        SetPropertyRequestAsyncTask
                .create(requestIdentifier, mRemoteClimateService, mNativeRemoteClimateCtrl,
                        remoteCtrlPropertyValue)
                .execute();
    }

    public void registerClimateControlHandler(
            HwBinder.DeathRecipient deathRecipient, int deathRecipientCookie) {
        try {
            Log.v(TAG, "Registering climate control handler on the native daemon");

            mNativeRemoteClimateCtrl.registerRemoteCtrlPropertyHandler(this);

            mNativeRemoteClimateCtrl.linkToDeath(deathRecipient, deathRecipientCookie);

        } catch (RemoteException ex) {
            Log.e(TAG, "RemoteException: ", ex);
        }
    }

    public void unregisterClimateControlHandler() {
        try {
            mNativeRemoteClimateCtrl.unregisterRemoteCtrlPropertyHandler(this);
        } catch (RemoteException ex) {
            Log.e(TAG, "RemoteException: ", ex);
        }
    }
}
