/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.RemoteException;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.Switch;
import com.volvocars.softwareupdate.*;
import java.util.*;

public class SettingsActivity extends AppCompatActivity {

    private final String LOG_TAG = "SoftwareUpdateApp";
    private final String LOG_PREFIX = "SettingsActivity";
    private SoftwareUpdateManager softwareUpdateManager = null;

    private LinearLayout settingsLayout;
    private Switch enableOtaSwitch;
    private Switch autoDownloadSwitch;

    private Intent intent;

    private final String ENABLE_OTA = "ota_enabled";
    private final String AUTO_DOWNLOAD = "automatic_download_enabled";

    private ISoftwareUpdateSettingsCallback callback = new ISoftwareUpdateSettingsCallback.Stub() {

        @Override
        public void UpdateSetting(String key, boolean value) {
            updateSettings(key, value);
        }

    };
    private SoftwareUpdateManagerCallback softwareUpdateManagerCallback = new SoftwareUpdateManagerCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, LOG_PREFIX + " onServiceConnected app");
            try {
                softwareUpdateManager.RegisterSettingsClient(callback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, LOG_PREFIX + " Error RegisterSettingsClient");
            }
        }

        @Override
        public void onServiceDisconnected() {
            Log.v(LOG_TAG, LOG_PREFIX + " onServiceDisconnected app");
            try {
                softwareUpdateManager.UnregisterSettingsClient(callback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, LOG_PREFIX + " Error UnegisterSettingsClient");
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.v(LOG_TAG, LOG_PREFIX + " onCreate");
        softwareUpdateManager = new SoftwareUpdateManager(this, softwareUpdateManagerCallback);

        setContentView(R.layout.layout_settings);

        Toolbar myChildToolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(myChildToolbar);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        settingsLayout = (LinearLayout) findViewById(R.id.settingsLayout);
        enableOtaSwitch = (Switch) findViewById(R.id.enableOta);

        autoDownloadSwitch = (Switch) findViewById(R.id.autoDownload);

        enableOtaSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                try {
                    softwareUpdateManager.SetSetting(ENABLE_OTA, isChecked);
                } catch (RemoteException e) {
                    Log.w(LOG_TAG, LOG_PREFIX + " RemoteException " + e.getMessage());
                }
            }
        });

        autoDownloadSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                try {
                    softwareUpdateManager.SetSetting(AUTO_DOWNLOAD, isChecked);
                } catch (RemoteException e) {
                    Log.w(LOG_TAG, LOG_PREFIX + " RemoteException " + e.getMessage());
                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.v(LOG_TAG, LOG_PREFIX + " onDestroy");
        softwareUpdateManager.disconnect();
    }

    private void updateSettings(String key, boolean value) {
        Log.v(LOG_TAG, LOG_PREFIX + " updateSettings");

        if (key.equals(ENABLE_OTA)) runOnUiThread(() -> enableOtaSwitch.setChecked(value));
        if (key.equals(AUTO_DOWNLOAD)) runOnUiThread(() -> autoDownloadSwitch.setChecked(value));
    }
}