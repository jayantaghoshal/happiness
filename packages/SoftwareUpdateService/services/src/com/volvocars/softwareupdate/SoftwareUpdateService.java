/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.app.Service;

import android.content.Context;
import android.content.Intent;

import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.FoundationServicesApi;
import com.volvocars.cloudservice.FoundationServicesApiConnectionCallback;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareManagementApi;
import com.volvocars.cloudservice.SoftwareManagementApiConnectionCallback;
import com.volvocars.cloudservice.SoftwareAssignment;

/**
*
*/
public class SoftwareUpdateService extends Service {
    private static final String LOG_TAG = "SwUpdService";

    private Context context;

    private SoftwareUpdateManagerImpl software_update_manager;

    private SoftwareManagementApi swapi = null;

    private int state = 0; // Dummy state

    private SoftwareManagementApiConnectionCallback swapi_callback = new SoftwareManagementApiConnectionCallback() {
        @Override
        public void onServiceConnected() {
            Log.d(LOG_TAG, "Connected to SWAPI");

            state = 1;
            software_update_manager.UpdateState(state);

            ISoftwareManagementApiCallback.Stub swapi_callback = new ISoftwareManagementApiCallback.Stub() {

                @Override
                public void CommissionStatus(int code) {
                }

                @Override
                public void SoftwareAssignmentList(int code, List<SoftwareAssignment> software_list) {
                    Log.v(LOG_TAG, "SoftwareAssignmentList callback when SoftwareUpdateService started, what to do with this list?");
                }

                @Override
                public void PendingInstallations(int code, List<InstallationOrder> installation_order_list) {
                    Log.v(LOG_TAG, "PendingInstallations callback when SoftwareUpdateService started, what to do with this list?");
                }

                @Override
                public void DownloadInfo(String uuid, DownloadInfo info) {
                    //TODO: implement
                }
            };

            if (swapi != null) {
                try {
                    swapi.GetSoftwareAssigmentList(swapi_callback);
                    swapi.GetPendingInstallations(swapi_callback);
                } catch (RemoteException e) {
                    Log.e(LOG_TAG, "Cannot fetch Software Assignment List.. No contact with SWAPI.. I'm sad...");
                }
            }

        }

        @Override
        public void onServiceDisconnected() {
            Log.d(LOG_TAG, "Connection to SWAPI was lost. How to handle?");
            state = 0;
        }
    };

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();

        // Save context in order to be able to send it to SoftwareManagementApiConnectionCallback
        context = this;

        // Connect to FSApi
        swapi = new SoftwareManagementApi(context, swapi_callback);

        // Provide SUSApi
        software_update_manager = new SoftwareUpdateManagerImpl(this);

    }

    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");
        return software_update_manager.asBinder(); // Binder to SUSApi
    }

    public int GetState() {
        Log.e(LOG_TAG, "GetState not implemented");
        return state;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(LOG_TAG, "onStartCommand");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.v(LOG_TAG, "onDestroy");
        super.onDestroy();
    }

    public void GetSoftwareAssignmentList(ISoftwareUpdateManagerCallback callback) {
        /**
         * Construct a Callback tailored to the needs of this specific call. Maybe we can solve this in a much nicer way?
         */
        SoftwareManagementApiCallback swapiCallback = new SoftwareManagementApiCallback(callback);
        if (swapi != null) {
            try {
                swapi.GetSoftwareAssigmentList(swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Cannot fetch Software Assignment List.. No contact with SWAPI.. I'm sad...");
            }
        }
    }

    public void CommissionAssignment(ISoftwareUpdateManagerCallback callback, String uuid) {
        /**
         * Construct a Callback tailored to the needs of this specific call. Maybe we can solve this in a much nicer way?
         */
        SoftwareManagementApiCallback swapiCallback = new SoftwareManagementApiCallback(callback) {
            @Override
            public void CommissionStatus(int code) {
                if (200 == code) {
                    try {
                        callback.UpdateSoftwareState(uuid, 0); // Dont know what state to set, and state is not defined..
                    } catch (RemoteException e) {
                        Log.w(LOG_TAG, "Cannot event send error message. Client is super stupid...");
                    }
                } else {
                    try {
                        callback.ProvideErrorMessage(code, "Request for Software Assignment List failed.");
                    } catch (RemoteException e) {
                        Log.w(LOG_TAG, "Cannot event send error message. Client is super stupid...");
                    }
                }
            }
        };

        if (swapi != null) {
            try {
                swapi.CommissionSoftwareAssignment(uuid, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Cannot commission software assignment.. No contact with SWAPI.. I'm sad...");
            }
        } else {
            Log.e(LOG_TAG, "SWAPI null");
        }
    }

    public void GetPendingInstallations(ISoftwareUpdateManagerCallback callback) {
        /**
        * Construct a Callback tailored to the needs of this specific call. Maybe we can solve this in a much nicer way?
        */
        SoftwareManagementApiCallback swapiCallback = new SoftwareManagementApiCallback(callback);

        if (swapi != null) {
            try {
                Log.v(LOG_TAG, "GetPendingInstallations");
                swapi.GetPendingInstallations(swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Cannot fetch Pending Installations... No contact with SWAPI.. I'm sad...");
            }
        }
    }
}