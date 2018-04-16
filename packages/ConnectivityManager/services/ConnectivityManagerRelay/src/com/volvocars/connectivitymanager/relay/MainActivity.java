/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.relay;

import android.support.v7.app.AppCompatActivity;

import com.volvocars.connectivitymanager.WifiStationModeAidl;
import com.volvocars.connectivitymanager.WifiStationModeAidl.Mode;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;

import android.os.Bundle;
import android.os.IBinder;

import android.util.Log;

import android.widget.Switch;
import android.widget.CompoundButton;

import com.volvocars.connectivitymanager.relay.IConnectivityManagerRelayCallback;

public class MainActivity extends AppCompatActivity {
    private final String LOG_TAG = "ConManRelay.MainActivity";

    private Switch wifiStationModeSwitch = null;

    private ConnectivityManagerRelay conManRelayService = null;
    private Boolean boundToService = false;

    private ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.v(LOG_TAG, "onServiceConnected " + name + " " + service);
            conManRelayService = (ConnectivityManagerRelay) service;
            conManRelayService.registerCallback(callback);
            boundToService = true;
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            Log.v(LOG_TAG, "onServiceConnected " + name);
            conManRelayService = null;
            boundToService = false;
        }

        @Override
        public void onBindingDied(ComponentName name) {
            Log.v(LOG_TAG, "onBindingDied: " + name);
        }
    };

    private IConnectivityManagerRelayCallback.Stub callback =
            new IConnectivityManagerRelayCallback.Stub() {

                @Override
                public void notifyWifiStationMode(WifiStationModeAidl mode) {
                    Log.v(LOG_TAG, "notifyWifiStationMode " + mode);
                    wifiStationModeSwitch.setClickable(true);
                    wifiStationModeSwitch.setAlpha(1.0f);

                    if (Mode.STATION_MODE == mode.mode) {
                        wifiStationModeSwitch.setChecked(true);
                    } else {
                        wifiStationModeSwitch.setChecked(false);
                    }
                }
            };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.v(LOG_TAG, "onCreate");

        wifiStationModeSwitch = (Switch) findViewById(R.id.wifiStationSwitch);
        wifiStationModeSwitch.setOnCheckedChangeListener(
                new CompoundButton.OnCheckedChangeListener() {
                    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                        wifiStationModeSwitch.setClickable(false);
                        wifiStationModeSwitch.setAlpha(.5f);
                        WifiStationModeAidl wifiMode = new WifiStationModeAidl();
                        if (isChecked) {
                            // The toggle is enabled
                            Log.v(LOG_TAG, "WifiStationMode On");
                            wifiMode.mode = Mode.STATION_MODE;

                        } else {
                            // The toggle is disabled
                            Log.v(LOG_TAG, "WifiStationMode Off");
                            wifiMode.mode = Mode.AP_MODE;
                        }
                        if (boundToService) {
                            conManRelayService.setWifiStationMode(wifiMode);
                        } else {
                            Log.v(LOG_TAG, "Service not bound, cannot send request..");
                        }
                    }
                });

        Intent intent = new Intent(this, ConnectivityManagerRelayService.class);
        bindService(intent, serviceConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.v(LOG_TAG, "onDestroy");
    }
}