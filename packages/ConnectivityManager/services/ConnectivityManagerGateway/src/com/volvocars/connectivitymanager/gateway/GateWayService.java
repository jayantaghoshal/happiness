/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.gateway;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.content.Intent;
import android.os.Bundle;
import vendor.volvocars.hardware.connectivitymanager.V1_0.*;

import android.os.HwBinder;

/**
*
*/
public class GateWayService extends Service {
    private static final String LOG_TAG = "ConManGw.Service";

    private static final String GATEWAY_SERVICE = "ConnectivityManagerGateway";

    private SystemConnectivityManager connectivityManager = null;
    private ConnectivityManagerGateway connectivityManagerGateway = null;

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();

        connectivityManagerGateway = new ConnectivityManagerGateway();
        connectivityManager = new SystemConnectivityManager(connectivityManagerGateway);

        connectivityManagerGateway.init(connectivityManager);
    }

    @Override
    public void onDestroy() {
        Log.v(LOG_TAG, "onDestroy");
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(LOG_TAG, "onStartCommand");

        connectivityManager.init();

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");
        String action = intent.getAction();

        if (action.equals(GATEWAY_SERVICE)) {
            Log.v(LOG_TAG, "Bind on " + GATEWAY_SERVICE);
            return connectivityManagerGateway;
        } else {
            //handling for when couldnt find mathing binder?
            Log.d(LOG_TAG, "Trying to bind with unknown action: " + action);
            return null;
        }
    }
}
