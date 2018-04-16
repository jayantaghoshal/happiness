/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager;
/**
* For clients to implement to get callbacks
*/
import com.volvocars.connectivitymanager.WifiStationModeAidl;

public interface IConnectivityManagerCallback {
   void onServiceConnected();
   void onServiceDisconnected();

   void notifyWifiStationMode(WifiStationModeAidl mode);
}