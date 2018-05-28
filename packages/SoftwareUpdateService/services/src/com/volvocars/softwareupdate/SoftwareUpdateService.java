/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.app.Service;

import android.content.Context;
import android.content.Intent;
import android.content.ComponentName;
import android.content.ServiceConnection;

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

// Car related...
import android.car.Car;
import android.car.CarNotConnectedException;
import android.car.hardware.CarSensorManager;
import android.car.hardware.CarSensorEvent;
import android.car.hardware.CarSensorManager.OnSensorChangedListener;

import com.volvocars.cloudservice.AssignmentType;
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
import com.volvocars.softwareupdate.InstallOption;
import com.volvocars.softwareupdate.SoftwareInformation.SoftwareState;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
import com.volvocars.cloudservice.InstallNotification;
import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.cloudservice.SoftwareManagementApi;
import com.volvocars.cloudservice.SoftwareManagementApiConnectionCallback;
import com.volvocars.cloudservice.Status;
import com.volvocars.cloudservice.CommissionElement.Reason;
import com.volvocars.cloudservice.SoftwareAssignment.DeliverableType;
import com.volvocars.cloudservice.Query;
import com.volvocars.settingsstorageservice.SettingsStorageManager;
import com.volvocars.settingsstorageservice.SettingsStorageManagerConnectionCallback;

/**
*
*/
public class SoftwareUpdateService extends Service {
    private static final String LOG_TAG = "SoftwareUpdateService";
    private static final String LOG_PREFIX = "[SoftwareUpdateService]";

    private Context context;

    private SoftwareUpdateManagerImpl softwareUpdateManager;

    private SoftwareManagementApi swapi = null;
    private static InstallationMaster installationMaster = null;

    private SettingsStorageManager settingsStorageManager;
    private SettingsStorageManagerCallback settingsCallback;
    private final String appId = "SoftwareUpdate";

    private int state = 0; // Dummy state

    private ArrayList<SoftwareInformation> softwareInformationListUpdates;
    private ArrayList<SoftwareInformation> softwareInformationListAccessory;
    private ArrayList<SoftwareInformation> mergedSoftwareInformationList;

    private Map<String, Boolean> settings;
    private SoftwareManagementApiCallback swapiCallback;

    private Car car = null;
    private CarSensorManager carSensorManager = null;
    private int ignitionState = CarSensorEvent.IGNITION_STATE_UNDEFINED;

