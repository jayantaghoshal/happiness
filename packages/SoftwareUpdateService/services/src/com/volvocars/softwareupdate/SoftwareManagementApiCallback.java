/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.os.RemoteException;

import com.volvocars.cloudservice.AssignmentType;
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
            Log.d(LOG_TAG, "updating softwarestate, remove when MQTT is in place!");
            service.UpdateSoftwareState(uuid, SoftwareState.COMMISSIONED);
        } else {
            Log.w(LOG_TAG, "CommissionStatus for uuid: " + uuid + "failed with code: " + code);
        }
    }

    @Override
    public void SoftwareAssignmentList(int code, AssignmentType type, List<SoftwareAssignment> software_list) {
        Log.v(LOG_TAG,
                "Got result of getting software assingment list (type: " + type + ") [size of list: " + software_list.size() + "]: " + code);
        if (code == 200) {
            service.onNewSoftwareAssignmentList(software_list, type);
        } else {
            Log.w(LOG_TAG, "SoftwareAssignmentList for list of type: " + type + "failed with code: " + code);
        }

    }

    @Override
    public void DownloadInfo(int code, DownloadInfo info) {
        Log.v(LOG_TAG, "Got result of getting download information [" + info.id + "]: " + code);
        if (200 == code) {
            service.UpdateSoftwareList(info);
            Log.v(LOG_TAG, "Received download information, ask service to download according to information");
            service.download(info);
        }
        else {
            Log.w(LOG_TAG, "DownloadInfo for uuid: " + info.id + "failed with code: " + code);
        }
    }

    /**
    * Return the result of GetDownloadData
    * @param code         The latest HTTP code when downloading
    * @param downloadInfo The latest information regaring the download
    */
    @Override
    public void DownloadData(int code, DownloadInfo downloadInfo) {
        Log.v(LOG_TAG, "Got result of downloading assignment [" + downloadInfo.id + "]: " + code);
        if (200 == code) {
            service.UpdateSoftwareList(downloadInfo);
        }
        else {
            Log.w(LOG_TAG, "DownloadData for uuid: " + downloadInfo.id + "failed with code: " + code);
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