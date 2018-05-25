/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.app.Service;
import android.app.Application;
import android.content.Intent;
import android.util.Log;

public class MainApplication extends Application {
    private static final String LOG_TAG = "SoftwareUpdateService";
    private static final String LOG_PREFIX = "[MainApplication]";

    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(LOG_TAG,
                LOG_PREFIX + " SoftwareUpdate.MainApplication created, sending intent to start SoftwareUpdateService");
        Intent intent = new Intent(this, SoftwareUpdateService.class);
        startService(intent);
    }
}