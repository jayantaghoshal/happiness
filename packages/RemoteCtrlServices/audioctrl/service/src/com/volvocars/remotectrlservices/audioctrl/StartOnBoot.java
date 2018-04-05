/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.audioctrl;

import android.app.Service;
import android.content.Intent;
import android.content.Context;
import android.content.BroadcastReceiver;
import android.util.Log;
import android.os.SystemProperties;
import android.app.ActivityManager;

public class StartOnBoot extends BroadcastReceiver {
    private static final String TAG = "RemoteCtrl.AudioCtrl.StartOnBoot";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.v(TAG, "Intent received: " + intent.getAction());

        if (intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {
            if (!isMyServiceRunning(AudioCtrlService.class, context)) {
                try {
                    String autonomousDriveValue =
                            SystemProperties.get("vendor.carconfig.CC100_AutonomousDriveType");
                    if (autonomousDriveValue.equals("3")) {
                        Intent serviceIntent = new Intent(context, AudioCtrlService.class);
                        context.startService(serviceIntent);
                    } else {
                        Log.e(TAG, "Unset or Wrong AutonomouseDriveType");
                    }
                } catch (Exception ex) {
                    Log.e(TAG, "Error occured while starting service" + ex.getMessage());
                }
            } else {
                Log.d(TAG, "AudioCtrlService is already running");
            }
        } else {
            Log.e(TAG, "Invalid intent action received.");
        }
    }

    private boolean isMyServiceRunning(Class<?> serviceClass, Context context) {
        ActivityManager manager =
                (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningServiceInfo service :
                manager.getRunningServices(Integer.MAX_VALUE)) {
            if (serviceClass.getName().equals(service.service.getClassName())) {
                return true;
            }
        }
        return false;
    }
}