/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.audioctrl;

import android.app.Service;
import android.content.Intent;
import android.content.Context;
import android.content.BroadcastReceiver;
import android.util.Log;
import android.os.SystemProperties;

public class StartOnBoot extends BroadcastReceiver {
  private static final String TAG = "RemoteCtrl.AudioCtrl.StartOnBoot";

  @Override
  public void onReceive(Context context, Intent intent) {
    if (intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {
      try{
        String autonomousDriveValue = SystemProperties.get("vendor.carconfig.CC100_AutonomousDriveType");
        if (autonomousDriveValue.equals("3")) {
          Intent serviceIntent = new Intent(context, AudioCtrlService.class);
          context.startService(serviceIntent);
        }else{
          Log.e(TAG, "Unset or Wrong AutonomouseDriveType");
        }
      }catch (Exception ex) {
        Log.e(TAG, "Error occured while starting service" + ex.getMessage());
      }
    }else {
      Log.e(TAG, "Invalid intent action received.");
    }
  }
}