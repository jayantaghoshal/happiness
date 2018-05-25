/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.app.Service;
import android.app.Application;
import android.content.Intent;
import android.util.Log;

public class MainApplication extends Application {
    private static final String LOG_TAG = "CloudService";
    private static final String LOG_PREFIX = "[MainApplication]";
    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(LOG_TAG, LOG_PREFIX + " CloudService.MainApplication created, sending intent to start CloudService");
        Intent intent = new Intent(this, CloudService.class);
        startService(intent);
    }
}