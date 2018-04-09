/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.relay;

import android.app.Service;

import android.content.Intent;

import android.os.Bundle;
import android.os.IBinder;

import android.util.Log;

import com.volvocars.connectivitymanager.ConnectivityManager;
import com.volvocars.connectivitymanager.ConnectivityManagerGatewayConnectionCallback;

/**
*
*/
public class ConnectivityManagerRelay extends Service {
    private static final String LOG_TAG = "ConManRelay.Service";

    private ConnectivityManager connectivityManager = null;

    private ConnectivityManagerGatewayConnectionCallback connectionCallback =
            new ConnectivityManagerGatewayConnectionCallback() {
                @Override
                public void onServiceConnected() {
                    Log.d(LOG_TAG, "Connected to Gateway");
                }

                @Override
                public void onServiceDisconnected() {
                    Log.d(LOG_TAG, "Disconnected");
                }
            };

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();

        connectivityManager = new ConnectivityManager(this, connectionCallback);
    }

    @Override
    public void onDestroy() {
        Log.v(LOG_TAG, "onDestroy");
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(LOG_TAG, "onStartCommand");

        connectivityManager.connect();

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");
        return null;
    }
}
