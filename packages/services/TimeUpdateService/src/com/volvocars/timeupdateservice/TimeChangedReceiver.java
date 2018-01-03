/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.timeupdateservice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import android.util.Log;

public class TimeChangedReceiver extends BroadcastReceiver {
    private static final String LOGTAG = "ClockChangeNotifier";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(LOGTAG, String.format("Intent Received: Time Changed Manually"));
    }

}