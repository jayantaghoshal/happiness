/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.service.task;

import android.os.AsyncTask;
import android.car.CarNotConnectedException;
import android.car.hardware.CarPropertyValue;
import android.car.hardware.hvac.CarHvacManager;
import android.util.Log;

import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyResponseService;
import com.volvocars.remotectrl.interfaces.RemoteCtrlPropertyValue;
import com.volvocars.remotectrlservices.carproperty.CarPropertyUtils;
import com.volvocars.remotectrlservices.carproperty.CarHvacWrapper;

// TODO: See if this can be made more generic. (Philip Werner 2018-04-20)
public class SetCarPropertyAsyncTask extends AsyncTask<Void, Void, Void> {
    private static final String TAG =
            "RemoteCtrl.ClimateCtrl.RemoteClimateService.SetCarPropertyAsyncTask";

    int mRequestIdentifier;
    CarHvacManager mCarHvacManager = null;
    IRemoteCtrlPropertyResponseService mIRemoteClimateResponseService = null;
    RemoteCtrlPropertyValue mRemoteCtrlPropertyValue = null;

    protected SetCarPropertyAsyncTask(int requestIdentifier,
                                      IRemoteCtrlPropertyResponseService remoteClimateResponseService,
            CarHvacManager carHvacManager, RemoteCtrlPropertyValue carPropertyValue) {
        mRequestIdentifier = requestIdentifier;
        mIRemoteClimateResponseService = remoteClimateResponseService;
        mCarHvacManager = carHvacManager;
        mRemoteCtrlPropertyValue = carPropertyValue;
    }

    public static SetCarPropertyAsyncTask create(int requestIdentifier,
                                                 IRemoteCtrlPropertyResponseService remoteClimateResponseService,
            CarHvacManager carHvacManager, RemoteCtrlPropertyValue propValue) {
        return new SetCarPropertyAsyncTask(
                requestIdentifier, remoteClimateResponseService, carHvacManager, propValue);
    }

    @Override
    protected Void doInBackground(Void... unused) {
        try {
            Log.v(TAG, "setProperty: " + mRemoteCtrlPropertyValue.toString());

            CarPropertyValue value = CarPropertyUtils.toCarPropertyValue(mRemoteCtrlPropertyValue);

            CarPropertyValue responseCarPropertyValue =
                    new CarHvacWrapper(mCarHvacManager).setProperty(value);

            RemoteCtrlPropertyValue response = CarPropertyUtils.toRemoteCtrlPropertyValue(responseCarPropertyValue);

            Log.v(TAG, "property Set: " + responseCarPropertyValue.toString());

            mIRemoteClimateResponseService.sendSetPropertyResponse(
                    mRequestIdentifier, response);
        } catch (CarNotConnectedException ex) {
            try {
                Log.v(TAG, "CarNotConnectedException: " + ex.toString());

                RemoteCtrlPropertyValue<?> errorResponse =
                        CarPropertyUtils.createRemoteCtrlPropertyValueWithErrorStatus(mRemoteCtrlPropertyValue);

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
