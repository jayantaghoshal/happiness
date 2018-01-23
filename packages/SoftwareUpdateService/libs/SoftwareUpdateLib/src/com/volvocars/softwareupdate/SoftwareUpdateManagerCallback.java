/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;
/**
* For clients to implement to get callbacks
*/
public interface SoftwareUpdateManagerCallback {
   void onServiceConnected();
   void onServiceDisconnected();
}