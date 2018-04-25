/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.settingsstorageservice;

import android.app.Service;
import android.content.Intent;
import android.content.Context;
import android.util.Log;
import android.os.IBinder;

public class SettingsStorageService extends Service {
    private static final String LOG_TAG = "SettingsStorageService";

    private SettingsStorageManagerImpl settingsStorageManagerImpl = null;
    private SettingsStorage settingsStorage = null;
    private SettingsReader settingsReader = null;

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "[Service] onCreate");
        super.onCreate();
        if (null == settingsStorageManagerImpl) {
            settingsStorageManagerImpl = new SettingsStorageManagerImpl();
        }
        if (null == settingsReader) {
            settingsReader = new SettingsReader();
            settingsReader.init(this);
        }
        if (null == settingsStorage) {
            settingsStorage = new SettingsStorage(this);
            settingsStorage.init();
            settingsStorageManagerImpl.init(settingsStorage, settingsReader);
        }

    }

    @Override
    public void onDestroy() {
        Log.v(LOG_TAG, "[Service] onDestroy");
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(LOG_TAG, "[Service] onStartCommand");
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "[Service] OnBind");
        return settingsStorageManagerImpl;
    }

}