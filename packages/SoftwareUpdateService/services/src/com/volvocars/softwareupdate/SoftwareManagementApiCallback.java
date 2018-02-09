/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.os.RemoteException;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;
import java.util.List;
import android.util.Log;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;

public class SoftwareManagementApiCallback extends ISoftwareManagementApiCallback.Stub {
    private ISoftwareUpdateManagerCallback callback;
    private static final String LOG_TAG = "SwUpManagementApiCallback";

    public SoftwareManagementApiCallback(ISoftwareUpdateManagerCallback callback) {
        this.callback = callback;
    }

    @Override
    public void CommissionStatus(int code) {
        if (code != 200) {
            try {
                callback.ProvideErrorMessage(code, "Request for Software Assignment List failed.");
            } catch (RemoteException e) {
                Log.w(LOG_TAG, "Cannot event send error message. Client is super stupid...");
            }
        }
    }

    @Override
    public void SoftwareAssignmentList(int code, List<SoftwareAssignment> software_list) {
        if (code == 200) {
            try {
                callback.UpdateSoftwareAssignmentList(software_list);
            } catch (RemoteException e) {
                Log.w(LOG_TAG, "Cannot update Software Assignment List. Client is stupid...");
            }
        } else {
            try {
                callback.ProvideErrorMessage(code, "Request for Software Assignment List failed.");
            } catch (RemoteException e) {
                Log.w(LOG_TAG, "Cannot event send error message. Client is super stupid...");
            }
        }

    }

    @Override
    public void PendingInstallations(int code, List<InstallationOrder> installation_order_list) {
        if (200 == code) {
            try {
                callback.UpdatePendingInstallations(installation_order_list);
            } catch (RemoteException e) {
                Log.w(LOG_TAG, "Cannot update Pending Installations. Client is stupid...");
            }

        } else {
            try {
                callback.ProvideErrorMessage(code, "Request for Pending Installations failed.");
            } catch (RemoteException e) {
                Log.w(LOG_TAG, "Cannot even send error message. Client is super stupid...");
            }
        }
    }

    @Override
    public void DownloadInfo(String uuid, DownloadInfo info) {
        //TODO: implement
    }
}