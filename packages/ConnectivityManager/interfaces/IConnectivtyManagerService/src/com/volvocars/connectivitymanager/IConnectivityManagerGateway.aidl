package com.volvocars.connectivitymanager;

import com.volvocars.connectivitymanager.IConnectivityManager;
import com.volvocars.connectivitymanager.WifiStationMode;

/**
 * Interface implemented by the gateway to the Connectivity Manager Daemon.
 */

interface IConnectivityManagerGateway {

    /**
     * Register a manager interface to receive broadcasted updates.
     */
    void registerManager(in IConnectivityManager manager);


    ///// Wifi Control /////

    /**
     * Request a broadcast of the current Wifi Station Mode
     * @return True if request was successful, False otherwise
     */
    boolean getWifiStationMode();

    /**
     * Request to set the Wifi Station mode. Changes will be broadcasted.
     * @param mode The mode that is requested
     * @return True if request was successful, False otherwise
     */
    boolean setWifiStationMode(in WifiStationMode mode);

    ///// End of Wifi Control /////
}