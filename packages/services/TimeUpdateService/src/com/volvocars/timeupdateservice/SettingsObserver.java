/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.timeupdateservice;


import android.os.Handler;
import android.content.Context;
import android.content.ContentResolver;
import android.database.ContentObserver;
import android.os.Message;
import android.util.Log;
import android.provider.Settings;

public class SettingsObserver extends ContentObserver {
    private int mMsg;
    private Handler mHandler;
    private ContentResolver resolver;
    private static Context mContext;

    private final MainApplication mApplication;

    SettingsObserver(Handler handler, int msg, MainApplication application) {
        super(handler);
        mHandler = handler;
        mMsg = msg;
        mApplication = application;

    }

    void observe(Context context) {
        mContext = context;
        resolver = mContext.getContentResolver();
        resolver.registerContentObserver(Settings.Global.getUriFor(Settings.Global.AUTO_TIME), false, this);
    }

    @Override
    public void onChange(boolean selfChange) {
        Log.d(TimeUpdateLog.SERVICE_TAG, "Time Settings changed");
        try {
            Message message = mHandler.obtainMessage();
            boolean isAuto = mApplication.isAutomaticTimeRequested();
            mApplication.updateTimeOnSettingsChange(isAuto);

        } catch (Exception e) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "Settings Observer Unhandled exception" + e.getMessage());
        }
    }
}