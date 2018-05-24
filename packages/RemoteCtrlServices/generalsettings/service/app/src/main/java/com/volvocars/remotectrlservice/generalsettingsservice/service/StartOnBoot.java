/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservice.generalsettingsservice.service;

import android.app.ActivityManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.SystemProperties;
import android.util.Log;

public class StartOnBoot extends BroadcastReceiver {
    private static final String TAG = "RemoteCtrl.GeneralSettings.StartOnBoot";
    private static final String REMOTE_CTRL_AUTONOMOUS_DRIVE_MODE =
            "vendor.carconfig.CC100_AutonomousDriveType";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.v(TAG, "Intent received: " + intent.getAction());

        String autonomousDriveValue = SystemProperties.get(REMOTE_CTRL_AUTONOMOUS_DRIVE_MODE);
        if (!autonomousDriveValue.equals("3")) { // TODO: Remove magic number /Arjun Snehaj Lal
            Log.v(TAG, "Autonomous drive not enabled. Remote media control services not starting.");
            return;
        }

        if (intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {
            if (!isMyServiceRunning(com.volvocars.remotectrlservice.generalsettingsservice.service
                                            .RemoteGeneralSettingsService.class,
                        context)) {
                Intent serviceIntent =
                        new Intent(context, com.volvocars.remotectrlservice.generalsettingsservice.service
                                                    .RemoteGeneralSettingsService.class);
                context.startService(serviceIntent);

            } else {
                Log.v(TAG, "RemoteGeneralSettingsService is already running");
            }
        } else {
            Log.v(TAG, "Invalid intent action received.");
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
