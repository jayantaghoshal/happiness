/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.gateway;

import android.util.Log;
import android.os.RemoteException;
import android.os.SystemProperties;

import java.util.ArrayList;
import java.util.ListIterator;

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

    private ArrayList<IConnectivityManager> subscribers = new ArrayList<IConnectivityManager>();

    public ConnectivityManagerGateway() {}

    public void init(SystemConnectivityManager sysMan) {
        systemManager = sysMan;
    }

    public void notifyWifiStationModeChange(byte mode) {
        if (subscribers.isEmpty()) {
            return;
        }

        WifiStationModeAidl stationMode = new WifiStationModeAidl();
        switch (mode) {
            case WifiStationMode.AP_MODE:
                stationMode.mode = WifiStationModeAidl.Mode.AP_MODE;
                break;
            case WifiStationMode.STATION_MODE:
                stationMode.mode = WifiStationModeAidl.Mode.STATION_MODE;
                break;
            case WifiStationMode.UNKNOWN:
            default:
                stationMode.mode = WifiStationModeAidl.Mode.UNKNOWN;
        }

        ListIterator<IConnectivityManager> it = subscribers.listIterator();
        do {
            try {
                it.next().updateWifiStationMode(stationMode);
            } catch (RemoteException e) {
                Log.w(LOG_TAG, "Subscriber not responding, removing from list");
                it.remove();
            }
        } while (it.hasNext());
    }

    /**
     * Register a manager interface to receive broadcasted updates.
     */
    @Override
    public void registerManager(IConnectivityManager manager) {
        subscribers.add(manager);
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
            case AP_MODE:
                hidlMode = WifiStationMode.AP_MODE;
                break;
            case STATION_MODE:
                hidlMode = WifiStationMode.STATION_MODE;
                break;
            case UNKNOWN:
            default:
                hidlMode = WifiStationMode.UNKNOWN;
        }
        return systemManager.setWifiStationMode(hidlMode);
    }
}
