/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import com.volvocars.remotectrlservices.climatectrl.*;

import android.content.Context;
import android.content.ComponentName;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.HwBinder;
import android.os.RemoteException;
import android.util.Log;
import android.content.ServiceConnection;

public class RemoteClimateGateway extends Service {
    public static final String TAG = "RemoteCtrl.ClimateCtrl.Gateway";

    private static final String REMOTE_CLIMATE_SERVICE_PACKAGE =
            "com.volvocars.remotectrlservices.climatectrl.service";
    private static final String REMOTE_CLIMATE_SERVICE_CLASS =
            "com.volvocars.remotectrlservices.climatectrl.service.RemoteClimateService";
    private static final String REMOTE_CLIMATE_SERVICE_BIND_INTENT = ".RemoteClimateService.BIND";

    private static final String REMOTE_CLIMATE_RESPONSE_SERVICE_BIND_INTENT =
            ".RemoteClimateResponseService.BIND";

    private static final int COOKIE = 2090;

    private RemoteClimateResponseServiceHandler mRemoteClimateResponseServiceHandler =
            null; // Service -> Gateway Communication
    private RemoteClimateServiceHandler mRemoteClimateServiceHandler =
            null; // HAL -> Service Communication
    private DeathRecipient mNativeServiceDeath = null;

    private IRemoteCtrlPropertyResponse mRemoteCtrlPropertyResponse = null;
    private IRemoteClimateService mRemoteClimateService = null;

    private boolean mIsRemoteClimateServiceBound = false;

    /**
     *  Abstract methods - Service class
     */
    @Override
    public void onCreate() {
        super.onCreate();

        try {
            Log.v(TAG, "onCreate");

            mRemoteCtrlPropertyResponse = IRemoteCtrlPropertyResponse.getService();

            mRemoteClimateServiceHandler =
                    new RemoteClimateServiceHandler(mRemoteCtrlPropertyResponse);
            Log.v(TAG, "RemoteClimateServiceHandler created");

            mRemoteClimateResponseServiceHandler =
                    new RemoteClimateResponseServiceHandler(mRemoteCtrlPropertyResponse);
            Log.v(TAG, "RemoteClimateResponseServiceHandler created");
        } catch (RemoteException ex) {
            Log.v(TAG, "Remote exception thrown: ", ex);
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "onStartCommand");
        return START_STICKY;
    }

    public boolean bindRemoteClimateService() {
        if (mIsRemoteClimateServiceBound == false) {
            Intent bindServiceIntent = new Intent(REMOTE_CLIMATE_SERVICE_BIND_INTENT);
            ComponentName componentName =
                    new ComponentName(REMOTE_CLIMATE_SERVICE_PACKAGE, REMOTE_CLIMATE_SERVICE_CLASS);
            bindServiceIntent.setComponent(componentName);

            if (!bindService(bindServiceIntent, mRemoteClimateServiceConnection,
                        Context.BIND_AUTO_CREATE)) {
                Log.e(TAG, "Failed to bind RemoteClimateService");

                return false;
            }

            mIsRemoteClimateServiceBound = true;

            Log.v(TAG, "Remote Climate Service bound");
        }

        return true;
    }

    private ServiceConnection mRemoteClimateServiceConnection = new ServiceConnection() {
        // Called when the connection with the service is established
        public void onServiceConnected(ComponentName className, IBinder service) {
            synchronized (this) {
                Log.e(TAG, "Connection established with IRemoteClimateService");

                mRemoteClimateService = IRemoteClimateService.Stub.asInterface(service);
                mRemoteClimateServiceHandler.setRemoteClimateService(mRemoteClimateService);

                mNativeServiceDeath = new DeathRecipient();
                mRemoteClimateServiceHandler.registerClimateControlHandler(mNativeServiceDeath);
            }
        }

        // Called when the connection with the service disconnects unexpectedly
        public void onServiceDisconnected(ComponentName className) {
            synchronized (this) {
                Log.e(TAG, "IRemoteClimateService has unexpectedly disconnected");

                mRemoteClimateService = null;
                mRemoteClimateServiceHandler.setRemoteClimateService(mRemoteClimateService);

                mRemoteClimateServiceHandler.unregisterClimateControlHandler();

                if (mRemoteClimateServiceConnection != null) {
                    getApplicationContext().unbindService(mRemoteClimateServiceConnection);
                    mRemoteClimateServiceConnection = null;
                }
            }
        }
    };

    @Override
    public void onDestroy() {
        Log.v(TAG, "onDestroy");

        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent intent) {
        synchronized (this) {
            Log.v(TAG, "onBind");

            if (intent.getAction().equals(REMOTE_CLIMATE_RESPONSE_SERVICE_BIND_INTENT)) {
                if (bindRemoteClimateService() == false) {
                    Log.e(TAG, "Error binding to IRemoteClimateService");
                    return null;
                }

                Log.v(TAG, "Returning IRemoteClimateResponseService interface");
                return mRemoteClimateResponseServiceHandler;
            }
            return null;
        }
    }

    @Override
    public boolean onUnbind(Intent intent) {
        synchronized (this) {
            Log.v(TAG, "onUnbind");

            if (intent.getAction().equals(REMOTE_CLIMATE_RESPONSE_SERVICE_BIND_INTENT)) {
                if (mRemoteClimateServiceHandler != null) {
                    mRemoteClimateServiceHandler.unregisterClimateControlHandler();
                    Log.v(TAG, "Unregistered ClimateControlHandler");
                }

                if (mRemoteClimateServiceConnection != null) {
                    getApplicationContext().unbindService(mRemoteClimateServiceConnection);
                    mRemoteClimateServiceConnection = null;
                    Log.v(TAG, "Unbound IRemoteClimateResponseService interface");
                    mIsRemoteClimateServiceBound = false;
                }

                return true;
            }

            return false;
        }
    }

    final class DeathRecipient implements HwBinder.DeathRecipient {
        // ServiceConnection mRemoteClimateServiceConnection;

        DeathRecipient(/* ServiceConnection remoteClimateServiceConnection */) {
            // this.mRemoteClimateServiceConnection = remoteClimateServiceConnection;
        }

        @Override
        public void serviceDied(long cookie) {
            if (cookie != COOKIE) {
                return;
            }

            Log.e(TAG, "NativeRemoteClimateCtrl HAL service died");
            Log.v(TAG, "TODO: Add argument and use in serviceDied (Philip Werner 2018-04-20)");
        }
    }
}

