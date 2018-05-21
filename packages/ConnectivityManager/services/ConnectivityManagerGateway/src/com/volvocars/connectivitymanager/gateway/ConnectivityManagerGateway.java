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

    public synchronized void notifyWifiStationModeChange(byte mode) {
        if (subscribers.isEmpty()) {
            return;
        }

        WifiStationModeAidl stationMode = new WifiStationModeAidl();
        switch (mode) {
            case WifiStationMode.OFF:
                stationMode.mode = WifiStationModeAidl.Mode.OFF;
                break;
            case WifiStationMode.STATION:
                stationMode.mode = WifiStationModeAidl.Mode.STATION;
                break;
            case WifiStationMode.AP:
                stationMode.mode = WifiStationModeAidl.Mode.AP;
                break;
            case WifiStationMode.WORKSHOP:
                stationMode.mode = WifiStationModeAidl.Mode.WORKSHOP;
                break;
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
    public synchronized void registerManager(IConnectivityManager manager) {
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
        if (systemManager != null) {
            return systemManager.getWifiStationMode();
        }
        return false;
    }

    /**
     * Request to set the Wifi Station mode. Changes will be broadcasted.
     * @param mode The mode that is requested
     * @return True if request was successful, False otherwise
     */
    @Override
    public boolean setWifiStationMode(WifiStationModeAidl mode) {
        byte hidlMode = WifiStationMode.OFF;
        switch (mode.mode) {
            case OFF:
                hidlMode = WifiStationMode.OFF;
                break;
            case STATION:
                hidlMode = WifiStationMode.STATION;
                break;
            case AP:
                hidlMode = WifiStationMode.AP;
                break;
            case WORKSHOP:
                hidlMode = WifiStationMode.WORKSHOP;
                break;
        }
        if (systemManager != null) {
            return systemManager.setWifiStationMode(hidlMode);
        }

        return false;
    }
}
