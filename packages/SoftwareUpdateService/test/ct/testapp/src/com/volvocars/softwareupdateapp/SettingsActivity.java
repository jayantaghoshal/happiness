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
import com.volvocars.softwareupdate.Setting;
import com.volvocars.softwareupdate.*;
import java.util.*;

public class SettingsActivity extends AppCompatActivity {

    private final String LOG_TAG = "SwUpdApp.SettingsActivity";
    private SoftwareUpdateManager softwareUpdateManager = null;

    private LinearLayout settingsLayout;
    private Switch enableOtaSwitch;
    private Switch autoDownloadSwitch;
    private Switch autoInstallSwitch;

    private Intent intent;
    private ISoftwareUpdateSettingsCallback callback = new ISoftwareUpdateSettingsCallback.Stub() {

        @Override
        public void UpdateSettings(List<Setting> settings) {
            updateSettings(settings);
        }

    };

    private SoftwareUpdateManagerCallback softwareUpdateManagerCallback = new SoftwareUpdateManagerCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, "onServiceConnected app");
            try {
                softwareUpdateManager.RegisterSettingsClient(callback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Error RegisterSettingsClient");
            }
        }

        @Override
        public void onServiceDisconnected() {
            Log.v(LOG_TAG, "onServiceDisconnected app");
            try {
                softwareUpdateManager.UnregisterSettingsClient(callback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Error UnegisterSettingsClient");
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.v(LOG_TAG, "onCreate");
        softwareUpdateManager = new SoftwareUpdateManager(this, softwareUpdateManagerCallback);

        setContentView(R.layout.layout_settings);

        Toolbar myChildToolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(myChildToolbar);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        settingsLayout = (LinearLayout) findViewById(R.id.settingsLayout);
        enableOtaSwitch = (Switch) findViewById(R.id.enableOta);
        //enableOtaSwitch.setChecked(enableOta);

        autoDownloadSwitch = (Switch) findViewById(R.id.autoDownload);
        autoInstallSwitch = (Switch) findViewById(R.id.autoInstall);
        //autoInstallSwitch.setChecked(autoInstall);

        enableOtaSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Setting setting = new Setting();
                setting.type = Setting.SettingType.ENABLE_OTA;
                setting.value = isChecked;

                try {
                    softwareUpdateManager.SetSetting(setting);
                } catch (RemoteException e) {
                    Log.w(LOG_TAG, "RemoteException " + e.getMessage());
                }
            }
        });

        autoDownloadSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Log.d(LOG_TAG, "onCheckChanged AutoDownload: " + isChecked);
                Setting setting = new Setting();
                setting.type = Setting.SettingType.AUTO_DOWNLOAD;
                setting.value = isChecked;

                try {
                    softwareUpdateManager.SetSetting(setting);
                } catch (RemoteException e) {
                    Log.w(LOG_TAG, "RemoteException " + e.getMessage());
                }
            }
        });

        autoInstallSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Setting setting = new Setting();
                setting.type = Setting.SettingType.AUTO_INSTALL;
                setting.value = isChecked;

                try {
                    softwareUpdateManager.SetSetting(setting);
                } catch (RemoteException e) {
                    Log.w(LOG_TAG, "RemoteException " + e.getMessage());
                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.v(LOG_TAG, "onDestroy");
        softwareUpdateManager.disconnect();
    }

    private void updateSettings(List<Setting> settings) {
        Log.v(LOG_TAG, "updateSettings");
        for (Setting setting : settings) {
            Log.d(LOG_TAG, "Type: " + setting.type + " Value: " + setting.value);
            switch (setting.type) {
            case ENABLE_OTA:
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        enableOtaSwitch.setChecked(setting.value);
                    }
                });
                break;
            case AUTO_DOWNLOAD:
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        autoDownloadSwitch.setChecked(setting.value);
                    }
                });
                break;
            case AUTO_INSTALL:
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        autoInstallSwitch.setChecked(setting.value);
                    }
                });
                break;
            default:
                Log.v(LOG_TAG, "Unknown setting, don't know what to do");
            }
        }

    }
}