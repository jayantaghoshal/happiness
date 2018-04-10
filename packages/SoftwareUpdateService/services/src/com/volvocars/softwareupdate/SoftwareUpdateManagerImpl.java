/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.util.Log;

import android.os.RemoteException;

import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.softwareupdate.ISoftwareUpdateManager;
import com.volvocars.softwareupdate.ISoftwareUpdateManagerCallback;

import java.util.ArrayList;
import java.util.List;

/**
 * Implementation of Foundation service API.
 */
public class SoftwareUpdateManagerImpl extends ISoftwareUpdateManager.Stub {
    private static final String LOG_TAG = "SoftwareUpdate.Manager";

    private ArrayList<ISoftwareUpdateManagerCallback> clients = new ArrayList();

    private SoftwareUpdateService service;

    public SoftwareUpdateManagerImpl(SoftwareUpdateService service) {
        this.service = service;
    }

    /**
     * Updates the state for all the clients. Remove all clients that doesn't respond.
     * @param state State to be sent to all registered clients
     */
    public void UpdateState(int state) {
        Log.v(LOG_TAG, "Update state to [" + state + "]");
        for (ISoftwareUpdateManagerCallback c : clients) {
            try {
                c.UpdateState(state);
            } catch (RemoteException e) {
                clients.remove(c);
            }
        }
    }

    public void UpdateSoftwareList(ArrayList<SoftwareInformation> software_list) {
        Log.v(LOG_TAG, "Update SoftwareList with a list of size " + software_list.size());
        for (ISoftwareUpdateManagerCallback c : clients) {
            try {
                c.UpdateSoftwareList(software_list);
            } catch (RemoteException e) {
                clients.remove(c);
            }
        }
    }

    public void UpdateSoftware(SoftwareInformation software) {
        Log.v(LOG_TAG, "Update software assinment with software id: " + software.softwareId);
        for (ISoftwareUpdateManagerCallback c : clients) {
            try {
                c.UpdateSoftware(software);
            } catch (Exception e) {
                clients.remove(c);
            }
        }
    }

    public void UpdateSettings(List<Setting> settings) {
        Log.v(LOG_TAG, "Update settings");
        for (ISoftwareUpdateManagerCallback c : clients) {
            try {
                c.UpdateSettings(settings);
            } catch (Exception e) {
                clients.remove(c);
            }
        }
    }

    @Override
    public void GetState(ISoftwareUpdateManagerCallback callback) {
        clients.add(callback);
        try {
            callback.UpdateState(service.GetState());
        } catch (RemoteException e) {
            clients.remove(callback);
        }
    }

    @Override
    public void GetSoftwareInformationList(ISoftwareUpdateManagerCallback callback) {
        Log.v(LOG_TAG, "GetSoftwareInformationList");
        try {
            callback.UpdateSoftwareList(service.GetSoftwareInformationList());
        } catch (RemoteException e) {
            Log.v(LOG_TAG, "GetSoftwareInformationList RemoteException: [" + e.getMessage() + "]");
        }
    }

    @Override
    public void GetSettings(ISoftwareUpdateManagerCallback callback) {
        Log.v(LOG_TAG, "GetSettings");
        try {
            callback.UpdateSettings(service.GetSettings());
        } catch (RemoteException e) {
            Log.v(LOG_TAG, "GetSoftwareInformationList RemoteException: [" + e.getMessage() + "]");
        }
    }

    @Override
    public void GetSoftwareAssignments() {
        service.GetSoftwareAssignmentList();
    }

    @Override
    public void CommissionAssignment(String uuid) {
        service.CommissionAssignment(uuid);
    }

    @Override
    public void GetPendingInstallations() {
        service.GetPendingInstallations();
    }

    @Override
    public void GetDownloadInfo(String uuid) throws RemoteException {
        service.GetDownloadInfo(uuid);
    }

    @Override
    public void GetInstallNotification(String installationOrderId) throws RemoteException {
        service.GetInstallNotification(installationOrderId);
    }

    @Override
    public void ShowInstallationPopup(String installationOrderId) throws RemoteException {
        service.showInstallationPopup(installationOrderId);
    }

    @Override
    public void SetSetting(Setting setting) throws RemoteException {
        service.SetSetting(setting);
    }
}