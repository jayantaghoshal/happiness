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
import com.volvocars.connectivitymanager.WifiStationModeAidl;

import vendor.volvocars.hardware.connectivitymanager.V1_0.WifiStationMode;

/**
 *
 */
public class ConnectivityManagerGateway extends IConnectivityManagerGateway.Stub {
    private static final String LOG_TAG = "ConManGw.";
    private SystemConnectivityManager systemManager = null;

    public ConnectivityManagerGateway() {}

    public void init(SystemConnectivityManager sysMan) {
        systemManager = sysMan;
    }

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
        return systemManager.getWifiStationMode();
    }

    /**
     * Request to set the Wifi Station mode. Changes will be broadcasted.
     * @param mode The mode that is requested
     * @return True if request was successful, False otherwise
     */
    @Override
    public boolean setWifiStationMode(WifiStationModeAidl mode) {
        byte hidlMode = WifiStationMode.UNKNOWN;
        switch (mode.mode) {
            case UNKNOWN:
                hidlMode = WifiStationMode.UNKNOWN;
            case AP_MODE:
                hidlMode = WifiStationMode.AP_MODE;
            case STATION_MODE:
                hidlMode = WifiStationMode.STATION_MODE;
        }
        return systemManager.setWifiStationMode(hidlMode);
    }
}
