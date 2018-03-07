/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.cloudservice;

import android.util.Log;
import android.app.Service;
import android.os.IBinder;
import android.content.*;
import android.os.RemoteException;
import java.util.List;
import java.util.ArrayList;

/**
 * Library/wrapper class that is used to access SoftwareManagement features
 */
public class SoftwareManagementApi implements ServiceConnection {
    private static final String LOG_TAG = "SWAPI";
    private static final String PACKAGENAME = "com.volvocars.cloudservice";
    private static final String PACKAGENAME_SERVICENAME = "com.volvocars.cloudservice.CloudService";
    private Context context = null;
    private ISoftwareManagementApi software_management = null;
    private boolean service_bound = false;

    private SoftwareManagementApiConnectionCallback softwareManagementApiConnectionCallback = null;

    private DeathRecipient death = new DeathRecipient(this);

    final class DeathRecipient implements IBinder.DeathRecipient {

        private SoftwareManagementApi client;
        private int retries = 0;

        DeathRecipient(SoftwareManagementApi client) {
            this.client = client;
        }

        @Override
        public void binderDied() {
            Log.e(LOG_TAG, "CloudService died");

            if(retries < 5) {
                Log.d(LOG_TAG, "Trying CloudService again... Attempt " + (retries + 1));
                retries++;
                client.connect();
            } else {
                Log.d(LOG_TAG, "CloudService seems unreliable, no more attempts to connect.");
            }
        }
    }

    @Override
    public void onServiceConnected(ComponentName className, IBinder service) {
        software_management = ISoftwareManagementApi.Stub.asInterface(service);
        service_bound = true;
        softwareManagementApiConnectionCallback.onServiceConnected();

        try {
            service.linkToDeath(death, 0 /* flags */);
        } catch (RemoteException e) {
            Log.w(LOG_TAG, "Unable to register Death Recipient. :(");
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName arg0) {
        service_bound = false;
        softwareManagementApiConnectionCallback.onServiceDisconnected();
    }

    /**
     *
     */
    public SoftwareManagementApi(Context context,
            SoftwareManagementApiConnectionCallback softwareManagementApiConnectionCallback) {
        this.context = context;
        this.softwareManagementApiConnectionCallback = softwareManagementApiConnectionCallback;
        connect();
    }

    /**
     * Used to bind to Cloud Service, can be used by client if service unbinds unexpectedly
     */
    public void connect() {
        Intent intent = new Intent();
        intent.setAction("SoftwareManagementApi");
        intent.setComponent(new ComponentName(PACKAGENAME, PACKAGENAME_SERVICENAME));
        context.bindService(intent, this, Context.BIND_AUTO_CREATE);
    }

    /**
     * Get a the list of available assignments
     * @param callback
     */
    public void GetSoftwareAssigmentList(ISoftwareManagementApiCallback callback) throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.GetSoftwareAssigmentList(callback);
        }
    }

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param id The id of the assignment to fetch.
     */
    public void CommissionSoftwareAssignment(String uuid, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.CommissionSoftwareAssignment(uuid, callback);
        }

    }

    /**
    * Get a list of pending installations (installation orders)
    * @param callback
    */
    public void GetPendingInstallations(ISoftwareManagementApiCallback callback) throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.GetPendingInstallations(callback);
        }
    }

    /**
    *
    */
    public void GetDownloadInfo(String uuid, ISoftwareManagementApiCallback callback) throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.GetDownloadInfo(uuid, callback);
        }
    }

    /**
    * Get Download data
    * @param downloadInfo Contains information of what to be downloaded
    * @param callback     Callback to be called when the status of the download changes
    */
    public void GetDownloadData(DownloadInfo downloadInfo, ISoftwareManagementApiCallback callback) throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.GetDownloadData(downloadInfo, callback);
        }
    }
}