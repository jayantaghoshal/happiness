/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.os.RemoteException;

import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.InstallNotification;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.Status;
import com.volvocars.softwareupdate.SoftwareInformation.SoftwareState;

import java.util.List;
import android.util.Log;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;

public class SoftwareManagementApiCallback extends ISoftwareManagementApiCallback.Stub {
    private static final String LOG_TAG = "SwManagementApiCallback";

    SoftwareUpdateService service = null;

    public SoftwareManagementApiCallback(SoftwareUpdateService service) {
        this.service = service;
    }

    public SoftwareManagementApiCallback() {
    }

    @Override
    public void CommissionStatus(String uuid, int code) {
        Log.v(LOG_TAG, "Got result of commissioning assignment [" + uuid + "]: " + code);
        if (code == 200) {
            service.UpdateSoftwareState(uuid, SoftwareState.COMMISSIONED);
            //try {
            //    callback.ProvideErrorMessage(code, "Request for Software Assignment List failed.");
            //} catch (RemoteException e) {
            //    Log.w(LOG_TAG, "Cannot event send error message. Client is super stupid...");
            //}
        }
    }

    @Override
    public void SoftwareAssignmentList(int code, List<SoftwareAssignment> software_list) {
        Log.v(LOG_TAG,
                "Got result of getting software assingment list [size of list: " + software_list.size() + "]: " + code);

        if (code == 200) {
            service.UpdateSoftwareList(software_list);
        } else {
            //try {
            //    callback.ProvideErrorMessage(code, "Request for Software Assignment List failed.");
            //} catch (RemoteException e) {
            //    Log.w(LOG_TAG, "Cannot event send error message. Client is super stupid...");
            //}
        }

    }

    @Override
    public void PendingInstallations(int code, List<InstallationOrder> installation_order_list) {
        Log.v(LOG_TAG, "Got result of getting pending installations [size of list: " + installation_order_list.size()
                + "]: " + code);
        if (200 == code) {
            service.UpdateSoftwareListWithInstallationOrders(installation_order_list);
            Log.v(LOG_TAG,
                    "Received pending installations, asking service to get download information of each installation order from the list");
            service.doGetDownloadInfo(installation_order_list);
        } else {
            //try {
            //    callback.ProvideErrorMessage(code, "Request for Pending Installations failed.");
            //} catch (RemoteException e) {
            //    Log.w(LOG_TAG, "Cannot even send error message. Client is super stupid...");
            //}
        }
    }

    @Override
    public void DownloadInfo(int code, DownloadInfo info) {
        Log.v(LOG_TAG, "Got result of getting download information [" + info.uuid + "]: " + code);
        if (200 == code) {
            service.UpdateSoftwareList(info);
            Log.v(LOG_TAG, "Received download information, ask service to download according to information");
            service.download(info);
        }
    }

    /**
    * Return the result of GetDownloadData
    * @param code         The latest HTTP code when downloading
    * @param downloadInfo The latest information regaring the download
    */
    @Override
    public void DownloadData(int code, DownloadInfo downloadInfo) {
        Log.v(LOG_TAG, "Got result of downloading assignment [" + downloadInfo.uuid + "]: " + code);
        if (200 == code) {
            service.UpdateSoftwareList(downloadInfo);
            //service.showInstallationPopup(downloadInfo);
        }
    }

    /**
     * Return the result of PostInstallationReport
     * @param code The HTTP code of the response
     * @param installationOrderId UUID of the posted Installation Report
     */
    public void InstallationReportStatus(int code, String installationOrderId) {
        Log.v(LOG_TAG, "Got result of posting Installation Report [" + installationOrderId + "]: " + code);
    }

    /**
    * Return the result of PostInstallNotification
    * @param code The HTTP code of the response
    * @param installationOrderId installation order Id
    */
    public void InstallNotificationStatus(int code, String installationOrderId) {
        Log.v(LOG_TAG, "Got result of posting Installation Notification [" + installationOrderId + "]: " + code);
        if (200 == code)
            service.UpdateSoftwareState(installationOrderId, SoftwareState.INSTALL_PENDING);
    }

    /**
    * Return the InstallNotification
    * @param code The HTTP code of the response
    * @param notification The InstallNotification
    */
    public void InstallNotification(int code, InstallNotification notification) {
        Log.v(LOG_TAG,
                "Got result of getting Installation Notification [" + notification.installationOrderId + "]: " + code);
        if (notification.notification.status.statusCode == Status.StatusCode.IN_PROGRESS)
            service.UpdateSoftwareState(notification.installationOrderId, SoftwareState.INSTALLING);
        else if (notification.notification.status.statusCode == Status.StatusCode.OK)
            service.UpdateSoftwareState(notification.installationOrderId, SoftwareState.INSTALLED);
        else
            Log.d(LOG_TAG, "InstallNotification with status " + notification.notification.status.statusCode.toString()
                    + " is not handled");
    }
}