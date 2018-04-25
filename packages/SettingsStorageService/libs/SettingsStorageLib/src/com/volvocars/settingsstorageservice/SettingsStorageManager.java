/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.settingsstorageservice;

import android.util.Log;
import android.app.Service;
import android.content.*;
import android.os.IBinder;
import android.os.RemoteException;

public class SettingsStorageManager implements ServiceConnection {
    private static final String LOG_TAG = "SettingsStorageService";
    private static final String PACKAGENAME = "com.volvocars.settingsstorageservice";
    private static final String PACKAGENAME_SERVICENAME = "com.volvocars.settingsstorageservice.SettingsStorageService";

    private Context context = null;
    private ISettingsStorageManager settingsStorageManager = null;
    private boolean serviceBound = false;

    private SettingsStorageManagerConnectionCallback settingsStorageManagerCallback = null;

    private DeathRecipient death = new DeathRecipient(this);

    final class DeathRecipient implements IBinder.DeathRecipient {

        private SettingsStorageManager client;
        private int retries = 0;

        DeathRecipient(SettingsStorageManager client) {
            this.client = client;
        }

        @Override
        public void binderDied() {
            Log.e(LOG_TAG, "[Lib] SettingsStorageService died");

            if (retries < 5) {
                Log.d(LOG_TAG, "[Lib] Trying SettingsStorageService again... Attempt " + (retries + 1));
                retries++;
                client.connect();
            } else {
                Log.d(LOG_TAG, "[Lib] SettingsStorageService seems unreliable, no more attempts to connect.");
            }
        }
    }

    @Override
    public void onServiceConnected(ComponentName className, IBinder service) {
        settingsStorageManager = ISettingsStorageManager.Stub.asInterface(service);
        serviceBound = true;
        settingsStorageManagerCallback.onServiceConnected();
    }

    @Override
    public void onServiceDisconnected(ComponentName arg0) {
        serviceBound = false;
        settingsStorageManagerCallback.onServiceDisconnected();
    }

    public SettingsStorageManager(Context context,
            SettingsStorageManagerConnectionCallback settingsStorageManagerCallback) {
        this.context = context;
        this.settingsStorageManagerCallback = settingsStorageManagerCallback;
        connect();
    }

    public void connect() {
        Intent intent = new Intent();
        intent.setAction("SettingsStorageManager");
        intent.setComponent(new ComponentName(PACKAGENAME, PACKAGENAME_SERVICENAME));
        context.bindService(intent, this, Context.BIND_AUTO_CREATE);
    }

    public void disconnect() {
        context.unbindService(this);
        serviceBound = false;
    }

    /**
     * Set value to specified key
     *
     * @param appId Identification of application
     * @param key   Key to set value to
     * @param value Value to set to key
     */
    public void setByString(String appId, String key, String value) throws RemoteException {
        if (settingsStorageManager != null && serviceBound) {
            settingsStorageManager.setByString(appId, key, value);
        } else {
            Log.w(LOG_TAG, "[Lib] couldn't set value "
                    + (serviceBound ? "settingsStorageManager is not initialized!" : "service is not bound!"));
        }
    }

    /**
     * Set value to specified key
     *
     * @param appId Identification of application
     * @param key   Key to set value to
     * @param value Value to set to key
     */
    public void setByInt(String appId, int key, String value) throws RemoteException {
        if (settingsStorageManager != null && serviceBound) {
            settingsStorageManager.setByInt(appId, key, value);
        } else {
            Log.w(LOG_TAG, "[Lib] couldn't set value "
                    + (serviceBound ? "settingsStorageManager is not initialized!" : "service is not bound!"));
        }
    }

    /**
     * Get value of specified key (value returned in ISettingsStorageManager
     * keyValue)
     *
     * @param appId    Identification of application
     * @param key      Key to get value of
     * @param callback Callback
     */
    public void getAsyncByString(String appId, String key, ISettingsStorageManagerCallback callback)
            throws RemoteException {
        if (settingsStorageManager != null && serviceBound) {
            settingsStorageManager.getAsyncByString(appId, key, callback);
        } else {
            Log.w(LOG_TAG, "[Lib] couldn't get value "
                    + (serviceBound ? "settingsStorageManager is not initialized!" : "service is not bound!"));
        }
    }

    /**
     * Get value of specified key (value returned in ISettingsStorageManager
     * keyValue)
     *
     * @param appId    Identification of application
     * @param key      Key to get value of
     * @param callback Callback
     */
    public void getAsyncByInt(String appId, int key, ISettingsStorageManagerCallback callback) throws RemoteException {
        if (settingsStorageManager != null && serviceBound) {
            settingsStorageManager.getAsyncByInt(appId, key, callback);
        } else {
            Log.w(LOG_TAG, "[Lib] couldn't get value "
                    + (serviceBound ? "settingsStorageManager is not initialized!" : "service is not bound!"));
        }
    }
}