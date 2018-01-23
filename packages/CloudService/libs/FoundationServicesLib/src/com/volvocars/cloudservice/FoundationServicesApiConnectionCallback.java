/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;
/**
* For clients to implement to get callbacks
*/
public interface FoundationServicesApiConnectionCallback {
   void onServiceConnected();
   void onServiceDisconnected();
}