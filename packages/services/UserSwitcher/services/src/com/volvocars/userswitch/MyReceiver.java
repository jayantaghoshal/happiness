/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.app.ActivityManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.UserHandle;
import android.os.UserManager;
import android.util.Log;

public class MyReceiver extends BroadcastReceiver {
    public static final String TAG = "MyReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i(UserSwitchService.TAG, "Received BOOT_COMPLETED intent");
        // Add on all entry points such as boot_completed or other manifest-listed receivers and providers
        UserManager userManager = context.getSystemService(UserManager.class);
        if (!userManager.isSystemUser()) {
            // Disable the service
            ComponentName targetServiceName = new ComponentName(context, UserSwitchService.class);
            context.getPackageManager().setComponentEnabledSetting(
                    targetServiceName, PackageManager.COMPONENT_ENABLED_STATE_DISABLED, 0);
        } else {
            Log.d(TAG, "user is the SystemUser");
            if (!isMyServiceRunning(UserSwitchService.class, context)) {
                Log.d(TAG, "UserSwitchService not running");
                Intent serviceIntent = new Intent(UserSwitchService.class.getName());

                /*this is service name*/
                serviceIntent.setAction(ServiceInfo.ACTION);

                /*From 5.0 annonymous intent calls are suspended so replacing with server app's package name*/
                serviceIntent.setPackage(ServiceInfo.PACKAGE_NAME);

                // binding to remote service
                context.startServiceAsUser(serviceIntent, UserHandle.SYSTEM);
            } else {
                Log.d(TAG, "UserSwitchService is already running");
            }
        }
    }

    private boolean isMyServiceRunning(Class<?> serviceClass, Context context) {
        ActivityManager manager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
            if (serviceClass.getName().equals(service.service.getClassName())) {
                return true;
            }
        }
        return false;
    }
}

