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

    private ICloudConnection cloud_connection = null;

    public CloudConnection(CloudService service) {
        this.service = service;
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

    public class DownloadResponseCallback extends ICloudConnectionDownloadResponseCallback.Stub {
        @Override
        public void updateDownloadStatus(Response response) {
            Log.v(LOG_TAG, "updateDownloadStatus: Calling service.notifyDownloadStatus()");
            service.notifyDownloadStatus(response);
        }
    }
    public Response doGetRequest(String uri, ArrayList<HttpHeaderField> headers, int timeout) {
        Response response = null;
        try {
            response = cloud_connection.doGetRequest(uri, headers, timeout);

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

        Log.v(LOG_TAG, "doPostRequest");

        Response response = null;
        try {
            response = cloud_connection.doPostRequest(uri, headers, body, timeout);

            if(!checkResponse(response)) {
                response = null;
            }

        } catch (RemoteException ex) {
            // Something went bananas with binder.. What do?
            Log.e(LOG_TAG, "Something went bananas with binder: " + ex.getMessage());
        }
        return response;
    }

    public void downloadRequest(String uri, ArrayList<HttpHeaderField> headers, String file_path, int timeout) {

        Log.v(LOG_TAG, "downloadRequest");
        DownloadResponseCallback downloadCallback = new DownloadResponseCallback();
        try {
            cloud_connection.downloadRequest(uri, headers, file_path, timeout, downloadCallback);
        } catch (RemoteException ex) {
            // Something went bananas with binder.. What do?
            Log.e(LOG_TAG, "Something went bananas with binder: " + ex.getMessage());
        }
    }

    private boolean checkResponse(Response response) {
        // TODO: Generic error handling
        return true;
    }

}