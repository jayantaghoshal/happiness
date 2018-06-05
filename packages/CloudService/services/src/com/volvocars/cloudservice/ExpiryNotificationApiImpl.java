/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.util.Log;
import android.os.RemoteException;
import android.os.SystemProperties;
import vendor.volvocars.hardware.cloud.V1_0.*;
import java.util.ArrayList;
import java.util.List;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.IOException;

/**
 * Implementation of Foundation service API.
 */
public class ExpiryNotificationApiImpl { // Todo extends IExpiryNotificationApi.Stub {
    private static final String LOG_TAG = "CloudService";
    private static final String LOG_PREFIX = "[ExpiryNotificationApiImpl]";

    private String topic;

    private class ExpiryNotificationMessageCallback extends ICloudMessageArrivedCallback.Stub {

        public ExpiryNotificationMessageCallback() {
        }

        @Override
        public void messageArrived(String msg) {
            Log.d(LOG_TAG, LOG_PREFIX + "Topic: " + topic + "\tMsg Recvd: " + msg);
        }
    }

    public ExpiryNotificationApiImpl() {
    }

    public void init(NotificationServiceImpl notification_service_client, String topic) {
        this.topic = topic;
        notification_service_client.registerTopicCallback(topic, new ExpiryNotificationMessageCallback());
    }
}