/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudtest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import vendor.volvocars.cloudservice.*;
import android.os.RemoteException;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;
import android.os.SystemProperties;


public class MainActivity extends Activity {
    private static final String LOG_TAG = "Cloudtest";
    private FsApi fsapi = null;
    private int counter = 0;
    private Boolean serviceConnected = false;

    private FsApiConnectionCallback fsApiConnectionCallback = new FsApiConnectionCallback(){
        @Override
        public void onServiceConnected(){
            serviceConnected=true;
            Log.d(LOG_TAG, "onServiceConnected app");
        }
        @Override
        public void onServiceDisconnected(){
            serviceConnected=false;
            Log.d(LOG_TAG, "onServiceDisconnected app");
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
    @Override
    protected void onStart() {
        super.onStart();
        fsapi = new FsApi(this,fsApiConnectionCallback);
    }
    public void IsFeatureAvailableCarsharing() throws RemoteException{
        Log.d(LOG_TAG, "Testing is IsFeatureAvailable carsharing");
        boolean avail=fsapi.IsFeatureAvailable("carsharing");
        Log.d(LOG_TAG, "IsFeatureAvailable:(carsharing) " + avail);
        assertEquals(avail,true);
    }
    public void IsFeatureAvailableOta2() throws RemoteException{
        Log.d(LOG_TAG, "Testing is IsFeatureAvailable ota2");
        boolean avail=fsapi.IsFeatureAvailable("ota2");
        Log.d(LOG_TAG, "IsFeatureAvailable:(ota2) " + avail);
        assertEquals(avail,false);
    }
    public void IsServiceConnected(){
        assertEquals(serviceConnected,true);
    }
}


