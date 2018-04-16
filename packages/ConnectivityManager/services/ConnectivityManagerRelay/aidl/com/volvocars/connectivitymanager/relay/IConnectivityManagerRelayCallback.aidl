package com.volvocars.connectivitymanager.relay;

import com.volvocars.connectivitymanager.WifiStationModeAidl;

oneway interface IConnectivityManagerRelayCallback {

    ///// Wifi Control /////

    void notifyWifiStationMode(in WifiStationModeAidl mode);

    ///// End of Wifi Control /////

}
