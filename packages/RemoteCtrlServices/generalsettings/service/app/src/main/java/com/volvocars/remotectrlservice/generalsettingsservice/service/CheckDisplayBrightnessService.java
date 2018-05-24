/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservice.generalsettingsservice.service;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
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

    // Intentional wrong values for brightness
    private int oldBrightness = -1;
    private int currentBrightness = -1;

    private Timer timer;
    private TimerTask timerTask = new TimerTask() {

        @Override
        public void run() {
            try {
                currentBrightness = getBrightness();
            } catch (Settings.SettingNotFoundException e) {
                Log.e(TAG, "error getting brightness" + e.getMessage());
            }
            if (currentBrightness != oldBrightness && currentBrightness == 0) {
                notifyGeneralSettingsService(false);
                oldBrightness = currentBrightness;
            } else if (currentBrightness != 0 && oldBrightness == 0) {
                notifyGeneralSettingsService(true);
                oldBrightness = currentBrightness;
            }
        }
    };
    @Override
    public void onCreate() {
        Log.v(TAG, " onCreate");

        start();
    }

    private void notifyGeneralSettingsService(boolean brightnessValue) {
        Intent intent = new Intent(broadcastAction);
        intent.putExtra("BrightnessValue", brightnessValue);
        // notifying GeneralSettingsService
        LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(TAG, "bind CheckdisplayBrightness");
        return null;
    }

    private void start() {
        if (timer != null) {
            return;
        }
        timer = new Timer();
        timer.schedule(timerTask, 0, 500L);
    }

    private int getBrightness() throws Settings.SettingNotFoundException {
        return Settings.System.getInt(this.getContentResolver(), Settings.System.SCREEN_BRIGHTNESS);
    }
}