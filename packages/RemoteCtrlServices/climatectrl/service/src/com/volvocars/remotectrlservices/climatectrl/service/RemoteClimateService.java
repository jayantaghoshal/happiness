/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservices.climatectrl.service;

import android.content.Context;
import android.content.ComponentName;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.IBinder.DeathRecipient;
import android.os.RemoteException;
import android.util.Log;
import android.content.ServiceConnection;

import java.util.AbstractList;
import java.util.ArrayList;

import android.car.Car;
import android.car.CarNotConnectedException;
import android.car.hardware.hvac.CarHvacManager;
import android.car.hardware.CarPropertyValue;

import com.volvocars.remotectrlservices.climatectrl.service.task.GetCarPropertyAsyncTask;
import com.volvocars.remotectrlservices.climatectrl.service.task.SetCarPropertyAsyncTask;

import com.volvocars.remotectrlservices.climatectrl.*;

public class RemoteClimateService extends Service {
    private static final String TAG = "RemoteCtrl.ClimateCtrl.RemoteClimateService";

    private static final String REMOTE_CLIMATE_SERVICE_PACKAGE =
            "com.volvocars.remotectrlservices.climatectrl.gateway";
    private static final String REMOTE_CLIMATE_SERVICE_CLASS =
            "com.volvocars.remotectrlservices.climatectrl.gateway.RemoteClimateGateway";

    private static final String REMOTE_CLIMATE_SERVICE_RESPONSE_BIND_INTENT =
            ".RemoteClimateResponseService.BIND";

    private boolean mIsRemoteClimateResponseServiceBound = false;
    IRemoteClimateResponseService mIRemoteClimateResponseService = null;

    // TODO: Move these to support library for testing purposes Philip Werner (2018-04-23)
    private static AbstractList<Integer> mPropertiesListeningOn = new ArrayList<Integer>();
    static {
        mPropertiesListeningOn.add(CarHvacManager.ID_ZONED_FAN_SPEED_SETPOINT);
        mPropertiesListeningOn.add(CarHvacManager.ID_ZONED_FAN_DIRECTION);
        mPropertiesListeningOn.add(CarHvacManager.ID_ZONED_AC_ON);
        mPropertiesListeningOn.add(CarHvacManager.ID_ZONED_MAX_DEFROST_ON);
        mPropertiesListeningOn.add(CarHvacManager.ID_ZONED_TEMP_SETPOINT);
        mPropertiesListeningOn.add(CarHvacManager.ID_WINDOW_DEFROSTER_ON);
    }

    // TODO: Encapsulate this behaviour in IVehiclePropertyService for easier testing /Philip Werner
    // (2018-04-23)
    private Car mCar = null;
    private CarHvacManager mCarHvacManager = null;

    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(TAG, "onCreate");
        Log.v(TAG, "Creating car");
        mCar = Car.createCar(this, mCarServiceConnection);
        Log.v(TAG, "Connecting to car");
        mCar.connect();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v(TAG, "onStartCommand");
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(TAG, "onBind");
        return mRemoteClimateServiceBinder;
    }

    private void bindRemoteClimateResponseService() {
        if (mIsRemoteClimateResponseServiceBound) {
            Log.v(TAG, "Already bound to IRemoteClimateResponseService");
            return;
        }

        Intent bindServiceIntent = new Intent(REMOTE_CLIMATE_SERVICE_RESPONSE_BIND_INTENT);
        ComponentName componentName =
                new ComponentName(REMOTE_CLIMATE_SERVICE_PACKAGE, REMOTE_CLIMATE_SERVICE_CLASS);
        bindServiceIntent.setComponent(componentName);
        if (bindService(bindServiceIntent, mRemoteClimateResponseServiceConnection,
                    Context.BIND_AUTO_CREATE)) {
            mIsRemoteClimateResponseServiceBound = true;
        } else {
            Log.e(TAG, "Failed to bind IRemoteClimateResponseService");
        }

        Log.v(TAG, "IRemoteClimateResponseService bound");
    }

    private void unbindRemoteClimateResponseService() {
        getApplicationContext().unbindService(mRemoteClimateResponseServiceConnection);
        mIsRemoteClimateResponseServiceBound = false;
        Log.v(TAG, "IRemoteClimateResponseService unbound");
    }

    private void registerCallbacks() {
        try {
            mCarHvacManager.registerCallback(onPropertyChangedCallback);
        } catch (CarNotConnectedException ex) {
            Log.e(TAG, "CarNotConnectedException: ", ex);
        }
    }

    private void unregisterCallbacks() {
        mCarHvacManager.unregisterCallback(onPropertyChangedCallback);
    }

    private final CarHvacManager.CarHvacEventCallback onPropertyChangedCallback =
            new CarHvacManager.CarHvacEventCallback() {

                @Override
                public void onErrorEvent(int propertyId, int zone) {
                    Log.e(TAG, "onErrorEvent: propertyId = " + propertyId + ", zone = " + zone);
                }

                @Override
                public void onChangeEvent(CarPropertyValue value) {
                    try {
                        Log.v(TAG, "CarHvacEventCallback: " + value.toString());
                        if (mPropertiesListeningOn.contains(value.getPropertyId())) {
                            mIRemoteClimateResponseService.notifyPropertyChanged(value);
                        } else {
                            Log.v(TAG, "Unhandled event callback: " + value.toString());
                        }
                    } catch (RemoteException ex) {
                        Log.e(TAG, "RemoteException: ", ex);
                    }
                }
            };

    private final IRemoteClimateService.Stub mRemoteClimateServiceBinder =
            new IRemoteClimateService.Stub() {

                public void setProperty(int requestIdentifier, CarPropertyValue propValue) {
                    SetCarPropertyAsyncTask
                            .create(requestIdentifier, mIRemoteClimateResponseService,
                                    mCarHvacManager, propValue)
                            .execute();
                }

                public void getProperty(
                        int requestIdentifier, CarPropertyValue requestedPropValue) {
                    GetCarPropertyAsyncTask
                            .create(requestIdentifier, mIRemoteClimateResponseService,
                                    mCarHvacManager, requestedPropValue)
                            .execute();
                }
            };

    private ServiceConnection mRemoteClimateResponseServiceConnection = new ServiceConnection() {

        public void onServiceConnected(ComponentName className, IBinder service) {
            synchronized (this) {
                Log.v(TAG, "IRemoteClimateResponseService connected.");

                mIRemoteClimateResponseService =
                        IRemoteClimateResponseService.Stub.asInterface(service);
            }
        }

        public void onServiceDisconnected(ComponentName className) {
            synchronized (this) {
                Log.v(TAG, "IRemoteClimateResponseService disconnected.");

                mIRemoteClimateResponseService = null;
            }
        }
    };

    private final ServiceConnection mCarServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            synchronized (this) {
                try {
                    Log.v(TAG, "Car service connected");

                    mCarHvacManager = (CarHvacManager) mCar.getCarManager(Car.HVAC_SERVICE);

                    bindRemoteClimateResponseService();

                    registerCallbacks();
                } catch (CarNotConnectedException ex) {
                    Log.e(TAG, "Exception thrown: " + ex);
                }
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            synchronized (this) {
                Log.v(TAG, "Car service disconnected");

                unregisterCallbacks();

                mCarHvacManager = null;

                unbindRemoteClimateResponseService();
            }
        }
    };
}
