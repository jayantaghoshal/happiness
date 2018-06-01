/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.os.RemoteException;
import android.os.SystemProperties;

import android.util.Log;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.IOException;

import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

import vendor.volvocars.hardware.cloud.V1_0.*;

import java.util.NoSuchElementException;

import android.os.HwBinder;

public class MqttClientService extends ICloudMessageArrivedCallback.Stub {
    private static final String LOG_TAG = "[MQTT_shoulderTapListener]";

    ICloudNotifications notifications_client = null;

    public MqttClientService() {
        try {
            notifications_client = ICloudNotifications.getService();
            boolean result = notifications_client.registerTopicCallback("ota2", this);
            if(result){
                Log.d(LOG_TAG, "Topic callback registered succesfully!");
            }
            else{
                Log.d(LOG_TAG, "Failed to register callback");
            }
        } catch (RemoteException e) {
            Log.e(LOG_TAG,
                    "Cannot connect to ICloudNotifications binder service...RemoteException [" + e.getMessage() + "] ");
        }
    }

    @Override
    public void messageArrived(String msg) {
        Log.d(LOG_TAG, "Message: " + msg);
    }

}