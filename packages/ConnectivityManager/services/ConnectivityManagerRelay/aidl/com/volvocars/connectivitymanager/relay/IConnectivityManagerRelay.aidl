package com.volvocars.connectivitymanager.relay;

import com.volvocars.connectivitymanager.relay.IConnectivityManagerRelayCallback;
import com.volvocars.connectivitymanager.WifiStationModeAidl;

oneway interface IConnectivityManagerRelay {


    void registerCallback(in IConnectivityManagerRelayCallback callback);

    ///// Wifi Control /////

    /**
     * Update the manager with the Wifi Station mode.
     * @param mode The new mode to send to the manager
     */
    void getWifiStationMode();


    /**
     * Update the manager with the Wifi Station mode.
     * @param mode The new mode to send to the manager
     */
    void setWifiStationMode(in WifiStationModeAidl mode);

    ///// End of Wifi Control /////

}
