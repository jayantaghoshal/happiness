/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.util.Log;

import android.os.RemoteException;

import com.volvocars.cloudservice.CommissionElement;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.Query;
import com.volvocars.softwareupdate.ISoftwareUpdateManager;
import com.volvocars.softwareupdate.ISoftwareUpdateManagerCallback;
import com.volvocars.softwareupdate.ISoftwareUpdateSettingsCallback;

import java.util.ArrayList;
import java.util.Map;
import java.util.List;

/**
 * Implementation of Foundation service API.
 */
public class SoftwareUpdateManagerImpl extends ISoftwareUpdateManager.Stub {
    private static final String LOG_TAG = "SoftwareUpdate.Manager";

    private ArrayList<ISoftwareUpdateManagerCallback> swUpdClients = new ArrayList();
    private ArrayList<ISoftwareUpdateSettingsCallback> settingClients = new ArrayList();

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

        ArrayList<ISoftwareUpdateManagerCallback> deadSwUpdClients = new ArrayList();

        for (ISoftwareUpdateManagerCallback c : swUpdClients) {
            try {
                c.UpdateState(state);
            } catch (RemoteException e) {
                deadSwUpdClients.add(c);
            }
        }

        if (deadSwUpdClients.size() > 0) {
            Log.v(LOG_TAG, "Removing " + deadSwUpdClients.size() + " dead client(s)");
            swUpdClients.removeAll(deadSwUpdClients);
        }
    }

    public void UpdateSoftwareList(ArrayList<SoftwareInformation> software_list) {
        Log.v(LOG_TAG, "Update SoftwareList with a list of size " + software_list.size());
        Log.v(LOG_TAG, "Calling UpdateSoftwareList on " + swUpdClients.size() + " clients");

        ArrayList<ISoftwareUpdateManagerCallback> deadSwUpdClients = new ArrayList();

        for (ISoftwareUpdateManagerCallback c : swUpdClients) {
            try {
                c.UpdateSoftwareList(software_list);
            } catch (RemoteException e) {
                deadSwUpdClients.add(c);
            }
        }

        if (deadSwUpdClients.size() > 0) {
            Log.v(LOG_TAG, "Removing " + deadSwUpdClients.size() + " dead client(s)");
            swUpdClients.removeAll(deadSwUpdClients);
        }
    }

    public void UpdateSoftware(SoftwareInformation software) {
        Log.v(LOG_TAG, "Update software assinment with software id: " + software.softwareAssignment.id);

        ArrayList<ISoftwareUpdateManagerCallback> deadSwUpdClients = new ArrayList();

        for (ISoftwareUpdateManagerCallback c : swUpdClients) {
            try {
                c.UpdateSoftware(software);
            } catch (Exception e) {
                deadSwUpdClients.add(c);
            }
        }

        if (deadSwUpdClients.size() > 0) {
            Log.v(LOG_TAG, "Removing " + deadSwUpdClients.size() + " dead client(s)");
            swUpdClients.removeAll(deadSwUpdClients);
        }
    }

    public void UpdateSettings(Map<String, Boolean> settings) {
        Log.v(LOG_TAG, "Update settings");

        ArrayList<ISoftwareUpdateSettingsCallback> deadSettingsClients = new ArrayList();

        for (ISoftwareUpdateSettingsCallback c : settingClients) {
            try {
                settings.forEach((key, value) -> {
                    try {
                        c.UpdateSetting(key, value);
                    } catch (RemoteException e) {
                        Log.w(LOG_TAG, "UpdateSettings RemoteException: " + e.getMessage());
                    }
                });
            } catch (Exception e) {
                deadSettingsClients.add(c);
            }
        }

        if (deadSettingsClients.size() > 0) {
            Log.v(LOG_TAG, "Removing " + deadSettingsClients.size() + " dead client(s)");
            swUpdClients.removeAll(deadSettingsClients);
        }
    }

    @Override
    public void RegisterSwUpdClient(ISoftwareUpdateManagerCallback callback) {
        Log.v(LOG_TAG, "Register SwUpd client (" + callback + ")");

        if (!swUpdClients.contains(callback)) {
            try {
                callback.UpdateState(service.GetState());
                callback.UpdateSoftwareList(service.GetSoftwareInformationList());
                swUpdClients.add(callback);
            } catch (RemoteException e) {
                Log.v(LOG_TAG, "Could not call callback method " + callback);
            }
        } else {
            Log.v(LOG_TAG, "Already registered");
        }
    }

    @Override
    public void UnregisterSwUpdClient(ISoftwareUpdateManagerCallback callback) {
        Log.v(LOG_TAG, "Unregister SwUpd client (" + callback + ")");
        if (swUpdClients.contains(callback)) {
            swUpdClients.remove(callback);
        } else {
            Log.v(LOG_TAG, "Client not found when trying to unregister");
        }
    }

    @Override
    public void RegisterSettingsClient(ISoftwareUpdateSettingsCallback callback) {
        Log.v(LOG_TAG, "Register settings client");
        if (!settingClients.contains(callback)) {
            service.GetSettings().forEach((key, value) -> {
                try {
                    callback.UpdateSetting(key, value);
                } catch (RemoteException e) {
                    Log.w(LOG_TAG, "UpdateSettings RemoteException: " + e.getMessage());
                }
            });
            settingClients.add(callback);
        } else {
            Log.v(LOG_TAG, "Already registered");
        }
    }

    @Override
    public void UnregisterSettingsClient(ISoftwareUpdateSettingsCallback callback) {
        Log.v(LOG_TAG, "Unregister settings client");
        if (settingClients.contains(callback)) {
            settingClients.remove(callback);
        } else {
            Log.v(LOG_TAG, "Client not found when trying to unregister");
        }
    }

    @Override
    public void GetSoftwareAssignment(Query query) {
        service.GetSoftwareAssignment(query);
    }

    @Override
    public void CommissionAssignment(String uuid) {
        service.CommissionAssignment(uuid, CommissionElement.Reason.USER);
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
    public void SetSetting(String key, boolean value) throws RemoteException {
        service.SetSetting(key, value);
    }
}