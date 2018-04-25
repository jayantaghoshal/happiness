/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.settingsstorageservice;

import android.app.Service;
import android.app.Application;
import android.content.Intent;
import android.util.Log;

public class MainApplication extends Application {
    private static final String LOG_TAG = "SettingsStorageService";
    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(LOG_TAG, "[MainApp] SettingsStorageService.MainApplication created, sending intent to start SettingsStorageService");
        Intent intent = new Intent(this, SettingsStorageService.class);
        startService(intent);
    }
}