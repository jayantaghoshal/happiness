/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.audioctrl;

import android.util.Log;
import android.content.Context;
import android.content.pm.PackageManager;

/**
 * Implementation of AudioCtrl Service
 */
public class ApiHandler extends IAudioCtrlService.Stub {

  // Permission necessary to access RemoteCtrl APIs.
  private static final String PERMISSION_REMOTE_CTRL = "android.volvocars.permission.REMOTE_CTRL";
  private static final String TAG = "RemoteCtrl.AudioCtrl.ApiHandler";
  private final Context mContext;
  private HalHandler mHalHandler = null;

  public ApiHandler(Context serviceContext) {
    mContext = serviceContext;
  }

  public void init(HalHandler handler) {
    Log.v(TAG, "Set handle to native HAL handler");
    this.mHalHandler = handler;
  }

  /**
   * Calls from AppService
   */
  @Override
  public void updateVolume(byte audioContext, byte volumeLevel) {
    synchronized(this){
      Log.v(TAG, "UpdateVolume" + audioContext + "," + volumeLevel);
      assertRemoteCtrlPermission(mContext);
      mHalHandler.updateVolume(audioContext, volumeLevel);
    }
  }

  @Override
  public void sendGetVolumeResp(int requestIdentifier, boolean requestStatus, byte audioContext, byte volumeLevel) {
    synchronized(this){
      Log.v(TAG, "sendGetVolumeResp :requestIdentifier-" + requestIdentifier + ", +audioContext-" + audioContext + ", volumeLevel-" + volumeLevel);
      assertRemoteCtrlPermission(mContext);
      mHalHandler.sendGetVolumeResp(requestIdentifier, requestStatus, audioContext, volumeLevel);
    }
  }

  @Override
  public void sendSetVolumeResp(int requestIdentifier, boolean requestStatus) {
    synchronized(this){
      Log.v(TAG, "sendSetVolumeResp: requestIdentifier-" + requestIdentifier);
      assertRemoteCtrlPermission(mContext);
      mHalHandler.sendSetVolumeResp(requestIdentifier, requestStatus);
    }
  }

  public static void assertRemoteCtrlPermission(Context context) {
    if (context.checkCallingOrSelfPermission(PERMISSION_REMOTE_CTRL) != PackageManager.PERMISSION_GRANTED) {
      throw new SecurityException("requires " + PERMISSION_REMOTE_CTRL);
    }
  }
}