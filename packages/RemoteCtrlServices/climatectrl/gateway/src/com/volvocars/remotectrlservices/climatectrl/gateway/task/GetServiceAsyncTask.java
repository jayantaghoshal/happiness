/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway.task;

import android.os.AsyncTask;

import android.util.Log;

import android.os.RemoteException;

import java.util.NoSuchElementException;

import com.volvocars.remotectrlservices.climatectrl.gateway.RemoteClimateGateway;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

public class GetServiceAsyncTask extends AsyncTask<Void, Void, Void> {
    protected final String TAG = "RemoteCtrl.ClimateCtrl.Gateway.GetServiceAsyncTask";

    protected RemoteClimateGateway mRemoteClimateGateway;

    protected int mSleepTime;

    protected IRemoteCtrlPropertyResponse mService = null;

    // TODO: Change Parameter to interface instead, to make it generic. /Philip Werner (2018-04-24)
    // The interface must have a callback as well.
    public GetServiceAsyncTask(RemoteClimateGateway remoteClimateGateway, int sleepTime) {
        mRemoteClimateGateway = remoteClimateGateway;
        mSleepTime = sleepTime;
    }

    @Override
    protected Void doInBackground(Void... unused) {
        // TODO: How to inject this for proper testing? /Philip Werner (2018-04-24)

        boolean isTryingToGetService = true;
        do {
            try {
                mService = IRemoteCtrlPropertyResponse.getService();
                mRemoteClimateGateway.setRemoteCtrlPropertyResponseService(mService);
                isTryingToGetService = false;
                break;
            } catch (NoSuchElementException ex) {
                Log.e(TAG, "IRemoteCtrlPropertyResponse service currently unavailable.");
            } catch (RemoteException ex) {
                Log.e(TAG, "Remote exception thrown: ", ex);
            }

            try {
                Log.v(TAG,
                        "Waiting " + mSleepTime + " milliseconds before retrying getService()...");
                Thread.sleep(mSleepTime);
            } catch (InterruptedException ex) {
                Log.v(TAG, "Thread interrupted. Aborting", ex);
                return null;
            }
        } while (isTryingToGetService);

        Log.v(TAG, "IRemoteCtrlPropertyResponse aquired");

        return null;
    }

    @Override
    protected void onPostExecute(Void result) {
        // Runs on main UI thread
        // Notify RemoteClimateGateway of event
        if (mService != null) {
            mRemoteClimateGateway.notifyServiceConnected();
        }
    }
}
