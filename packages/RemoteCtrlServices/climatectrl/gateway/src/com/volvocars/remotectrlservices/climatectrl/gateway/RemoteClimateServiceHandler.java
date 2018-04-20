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

import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;

import java.util.Timer;
import java.util.TimerTask;
import java.util.NoSuchElementException;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;
import vendor.volvocars.hardware.remotectrl.V1_0.StatusCode;

import com.volvocars.remotectrlservices.climatectrl.IRemoteClimateService;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

import com.volvocars.remotectrlservices.climatectrl.gateway.task.request.GetPropertyRequestAsyncTask;
import com.volvocars.remotectrlservices.climatectrl.gateway.task.request.SetPropertyRequestAsyncTask;

/**
 * Communication between HAL -> Gateway -> Service
 */
public class RemoteClimateServiceHandler extends IRemoteCtrlProperty.Stub {
    private static final String TAG = "RemoteCtrl.ClimateCtrl.RemoteClimateServiceHandler";

    private static final int COOKIE = 1190;

    private IRemoteClimateService mRemoteClimateService;
    private IRemoteCtrlPropertyResponse mNativeRemoteClimateCtrl;

    public RemoteClimateServiceHandler(IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl) {
        mNativeRemoteClimateCtrl = nativeRemoteClimateCtrl;
    }

    public void setRemoteClimateService(IRemoteClimateService remoteClimateService) {
        mRemoteClimateService = remoteClimateService;
    }

    @Override
    public void getProperty(short requestIdentifier, VehiclePropValue requestedPropValue) {
        Log.v(TAG, "getProperty: " + requestedPropValue.toString());
        GetPropertyRequestAsyncTask
                .create(requestIdentifier, mRemoteClimateService, mNativeRemoteClimateCtrl,
                        requestedPropValue)
                .execute();
    }

    @Override
    public void setProperty(short requestIdentifier, VehiclePropValue propValue) {
        Log.v(TAG, "setProperty: " + propValue.toString());
        SetPropertyRequestAsyncTask
                .create(requestIdentifier, mRemoteClimateService, mNativeRemoteClimateCtrl,
                        propValue)
                .execute();
    }

    public void registerClimateControlHandler(HwBinder.DeathRecipient deathRecipient) {
        try {
            Log.v(TAG, "Registering climate control handler on the native daemon");

            mNativeRemoteClimateCtrl.registerRemoteCtrlPropertyHandler(this);

            mNativeRemoteClimateCtrl.linkToDeath(deathRecipient, COOKIE /* cookie */);

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

