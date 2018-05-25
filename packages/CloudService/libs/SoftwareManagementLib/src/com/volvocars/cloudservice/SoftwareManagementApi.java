/*
 * Copyright 2017-2018 Volvo Car Corporation
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
    private static final String LOG_TAG = "CloudService";
    private static final String LOG_PREFIX = "[SoftwareManagementApi]";

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
            Log.e(LOG_TAG, LOG_PREFIX + " CloudService died");

            if (retries < 5) {
                Log.d(LOG_TAG, LOG_PREFIX + " Trying CloudService again... Attempt " + (retries + 1));
                retries++;
                client.connect();
            } else {
                Log.d(LOG_TAG, LOG_PREFIX + " CloudService seems unreliable, no more attempts to connect.");
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
            Log.w(LOG_TAG, LOG_PREFIX + " Unable to register Death Recipient. :(");
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
     * Used to bind to Cloud Service, can be used by client if service unbinds
     * unexpectedly
     */
    public void connect() {
        Intent intent = new Intent();
        intent.setAction("SoftwareManagementApi");
        intent.setComponent(new ComponentName(PACKAGENAME, PACKAGENAME_SERVICENAME));
        context.bindService(intent, this, Context.BIND_AUTO_CREATE);
    }

    /**
     * Get available assignments with specified query parameters
     *
     * @param query    Query
     * @param callback
     */
    public void GetSoftwareAssignment(Query query, AssignmentType type, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.GetSoftwareAssignment(query, type, callback);
        }
    }

    /**
     * Issue a commission of an SoftwareAssignment.
     *
     * @param commissionElement commission element containing id of the assignment
     *                          to fetch
     * @param callback
     */
    public void CommissionSoftwareAssignment(CommissionElement commissionElement,
            ISoftwareManagementApiCallback callback) throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.CommissionSoftwareAssignment(commissionElement, callback);
        }

    }

     /**
     * Get Download Info for an installation order
     * @param installationOrder installation order
     * @param callback
     */
    public void GetDownloadInfo(InstallationOrder installationOrder, ISoftwareManagementApiCallback callback) throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.GetDownloadInfo(installationOrder, callback);
        }
    }

    /**
     * Get Download data
     *
     * @param downloadInfo Contains information of what to be downloaded
     * @param callback     Callback to be called when the status of the download
     *                     changes
     */
    public void GetDownloadData(DownloadInfo downloadInfo, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.GetDownloadData(downloadInfo, callback);
        }
    }

    /**
     * Post InstallationReport
     *
     * @param installationReport Report to be posted
     * @param callback           Callback to be called when the status of the
     *                           download changes
     */
    public void PostInstallationReport(InstallationReport installationReport, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.PostInstallationReport(installationReport, callback);
        }
    }

    /**
     * Post InstallNotification
     *
     * @param notification InstallNotification to be posted
     * @param callback     Callback to be called
     */
    public void PostInstallNotification(InstallNotification notification, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.PostInstallNotification(notification, callback);
        }
    }

    /**
     * Get InstallNotification
     *
     * @param installationOrderId Installation order id
     * @param uri                 Install notification uri
     * @param callback            Callback to be called
     */
    public void GetInstallNotification(String installationOrderId, String uri, ISoftwareManagementApiCallback callback)
            throws RemoteException {
        if (software_management != null && service_bound) {
            software_management.GetInstallNotification(installationOrderId, uri, callback);
        }
    }
}