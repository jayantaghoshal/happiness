/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.util.Log;
import android.app.Service;
import android.os.IBinder;
import android.content.*;
import android.os.RemoteException;
import java.util.ArrayList;
import java.util.List;

import com.volvocars.cloudservice.Query;
/**
 * Library/wrapper class that is used to access SoftwareUpdateManager features
 */
public class SoftwareUpdateManager implements ServiceConnection {
    private static final String LOG_TAG = "SoftwareUpdateManager.Lib";
    private static final String PACKAGENAME = "com.volvocars.softwareupdate";
    private static final String PACKAGENAME_SERVICENAME = "com.volvocars.softwareupdate.SoftwareUpdateService";
    private Context context = null;
    private ISoftwareUpdateManager softwareUpdateManager = null;
    private boolean serviceBound = false;

    private SoftwareUpdateManagerCallback softwareUpdateManagerCallback = null;

    @Override
    public void onServiceConnected(ComponentName className, IBinder service) {
        softwareUpdateManager = ISoftwareUpdateManager.Stub.asInterface(service);
        serviceBound = true;
        softwareUpdateManagerCallback.onServiceConnected();
    }

    @Override
    public void onServiceDisconnected(ComponentName arg0) {
        serviceBound = false;
        softwareUpdateManagerCallback.onServiceDisconnected();
    }

    public SoftwareUpdateManager(Context context, SoftwareUpdateManagerCallback softwareUpdateManagerCallback) {
        this.context = context;
        this.softwareUpdateManagerCallback = softwareUpdateManagerCallback;
        connect();
    }

    /**
     * Used to bind to SoftwareUpdateService, can be used by client if service unbinds unexpectedly
     */
    public void connect() {
        Intent intent = new Intent();
        intent.setAction("SoftwareUpdateManager");
        intent.setComponent(new ComponentName(PACKAGENAME, PACKAGENAME_SERVICENAME));
        context.bindService(intent, this, Context.BIND_AUTO_CREATE);
    }

    public void disconnect() {
        context.unbindService(this);
    }

    public void GetState(ISoftwareUpdateManagerCallback callback) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.GetState(callback);
        }
    }

    public void GetSoftwareInformationList(ISoftwareUpdateManagerCallback callback) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.GetSoftwareInformationList(callback);
        }
    }

    public void GetSettings(ISoftwareUpdateManagerCallback callback) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.GetSettings(callback);
        }
    }

    public void GetSoftwareAssignment(Query query) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.GetSoftwareAssignment(query);
        }
    }

    public void CommissionAssignment(String uuid) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.CommissionAssignment(uuid);
        }
    }

    public void GetDownloadInfo(String uuid) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.GetDownloadInfo(uuid);
        }
    }

    public void GetInstallNotification(String installationOrderId) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.GetInstallNotification(installationOrderId);
        }
    }

    public void ShowInstallationPopup(String installationOrderId) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.ShowInstallationPopup(installationOrderId);
        }
    }

    public void SetSetting(Setting setting) throws RemoteException {
        if (softwareUpdateManager != null && serviceBound) {
            softwareUpdateManager.SetSetting(setting);
        }
    }
}