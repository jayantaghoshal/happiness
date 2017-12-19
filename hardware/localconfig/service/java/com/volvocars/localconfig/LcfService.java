/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.localconfig;

import android.content.Context;
import android.app.Service;
import android.volvocars.localconfig.LocalConfig;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class LcfService extends Service {

    private ILocalConfigImpl mILocalConfigImpl;

    @Override
    public void onCreate() {
        Log.i(LcfLog.TAG_SERVICE, "Service onCreate");
        mILocalConfigImpl = ILocalConfigImpl.getInstance(this);
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        Log.i(LcfLog.TAG_SERVICE, "Service onDestroy");
        ILocalConfigImpl.releaseInstance();
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(LcfLog.TAG_SERVICE, "Service onStartCommand");
        /* Tells the OS to recreate the service after it has enough memory */
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mILocalConfigImpl;
    }

}