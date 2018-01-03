/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;
/**
* For clients to implement to get callbacks
*/
public interface FsApiConnectionCallback {
   void onServiceConnected();
   void onServiceDisconnected();
}