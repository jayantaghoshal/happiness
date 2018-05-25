/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import com.volvocars.cloudservice.InstallationSummary;

import android.os.RemoteException;
import android.util.Log;
import java.util.List;

import vendor.volvocars.hardware.installationmaster.V1_0.*;

public class InstallationMaster extends IInstallationMasterEventListener.Stub {
    private static final String LOG_TAG = "SoftwareUpdateService";
    private static final String LOG_PREFIX = "[SoftwareUpdateService]";

    private IInstallationMaster installationmaster = null;

    private SoftwareUpdateService service = null;

    public InstallationMaster(SoftwareUpdateService service) {
        this.service = service;
    }

    public void init() {
        try {
            installationmaster = IInstallationMaster.getService();
            installationmaster.registerInstallationStatusListener(this);
        } catch (RemoteException ex) {
            Log.e(LOG_TAG,
                    LOG_PREFIX + " Cannot initialize InstallationMaster: RemoteException [" + ex.getMessage() + "]");
        }
    }

    @Override
    public void installNotification(String installationOrder, int notification) {
        Log.v(LOG_TAG, LOG_PREFIX + " installNotification [installationOrder: " + installationOrder + ", notification: "
                + InstallationStatus.toString(notification) + "]");
        service.onInstallationNotification(installationOrder, InstallationStatus.toString(notification));
    }

    @Override
    public void installationReport(String installationOrder, Summary summary) {
        Log.v(LOG_TAG, LOG_PREFIX + " installationReport [installationOrder: " + installationOrder);

        InstallationSummary installationSummary = new InstallationSummary();
        installationSummary.softwareId = summary.softwareId;
        installationSummary.timestamp = Long.toString(summary.timestamp);
        installationSummary.repeatResets = summary.repeatedResets;
        installationSummary.totalInstallationTime = summary.totalInstallationTime;
        installationSummary.ecus = (List) summary.ecus;
        service.onInstallationReport(installationOrder, installationSummary);
    }

    public void assignInstallation(String uuid) {
        Log.v(LOG_TAG, LOG_PREFIX + " assignInstallation [uuid: " + uuid + "]");
        try {
            installationmaster.assignInstallation(uuid);
        } catch (RemoteException e) {
            Log.e(LOG_TAG, LOG_PREFIX + " Error in assignInstallation: RemoteException [" + e.getMessage() + "]");
        }
    }

    public void verifyDownload(String installationOrderId) {
        Log.v(LOG_TAG, LOG_PREFIX + " verifyDownload [installationOrderId: " + installationOrderId + "]");
        try {
            installationmaster.verifyDownload(installationOrderId);
        } catch (RemoteException e) {
            Log.e(LOG_TAG, LOG_PREFIX + " Error in verifyDownload: RemoteException [" + e.getMessage() + "]");
        }
    }
}