/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl;

import android.app.Service;
import android.content.Intent;
import android.content.Context;
import android.content.BroadcastReceiver;
import android.util.Log;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningServiceInfo;

import com.volvocars.remotectrlservices.climatectrl.service.RemoteClimateService;

public class OnBootStarter extends BroadcastReceiver {
    private static final String TAG = "RemoteCtrl.ClimateCtrl.OnBootStarter";

    private static final String PERMISSION_REMOTE_CTRL = "android.volvocars.permission.REMOTE_CTRL";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.v(TAG, "On boot starter invoked.");

        if (intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {
            Log.v(TAG, "Intent received: android.intent.action.BOOT_COMPLETED");

            if (!isMyServiceRunning(context, RemoteClimateService.class)) {
                Intent serviceIntent = new Intent(context, RemoteClimateService.class);
                context.startService(serviceIntent);
                Log.v(TAG, "Sent intent to start Remote Climate Service");
            } else {
                Log.v(TAG, "Remote Climate Service is already running");
            }
        }
    }

    private boolean isMyServiceRunning(Context context, Class<?> serviceClass) {
        ActivityManager manager =
                (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        for (RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
            if (serviceClass.getName().equals(service.service.getClassName())) {
                return true;
            }
        }
        return false;
    }
}
