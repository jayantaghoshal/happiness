/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.timeupdateservice;

import android.app.Application;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;


public class MainApplication extends Application {
    private static final String LOGTAG = "TimeUpdateService";

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(LOGTAG, String.format("Time Update Service listening to clock change events"));

    }
}
