package com.volvocars.connectivitymanager;

import com.volvocars.connectivitymanager.WifiStationMode;

/*
 * Interface implemented by some application that is interested of changes
 * to connectivity management.
 */

interface IConnectivityManager {

    ///// Wifi Control /////

    /**
     * Update the manager with the Wifi Station mode.
     * @param mode The new mode to send to the manager
     */
    void updateWifiStationMode(in WifiStationMode mode);

    ///// End of Wifi Control /////

}