/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.app.Service;

import android.content.Context;
import android.content.Intent;

import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;

import android.util.Log;
import vendor.volvocars.hardware.installationmaster.V1_0.InstallationStatus;
import java.util.ArrayList;
import java.util.List;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.FoundationServicesApi;
import com.volvocars.cloudservice.FoundationServicesApiConnectionCallback;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.softwareupdate.InstallationPopup.InstallOption;
import com.volvocars.softwareupdate.SoftwareInformation.SoftwareState;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
import com.volvocars.cloudservice.InstallNotification;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.SoftwareManagementApi;
import com.volvocars.cloudservice.SoftwareManagementApiConnectionCallback;
import com.volvocars.cloudservice.Status;

/**
*
*/
public class SoftwareUpdateService extends Service {
    private static final String LOG_TAG = "SoftwareUpdate.Service";

    private Context context;

    private SoftwareUpdateManagerImpl softwareUpdateManager;

    private SoftwareManagementApi swapi = null;
    private static InstallationMaster installationMaster = null;

    private int state = 0; // Dummy state

    private ArrayList<SoftwareInformation> softwareInformationList;
    private SoftwareManagementApiCallback swapiCallback;

    private static Handler messageHandler = new MessageHandler();

    private SoftwareManagementApiConnectionCallback swapiConnectionCallback = new SoftwareManagementApiConnectionCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, "Connected to SoftwareManagementApi");

            state = 1;
            softwareUpdateManager.UpdateState(state);
        }

        @Override
        public void onServiceDisconnected() {
            Log.d(LOG_TAG, "Connection to SoftwareManagementApi was lost. How to handle?");
            state = 0;
        }
    };

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, "onCreate");
        super.onCreate();

        // Save context in order to be able to send it to SoftwareManagementApiConnectionCallback
        context = this;

        // Connect to FSApi
        swapi = new SoftwareManagementApi(context, swapiConnectionCallback);
        swapiCallback = new SoftwareManagementApiCallback(this);
        softwareInformationList = new ArrayList();

        installationMaster = new InstallationMaster(this);
        installationMaster.init();

        // Provide SUSApi
        softwareUpdateManager = new SoftwareUpdateManagerImpl(this);
    }

    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "OnBind");
        return softwareUpdateManager.asBinder(); // Binder to SUSApi
    }

    public int GetState() {
        Log.e(LOG_TAG, "GetState not implemented");
        return state;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(LOG_TAG, "onStartCommand");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.v(LOG_TAG, "onDestroy");
        super.onDestroy();
    }

    public void GetSoftwareAssignmentList() {
        if (swapi != null) {
            try {
                swapi.GetSoftwareAssigmentList(swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "GetSoftwareAssignmentList failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, "GetSoftwareAssignmentList failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void CommissionAssignment(String uuid) {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, "Commissioning assignment with uuid: " + uuid);
                swapi.CommissionSoftwareAssignment(uuid, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "CommissionAssignment failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, "CommissionAssignment failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void GetPendingInstallations() {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, "GetPendingInstallations");
                swapi.GetPendingInstallations(swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "GetPendingInstallations failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, "GetPendingInstallations failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void doGetDownloadInfo(List<InstallationOrder> list) {
        for (InstallationOrder io : list) {
            GetDownloadInfo(io.uuid);
        }
    }

    public void GetDownloadInfo(String uuid) {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, "Get download information for assignment with uuid: " + uuid);
                swapi.GetDownloadInfo(uuid, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "GetDownloadInfo failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, "GetDownloadInfo failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void download(DownloadInfo downloadInfo) {
        GetDownloadData(downloadInfo);
    }

    public void GetDownloadData(DownloadInfo downloadInfo) {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, "Get download data for downloadInfo with uuid: " + downloadInfo.uuid);
                swapi.GetDownloadData(downloadInfo, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "GetDownloadData failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, "GetDownloadData failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void assignInstallation(String uuid) {
        if (installationMaster != null) {

            Log.v(LOG_TAG, "Assigning installation for installation order with uuid:" + uuid);
            installationMaster.assignInstallation(uuid);
        } else {
            Log.e(LOG_TAG, "ConfirmInstallation failed: Local InstallationMaster variable is null");
        }
    }

    public void onInstallationNotification(String uuid, String notification) {
        Log.v(LOG_TAG, "onInstallNotificaion: [installationOrderID: " + uuid + ", notification: " + notification + "]");

        Log.w(LOG_TAG, "Todo: Construct a real install notification, only sending a \"hacked\" one for testing purpose...");
        InstallNotification installNotification = new InstallNotification();
        installNotification.installationOrderId = uuid;

        if (notification.equals(InstallationStatus.toString(InstallationStatus.INSTALLATION_STARTED))) {
            UpdateSoftwareState(uuid, SoftwareState.INSTALLING);
            installNotification.notification.status.statusCode = Status.StatusCode.IN_PROGRESS;
            installNotification.notification.status.subStatusCode = Status.SubStatusCode.INSTALLATION_STARTED;

        } else if (notification.equals(InstallationStatus.toString(InstallationStatus.INSTALLATION_COMPLETE))) {
            UpdateSoftwareState(uuid, SoftwareState.INSTALLED);
        } else {
            Log.d(LOG_TAG, "InstallNotification of type " + notification + "is not handled");
        }

        try {
            swapi.PostInstallNotification(installNotification, swapiCallback);
        } catch (RemoteException e) {
            Log.e(LOG_TAG, "onInstallationNotification failed: RemoteException [" + e.getMessage() + "]");
        }
    }

    public void UpdateSoftwareList(List<SoftwareAssignment> softwareAssignments) {
        for (SoftwareAssignment assignment : softwareAssignments) {
            boolean found = false;
            for (SoftwareInformation information : softwareInformationList) {
                if (assignment.uuid.equals(information.softwareId)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                softwareInformationList.add(new SoftwareInformation(assignment));
            }
        }

        softwareUpdateManager.UpdateSoftwareList(softwareInformationList);
    }

    public void UpdateSoftwareListWithInstallationOrders(List<InstallationOrder> installationOrders) {
        for (InstallationOrder order : installationOrders) {
            boolean found = false;
            for (SoftwareInformation information : softwareInformationList) {
                if (order.software.uuid.equals(information.softwareId)) {
                    found = true;
                    information.AddInstallationOrder(order);
                    break;
                }
            }
            if (!found) {
                softwareInformationList.add(new SoftwareInformation(order));
            }
        }
        softwareUpdateManager.UpdateSoftwareList(softwareInformationList);
    }

    public void UpdateSoftwareList(DownloadInfo downloadInfo) {
        boolean found = false;
        for (SoftwareInformation information : softwareInformationList) {
            if (downloadInfo.uuid.equals(information.installationId)) {
                found = true;
                information.AddDownloadInfo(downloadInfo);
                break;
            }
        }
        if (!found) {
            Log.v(LOG_TAG, "UpdateSoftwareList(downloadInfo), uuid [" + downloadInfo.uuid
                    + "] not found in list which is weird...");
        }

        softwareUpdateManager.UpdateSoftwareList(softwareInformationList);
    }

    public void UpdateSoftwareState(String uuid, SoftwareInformation.SoftwareState state) {
        boolean found = false;
        for (SoftwareInformation information : softwareInformationList) {
            if (uuid.equals(information.softwareId) || uuid.equals(information.installationId)) {
                found = true;
                information.softwareState = state;
                softwareUpdateManager.UpdateSoftware(information);
                break;
            }
        }
        if (!found) {
            Log.v(LOG_TAG, "UpdateSoftwareState, uuid [" + uuid + "] not found in list which is weird...");
        }
    }

    public void showInstallationPopup(DownloadInfo info) {
        Log.v(LOG_TAG, "showInstallationPopup,  Note: Temporary solution until framework for popups is in place!");

        for (SoftwareInformation information : softwareInformationList) {
            if (info.uuid.equals(information.installationId)) {
                Intent intent = new Intent(this, InstallationPopup.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_NEW_TASK);
                intent.putExtra(InstallationPopup.NAME, information.name);
                intent.putExtra(InstallationPopup.UUID, information.installationId);
                intent.putExtra("MESSENGER", new Messenger(messageHandler));
                startActivity(intent);
            }
        }
    }

    public static class MessageHandler extends Handler {
        @Override
        public void handleMessage(Message message) {
            Bundle bundle = message.getData();
            InstallOption option = (InstallOption) bundle.get(InstallationPopup.OPTION);
            String uuid = (String) bundle.get(InstallationPopup.UUID);
            switch (option) {
            case INSTALL:
                Log.v(LOG_TAG, "handleMessage: [option: INSTALL, uuid: " + uuid + "]");
                installationMaster.assignInstallation(uuid);
                break;
            case CANCEL:
                Log.v(LOG_TAG, "handleMessage: [option: CANCEL, uuid: " + uuid + "]");
                break;
            case POSTPONE:
                Log.v(LOG_TAG, "handleMessage: [option: POSTPONE, uuid: " + uuid + "]");
                break;
            }
        }
    }
}