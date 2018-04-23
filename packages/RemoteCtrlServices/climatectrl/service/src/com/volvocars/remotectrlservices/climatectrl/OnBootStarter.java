/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl;

import android.app.Service;
import android.content.Intent;
import android.content.Context;
import android.content.BroadcastReceiver;
import android.os.SystemProperties;
import android.util.Log;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningServiceInfo;

import com.volvocars.remotectrlservices.climatectrl.service.RemoteClimateService;

public class OnBootStarter extends BroadcastReceiver {
    private static final String TAG = "RemoteCtrl.ClimateCtrl.OnBootStarter";

    private static final String PERMISSION_REMOTE_CTRL = "android.volvocars.permission.REMOTE_CTRL";
    private static final String REMOTE_CTRL_AUTONOMOUS_DRIVE_MODE =
            "vendor.carconfig.CC100_AutonomousDriveType";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.v(TAG, "On boot starter invoked.");

        String autonomousDriveValue = SystemProperties.get(REMOTE_CTRL_AUTONOMOUS_DRIVE_MODE);
        if (!autonomousDriveValue.equals("3")) { // TODO: Remove magic number /Philip Werner
            Log.v(TAG, "Autonomous drive not enabled. Remote climate control services not starting.");
            return;
        }

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
