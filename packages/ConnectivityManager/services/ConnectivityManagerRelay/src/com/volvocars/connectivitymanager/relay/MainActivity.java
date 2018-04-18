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

import android.view.View;

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

            conManRelayService.getWifiStationMode();
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
            boundToService = false;
        }
    };

    private IConnectivityManagerRelayCallback.Stub callback =
            new IConnectivityManagerRelayCallback.Stub() {

                @Override
                public void notifyWifiStationMode(WifiStationModeAidl mode) {
                    Log.v(LOG_TAG, "notifyWifiStationMode: " + mode);

                    getMainExecutor().execute(new Runnable() {
                        @Override
                        public void run() {
                            boolean res = false;
                            if (Mode.STATION_MODE == mode.mode) {
                                res = true;
                            }

                            wifiStationModeSwitch.setClickable(true);
                            wifiStationModeSwitch.setAlpha(1.0f);
                            wifiStationModeSwitch.setChecked(res);
                        }
                    });
                }
            };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.v(LOG_TAG, "onCreate");

        boundToService = false;

        wifiStationModeSwitch = (Switch) findViewById(R.id.wifiStationSwitch);

        // Should not be clickable until we know the state of the chip. And/or we dont have
        // connection with tcam
        wifiStationModeSwitch.setClickable(false);
        wifiStationModeSwitch.setAlpha(.5f);

        wifiStationModeSwitch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View buttonView) {
                Log.v(LOG_TAG, "clicked");
                wifiStationModeSwitch.setClickable(false);
                wifiStationModeSwitch.setAlpha(.5f);
                WifiStationModeAidl wifiMode = new WifiStationModeAidl();
                if (wifiStationModeSwitch.isChecked()) {
                    Log.v(LOG_TAG, "WifiStationMode On");
                    wifiMode.mode = Mode.STATION_MODE;
                } else {
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
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.v(LOG_TAG, "onStart");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.v(LOG_TAG, "onResume");
        if (!boundToService) {
            Intent intent = new Intent(this, ConnectivityManagerRelayService.class);
            bindService(intent, serviceConnection, Context.BIND_AUTO_CREATE);
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.v(LOG_TAG, "onPause");
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.v(LOG_TAG, "onStop");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.v(LOG_TAG, "onDestroy");
    }
}