/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.content.Intent;
import android.os.Bundle;
import vendor.volvocars.hardware.cloud.V1_0.*;
import java.util.ArrayList;

/**
* CloudService is the main service for communcating with clouddeamon.
*/
public class CloudService extends Service {
    private static final String LOG_TAG = "CloudService";
    private static final String FSAPI = "FoundationServicesApi";
    private static final String SOFTWARE_MANAGEMENT = "SoftwareManagementApi";

    private FoundationServicesApiImpl foundation_services_api = null;
    private SoftwareManagementApiImpl software_management_api = null;
    private CloudConnection cloud_connection = null;

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();
        try {

            if (foundation_services_api == null) {
                foundation_services_api = new FoundationServicesApiImpl();
            }
            if (software_management_api == null) {
                software_management_api = new SoftwareManagementApiImpl();
            }
            if (cloud_connection == null) {
                cloud_connection = new CloudConnection(this);
                cloud_connection.init();
            }

        } catch (Exception ex) {
            Log.e(LOG_TAG, "Unhandled exception:\n" + ex.getMessage());
        }
    }

    @Override
    public void onDestroy() {
        Log.v(LOG_TAG, "onDestroy");
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(LOG_TAG, "onStartCommand");
        return START_STICKY;
    }

    /**
    * When a client connects to the service,
    * the intent action is read to identify which binder interface to return
    */
    @Override
    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");

        String action = intent.getAction();

        if (action.equals(FSAPI)) {
            Log.v(LOG_TAG, "Bind on FoundationServicesApi");
            return foundation_services_api;
        } else if (action.equals(SOFTWARE_MANAGEMENT)) {
            Log.v(LOG_TAG, "Bind on SoftwareManagementApi");

            return software_management_api;
        } else {
            //handling for when couldnt find mathing binder?
            Log.w(LOG_TAG, "Trying to bind with unknown action: " + action);
            return null;
        }
    }

    public void isConnected(boolean connected) {
        if (!connected) {
            return;
        }

        try {
            Log.v(LOG_TAG, "FSAPI init");
            foundation_services_api.init(cloud_connection);

            Log.v(LOG_TAG, "Call feature available");
            Feature software_management = foundation_services_api.getFeatureAvailable("SoftwareManagement");
            if (software_management != null) {
                Log.v(LOG_TAG, "SWMAPI init");
                software_management_api.init(cloud_connection, software_management.uri);
            } else {
                Log.w(LOG_TAG, "Software Management is not available");
            }
        } catch (Exception ex) {
            Log.e(LOG_TAG, "Unhandled exception:\n" + ex.getMessage());
        }
    }

    public void enteredErrorState(String reason) {
        Log.e(LOG_TAG, "Entered Error State: " + reason);
    }
}