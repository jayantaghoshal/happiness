/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlappservices.audioctrl;

import android.util.Log;
import android.content.Context;
import android.content.pm.PackageManager;
import com.volvocars.remotectrlservices.audioctrl.*;

/**
 * Implementation of AudioCtrl App Service
 */
public class AppApiHandler extends IAudioCtrlAppService.Stub {
    // Permission necessary to access RemoteCtrl APIs.
    private static final String PERMISSION_REMOTE_CTRL = "android.volvocars.permission.REMOTE_CTRL";
    private static final String TAG = "RemoteCtrlApp.AudioCtrl.AppApiHandler";
    private AudioCtrlAppService mAppService = null;

    public AppApiHandler(AudioCtrlAppService appService) {
        Log.v(TAG, "App service interface impl.");
        mAppService = appService;
    }

    /**
     * Calls from system service
     */
    @Override
    public void getVolume(int requestIdentifier, byte audioContext) {
        synchronized (this) {
            assertRemoteCtrlPermission(mAppService);
            Log.v(TAG,
                    "Get volume: requestIdentifier-" + requestIdentifier + ", audioContext-"
                            + audioContext);
            mAppService.getVolume(requestIdentifier, audioContext);
        }
    }

    @Override
    public void setVolume(int requestIdentifier, byte audioContext, byte volumeLevel) {
        synchronized (this) {
            assertRemoteCtrlPermission(mAppService);
            Log.v(TAG,
                    "Set volume: requestIdentifier-" + requestIdentifier + ", audioContext-"
                            + audioContext + ", volumeLevel-" + volumeLevel);
            mAppService.setVolume(requestIdentifier, audioContext, volumeLevel);
        }
    }

    /**
     * Validate permission
     */
    private static void assertRemoteCtrlPermission(Context context) {
        if (context.checkCallingOrSelfPermission(PERMISSION_REMOTE_CTRL)
                != PackageManager.PERMISSION_GRANTED) {
            throw new SecurityException("requires " + PERMISSION_REMOTE_CTRL);
        }
    }
}