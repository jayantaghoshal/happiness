/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.gateway;

import android.app.Service;

import android.content.Intent;

import android.os.Bundle;
import android.os.IBinder;
import android.os.HwBinder;
import android.os.RemoteException;

import android.util.Log;

import java.util.NoSuchElementException;
import java.util.Timer;
import java.util.TimerTask;

import vendor.volvocars.hardware.connectivitymanager.V1_0.*;

/**
*
*/
public class SystemConnectivityManager extends ISystemConnectivityManager.Stub {
    private static final String LOG_TAG = "ConManGw.Service";

    private INativeConnectivityManager nativeConnectivityManager = null;
    private DeathRecipient death = new DeathRecipient(this);

    final class DeathRecipient implements HwBinder.DeathRecipient {
        SystemConnectivityManager connection;

        DeathRecipient(SystemConnectivityManager connection) {
            this.connection = connection;
        }

        @Override
        public void serviceDied(long cookie) {
            Log.e(LOG_TAG, "Lost Connection to Native side");
            connection.daemonDied();
        }
    }

    public void init() {
        retryService();
    }

    public void daemonDied() {
        nativeConnectivityManager = null;
        retryService();
    }

    private void connectToService() {
        try {
            nativeConnectivityManager = INativeConnectivityManager.getService();
            nativeConnectivityManager.registerConnectivityManagerHandler(this);

            nativeConnectivityManager.linkToDeath(death, 1010 /* cookie */);

        } catch (RemoteException ex) {
            // Something went bananas with binder.. What do?
            Log.e(LOG_TAG, "Cannot connect to ICloudConnection: RemoteException [" + ex.getMessage()
                            + "]");
        }
    }

    private void retryService() {
        try {
            connectToService();
        } catch (NoSuchElementException e) {
            Log.e(LOG_TAG, "Native side not up yet.. Scheduling retry.");
            new Timer().schedule(new TimerTask() {
                @Override
                public void run() {
                    Log.d(LOG_TAG, "Retrying to connect to Native side");
                    retryService();
                }
            }, 2000);
        }
    }

    @Override
    public void updateWifiStationMode(byte mode) {}
}
