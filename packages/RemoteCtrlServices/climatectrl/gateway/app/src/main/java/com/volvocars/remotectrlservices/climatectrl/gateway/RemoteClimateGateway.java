/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.gateway;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlProperty;
import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

import android.content.Context;
import android.content.ComponentName;
import android.app.Service;
import android.content.Intent;
import android.os.AsyncTask.Status;
import android.os.IBinder;
import android.os.HwBinder;
import android.os.RemoteException;
import android.util.Log;
import android.content.ServiceConnection;

import com.volvocars.remotectrlservices.climatectrl.IRemoteClimateService;
import com.volvocars.remotectrlservices.climatectrl.gateway.task.GetServiceAsyncTask;

public class RemoteClimateGateway extends Service implements ServiceNotificationInterface {
    public static final String TAG = "RemoteCtrl.ClimateCtrl.Gateway";

    private static final String REMOTE_CLIMATE_SERVICE_PACKAGE =
            "com.volvocars.remotectrlservices.climatectrl.service";
    private static final String REMOTE_CLIMATE_SERVICE_CLASS =
            "com.volvocars.remotectrlservices.climatectrl.service.RemoteClimateService";
    private static final String REMOTE_CLIMATE_SERVICE_BIND_INTENT = ".RemoteClimateService.BIND";

    private static final String REMOTE_CLIMATE_RESPONSE_SERVICE_BIND_INTENT =
            ".RemoteClimateResponseService.BIND";

    private static final int DEATH_RECIPIENT_COOKIE = 2090;

    private RemoteClimateResponseServiceHandler mRemoteClimateResponseServiceHandler =
            null; // Service -> Gateway Communication
    private RemoteClimateServiceHandler mRemoteClimateServiceHandler =
            null; // HAL -> Service Communication
    private DeathRecipient mNativeServiceDeath = null;

    private IRemoteCtrlPropertyResponse mRemoteCtrlPropertyResponse = null;
    private IRemoteClimateService mRemoteClimateService = null;

    private boolean mIsRemoteClimateServiceBound = false;

    private GetServiceAsyncTask mGetServiceAsyncTask = null;

    @Override
    public void onCreate() {
        super.onCreate();

        Log.v(TAG, "onCreate");

        mRemoteClimateServiceHandler = new RemoteClimateServiceHandler();
        Log.v(TAG, "RemoteClimateServiceHandler created");

        mRemoteClimateResponseServiceHandler = new RemoteClimateResponseServiceHandler();
        Log.v(TAG, "RemoteClimateResponseServiceHandler created");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "onStartCommand");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.v(TAG, "onDestroy");
        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(TAG, "onBind");

        if (intent.getAction().equals(REMOTE_CLIMATE_RESPONSE_SERVICE_BIND_INTENT)) {
            getServiceAsync();
            Log.v(TAG, "Returning IRemoteClimateResponseService interface");
            return mRemoteClimateResponseServiceHandler;
        }
        return null;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.v(TAG, "onUnbind");

        if (intent.getAction().equals(REMOTE_CLIMATE_RESPONSE_SERVICE_BIND_INTENT)) {
            if (mRemoteClimateServiceHandler != null) {
                mRemoteClimateServiceHandler.unregisterClimateControlHandler();
                Log.v(TAG, "Unregistered ClimateControlHandler");
            }

            unbindRemoteClimateService();

            return true;
        }

        return false;
    }

    public void notifyServiceConnected() {
        Log.v(TAG, "notifyServiceConnected");
        bindRemoteClimateService();
    }

    public void notifyServiceDisconnected() {
        Log.v(TAG, "notifyServiceDisconnected");
        // TODO: Add service notifications here
    }

    protected void getServiceAsync() {
        synchronized (this) {
            if (mGetServiceAsyncTask == null
                    || mGetServiceAsyncTask.getStatus() == Status.FINISHED) {
                mGetServiceAsyncTask = new GetServiceAsyncTask(this, 2000);
                mGetServiceAsyncTask.execute();
            }
        }
    }

    public boolean bindRemoteClimateService() {
        Log.v(TAG, "bindRemoteClimateService");
        if (!mIsRemoteClimateServiceBound) {
            Intent bindServiceIntent = new Intent(REMOTE_CLIMATE_SERVICE_BIND_INTENT);
            ComponentName componentName =
                    new ComponentName(REMOTE_CLIMATE_SERVICE_PACKAGE, REMOTE_CLIMATE_SERVICE_CLASS);
            bindServiceIntent.setComponent(componentName);

            if (!(mIsRemoteClimateServiceBound = bindService(bindServiceIntent,
                          mRemoteClimateServiceConnection, Context.BIND_AUTO_CREATE))) {
                Log.e(TAG, "Failed to bind RemoteClimateService");

                return false;
            }

            Log.v(TAG, "Remote Climate Service bound ");
        }

        return true;
    }

    protected void unbindRemoteClimateService() {
        try {
            if (mIsRemoteClimateServiceBound) {
                Log.v(TAG, "Unbound IRemoteClimateService interface");
                unbindService(mRemoteClimateServiceConnection);
                mIsRemoteClimateServiceBound = false;
            }
        } catch (Exception ex) {
            Log.v(TAG, "Unbind remote climate service failed: " + ex);
        }
    }

    public void setRemoteCtrlPropertyResponseService(
            IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService) {
        mRemoteCtrlPropertyResponse = remoteCtrlPropertyResponseService;
        mRemoteClimateServiceHandler.setRemoteCtrlPropertyResponseService(
                mRemoteCtrlPropertyResponse);
        mRemoteClimateResponseServiceHandler.setRemoteCtrlPropertyResponseService(
                mRemoteCtrlPropertyResponse);
    }

    private ServiceConnection mRemoteClimateServiceConnection = new ServiceConnection() {
        // Called when the connection with the service is established
        public void onServiceConnected(ComponentName className, IBinder service) {
            synchronized (this) {
                Log.e(TAG, "Connection established with IRemoteClimateService");

                mRemoteClimateService = IRemoteClimateService.Stub.asInterface(service);
                mRemoteClimateServiceHandler.setRemoteClimateService(mRemoteClimateService);

                mNativeServiceDeath = new DeathRecipient(mRemoteClimateServiceConnection);
                mRemoteClimateServiceHandler.registerClimateControlHandler(
                        mNativeServiceDeath, DEATH_RECIPIENT_COOKIE);
            }
        }

        // Called when the connection with the service disconnects unexpectedly
        public void onServiceDisconnected(ComponentName className) {
            synchronized (this) {
                Log.e(TAG, "IRemoteClimateService has unexpectedly disconnected");

                mRemoteClimateService = null;
                mRemoteClimateServiceHandler.setRemoteClimateService(mRemoteClimateService);

                mRemoteClimateServiceHandler.unregisterClimateControlHandler();
            }
        }
    };

    final class DeathRecipient implements HwBinder.DeathRecipient {
        ServiceConnection mRemoteClimateServiceConnection;

        DeathRecipient(ServiceConnection remoteClimateServiceConnection) {
            this.mRemoteClimateServiceConnection = remoteClimateServiceConnection;
        }

        @Override
        public void serviceDied(long cookie) {
            if (cookie != DEATH_RECIPIENT_COOKIE) {
                return;
            }

            Log.v(TAG, "Native Remote Control Climate HAL service died");

            unbindRemoteClimateService();

            getServiceAsync();
        }
    }
}
