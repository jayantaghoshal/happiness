/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.gateway;

import android.util.Log;
import android.os.RemoteException;
import android.os.SystemProperties;

import com.volvocars.connectivitymanager.IConnectivityManager;
import com.volvocars.connectivitymanager.IConnectivityManagerGateway;
import com.volvocars.connectivitymanager.WifiStationMode;

/**
 *
 */
public class ConnectivityManagerGateway extends IConnectivityManagerGateway.Stub {
    private static final String LOG_TAG = "ConManGw.";

    public ConnectivityManagerGateway() {}

    public void init() {}

    /**
     * Register a manager interface to receive broadcasted updates.
     */
    @Override
    public void registerManager(IConnectivityManager manager) {
        return;
    }
    ///// Wifi Control /////
    /**
     * Request a broadcast of the current Wifi Station Mode
     * @return True if request was successful, False otherwise
     */
    @Override
    public boolean getWifiStationMode() {
        return false;
    }
    /**
     * Request to set the Wifi Station mode. Changes will be broadcasted.
     * @param mode The mode that is requested
     * @return True if request was successful, False otherwise
     */
    @Override
    public boolean setWifiStationMode(WifiStationMode mode) {
        return false;
    }
}