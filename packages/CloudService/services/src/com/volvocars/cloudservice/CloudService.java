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
    private IFoundationServicesApi.Stub fsapiBinder = null;
    private ICloudConnection cloud_connection = null;

    private SoftwareManagementApiImpl software_management_api = null;

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();
        try{
            cloud_connection = ICloudConnection.getService();

            // Do some registration for knowing when the Cloud Daemon has established a connection
            // cloud_connection(some_callback);
            // This callback would call Initialize if connection is made. And maybe some deinitalize or something if it is lost.
            Initialize();

        } catch (Exception ex) {
            Log.e(LOG_TAG, "Unhandled exception:\n" + ex.getMessage());
        }
    }

    public void Initialize() {
        Log.v(LOG_TAG, "Initialize");
        try {

            Log.v(LOG_TAG, "FSAPI init");
            fsapiBinder = new FoundationServicesApiImpl(cloud_connection).binder;

            // Do this for now, later on we will have some callback and stuff..
            Log.v(LOG_TAG, "Call feature available");
            if(fsapiBinder.IsFeatureAvailable("SoftwareManagement")) {
                Log.v(LOG_TAG, "SWMAPI init");
                software_management_api = new SoftwareManagementApiImpl(cloud_connection);
            } else {
                Log.w(LOG_TAG, "Software Management is not available");
            }
        }
        catch(Exception ex){
            Log.e(LOG_TAG, "Unhandled exception:\n" + ex.getMessage());
        }
    }

    /**
    * When a client connects to the service,
    * the intent action is read to identify which binder interface to return
    */
    @Override
    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");

        String action = intent.getAction();

        if(action.equals(FSAPI)) {
            Log.v(LOG_TAG, "Bind on FoundationServicesApi");
            return fsapiBinder;
        } else if (action.equals(SOFTWARE_MANAGEMENT)) {
            Log.v(LOG_TAG, "Bind on SoftwareManagementApi");
            return software_management_api;
        } else {
            //handling for when couldnt find mathing binder?
            Log.w(LOG_TAG, "Trying to bind with unknown action: " + action);
            return null;
        }
    }
}