/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.softwareupdate;


import vendor.volvocars.cloudservice.FoundationServicesApi;
import vendor.volvocars.cloudservice.FoundationServicesApiConnectionCallback;
import vendor.volvocars.cloudservice.SoftwareManagementApi;
import vendor.volvocars.cloudservice.SoftwareManagementApiConnectionCallback;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.content.Intent;
import android.os.Bundle;
import java.util.ArrayList;
import android.content.*;

/**
*
*/
public class SoftwareUpdateService extends Service {

    private static final String LOG_TAG = "SwUpdService";

    private Context context;

    private FoundationServicesApi fsapi = null;
    private SoftwareManagementApi swapi = null;


    private FoundationServicesApiConnectionCallback fsapi_callback = new FoundationServicesApiConnectionCallback() {
        @Override
        public void onServiceConnected() {
            Log.d(LOG_TAG, "onServiceConnected app");

            // Check SWApi availability...



            // Start SWApi if available....
            swapi = new SoftwareManagementApi(context, swapi_callback);

        }
        @Override
        public void onServiceDisconnected() {
            Log.d(LOG_TAG, "Connection Lost");
        }
    };

    private SoftwareManagementApiConnectionCallback swapi_callback = new SoftwareManagementApiConnectionCallback() {
        @Override
        public void onServiceConnected(){
            Log.d(LOG_TAG, "onServiceConnected app");
        }
        @Override
        public void onServiceDisconnected(){
            Log.d(LOG_TAG, "Connection Lost");
        }
    };

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();

        context = this;

        // Connect to FSApi
        fsapi = new FoundationServicesApi(context, fsapi_callback);


        // Provide SUSApi

    }

    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");
        return null; // Binder to SUSApi
    }

}