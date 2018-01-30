/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class MyReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i(UserSwitchService.TAG, "Received BOOT_COMPLETED intent");
        Intent myIntent = new Intent(context, UserSwitchService.class);
        context.startService(myIntent);
    }
}

