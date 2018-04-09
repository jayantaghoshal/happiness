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

import com.volvocars.connectivitymanager.WifiStationMode;

/**
 *
 */
public class ConnectivityManager implements ServiceConnection {
    private static final String LOG_TAG = "ConnectivityManager";
    private static final String PACKAGENAME = "com.volvocars.connectivitymanager.gateway";
    private static final String PACKAGENAME_SERVICENAME =
            "com.volvocars.connectivitymanager.gateway.GateWayService";

    private Context context = null;
    private IConnectivityManagerGateway gateway = null;
    private boolean gatewayBound = false;

    private ConnectivityManagerGatewayConnectionCallback
            connectivityManagerGatewayConnectionCallback = null;

    private DeathRecipient death = new DeathRecipient(this);

    final class DeathRecipient implements IBinder.DeathRecipient {
        private ConnectivityManager client;

        DeathRecipient(ConnectivityManager client) {
            this.client = client;
        }

        @Override
        public void binderDied() {
            Log.e(LOG_TAG, "Gateway died, reconnecting...");
            client.connect();
        }
    }

    @Override
    public void onServiceConnected(ComponentName className, IBinder service) {
        gateway = IConnectivityManagerGateway.Stub.asInterface(service);
        gatewayBound = true;
        connectivityManagerGatewayConnectionCallback.onServiceConnected();

        try {
            service.linkToDeath(death, 0 /* flags */);
        } catch (RemoteException e) {
            Log.w(LOG_TAG, "Unable to register Death Recipient. :(");
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName arg0) {
        gatewayBound = false;
        connectivityManagerGatewayConnectionCallback.onServiceDisconnected();
    }

    /**
     *
     */
    public ConnectivityManager(Context context,
            ConnectivityManagerGatewayConnectionCallback
                    connectivityManagerGatewayConnectionCallback) {
        this.context = context;
        this.connectivityManagerGatewayConnectionCallback =
                connectivityManagerGatewayConnectionCallback;

        connect();
    }

    /**
     * Used to bind to Gateway Service, can be used by client if service unbinds unexpectedly
     */
    public void connect() {
        Intent intent = new Intent();
        intent.setAction("ConnectivityManagerGateway");
        intent.setComponent(new ComponentName(PACKAGENAME, PACKAGENAME_SERVICENAME));
        context.bindService(intent, this, Context.BIND_AUTO_CREATE);
    }

    /**
     * Register a manager interface to receive broadcasted updates.
     */
    public void registerManager(IConnectivityManager manager) {

    }

    ///// Wifi Control /////

    /**
     * Request a broadcast of the current Wifi Station Mode
     * @return True if request was successful, False otherwise
     */
    public boolean getWifiStationMode() {
        return false;
    }

    /**
     * Request to set the Wifi Station mode. Changes will be broadcasted.
     * @param mode The mode that is requested
     * @return True if request was successful, False otherwise
     */
    public boolean setWifiStationMode(WifiStationMode mode) {
        return false;
    }

    ///// End of Wifi Control /////

}