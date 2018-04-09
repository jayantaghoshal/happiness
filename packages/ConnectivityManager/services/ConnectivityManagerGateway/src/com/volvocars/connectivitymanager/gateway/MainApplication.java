/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.gateway;

import android.app.Service;
import android.app.Application;
import android.content.Intent;
import android.util.Log;

public class MainApplication extends Application {
    private static final String LOGTAG = "ConManGw.MainApp";
    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(LOGTAG, "Starting Service");
        Intent intent = new Intent(this, GateWayService.class);
        startService(intent);
    }
}