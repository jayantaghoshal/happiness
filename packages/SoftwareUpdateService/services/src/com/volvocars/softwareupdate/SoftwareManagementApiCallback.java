/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.os.RemoteException;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.softwareupdate.SoftwareInformation.SoftwareState;

import java.util.List;
import android.util.Log;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;

public class SoftwareManagementApiCallback extends ISoftwareManagementApiCallback.Stub {
    private static final String LOG_TAG = "SwUpManagementApiCallback";

    SoftwareUpdateService service = null;


    public SoftwareManagementApiCallback(SoftwareUpdateService service) {
        this.service = service;
    }

    public SoftwareManagementApiCallback() {
    }

    @Override
    public void CommissionStatus(String uuid, int code) {
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
        if (200 == code) {
            service.UpdateSoftwareListWithInstallationOrders(installation_order_list);
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
        if (200 == code) {
            service.UpdateSoftwareList(info);
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
        if (200 == code) {
            Log.v(LOG_TAG, "Download succeeded");
            service.UpdateSoftwareList(downloadInfo);
        } else {
            Log.v(LOG_TAG, "Download failed: " + code);
        }
    }

}