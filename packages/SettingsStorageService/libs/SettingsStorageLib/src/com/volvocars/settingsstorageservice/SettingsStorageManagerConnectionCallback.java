/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.settingsstorageservice;
/**
* For clients to implement to get callbacks
*/
public interface SettingsStorageManagerConnectionCallback {
   void onServiceConnected();
   void onServiceDisconnected();
}