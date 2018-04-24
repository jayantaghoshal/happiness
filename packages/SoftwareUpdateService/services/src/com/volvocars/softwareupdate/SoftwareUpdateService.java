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

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.List;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.DownloadSummary;
import com.volvocars.cloudservice.InstallationSummary;
import com.volvocars.cloudservice.CommissionElement;
import com.volvocars.cloudservice.DataFile;
import com.volvocars.cloudservice.Ecu;
import com.volvocars.cloudservice.SoftwarePart;
import com.volvocars.cloudservice.FoundationServicesApi;
import com.volvocars.cloudservice.FoundationServicesApiConnectionCallback;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.InstallationReport;
import com.volvocars.softwareupdate.InstallationPopup.InstallOption;
import com.volvocars.softwareupdate.SoftwareInformation.SoftwareState;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
import com.volvocars.cloudservice.InstallNotification;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.SoftwareManagementApi;
import com.volvocars.cloudservice.SoftwareManagementApiConnectionCallback;
import com.volvocars.cloudservice.Status;
import com.volvocars.cloudservice.Query;

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
    private Map<String, Boolean> settings;

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
        settings = new HashMap();

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

    public ArrayList<SoftwareInformation> GetSoftwareInformationList() {
        return softwareInformationList;
    }

    public Map<String, Boolean> GetSettings() {
        return settings;
    }

    public void GetSoftwareAssignment(Query query) {
        if (swapi != null) {
            try {
                swapi.GetSoftwareAssignment(query, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "GetSoftwareAssignmentList failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, "GetSoftwareAssignmentList failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void onNewSoftwareAssignmentList(List<SoftwareAssignment> software_list) {
        //Check if software has been removed from list (and downloaded) to clear local storage
        clearStorageOnRemovedSoftwareAssignment(software_list);

        //Check status of software assignment if there is an action to be taken
        for (SoftwareAssignment software : software_list) {
            if (SoftwareAssignment.Status.COMMISSIONED == software.status) {
                doGetDownloadInfo(software.installationOrder);
            }
            if ((SoftwareAssignment.Status.COMMISSIONABLE == software.status)
                    && (settings.getOrDefault("AUTO_DOWNLOAD", false))) {
                CommissionAssignment(software.id, CommissionElement.Reason.AUTOMATIC_UPDATE);
            }
        }

        UpdateSoftwareList(software_list);
    }

    public void clearStorageOnRemovedSoftwareAssignment(List<SoftwareAssignment> software_list) {
        for (SoftwareInformation swInfo : softwareInformationList) {
            boolean found = false;
            for (SoftwareAssignment swAssignment : software_list) {
                if (swInfo.softwareAssignment.id.equals(swAssignment.id)) {
                    found = true;
                    break;
                }
            }

            if (!found && !swInfo.downloadInfo.downloadedResources.isEmpty()) {
                Log.d(LOG_TAG, "SoftwareAssignment " + swInfo.softwareAssignment.name + " removed from list");

                String directory = "/data/vendor/ota/" + swInfo.softwareAssignment.installationOrder.id;
                File file = new File(directory);
                try {
                    delete(file);
                } catch (IOException e) {
                    Log.w(LOG_TAG, "Directory " + directory + "could not be deleted [IOException: " + e.getMessage() + "]");
                }

            }
        }
    }

    private void delete(File file) throws IOException {
        for (File childFile : file.listFiles()) {
            if (childFile.isDirectory()) {
                delete(childFile);
            } else {
                if (!childFile.delete()) {
                    throw new IOException();
                }
            }
        }

        if (!file.delete()) {
            throw new IOException();
        }
    }

    public void CommissionAssignment(String uuid, CommissionElement.Reason reason) {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, "CommissionAssignment called with uuid: " + uuid + " and reason: " + reason.name());
                for (SoftwareInformation software : softwareInformationList) {
                    //check if assignment is valid for commssion (i.e in status COMMISSONABLE)
                    if (software.softwareAssignment.id.equals(uuid)) {
                        if (software.softwareAssignment.status.equals(SoftwareAssignment.Status.COMMISSIONABLE)) {
                            CommissionElement commissionElement = new CommissionElement();
                            commissionElement.id = uuid;
                            commissionElement.action = CommissionElement.Action.ORDER_SOFTWARE_INSTALLATION;
                            commissionElement.reason = reason;
                            commissionElement.commissionUri = software.softwareAssignment.commissionUri;
                            Log.w(LOG_TAG,
                                    "Calling CommissionSoftwareAssignment: client_id not set (implement!)");
                            swapi.CommissionSoftwareAssignment(commissionElement, swapiCallback);
                        } else {
                            Log.v(LOG_TAG, "CommissionSoftwareAssignment not called, software status is: "
                                    + software.softwareAssignment.status.name());
                        }
                    }
                }
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "CommissionAssignment failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, "CommissionAssignment failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void doGetDownloadInfo(InstallationOrder installationOrder) {
        GetDownloadInfo(installationOrder.id);
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
                Log.v(LOG_TAG, "Get download data for downloadInfo with id: " + downloadInfo.id);
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

        Log.w(LOG_TAG,
                "Todo: Construct a real install notification, only sending a \"hacked\" one for testing purpose...");
        InstallNotification installNotification = new InstallNotification();
        if (notification.equals("INSTALLATION_STARTED"))
            installNotification.notification.status.statusCode = Status.StatusCode.IN_PROGRESS;
        else if (notification.equals("INSTALLATION_COMPLETE"))
            installNotification.notification.status.statusCode = Status.StatusCode.OK;
        installNotification.installationOrderId = uuid;

        try {
            swapi.PostInstallNotification(installNotification, swapiCallback);
        } catch (RemoteException e) {
            Log.e(LOG_TAG, "onInstallationNotification failed: RemoteException [" + e.getMessage() + "]");
        }
    }

    public void onInstallationReport(String installationOrder, InstallationSummary installationSummary) {
        Log.v(LOG_TAG, "onInstallationReport: [installationOrderID: " + installationOrder + ", installation summary: "
                + installationSummary.softwareId + "]");
        Log.w(LOG_TAG,
                "Todo: Construct a real installation report, only sending a \"hacked\" one for testing purpose...");
        InstallationReport installationReport = new InstallationReport();
        DownloadSummary downloadSummary = new DownloadSummary();
        DataFile file = new DataFile();
        file.identifier = "hello world";
        List<DataFile> dataFiles = new ArrayList();
        dataFiles.add(file);

        downloadSummary.dataFiles = dataFiles;

        installationReport.installationOrderId = installationOrder;
        installationReport.downloadSummary = downloadSummary;
        installationReport.installationSummary = installationSummary;

        Log.d(LOG_TAG, "created report");
        try {
            Log.i(LOG_TAG, "sending installation report");
            swapi.PostInstallationReport(installationReport, swapiCallback);
        } catch (RemoteException e) {
            Log.e(LOG_TAG, "onInstallationReport failed: RemoteException [" + e.getMessage() + "]");
        }
    }

    public void GetInstallNotification(String installationOrderId) {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, "GetInstallNotification [installationOrderId: " + installationOrderId + "]");
                swapi.GetInstallNotification(installationOrderId, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "GetInstallNotification failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, "GetInstallNotification failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void SetSetting(String key, boolean value) {
        settings.put(key, value);
    }

    public void UpdateSoftwareList(List<SoftwareAssignment> softwareAssignments) {
        Log.v(LOG_TAG, "UpdateSoftwareList: Clearing softwareInformationList");
        softwareInformationList.clear();
        Log.v(LOG_TAG,
                "UpdateSoftwareList: Before update, softwareInformationList.size = " + softwareInformationList.size());

        for (SoftwareAssignment assignment : softwareAssignments) {
            boolean found = false;
            for (SoftwareInformation information : softwareInformationList) {
                if (assignment.id.equals(information.softwareAssignment.id)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                softwareInformationList.add(new SoftwareInformation(assignment));
            }
        }

        Log.v(LOG_TAG,
                "UpdateSoftwareList: After update, softwareInformationList.size = " + softwareInformationList.size());

        softwareUpdateManager.UpdateSoftwareList(softwareInformationList);
    }

    public void UpdateSoftwareList(DownloadInfo downloadInfo) {
        boolean found = false;
        for (SoftwareInformation information : softwareInformationList) {
            if (downloadInfo.installationOrderId.equals(information.softwareAssignment.installationOrder.id)) {
                found = true;
                information.AddDownloadInfo(downloadInfo);
                Log.e(LOG_TAG, "" + information.softwareState);
                break;
            }
        }
        if (!found) {
            Log.v(LOG_TAG, "UpdateSoftwareList(downloadInfo), id [" + downloadInfo.id
                    + "] not found in list which is weird...");
        }

        softwareUpdateManager.UpdateSoftwareList(softwareInformationList);
    }

    public void UpdateSoftwareState(String uuid, SoftwareInformation.SoftwareState state) {
        boolean found = false;
        for (SoftwareInformation information : softwareInformationList) {
            if (uuid.equals(information.softwareAssignment.id)
                    || uuid.equals(information.softwareAssignment.installationOrder.id)) {
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

    public void showInstallationPopup(String installationOrderId) {
        Log.v(LOG_TAG, "showInstallationPopup,  Note: Temporary solution until framework for popups is in place!");

        for (SoftwareInformation information : softwareInformationList) {
            if (installationOrderId.equals(information.softwareAssignment.installationOrder.id)) {
                Intent intent = new Intent(this, InstallationPopup.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_NEW_TASK);
                intent.putExtra(InstallationPopup.NAME, information.softwareAssignment.name);
                intent.putExtra(InstallationPopup.UUID, information.softwareAssignment.installationOrder.id);
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