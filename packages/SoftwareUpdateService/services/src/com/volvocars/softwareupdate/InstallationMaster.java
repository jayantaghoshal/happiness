/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.os.RemoteException;
import android.util.Log;
import vendor.volvocars.hardware.installationmaster.V1_0.*;

public class InstallationMaster extends IInstallationMasterEventListener.Stub {
    private static final String LOG_TAG = "SoftwareUpdate.IM";

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
            Log.e(LOG_TAG, "Cannot initialize InstallationMaster: RemoteException [" + ex.getMessage() + "]");
        }
    }

    @Override
    public void installNotification(String installationOrder, int notification) {
        Log.d(LOG_TAG, "installNotification [installationOrder: " + installationOrder + ", notification: "
                + InstallNotification.toString(notification) + "]");
        service.onInstallationNotification(installationOrder, InstallNotification.toString(notification));
    }

    @Override
    public void installationReport(String installationOrder, InstallationSummary installationSummary) {
        Log.d(LOG_TAG, "installationReport [installationOrder: " + installationOrder);
        //TODO: send this info to service
    }

    public void assignInstallation(String uuid) {
        Log.v(LOG_TAG, "assignInstallation [uuid: " + uuid + "]");
        try {
            installationmaster.assignInstallation(uuid);
        } catch (RemoteException e) {
            Log.e(LOG_TAG, "Error in assignInstallation: RemoteException [" + e.getMessage() + "]");
        }
    }
}