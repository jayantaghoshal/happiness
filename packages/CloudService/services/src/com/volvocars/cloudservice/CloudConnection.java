/*
 * Copyright 2017 Volvo Car Corporation
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

import vendor.volvocars.hardware.cloud.V1_0.*;

/**
 * Implementation of Foundation service API.
 */
public class CloudConnection extends ICloudConnectionEventListener.Stub {
    private CloudService service;
    private static final String LOG_TAG = "CloudService.ClConn";
    private Boolean use_https = true;

    private ICloudConnection cloud_connection = null;

    public CloudConnection(CloudService service) {
        this.service = service;
        use_https = SystemProperties.getBoolean("service.cloudservice.use_https", true);
        Log.d(LOG_TAG, "service.cloudservice.use_https: " + use_https);
    }

    public void init() {
        try {
            cloud_connection = ICloudConnection.getService();

            cloud_connection.registerCloudConnectionEventListener(this);

        } catch (RemoteException ex) {
            // Something went bananas with binder.. What do?
            Log.e(LOG_TAG, "Something went bananas with binder: " + ex.getMessage());
        }
    }

    @Override
    public void isConnected(boolean connected) {
        Log.e(LOG_TAG, "Connected: " + connected);
        service.isConnected(connected);
    }

    @Override
    public void enteredErrorState(String reason) {
        service.enteredErrorState(reason);
    }

    @Override
    public void shoulderTap(String tap) {
        // TODO: Place holder for MQTT
    }

    public Response doGetRequest(String uri, ArrayList<HttpHeaderField> headers, int timeout) {
        Response response = null;
        try {
            response = cloud_connection.doGetRequest(uri, headers, use_https, timeout);

            if(!checkResponse(response)) {
                response = null;
            }
        } catch (RemoteException ex) {
            // Something went bananas with binder.. What do?
            Log.e(LOG_TAG, "Something went bananas with binder: " + ex.getMessage());
        }
        return response;
    }

    public Response doPostRequest(String uri, ArrayList<HttpHeaderField> headers, String body, int timeout) {
        Response response = null;
        try {
            response = cloud_connection.doPostRequest(uri, headers, body, use_https, timeout);

            if(!checkResponse(response)) {
                response = null;
            }

        } catch (RemoteException ex) {
            // Something went bananas with binder.. What do?
            Log.e(LOG_TAG, "Something went bananas with binder: " + ex.getMessage());
        }
        return response;
    }

    private boolean checkResponse(Response response) {
        // TODO: Generic error handling
        return true;
    }

}