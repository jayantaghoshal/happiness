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

import com.volvocars.cloudservice.FoundationServicesApi;
import com.volvocars.cloudservice.FoundationServicesApiConnectionCallback;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
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

    private FoundationServicesApi fsapi = null;
    private SoftwareManagementApi swapi = null;

    private int state = 0; // Dummy state

    private FoundationServicesApiConnectionCallback fsapi_callback = new FoundationServicesApiConnectionCallback() {
        @Override
        public void onServiceConnected() {
            Log.d(LOG_TAG, "Connected to FSAPI");

            // Set some state?

            // Check SWApi availability...
            try {
                if(fsapi.IsFeatureAvailable("SoftwareManagement")) {
                    // Start SWApi if available...
                    swapi = new SoftwareManagementApi(context, swapi_callback);
                } else {
                    Log.w(LOG_TAG, "Software Management not available, and will not be used.\n" +
                                   "TODO: Make IsFeatureAvailable to accept a callback that can be used from FSAPI");
                }
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Connection to FSAPI was lost. How to handle?");
                // Set some state?
            }
        }
        @Override
        public void onServiceDisconnected() {
            Log.d(LOG_TAG, "Connection Lost");
        }
    };

    private SoftwareManagementApiConnectionCallback swapi_callback = new SoftwareManagementApiConnectionCallback() {
        @Override
        public void onServiceConnected(){
            Log.d(LOG_TAG, "Connected to SWAPI");

            ISoftwareManagementApiCallback.Stub swapi_callback = new ISoftwareManagementApiCallback.Stub() {

                @Override
                public void CommissionStatus(int code) {
                    // Shouldnt be called...
                    Log.w(LOG_TAG, "GetSoftwareAssignmentList::CommissionStatus: Why am I called? o_O Please stop.");
                }

                @Override
                public void SoftwareAssignmentList(int code, List<SoftwareAssignment> software_list) {
                    String list = "";
                    if(code == 200) {
                        list +=  "#### SOFTWARE ASSIGNMENT LIST ####\n";
                        for(SoftwareAssignment s : software_list) {
                            list += s.toString() + "\n-------------\n";
                        }
                    } else {
                        list = "No List, HTTP Code: " + code;
                    }

                    Log.i(LOG_TAG, list);

                }};

            if(swapi != null) {
                try {
                    swapi.GetSoftwareAssigmentList(swapi_callback);
                } catch (RemoteException e) {
                    Log.e(LOG_TAG, "Cannot fetch Software Assignment List.. No contact with SWAPI.. I'm sad...");
                }
            }

            // Set some state?
        }
        @Override
        public void onServiceDisconnected(){
            Log.d(LOG_TAG, "Connection to SWAPI was lost. How to handle?");
            // Set some state?
        }
    };

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();

        // Save context in order to be able to send it to SoftwareManagementApiConnectionCallback
        context = this;

        // Connect to FSApi
        fsapi = new FoundationServicesApi(context, fsapi_callback);

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
        ISoftwareManagementApiCallback.Stub swapi_callback = new ISoftwareManagementApiCallback.Stub() {

            @Override
            public void CommissionStatus(int code) {
                // Shouldnt be called...
                Log.w(LOG_TAG, "GetSoftwareAssignmentList::CommissionStatus: Why am I called? o_O Please stop.");
            }

            @Override
            public void SoftwareAssignmentList(int code, List<SoftwareAssignment> software_list) {
                if(code == 200) {
                    try {
                        callback.UpdateSoftwareAssignmentList(software_list);
                    } catch (RemoteException e) {
                        Log.w(LOG_TAG, "Cannot update Software Assignment List. Client is stupid...");
                    }
                } else {
                    try {
                        callback.ProvideErrorMessage(code, "Request for Software Assignment List failed.");
                    } catch (RemoteException e) {
                        Log.w(LOG_TAG, "Cannot event send error message. Client is super stupid...");
                    }
                }
            }};

        if(swapi != null) {
            try {
                swapi.GetSoftwareAssigmentList(swapi_callback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Cannot fetch Software Assignment List.. No contact with SWAPI.. I'm sad...");
            }
        }
    }

    public void CommissionAssignment(String uuid) {
        /**
         * Construct a Callback tailored to the needs of this specific call. Maybe we can solve this in a much nicer way?
         */
        ISoftwareManagementApiCallback.Stub swapi_callback = new ISoftwareManagementApiCallback.Stub() {

            @Override
            public void CommissionStatus(int code) {
                if(code == 200) {
                    software_update_manager.UpdateSoftwareAssignmentState(uuid, 0); // Dont know what state to set, and state is not defined..
                } else {
                    Log.w(LOG_TAG, "Commissioning of Sofware " + uuid +" failed. I dont know what to do?");
                }
            }

            @Override
            public void SoftwareAssignmentList(int code, List<SoftwareAssignment> software_list) {
                // Shouldnt be called...
                Log.w(LOG_TAG, "CommissionAssignment::SoftwareAssignmentList: Why am I called? o_O Please stop.");
            }};

        if(swapi != null) {
            try {
                swapi.CommissionSoftwareAssignment(uuid, swapi_callback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Cannot commission software assignment.. No contact with SWAPI.. I'm sad...");
            }
        }
    }

}