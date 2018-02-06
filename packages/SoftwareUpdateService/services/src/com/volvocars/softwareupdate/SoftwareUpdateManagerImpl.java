/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.util.Log;

import android.os.RemoteException;

import com.volvocars.cloudservice.SoftwareAssignment;
import com.volvocars.softwareupdate.ISoftwareUpdateManager;
import com.volvocars.softwareupdate.ISoftwareUpdateManagerCallback;

import java.util.ArrayList;
import java.util.List;

/**
 * Implementation of Foundation service API.
 */
public class SoftwareUpdateManagerImpl extends ISoftwareUpdateManager.Stub {
    private static final String LOG_TAG = "SwUpdManager";

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
        Log.v(LOG_TAG, "UpdateState");
        for(ISoftwareUpdateManagerCallback c : clients) {
            try {
                c.UpdateState(state);
            } catch(RemoteException e) {
                clients.remove(c);
            }
        }
    }

    /**
     * Update the Software Assignment List for all registered clients. remove all clients that doesn't respond.
     * @param software_list The list with software that all clients should be updated with.
     */
    public void UpdateSoftwareAssignmentList(List<SoftwareAssignment> software_list) {
        for(ISoftwareUpdateManagerCallback c : clients) {
            try {
                c.UpdateSoftwareAssignmentList(software_list);
            } catch(RemoteException e) {
                clients.remove(c);
            }
        }
    }

    /**
     * Update the state of a Software Assignment
     * @param uuid The UUID of the concerned Software Assignment
     * @param sate The new state of the Software Assignment
     */
    public void UpdateSoftwareAssignmentState(String uuid, int state) {
        for(ISoftwareUpdateManagerCallback c : clients) {
            try {
                c.UpdateSoftwareState(uuid, state);
            } catch(RemoteException e) {
                clients.remove(c);
            }
        }
    }

    @Override
    public void GetState(ISoftwareUpdateManagerCallback callback) {
        Log.v(LOG_TAG, "GetState");
        clients.add(callback);
        try {
            callback.UpdateState(service.GetState());
        } catch(RemoteException e) {
            clients.remove(callback);
        }
    }

    @Override
    public void GetSoftwareAssignments(ISoftwareUpdateManagerCallback callback) {
        service.GetSoftwareAssignmentList(callback);
    }

    @Override
    public void CommissionAssignment(String uuid) {
        service.CommissionAssignment(uuid);
    }


}