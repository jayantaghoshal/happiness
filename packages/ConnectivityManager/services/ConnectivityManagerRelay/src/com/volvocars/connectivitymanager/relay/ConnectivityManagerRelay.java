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
import android.os.RemoteException;

import android.util.Log;

import com.volvocars.connectivitymanager.WifiStationModeAidl;
import com.volvocars.connectivitymanager.relay.IConnectivityManagerRelay;

/**
*
*/
public class ConnectivityManagerRelay extends IConnectivityManagerRelay.Stub {
    private static final String LOG_TAG = "ConManRelay.ConManRelay";

    private ConnectivityManagerRelayService service = null;
    private IConnectivityManagerRelayCallback callback = null;

    public ConnectivityManagerRelay(ConnectivityManagerRelayService service) {
        this.service = service;
    }

    @Override
    public void registerCallback(IConnectivityManagerRelayCallback callback) {
        Log.v(LOG_TAG, "registerCallback");
        this.callback = callback;
    }

    ///// Wifi Control /////

    @Override
    public void getWifiStationMode() {
        Log.v(LOG_TAG, "getWifiStationMode");
        service.getWifiStationMode();
    }

    @Override
    public void setWifiStationMode(WifiStationModeAidl mode) {
        Log.v(LOG_TAG, "setWifiStationMode " + mode);
        service.setWifiStationMode(mode);
    }

    public void notifyWifiStationMode(WifiStationModeAidl mode) {
        Log.v(LOG_TAG, "notifyWifiStationMode " + mode);
        if (null != callback) {
            try {
                callback.notifyWifiStationMode(mode);
            } catch (RemoteException e) {
                callback = null;
            }
        } else {
            Log.v(LOG_TAG, "No Callback registered");
        }
    }
}
