/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.relay;

import android.app.Service;

import android.content.Intent;

import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;

import android.util.Log;

import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.LinkProperties;

import com.volvocars.connectivitymanager.ConnectivityManager;
import com.volvocars.connectivitymanager.IConnectivityManagerCallback;
import com.volvocars.connectivitymanager.WifiStationModeAidl;

import com.volvocars.connectivitymanager.relay.IConnectivityManagerRelay;
import com.volvocars.connectivitymanager.relay.IConnectivityManagerRelayCallback;

/**
*
*/
public class ConnectivityManagerRelayService extends Service {
    private static final String LOG_TAG = "ConManRelay.Service";


    // Handles to underlaying connection managers
    private com.volvocars.connectivitymanager.ConnectivityManager vccConnectivityManager = null;
    private android.net.ConnectivityManager androidConnectivityManager = null;

    // Interface to Settings (and maybe other apps)
    private ConnectivityManagerRelay connectivityManagerRelay = null;


    // private State state = NotActive;
    private WifiStationModeAidl wifiStationMode;

    // Callback given to VCC Connection Manager Gateway
    private IConnectivityManagerCallback connectionCallback = new IConnectivityManagerCallback() {
        @Override
        public void onServiceConnected() {
            Log.d(LOG_TAG, "Connected to Gateway");
            vccConnectivityManager.getWifiStationMode();
            //state = Active;
        }

        @Override
        public void onServiceDisconnected() {
            Log.d(LOG_TAG, "Disconnected");
            vccConnectivityManager = null;
            //state = NotActive;
        }

        @Override
        public void notifyWifiStationMode(WifiStationModeAidl mode) {
            Log.d(LOG_TAG, "Wifi Station Mode Notification with value: " + mode.mode);

            // Check some settings and then do something appropriate? Like setting the correct mode
            // and then act further on that.
            wifiStationMode = mode;
            connectivityManagerRelay.notifyWifiStationMode(mode);
        }
    };

    // Android Network Listener
    private android.net.ConnectivityManager.NetworkCallback networkActiveListener =
            new android.net.ConnectivityManager.NetworkCallback() {

                @Override
                public void onAvailable(Network network) {
                    Log.d(LOG_TAG, "onAvailable: " + network);
                }

                @Override
                public void onCapabilitiesChanged(
                        Network network, NetworkCapabilities networkCapabilities) {
                    Log.d(LOG_TAG, "onCapabilitiesChanged: " + network);
                }

                @Override
                public void onLinkPropertiesChanged(
                        Network network, LinkProperties linkProperties) {
                    Log.d(LOG_TAG, "onLinkPropertiesChanged: " + network);
                }

                @Override
                public void onLosing(Network network, int maxMsToLive) {
                    Log.d(LOG_TAG, "onLosing: " + network);
                }

                @Override
                public void onLost(Network network) {
                    Log.d(LOG_TAG, "onLost: " + network);
                }

                @Override
                public void onUnavailable() {
                    Log.d(LOG_TAG, "onUnavailable");
                }
            };

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();

        vccConnectivityManager =
                new com.volvocars.connectivitymanager.ConnectivityManager(this, connectionCallback);

        androidConnectivityManager =
                (android.net.ConnectivityManager) getSystemService(CONNECTIVITY_SERVICE);
        androidConnectivityManager.registerDefaultNetworkCallback(networkActiveListener);

        connectivityManagerRelay = new ConnectivityManagerRelay(this);
    }

    @Override
    public void onDestroy() {
        Log.v(LOG_TAG, "onDestroy");
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(LOG_TAG, "onStartCommand");

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");
        return connectivityManagerRelay.asBinder();
    }

    public void setWifiStationMode(WifiStationModeAidl mode) {
        Log.v(LOG_TAG, "setWifiStationMode");
        vccConnectivityManager.setWifiStationMode(mode);
    }

    public void getWifiStationMode() {
        Log.v(LOG_TAG, "getWifiStationMode");
        if(wifiStationMode != null)
            connectivityManagerRelay.notifyWifiStationMode(wifiStationMode);
    }
}
