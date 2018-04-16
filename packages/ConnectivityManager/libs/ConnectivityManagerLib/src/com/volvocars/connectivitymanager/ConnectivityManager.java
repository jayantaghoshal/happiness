/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager;

import android.util.Log;
import android.app.Service;
import android.os.IBinder;
import android.content.Context;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.content.Intent;
import android.os.RemoteException;

import com.volvocars.connectivitymanager.WifiStationModeAidl;

/**
 *
 */
public class ConnectivityManager extends IConnectivityManager.Stub {
    private static final String LOG_TAG = "ConnectivityManager";
    private static final String PACKAGENAME = "com.volvocars.connectivitymanager.gateway";
    private static final String PACKAGENAME_SERVICENAME =
            "com.volvocars.connectivitymanager.gateway.GateWayService";

    private Context context = null;
    private IConnectivityManagerGateway gateway = null;
    private boolean gatewayBound = false;

    private IConnectivityManagerCallback connectivityManagerCallback = null;

    private IBinder.DeathRecipient death = new IBinder.DeathRecipient() {

        @Override
        public void binderDied() {
            Log.w(LOG_TAG, "Gateway died, reconnecting...");
            connectToGateway();
        }
    };

    private ServiceConnection connection = new ServiceConnection() {

        public void onServiceConnected(ComponentName className, IBinder service) {
            gateway = IConnectivityManagerGateway.Stub.asInterface(service);
            gatewayBound = true;
            registerManagerAtGateway();
            connectivityManagerCallback.onServiceConnected();

            try {
                service.linkToDeath(death, 0 /* flags */);
            } catch (RemoteException e) {
                Log.w(LOG_TAG, "Unable to register Death Recipient. :(");
            }
        }

        public void onServiceDisconnected(ComponentName arg0) {
            gateway = null;
            gatewayBound = false;
            connectivityManagerCallback.onServiceDisconnected();
        }
    };

    /**
     *
     */
    public ConnectivityManager(
            Context context, IConnectivityManagerCallback connectivityManagerCallback) {
        this.context = context;
        this.connectivityManagerCallback = connectivityManagerCallback;

        connectToGateway();
    }

    /**
     * Used to bind to Gateway Service, can be used by client if service unbinds unexpectedly
     */
    private void connectToGateway() {
        Intent intent = new Intent();
        intent.setAction("ConnectivityManagerGateway");
        intent.setComponent(new ComponentName(PACKAGENAME, PACKAGENAME_SERVICENAME));
        context.bindService(intent, connection, Context.BIND_AUTO_CREATE);
    }

    private void registerManagerAtGateway() {
        try {
            gateway.registerManager(this);
        } catch (RemoteException e) {
            Log.d(LOG_TAG, "Cannot register Manager. Received exception: " + e.getMessage());
        }
    }

    @Override
    public void updateWifiStationMode(WifiStationModeAidl mode) {
        connectivityManagerCallback.notifyWifiStationMode(mode);
    }

    ///// Wifi Control /////

    /**
     * Request a broadcast of the current Wifi Station Mode
     * @return True if request was successful, False otherwise
     */
    public boolean getWifiStationMode() {
        if (gatewayBound) {
            try {
                return gateway.getWifiStationMode();
            } catch (RemoteException e) {
                Log.d(LOG_TAG,
                        "Cannot request WifiStatioMode. Received exception: " + e.getMessage());
            }
        }
        return false;
    }

    /**
     * Request to set the Wifi Station mode. Changes will be broadcasted.
     * @param mode The mode that is requested
     * @return True if request was successful, False otherwise
     */
    public boolean setWifiStationMode(WifiStationModeAidl mode) {
        if (gatewayBound) {
            try {
                return gateway.setWifiStationMode(mode);

            } catch (RemoteException e) {
                Log.d(LOG_TAG, "Cannot request to set WifiStationMode. Received exception: "
                                + e.getMessage());
            }
        }
        return false;
    }

    ///// End of Wifi Control /////
}