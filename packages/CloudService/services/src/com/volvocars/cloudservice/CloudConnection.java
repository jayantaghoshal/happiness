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

/**
 * Implementation of Foundation service API.
 */
public class CloudConnection extends ICloudConnectionEventListener.Stub {
    private CloudService service;
    private static final String LOG_TAG = "CloudService";
    private static final String LOG_PREFIX = "[CloudConnection]";

    private ICloudConnection cloud_connection = null;

    private DeathRecipient death = new DeathRecipient(this);

    public CloudConnection(CloudService service) {
        this.service = service;
    }

    final class DeathRecipient implements HwBinder.DeathRecipient {
        CloudConnection connection;

        DeathRecipient(CloudConnection connection) {
            this.connection = connection;
        }

        @Override
        public void serviceDied(long cookie) {
            Log.e(LOG_TAG, LOG_PREFIX + " Lost Connection to CloudDaemon");
            connection.daemonDied();
        }
    }

    public void init() {
        retryService();
    }

    public void daemonDied() {
        cloud_connection = null;
        retryService();
    }

    private void connectToService() {
        try {
            cloud_connection = ICloudConnection.getService();
            cloud_connection.registerCloudConnectionEventListener(this);

            cloud_connection.linkToDeath(death, 1010 /* cookie */);

        } catch (RemoteException ex) {
            // Something went bananas with binder.. What do?
            Log.e(LOG_TAG, LOG_PREFIX + " Cannot connect to ICloudConnection: RemoteException [" + ex.getMessage() + "]");
        }
    }

    private void retryService() {
        try {
            connectToService();
        } catch (NoSuchElementException e) {
            Log.e(LOG_TAG, LOG_PREFIX + " CloudDaemon not up yet.. Scheduling retry.");
            new Timer().schedule(new TimerTask() {
                    @Override
                    public void run() {
                        Log.d(LOG_TAG, LOG_PREFIX + " Retrying to connect to CloudDaemon");
                        retryService();
                    }
                }, 2000);
        }
    }

    @Override
    public void isConnected(boolean connected, String clientUri) {
        Log.d(LOG_TAG, LOG_PREFIX + " Backend connection status changed to  " + connected + " (clientUri = " + clientUri + ")");
        service.isConnected(connected, clientUri);
    }

    @Override
    public void enteredErrorState(String reason) {
        Log.e(LOG_TAG, LOG_PREFIX + " Backend entered error state with reason: [" + reason + "]");
        service.enteredErrorState(reason);
    }

    // @Override
    // public void shoulderTap(String tap) {
    //     // TODO: Place holder for MQTT
    // }

    public class DownloadResponseCallback extends ICloudConnectionDownloadResponseCallback.Stub {
        @Override
        public void updateDownloadStatus(Response response) {
            Log.v(LOG_TAG, LOG_PREFIX + " updateDownloadStatus: Calling service.notifyDownloadStatus()");
            service.notifyDownloadStatus(response);
        }
    }

    public Response doGetRequest(String uri, ArrayList<HttpHeaderField> headers, int timeout) {
        Log.v(LOG_TAG, LOG_PREFIX + " doGetRequest");
        Response response = null;
        if(cloud_connection != null) {
            try {
                response = cloud_connection.doGetRequest(uri, headers, timeout);

                if (!checkResponse(response)) {
                    response = null;
                }
            } catch (RemoteException ex) {
                // Something went bananas with binder.. What do?
                Log.e(LOG_TAG, LOG_PREFIX + " Cannot send getRequest: RemoteException [" + ex.getMessage() + "]");
            }
        }
        return response;
    }

    public Response doPostRequest(String uri, ArrayList<HttpHeaderField> headers, String body, int timeout) {
        Log.v(LOG_TAG, LOG_PREFIX + " doPostRequest");
        Response response = null;
        if(cloud_connection != null) {
            try {
                response = cloud_connection.doPostRequest(uri, headers, body, timeout);

                if (!checkResponse(response)) {
                    response = null;
                }

             } catch (RemoteException ex) {
                // Something went bananas with binder.. What do?
                Log.e(LOG_TAG, LOG_PREFIX + " Cannot send postRequest: RemoteException [" + ex.getMessage() + "]");
             }
        }
        return response;
    }

    public void downloadRequest(String uri, ArrayList<HttpHeaderField> headers, String file_path, int timeout) {
        Log.v(LOG_TAG, LOG_PREFIX + " downloadRequest");
        if(cloud_connection != null) {
            DownloadResponseCallback downloadCallback = new DownloadResponseCallback();
            try {
                cloud_connection.downloadRequest(uri, headers, file_path, timeout, downloadCallback);
            } catch (RemoteException ex) {
                // Something went bananas with binder.. What do?
                Log.e(LOG_TAG, LOG_PREFIX + " Cannot send downloadRequest: RemoteException [" + ex.getMessage() + "]");
            }
        }
    }

    private boolean checkResponse(Response response) {
        // TODO: Generic error handling
        return true;
    }

}