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
import android.util.Log;

import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;
import com.volvocars.remotectrlservices.carproperty.CarHvacWrapper;
import com.volvocars.remotectrlservices.climatectrl.IRemoteClimateResponseService;
import com.volvocars.remotectrlservices.climatectrl.RemoteCtrlPropertyValue;

// TODO: See if this can be made more generic. (Philip Werner 2018-04-20)
public class GetCarPropertyAsyncTask extends AsyncTask<Void, Void, Void> {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.RemoteClimateService.GetCarPropertyAsyncTask";

    int mRequestIdentifier;
    CarHvacManager mCarHvacManager = null;
    IRemoteClimateResponseService mIRemoteClimateResponseService = null;
    RemoteCtrlPropertyValue mRemoteCtrlPropertyValue = null;

    protected GetCarPropertyAsyncTask(int requestIdentifier,
            IRemoteClimateResponseService remoteCtrlPropertyResponseService,
            CarHvacManager carHvacManager,
                                      RemoteCtrlPropertyValue remoteCtrlPropertyValue) {
        mRequestIdentifier = requestIdentifier;
        mIRemoteClimateResponseService = remoteCtrlPropertyResponseService;
        mCarHvacManager = carHvacManager;
        mRemoteCtrlPropertyValue = remoteCtrlPropertyValue;
    }

    public static GetCarPropertyAsyncTask create(int requestIdentifier,
                                                 IRemoteClimateResponseService remoteClimateResponseService,
            CarHvacManager carHvacManager, RemoteCtrlPropertyValue propValue) {
        return new GetCarPropertyAsyncTask(
                requestIdentifier, remoteClimateResponseService, carHvacManager, propValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            Log.v(TAG, "getProperty: " + mRemoteCtrlPropertyValue.toString());

            CarPropertyValue value = CarPropertyUtils.toCarPropertyValue(mRemoteCtrlPropertyValue);

            CarPropertyValue responseCarPropertyValue =
                    new CarHvacWrapper(mCarHvacManager).getProperty(value);

            Log.v(TAG, "CarPropertyValue: " + responseCarPropertyValue);

            RemoteCtrlPropertyValue response = CarPropertyUtils.toRemoteCtrlPropertyValue(responseCarPropertyValue);

            Log.v(TAG, "RemoteCtrlPropertyValue: " + responseCarPropertyValue);

            mIRemoteClimateResponseService.sendGetPropertyResponse(
                    mRequestIdentifier, response);
        } catch (CarNotConnectedException ex) {
            try {
                Log.v(TAG, "CarNotConnectedException: " + ex.toString());

                RemoteCtrlPropertyValue<?> errorResponse =
                        CarPropertyUtils.createRemoteCtrlPropertyValueWithErrorStatus(mRemoteCtrlPropertyValue);

                mIRemoteClimateResponseService.sendSetPropertyResponse(
                        mRequestIdentifier, errorResponse);
            } catch (RemoteException innerEx) {
                Log.e(TAG, "RemoteException: ", innerEx);
            }
        } catch (RemoteException ex) {
            Log.e(TAG, "RemoteException: ", ex);
        } catch (Exception ex) {
            Log.e(TAG, "RemoteException: ", ex);
        }

        // Necessary but not used
        return null;
    }
}
