/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.app.Service;

import android.content.Context;
import android.content.Intent;

import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;

import com.volvocars.cloudservice.DownloadInfo;
import com.volvocars.cloudservice.FoundationServicesApi;
import com.volvocars.cloudservice.FoundationServicesApiConnectionCallback;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.ISoftwareManagementApiCallback;
import com.volvocars.cloudservice.InstallationOrder;
import com.volvocars.cloudservice.SoftwareManagementApi;
import com.volvocars.cloudservice.SoftwareManagementApiConnectionCallback;
import com.volvocars.cloudservice.SoftwareAssignment;

/**
*
*/
public class SoftwareUpdateService extends Service {
    private static final String LOG_TAG = "SwUpdService";

    private Context context;

    private SoftwareUpdateManagerImpl softwareUpdateManager;

    private SoftwareManagementApi swapi = null;

    private int state = 0; // Dummy state

    private ArrayList<SoftwareInformation> softwareInformationList;
    private SoftwareManagementApiCallback swapiCallback;
    private SoftwareManagementApiConnectionCallback swapiConnectionCallback = new SoftwareManagementApiConnectionCallback() {
        @Override
        public void onServiceConnected() {
            Log.d(LOG_TAG, "Connected to SWAPI");

            state = 1;
            softwareUpdateManager.UpdateState(state);

            ISoftwareManagementApiCallback.Stub swapiCallback = new ISoftwareManagementApiCallback.Stub() {

                @Override
                public void CommissionStatus(int code) {
                }

                @Override
                public void SoftwareAssignmentList(int code, List<SoftwareAssignment> softwareAssigmentList) {
                    Log.v(LOG_TAG,
                            "SoftwareAssignmentList callback when SoftwareUpdateService started, what to do with this list?");
                }

                @Override
                public void PendingInstallations(int code, List<InstallationOrder> installationOrderList) {
                    Log.v(LOG_TAG,
                            "PendingInstallations callback when SoftwareUpdateService started, what to do with this list?");
                }

                @Override
                public void DownloadInfo(int code, DownloadInfo info) {
                    //TODO: implement
                }
            };

            if (swapi != null) {
                try {
                    swapi.GetSoftwareAssigmentList(swapiCallback);
                    swapi.GetPendingInstallations(swapiCallback);
                } catch (RemoteException e) {
                    Log.e(LOG_TAG, "Cannot fetch Software Assignment List.. No contact with SWAPI.. I'm sad...");
                }
            }

        }

        @Override
        public void onServiceDisconnected() {
            Log.d(LOG_TAG, "Connection to SWAPI was lost. How to handle?");
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
                Log.e(LOG_TAG, "Cannot fetch Software Assignment List.. No contact with SWAPI.. I'm sad...");
            }
        }
    }

    public void CommissionAssignment(String uuid) {
        if (swapi != null) {
            try {
                swapi.CommissionSoftwareAssignment(uuid, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Cannot commission software assignment.. No contact with SWAPI.. I'm sad...");
            }
        } else {
            Log.e(LOG_TAG, "SWAPI null");
        }
    }

    public void GetPendingInstallations() {
        if (swapi != null) {
            try {
                Log.v(LOG_TAG, "GetPendingInstallations");
                swapi.GetPendingInstallations(swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Cannot fetch Pending Installations... No contact with SWAPI.. I'm sad...");
            }
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
                swapi.GetDownloadInfo(uuid, swapiCallback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Cannot get download information.. No contact with SWAPI.. I'm sad...");
            }
        } else {
            Log.e(LOG_TAG, "SWAPI null");
        }
    }

    public void UpdateSoftwareList(List<SoftwareAssignment> softwareAssignments) {

        softwareInformationList.clear();

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
            // Weird..?
            Log.e(LOG_TAG, "UpdateSoftwareList(downloadInfo), uuid not found in list which is weird...");
        }

        softwareUpdateManager.UpdateSoftwareList(softwareInformationList);
    }
}