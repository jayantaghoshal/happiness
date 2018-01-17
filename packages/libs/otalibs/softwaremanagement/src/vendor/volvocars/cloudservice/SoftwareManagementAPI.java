/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;

import android.util.Log;
import android.app.Service;
import android.os.IBinder;
import android.content.*;
import android.os.RemoteException;
import java.util.List;
import java.util.ArrayList;

/**
 * Library/wrapper class that is used to access SoftwareManagement features
 */
public class SoftwareManagementAPI implements ServiceConnection {
    private static final String LOG_TAG = "SWAPI";
    private static final String PACKAGENAME = "vendor.volvocars.cloudservice";
    private static final String PACKAGENAME_SERVICENAME = "vendor.volvocars.cloudservice.CloudService";
    private Context context = null;
    private ISoftwareManagementAPI software_management = null;
    private boolean service_bound = false;

    private SoftwareManagementAPIConnectionCallback softwareManagementAPIConnectionCallback = null;

    @Override
    public void onServiceConnected(ComponentName className, IBinder service) {
        software_management = ISoftwareManagementAPI.Stub.asInterface(service);
        service_bound = true;
        softwareManagementAPIConnectionCallback.onServiceConnected();
    }

    @Override
    public void onServiceDisconnected(ComponentName arg0) {
        service_bound = false;
        softwareManagementAPIConnectionCallback.onServiceDisconnected();
    }

    /**
     *
     */
    public SoftwareManagementAPI(Context context,
            SoftwareManagementAPIConnectionCallback softwareManagementAPIConnectionCallback) {
        this.context = context;
        this.softwareManagementAPIConnectionCallback = softwareManagementAPIConnectionCallback;
        connect();
    }

    /**
     * Used to bind to Cloud Service, can be used by client if service unbinds unexpectedly
     */
    public void connect() {
        Intent intent = new Intent();
        intent.setAction("SoftwareManagement");
        intent.setComponent(new ComponentName(PACKAGENAME, PACKAGENAME_SERVICENAME));
        context.bindService(intent, this, Context.BIND_AUTO_CREATE);
    }

    /**
     * Get a the list of available assignments
     * @param callback
     */
    public void GetSoftwareAssigmentList(ISoftwareManagementAPICallback callback) throws RemoteException {
        if(software_management!=null && service_bound) {
            software_management.GetSoftwareAssigmentList(callback);
        }
    }

    /**
     * Issue a commission of an SoftwareAssignment.
     * @param id The id of the assignment to fetch.
     */
     public void CommissionSoftwareAssignment(String uuid, ISoftwareManagementAPICallback callback) throws RemoteException {
        if(software_management!=null && service_bound) {
            software_management.CommissionSoftwareAssignment(uuid, callback);
        }

     }
}