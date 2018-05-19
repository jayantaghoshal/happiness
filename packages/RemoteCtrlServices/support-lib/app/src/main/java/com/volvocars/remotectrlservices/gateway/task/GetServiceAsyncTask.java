/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway.task;

import android.os.AsyncTask;

import android.util.Log;

import android.os.RemoteException;

import java.util.NoSuchElementException;

import java.text.MessageFormat;

import com.volvocars.remotectrlservices.gateway.RemoteGateway;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

public class GetServiceAsyncTask extends AsyncTask<Void, Void, Void> {
    protected String BASE_LOG_TAG;

    protected RemoteGateway mRemoteGateway;

    protected int mSleepTime;

    protected IRemoteCtrlPropertyResponse mService = null;

    // TODO: Change Parameter to interface instead, to make it generic. /Philip Werner (2018-04-24)
    // The interface must have a callback as well.
    public GetServiceAsyncTask(String parentLogTag, RemoteGateway remoteGateway, int sleepTime) {
        BASE_LOG_TAG = parentLogTag;
        mRemoteGateway = remoteGateway;
        mSleepTime = sleepTime;
    }

    @Override
    protected Void doInBackground(Void... unused) {
        // TODO: How to inject this for proper testing? /Philip Werner (2018-04-24)

        boolean isTryingToGetService = true;
        do {
            try {
                mService = IRemoteCtrlPropertyResponse.getService(mRemoteGateway.getHalServiceName());
                mRemoteGateway.setRemoteCtrlPropResponseService(mService);
                isTryingToGetService = false;
                break;
            } catch (NoSuchElementException ex) {
                Log.e(getLogTag(), "IRemoteCtrlPropertyResponse service currently unavailable.");
            } catch (RemoteException ex) {
                Log.e(getLogTag(), "Remote exception thrown: ", ex);
            }

            try {
                Log.v(getLogTag(),
                        "Waiting " + mSleepTime + " milliseconds before retrying getService()...");
                Thread.sleep(mSleepTime);
            } catch (InterruptedException ex) {
                Log.v(getLogTag(), "Thread interrupted. Aborting", ex);
                return null;
            }
        } while (isTryingToGetService);

        Log.v(getLogTag(), "IRemoteCtrlPropertyResponse aquired");

        return null;
    }

    @Override
    protected void onPostExecute(Void result) {
        // Runs on main UI thread
        // Notify RemoteClimateGateway of event
        if (mService != null) {
            mRemoteGateway.notifyServiceConnected();
        }
    }

    private String getLogTag() {
        return MessageFormat.format(BASE_LOG_TAG + ".", this.getClass().getName());
    }
}
