/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnessservice;

import android.app.Application;
import android.content.Intent;
import android.util.Log;

public class MainApplication extends Application {
    private static final String TAG = "BrightnessService";
    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(TAG, "onCreate() MainApplication BrightnessService");
        Intent intent = new Intent(this, BrightnessService.class);
        startService(intent);
    }
}