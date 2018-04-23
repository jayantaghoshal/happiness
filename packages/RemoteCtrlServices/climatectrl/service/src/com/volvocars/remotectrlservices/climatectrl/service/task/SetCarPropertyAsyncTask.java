/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.service.task;

import android.os.AsyncTask;
import android.os.RemoteException;
import android.car.CarNotConnectedException;
import android.car.hardware.CarPropertyValue;
import android.car.hardware.hvac.CarHvacManager;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.util.Log;

import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;
import com.volvocars.remotectrlservices.carproperty.CarHvacWrapper;

import com.volvocars.remotectrlservices.climatectrl.*;

// TODO: See if this can be made more generic. (Philip Werner 2018-04-20)
public class SetCarPropertyAsyncTask extends AsyncTask<Void, Void, Void> {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.RemoteClimateService.SetCarPropertyAsyncTask";

    int mRequestIdentifier;
    CarHvacManager mCarHvacManager = null;
    IRemoteClimateResponseService mIRemoteClimateResponseService = null;
    CarPropertyValue mCarPropertyValue = null;

    protected SetCarPropertyAsyncTask(int requestIdentifier,
            IRemoteClimateResponseService remoteClimateResponseService,
            CarHvacManager carHvacManager, CarPropertyValue carPropertyValue) {
        mRequestIdentifier = requestIdentifier;
        mIRemoteClimateResponseService = remoteClimateResponseService;
        mCarHvacManager = carHvacManager;
        mCarPropertyValue = carPropertyValue;
    }

    public static SetCarPropertyAsyncTask create(int requestIdentifier,
            IRemoteClimateResponseService remoteClimateResponseService,
            CarHvacManager carHvacManager, CarPropertyValue propValue) {
        return new SetCarPropertyAsyncTask(
                requestIdentifier, remoteClimateResponseService, carHvacManager, propValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            Log.v(TAG, "setProperty: " + mCarPropertyValue.toString());

            CarPropertyValue responseCarPropertyValue =
                    new CarHvacWrapper(mCarHvacManager).setProperty(mCarPropertyValue);

            Log.v(TAG, "property Set: " + responseCarPropertyValue.toString());

            mIRemoteClimateResponseService.sendSetPropertyResponse(
                    mRequestIdentifier, responseCarPropertyValue);
        } catch (CarNotConnectedException ex) {
            try {
                Log.v(TAG, "CarNotConnectedException: " + ex.toString());

                CarPropertyValue<?> errorResponse =
                        CarPropertyUtils.createCarPropertyValueWithErrorStatus(mCarPropertyValue);

                mIRemoteClimateResponseService.sendSetPropertyResponse(
                        mRequestIdentifier, errorResponse);
            } catch (Exception innerEx) {
                Log.e(TAG, "Exception: ", innerEx);
            }
        } catch (Exception ex) {
            Log.e(TAG, "Exception: ", ex);
        }

        // Necessary but not used
        return null;
    }
}
