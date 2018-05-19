/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.gateway;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.AsyncTask;
import android.os.HwBinder;
import android.os.IBinder;
import android.util.Log;

import com.volvocars.remotectrl.interfaces.IRemoteCtrlPropertyService;
import com.volvocars.remotectrlservices.gateway.task.GetServiceAsyncTask;

import java.text.MessageFormat;

import vendor.volvocars.hardware.remotectrl.V1_0.IRemoteCtrlPropertyResponse;

public abstract class RemoteGateway extends Service implements ServiceNotificationInterface {
    private static final String BASE_LOG_TAG = "RemoteCtrl.{0}";

    private PropertyResponseServiceHandler mPropertyResponseServiceHandler = null; // Service -> Gateway Communication
    private PropertyServiceHandler mPropertyServiceHandler = null; // HAL -> Service Communication

    private IRemoteCtrlPropertyResponse mRemoteCtrlPropertyResponseService = null;
    private IRemoteCtrlPropertyService mRemoteCtrlPropertyService = null;

    private DeathRecipient mNativeServiceDeath = null;

    private boolean mIsRemoteCtrlPropServiceBound = false;

    private GetServiceAsyncTask mGetServiceAsyncTask = null;

    private ServiceConnection mRemoteCtrlPropServiceConnection = new ServiceConnection() {
        // Called when the connection with the service is established
        public void onServiceConnected(ComponentName className, IBinder service) {
            synchronized (this) {
                Log.v(getLogTag(), "onServiceConnected");

                mRemoteCtrlPropertyService = IRemoteCtrlPropertyService.Stub.asInterface(service);
                mPropertyServiceHandler.setRemoteCtrlPropService(mRemoteCtrlPropertyService);

                mNativeServiceDeath = new DeathRecipient(mRemoteCtrlPropServiceConnection);
                mPropertyServiceHandler.registerRemoteCtrlPropertyHandler(
                        mNativeServiceDeath, getDeathRecipientCookie());
            }
        }

        // Called when the connection with the service disconnects unexpectedly
        public void onServiceDisconnected(ComponentName className) {
            synchronized (this) {
                Log.v(getLogTag(), "onServiceDisconnected");

                mRemoteCtrlPropertyService = null;
                mPropertyServiceHandler.setRemoteCtrlPropService(mRemoteCtrlPropertyService);

                mPropertyServiceHandler.unregisterRemoteCtrlPropertyHandler();
            }
        }
    };

    public abstract String getRemoteCtrlPropServicePackage();

    public abstract String getRemoteCtrlPropServiceClass();

    public abstract String getRemoteCtrlPropServiceBindIntent();

    public abstract String getRemoteCtrlPropResponseServiceOnBindIntent();

    public abstract int getDeathRecipientCookie();

    public abstract String getHalServiceName();

    @Override
    public void onCreate() {
        Log.v(getLogTag(), "onCreate");

        super.onCreate();

        mPropertyServiceHandler = new PropertyServiceHandler(getLogTag());
        mPropertyResponseServiceHandler = new PropertyResponseServiceHandler(getLogTag());
    }

    @Override
    public void onDestroy() {
        Log.v(getLogTag(), "onDestroy");

        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(getLogTag(), "onStartCommand");

        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(getLogTag(), "onBind");

        if (intent.getAction().equals(getRemoteCtrlPropResponseServiceOnBindIntent())) {
            getServiceAsync();

            return mPropertyResponseServiceHandler;
        }

        return null;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.v(getLogTag(), "onUnbind");

        if (intent.getAction().equals(getRemoteCtrlPropResponseServiceOnBindIntent())) {
            if (mPropertyServiceHandler != null) {
                mPropertyServiceHandler.unregisterRemoteCtrlPropertyHandler();
            }

            unbindRemoteCtrlPropService();

            return true;
        }

        return false;
    }

    public void notifyServiceConnected() {
        Log.v(getLogTag(), "notifyServiceConnected");

        bindRemoteCtrlPropService();
    }

    public void notifyServiceDisconnected() {
        Log.v(getLogTag(), "notifyServiceDisconnected");
        // TODO: Add service notifications here
    }

    public void setRemoteCtrlPropResponseService(IRemoteCtrlPropertyResponse remoteCtrlPropertyResponseService) {
        mRemoteCtrlPropertyResponseService = remoteCtrlPropertyResponseService;
        mPropertyServiceHandler.setRemoteCtrlPropertyResponseService(mRemoteCtrlPropertyResponseService);
        mPropertyResponseServiceHandler.setRemoteCtrlPropertyResponseService(mRemoteCtrlPropertyResponseService);
    }

    protected boolean bindRemoteCtrlPropService() {
        Log.v(getLogTag(), "bindRemoteCtrlPropService");

        if (mIsRemoteCtrlPropServiceBound)
            return true;

        Intent bindServiceIntent = new Intent(getRemoteCtrlPropServiceBindIntent());
        ComponentName componentName = new ComponentName(getRemoteCtrlPropServicePackage(), getRemoteCtrlPropServiceClass());
        bindServiceIntent.setComponent(componentName);

        if (!(mIsRemoteCtrlPropServiceBound = bindService(bindServiceIntent,
                mRemoteCtrlPropServiceConnection, Context.BIND_AUTO_CREATE))) {
            Log.e(getLogTag(), "Failed to bind IRemoteCtrlPropService");

            return false;
        }

        return true;
    }

    protected void unbindRemoteCtrlPropService() {
        Log.v(getLogTag(), "unbindRemoteCtrlPropService");

        try {
            if (mIsRemoteCtrlPropServiceBound) {
                unbindService(mRemoteCtrlPropServiceConnection);
                mIsRemoteCtrlPropServiceBound = false;
            }
        } catch (Exception ex) {
            Log.v(getLogTag(), "Unbind IRemoteCtrlPropService failed: " + ex.toString());
        }
    }

    protected void getServiceAsync() {
        synchronized (this) {
            if (mGetServiceAsyncTask == null || mGetServiceAsyncTask.getStatus() == AsyncTask.Status.FINISHED) {
                mGetServiceAsyncTask = new GetServiceAsyncTask(getLogTag(), this, 2000);
                mGetServiceAsyncTask.execute();
            }
        }
    }

    protected String getLogTag() {
        return MessageFormat.format(BASE_LOG_TAG + ".", this.getClass().getSimpleName());
    }

    final class DeathRecipient implements HwBinder.DeathRecipient {
        ServiceConnection mRemoteClimateServiceConnection;

        DeathRecipient(ServiceConnection remoteClimateServiceConnection) {
            this.mRemoteClimateServiceConnection = remoteClimateServiceConnection;
        }

        @Override
        public void serviceDied(long cookie) {
            if (cookie != getDeathRecipientCookie()) {
                return;
            }

            Log.v(getLogTag(), "Native HAL service died");

            unbindRemoteCtrlPropService();

            getServiceAsync();
        }
    }
}