    private SoftwareManagementApiConnectionCallback swapiConnectionCallback = new SoftwareManagementApiConnectionCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, LOG_PREFIX + " Connected to SoftwareManagementApi");

            state = 1;
            softwareUpdateManager.UpdateState(state);
        }

        @Override
        public void onServiceDisconnected() {
            Log.d(LOG_TAG, LOG_PREFIX + " Connection to SoftwareManagementApi was lost. How to handle?");
            state = 0;
        }
    };

    private SettingsStorageManagerConnectionCallback settingsConnectionCallback = new SettingsStorageManagerConnectionCallback() {

        @Override
        public void onServiceDisconnected() {
            Log.d(LOG_TAG, LOG_PREFIX + " Connection to SettingsStorageManager was lost, How to handle?");
        }

        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, LOG_PREFIX + " Connected to SettingsStorageManager");
            try {
                settingsStorageManager.getAsyncByString(appId, "ota_enabled", settingsCallback);
                settingsStorageManager.getAsyncByString(appId, "automatic_download_enabled", settingsCallback);

            } catch (RemoteException e) {
                Log.d(LOG_TAG, LOG_PREFIX + " settingsStorageManager.registerClient RemoteException: [" + e.getMessage()
                        + "]");
            }
        }
    };

    @Override
    public void onCreate() {
        Log.v(LOG_TAG, LOG_PREFIX + " onCreate");
        super.onCreate();

        // Save context in order to be able to send it to
        // SoftwareManagementApiConnectionCallback
        context = this;

        // Connect to FSApi
        swapi = new SoftwareManagementApi(context, swapiConnectionCallback);
        swapiCallback = new SoftwareManagementApiCallback(this);

        // Connect so SettingsStorageService
        settingsStorageManager = new SettingsStorageManager(context, settingsConnectionCallback);
        settingsCallback = new SettingsStorageManagerCallback(this);

        softwareInformationListUpdates = new ArrayList();
        softwareInformationListAccessory = new ArrayList();
        mergedSoftwareInformationList = new ArrayList();

        settings = new HashMap();

        installationMaster = new InstallationMaster(this);
        installationMaster.init();

        // Provide SUSApi
        softwareUpdateManager = new SoftwareUpdateManagerImpl(this);

        Log.v(LOG_TAG, LOG_PREFIX + " Creating car");
        car = Car.createCar(this, carServiceConnection);
        Log.v(LOG_TAG, LOG_PREFIX + " Connecting to car");
        car.connect();
    }

    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, LOG_PREFIX + " OnBind");
        return softwareUpdateManager.asBinder(); // Binder to SUSApi
    }

    public int GetState() {
        Log.e(LOG_TAG, LOG_PREFIX + " GetState not implemented");
        return state;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(LOG_TAG, LOG_PREFIX + " onStartCommand");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.v(LOG_TAG, LOG_PREFIX + " onDestroy");
        super.onDestroy();
    }

    public ArrayList<SoftwareInformation> GetSoftwareInformationList() {
        return mergedSoftwareInformationList;
    }

    public Map<String, Boolean> GetSettings() {
        return settings;
    }

    public void GetSoftwareAssignment(Query query, AssignmentType type) {
        if (swapi != null) {
            try {
                swapi.GetSoftwareAssignment(query, type, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG,
                        LOG_PREFIX + " GetSoftwareAssignmentList failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG,
                    LOG_PREFIX + " GetSoftwareAssignmentList failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void onNewSoftwareAssignmentList(List<SoftwareAssignment> software_list, AssignmentType type) {
        // Check if software has been removed from list (and downloaded) to clear local
        // storage
        clearStorageOnRemovedSoftwareAssignment(software_list);
        UpdateSoftwareList(software_list, type);

        // Check status of software assignment if there is an action to be taken
        for (SoftwareAssignment software : software_list) {

            if (SoftwareAssignment.Status.COMMISSIONED == software.status) {
                doGetDownloadInfo(software.installationOrder);
            }
            if ((SoftwareAssignment.Status.COMMISSIONABLE == software.status)
                    && (settings.getOrDefault("automatic_download_enabled", false))) {
                CommissionAssignment(software.id, CommissionElement.Reason.AUTOMATIC_UPDATE);
            }
        }
    }

    public void onNewSoftwareAssignment(SoftwareAssignment assignment, AssignmentType type) {
        for (SoftwareInformation swInfo : ((AssignmentType.UPDATE == type) ? softwareInformationListUpdates
                : softwareInformationListAccessory)) {
            if ((SoftwareState.DOWNLOADED == swInfo.softwareState)
                    && swInfo.softwareAssignment.id.equals(assignment.id)) {
                softwareUpdateManager.showInstallationPopup(assignment);
                break;
            }
        }
    }

    public void onDownloadedAssignment(DownloadInfo info) {
        Query query = new Query();
        query.installationOrderId = info.installationOrderId;
        Log.d(LOG_TAG, LOG_PREFIX + " id " + info.id);

        boolean found = false;
        for (SoftwareInformation software : mergedSoftwareInformationList) {
            if (software.softwareAssignment.id.equals(info.id)) {
                found = true;
                if (DeliverableType.NEW == software.softwareAssignment.deliverableType) {
                    GetSoftwareAssignment(query, AssignmentType.ACCESSORY);
                } else if (DeliverableType.UPDATE == software.softwareAssignment.deliverableType) {
                    GetSoftwareAssignment(query, AssignmentType.UPDATE);
                } else {
                    Log.w(LOG_TAG, LOG_PREFIX + " Unknown deliverable type of software " + info.id);
                }
            }
        }

        if (!found) {
            Log.w(LOG_TAG, LOG_PREFIX + " Software " + info.id + "was not found in any list... what to do?");
        }
    }

    public void clearStorageOnRemovedSoftwareAssignment(List<SoftwareAssignment> software_list) {

        for (SoftwareInformation swInfo : mergedSoftwareInformationList) {
            boolean found = false;
            for (SoftwareAssignment swAssignment : software_list) {
                if (swInfo.softwareAssignment.id.equals(swAssignment.id)) {
                    found = true;
                    break;
                }
            }

            if (!found && !swInfo.downloadInfo.downloadedResources.isEmpty()) {
                Log.d(LOG_TAG,
                        LOG_PREFIX + " SoftwareAssignment " + swInfo.softwareAssignment.name + " removed from list");

                String directory = "/data/vendor/ota/" + swInfo.softwareAssignment.installationOrder.id;
                File file = new File(directory);
                try {
                    delete(file);
                } catch (IOException e) {
                    Log.w(LOG_TAG, LOG_PREFIX + " Directory " + directory + "could not be deleted [IOException: "
                            + e.getMessage() + "]");
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
                Log.v(LOG_TAG, LOG_PREFIX + " CommissionAssignment called with uuid: " + uuid + " and reason: "
                        + reason.name());

                for (SoftwareInformation software : mergedSoftwareInformationList) {
                    if (software.softwareAssignment.id.equals(uuid)) {
                        // check if assignment is valid for commission (i.e in status COMMISSONABLE)
                        if (software.softwareAssignment.status.equals(SoftwareAssignment.Status.COMMISSIONABLE)) {
                            CommissionElement commissionElement = new CommissionElement();
                            commissionElement.id = uuid;
                            commissionElement.action = CommissionElement.Action.ORDER_SOFTWARE_INSTALLATION;
                            commissionElement.reason = reason;
                            commissionElement.commissionUri = software.softwareAssignment.commissionUri;
                            Log.w(LOG_TAG, LOG_PREFIX + " Calling CommissionSoftwareAssignment: client_id not set (implement!)");
                            swapi.CommissionSoftwareAssignment(commissionElement, swapiCallback);
                        } else {
                            Log.v(LOG_TAG, LOG_PREFIX + " CommissionSoftwareAssignment not called, software status is: "
                                    + software.softwareAssignment.status.name());
                        }
                    }
                }
            } catch (RemoteException e) {
                Log.e(LOG_TAG, LOG_PREFIX + " CommissionAssignment failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, LOG_PREFIX + " CommissionAssignment failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void doGetDownloadInfo(InstallationOrder installationOrder) {
        GetDownloadInfo(installationOrder);
    }

    public void GetDownloadInfo(InstallationOrder installationOrder) {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, LOG_PREFIX + " Get download information for assignment with installationOrderId: "
                        + installationOrder.id);
                swapi.GetDownloadInfo(installationOrder, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, LOG_PREFIX + " GetDownloadInfo failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, LOG_PREFIX + " GetDownloadInfo failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void download(DownloadInfo downloadInfo) {
        GetDownloadData(downloadInfo);
    }

    public void GetDownloadData(DownloadInfo downloadInfo) {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, LOG_PREFIX + " Get download data for downloadInfo with id: " + downloadInfo.id);
                swapi.GetDownloadData(downloadInfo, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, LOG_PREFIX + " GetDownloadData failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, LOG_PREFIX + " GetDownloadData failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void assignInstallation(String uuid) {
        if (installationMaster != null) {

            Log.v(LOG_TAG, LOG_PREFIX + " Assigning installation for installation order with uuid:" + uuid);
            installationMaster.assignInstallation(uuid);
        } else {
            Log.e(LOG_TAG, LOG_PREFIX + " ConfirmInstallation failed: Local InstallationMaster variable is null");
        }
    }

    public void onInstallationNotification(String uuid, String notification) {
        Log.v(LOG_TAG, LOG_PREFIX + " onInstallNotificaion: [installationOrderID: " + uuid + ", notification: "
                + notification + "]");

        Log.w(LOG_TAG, LOG_PREFIX
                + " Todo: Construct a real install notification, only sending a \"hacked\" one for testing purpose...");
        InstallNotification installNotification = new InstallNotification();

        for (SoftwareInformation softwareInformation : mergedSoftwareInformationList) {
            if (softwareInformation.softwareAssignment.installationOrder.id.equals(uuid)) {
                installNotification.uri = softwareInformation.softwareAssignment.installationOrder.installNotificationsUri;
            } else {
                Log.w(LOG_TAG, LOG_PREFIX + " InstallationOrder " + uuid
                        + " was not found in list! Couldn't set installation notification uri!");
            }
        }

        if (notification.equals("INSTALLATION_STARTED"))
            installNotification.notification.status.statusCode = Status.StatusCode.IN_PROGRESS;
        else if (notification.equals("INSTALLATION_COMPLETE"))
            installNotification.notification.status.statusCode = Status.StatusCode.OK;
        installNotification.installationOrderId = uuid;

        try {
            swapi.PostInstallNotification(installNotification, swapiCallback);
        } catch (RemoteException e) {
            Log.e(LOG_TAG, LOG_PREFIX + " onInstallationNotification failed: RemoteException [" + e.getMessage() + "]");
        }
    }

    public void onInstallationReport(String installationOrder, InstallationSummary installationSummary) {
        Log.v(LOG_TAG, LOG_PREFIX + " onInstallationReport: [installationOrderID: " + installationOrder
                + ", installation summary: " + installationSummary.softwareId + "]");
        Log.w(LOG_TAG, LOG_PREFIX
                + " Todo: Construct a real installation report, only sending a \"hacked\" one for testing purpose...");
        InstallationReport installationReport = new InstallationReport();

        for (SoftwareInformation softwareInformation : mergedSoftwareInformationList) {
            if (softwareInformation.softwareAssignment.installationOrder.id.equals(installationOrder)) {
                installationReport.uri = softwareInformation.softwareAssignment.installationOrder.installationReportUri;
            } else {
                Log.w(LOG_TAG, LOG_PREFIX + " InstallationOrder " + installationOrder
                        + " was not found in list! Couldn't set installation notification uri!");
            }
        }

        installationReport.installationOrderId = installationOrder;
        installationReport.reportReason = InstallationReport.ReportReason.OK;
        installationReport.timestamp = "2018-05-08T10:00:00";

        Log.d(LOG_TAG, LOG_PREFIX + " created report");
        try {
            Log.i(LOG_TAG, LOG_PREFIX + " sending installation report");
            swapi.PostInstallationReport(installationReport, swapiCallback);
        } catch (RemoteException e) {
            Log.e(LOG_TAG, LOG_PREFIX + " onInstallationReport failed: RemoteException [" + e.getMessage() + "]");
        }
    }

    public void GetInstallNotification(String installationOrderId) {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG,
                        LOG_PREFIX + " GetInstallNotification [installationOrderId: " + installationOrderId + "]");
                for (SoftwareInformation softwareInformation : mergedSoftwareInformationList) {
                    if (softwareInformation.softwareAssignment.installationOrder.id.equals(installationOrderId)) {
                        swapi.GetInstallNotification(installationOrderId,
                                softwareInformation.softwareAssignment.installationOrder.installNotificationsUri,
                                swapiCallback);
                    } else {
                        Log.w(LOG_TAG, LOG_PREFIX + " installationorder " + installationOrderId
                                + "was not found in list! No uri to install notification => GetInstallNotification not sent!");
                    }
                }
            } catch (RemoteException e) {
                Log.e(LOG_TAG, LOG_PREFIX + " GetInstallNotification failed: RemoteException [" + e.getMessage() + "]");
            }
        } else {
            Log.e(LOG_TAG, LOG_PREFIX + " GetInstallNotification failed: Local SoftwareManagementApi variable is null");
        }
    }

    public void SetSetting(String key, boolean value) {
        settings.put(key, value);
        try {
            settingsStorageManager.setByString(appId, key, value ? "1" : "0");
        } catch (RemoteException e) {
            // TODO: handle exception
        }
    }

    public void OnInstallationPopup(InstallOption option, String uuid) {
        switch (option) {
        case INSTALL:
            Log.v(LOG_TAG, LOG_PREFIX + " OnInstallationPopup: [option: INSTALL, uuid: " + uuid + "]");
            installationMaster.assignInstallation(uuid);
            break;
        case CANCEL:
            Log.v(LOG_TAG,
                    LOG_PREFIX + " OnInstallationPopup: [option: CANCEL, uuid: " + uuid + "] (Option not implemented)");
            break;
        case POSTPONE:
            Log.v(LOG_TAG, LOG_PREFIX + " OnInstallationPopup: [option: POSTPONE, uuid: " + uuid
                    + "] (Option not implemented)");
            break;
        }
    }

    public void UpdateSoftwareList(List<SoftwareAssignment> softwareAssignments, AssignmentType type) {
        Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftwareList of type " + type);
        if (AssignmentType.UPDATE == type) {
            Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftwareList: Before update, softwareInformationListUpdates.size = "
                    + softwareInformationListUpdates.size());
            softwareInformationListUpdates.clear();
            for (SoftwareAssignment assignment : softwareAssignments) {
                softwareInformationListUpdates.add(new SoftwareInformation(assignment));
            }
            Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftwareList: After update, softwareInformationListUpdates.size = "
                    + softwareInformationListUpdates.size());
        } else if (AssignmentType.ACCESSORY == type) {
            Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftwareList: Before update, softwareInformationListAccessory.size = "
                    + softwareInformationListAccessory.size());
            softwareInformationListAccessory.clear();
            for (SoftwareAssignment assignment : softwareAssignments) {
                softwareInformationListAccessory.add(new SoftwareInformation(assignment));
            }
            Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftwareList: After update, softwareInformationListAccessory.size = "
                    + softwareInformationListAccessory.size());
        } else {
            Log.w(LOG_TAG, LOG_PREFIX + " Unknown AssignmentType: " + type);
        }

        mergedSoftwareInformationList.clear();
        mergedSoftwareInformationList.addAll(softwareInformationListUpdates);
        mergedSoftwareInformationList.addAll(softwareInformationListAccessory);

        softwareUpdateManager.UpdateSoftwareList(mergedSoftwareInformationList);
    }

    public void UpdateSoftwareList(DownloadInfo downloadInfo) {
        boolean found = false;

        for (SoftwareInformation information : mergedSoftwareInformationList) {
            if (downloadInfo.installationOrderId.equals(information.softwareAssignment.installationOrder.id)) {
                found = true;
                information.AddDownloadInfo(downloadInfo);
                Log.e(LOG_TAG, LOG_PREFIX + "" + information.softwareState);
                break;
            }
        }
        if (!found) {
            Log.v(LOG_TAG, LOG_PREFIX + " UpdateSoftwareList(downloadInfo), id [" + downloadInfo.id
                    + "] not found in list which is weird...");
        }

        ArrayList<SoftwareInformation> mergedList = new ArrayList<>(softwareInformationListUpdates);
        mergedList.addAll(softwareInformationListAccessory);
        softwareUpdateManager.UpdateSoftwareList(mergedList);
    }

    public void UpdateSoftwareState(String uuid, SoftwareInformation.SoftwareState state) {
        boolean found = false;

        for (SoftwareInformation information : mergedSoftwareInformationList) {
            if (uuid.equals(information.softwareAssignment.id)
                    || uuid.equals(information.softwareAssignment.installationOrder.id)) {
                found = true;
                information.softwareState = state;
                softwareUpdateManager.UpdateSoftware(information);
                break;
            }
        }

        if (!found) {
            Log.v(LOG_TAG,
                    LOG_PREFIX + " UpdateSoftwareState, uuid [" + uuid + "] not found in list which is weird...");
        }
    }

    public void UpdateSetting(String setting, String value) {
        Log.v(LOG_TAG, LOG_PREFIX + " UpdateSetting [setting: " + setting + ", value: " + value + "]");
        if (!value.isEmpty()) {
            settings.put(setting, (Integer.parseInt(value) != 0));
            softwareUpdateManager.UpdateSettings(settings);
        } else {
            Log.w(LOG_TAG, LOG_PREFIX + " Setting: " + setting + "is empty, what to do?");
        }
    }

    private final OnSensorChangedListener ignitionStateChangeHandler = new OnSensorChangedListener() {

        @Override
        public void onSensorChanged(final CarSensorEvent event) {
            if (ignitionState != event.intValues[0]) {
                ignitionState = event.intValues[0];
                Log.i(LOG_TAG, LOG_PREFIX + " Drive state changed");
            }
        }
    };

    private final ServiceConnection carServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            synchronized (this) {
                try {
                    Log.v(LOG_TAG, LOG_PREFIX + " Car service connected");

                    carSensorManager = (CarSensorManager) car.getCarManager(Car.SENSOR_SERVICE);

                    ignitionState = carSensorManager
                            .getLatestSensorEvent(CarSensorManager.SENSOR_TYPE_IGNITION_STATE).intValues[0];

                    carSensorManager.registerListener(ignitionStateChangeHandler,
                            CarSensorManager.SENSOR_TYPE_IGNITION_STATE, CarSensorManager.SENSOR_RATE_UI);
                } catch (CarNotConnectedException ex) {
                    Log.e(LOG_TAG, LOG_PREFIX + " Exception thrown: " + ex);
                }
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            synchronized (this) {
                Log.v(LOG_TAG, LOG_PREFIX + " Car service disconnected");

                carSensorManager.unregisterListener(ignitionStateChangeHandler);

                carSensorManager = null;

                ignitionState = CarSensorEvent.IGNITION_STATE_UNDEFINED;
            }
        }
    };
}