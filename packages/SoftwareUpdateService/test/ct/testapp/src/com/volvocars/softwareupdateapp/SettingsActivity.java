/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.app.Activity;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.Switch;

public class SettingsActivity extends AppCompatActivity {

    private final String LOG_TAG = "SwUpdApp.SettingsActivity";

    private Switch enableOta;
    private Switch autoDownload;
    private Switch autoInstall;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(LOG_TAG, "onCreate");
        setContentView(R.layout.layout_settings);

        Toolbar myChildToolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(myChildToolbar);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        enableOta = (Switch) findViewById(R.id.enableOta);
        autoDownload = (Switch) findViewById(R.id.autoDownload);
        autoInstall = (Switch) findViewById(R.id.autoInstall);

        enableOta.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Log.d(LOG_TAG, "enableOta: " + isChecked);
            }
        });

        autoDownload.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Log.d(LOG_TAG, "autoDownload: " + isChecked);
            }
        });

        autoInstall.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                Log.d(LOG_TAG, "autoInstall: " + isChecked);
            }
        });

    }

}