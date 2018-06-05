/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservice.generalsettingsservice.service;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.os.IBinder;
import android.os.UserHandle;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

/*Checks the brightness of the screen and notifies RemoteGeneralSettingsService on a brightness
 * change*/
@SuppressLint("NewApi")
public class CheckDisplayBrightnessService extends Service {
    private static final String broadcastAction =
            "generalsettingsservice.broadcastaction.checkoverlaystate";
    private static final String TAG =
            "RemoteCtrl.GeneralSettings.Service.CheckDisplayBrightnessService";
    private static final String sharedPreferencesKey = "com.volvocars.generalsettingsservice.displayinformation";
    // Intentional wrong values for brightness
    private int oldBrightness = -1;
    private int currentBrightness = -1;
    private Context mContext;
    private Timer timer;
    private TimerTask timerTask = new TimerTask() {
        @Override
        public void run() {
            try {
                currentBrightness = getBrightness();
                if (oldBrightness != currentBrightness) {
                    notifyGeneralSettingsService();
                    oldBrightness = currentBrightness;
                }
            } catch (Settings.SettingNotFoundException e) {
                Log.e(TAG, "error getting brightness " + e.getMessage());
            }
        }

    };
    @Override
    public void onCreate() {
        Log.v(TAG, "onCreate");
        mContext = this;
        SharedPreferences mSharedPreferences;
        mSharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
        int displaystate = mSharedPreferences.getInt(sharedPreferencesKey, MODE_PRIVATE);

        if (displaystate == 0) {
            oldBrightness = 0;
        } else {
            oldBrightness = 255;
        }
        start();
    }

    private void notifyGeneralSettingsService() throws Settings.SettingNotFoundException {
        Intent intent = new Intent(broadcastAction);
        oldBrightness = getBrightness();
        intent.putExtra("BrightnessValue", oldBrightness);
        // notifying GeneralSettingsService
        LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
    }
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.e(TAG, "onStartCommand");
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(TAG, "bind CheckdisplayBrightness");
        return null;
    }

    private void start() {
        Log.v(TAG, "Start");

        if (timer != null) {
            return;
        }
        timer = new Timer();
        timer.schedule(timerTask, 0, 500L);
    }

    @Override
    public void onDestroy() {
        Log.i(TAG, "onDestroy");
        timer.purge();
        super.onDestroy();
    }

    private int getBrightness() throws Settings.SettingNotFoundException {
        int curBrightnessValue = android.provider.Settings.System.getIntForUser(
                getContentResolver(), android.provider.Settings.System.SCREEN_BRIGHTNESS,
                UserHandle.USER_CURRENT);

        return curBrightnessValue;
    }
}