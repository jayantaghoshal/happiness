/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task.response;

import android.car.hardware.CarPropertyValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.os.AsyncTask;
import android.os.RemoteException;
import android.util.Log;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrlservices.climatectrl.*;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;

public class NotifyPropertyResponseAsyncTask extends BasePropertyResponseAsyncTask {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.Gateway.RemoteClimateResponseServiceHandler.NotifyPropertyResponseAsyncTask";

    protected NotifyPropertyResponseAsyncTask(
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService,
            CarPropertyValue propValue) {
        super(0 /* dummy */, remoteCtrlPropertyResponseService, propValue);
    }

    public static NotifyPropertyResponseAsyncTask create(
            IRemoteCtrlPropertyResponse nativeRemoteClimateCtrl, CarPropertyValue propValue) {
        return new NotifyPropertyResponseAsyncTask(nativeRemoteClimateCtrl, propValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            VehiclePropValue vehiclePropValue = CarPropertyUtils.toVehiclePropValue(mPropValue);

            Log.v(TAG, "vehiclePropValue: " + vehiclePropValue);

            mNativeRemoteClimateCtrl.notifyPropertyChanged(vehiclePropValue);
        } catch (IllegalArgumentException ex) {
            Log.e(TAG, "IllegalArgumentException: ", ex);
        } catch (Exception ex) {
            Log.v(TAG, "RemoteException: " + ex);
        }

        // Necessary but not used
        return null;
    }
}
