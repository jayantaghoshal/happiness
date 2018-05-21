/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.timeupdateservice;

import android.app.Application;

import android.content.Intent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ContentResolver;
import android.content.pm.PackageManager;

import android.database.ContentObserver;
import android.os.IHwBinder.DeathRecipient;

import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemClock;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;

import android.util.Log;

import android.support.v4.content.ContextCompat;

import android.provider.Settings;

import java.lang.ref.WeakReference;

import java.util.List;
import java.util.ArrayList;

import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.IVehicleCallback;
import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.hardware.automotive.vehicle.V2_0.VehiclePropertyType;
import android.hardware.automotive.vehicle.V2_0.StatusCode;
import android.hardware.automotive.vehicle.V2_0.SubscribeOptions;
import android.hardware.automotive.vehicle.V2_0.SubscribeFlags;

import vendor.volvocars.hardware.gps.V1_0.ILocationCallback;
import vendor.volvocars.hardware.gps.V1_0.GnssTimeLocinfo;
import vendor.volvocars.hardware.gps.V1_0.ILocationUpdate;


import java.util.NoSuchElementException;

public class MainApplication extends Application {
    private BroadcastReceiver receiver;
    private SettingsObserver mSettingsObserver;
    private Handler mHandler;
    private static final String AUTO_TIME_LOG = "Time & Date MODE : [AUTO]";
    private static final String MANUAL_TIME_LOG = "Time & Date MODE : [MANUAL]";

    private static final int EVENT_AUTO_TIME_CHANGED = 1;
    /*Make this common to all classes*/
    private static final Boolean MANUAL_MODE = false;
    private static final Boolean AUTO_MODE = true;


    private static final int VHAL_COOKIE = 1010;  //Dummy cookie for VHAL
    private static final int GPSHAL_COOKIE = 1020;  //Dummy cookie for GPS HAL

    private static long gpsTimeMilis;
    private static long gpsTimeOffset;
    private static long cemTime;

    private IVehicle mVehicle = null;
    private ILocationUpdate mGnsshal = null;
    private VehicleCallback mInternalCallback;
    private GnssCallback mGnssCallback;

    private static int locationCounter = 0;
    private static int unixTimeCounter = 0;


    private final static long maxDiffinCem = 2000L;
    private final static long maxDiffinIhu = 5000L;

    private final TimeUpdateDeathRecipient mTimeUpdateDeathRecipient = new TimeUpdateDeathRecipient();


    private void startLocationUpdates() {
        int retry = 0;
        boolean IsLocServiceOk = false;
        try
        {
            while(retry <= 10) {
                mGnsshal = ILocationUpdate.getService();
                if (mGnsshal == null) {
                    Log.i(TimeUpdateLog.SERVICE_TAG, "GPS-HAL ILocationUpdate::getService: failed !!");
                    retry++;
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        Log.e(TimeUpdateLog.SERVICE_TAG, "Sleep was interrupted: " + e.getMessage());
                        ExitServiceApp(); // terminate ourself to get re-spawne
                    }
                    continue;
                } else {
                    IsLocServiceOk = true;
                    break;
                }
            }
            if (!IsLocServiceOk) {
               ExitServiceApp(); // terminate ourself to get re-spawne
            }
            mGnsshal.linkToDeath(mTimeUpdateDeathRecipient, GPSHAL_COOKIE /* dummy cookie */);
            mGnssCallback = new GnssCallback();
            // Register GNSS Location update
            mGnsshal.requestGNSSLocationUpdates(mGnssCallback);
        } catch (RemoteException ex) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "GNSS Location HAL failure: " + ex.getMessage());
            ExitServiceApp(); // terminate ourself to get re-spawne
        }

    }

    /**
     * On Application startup read stored gps offset and start settigs observer and the broadcast receiver
     */

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TimeUpdateLog.SERVICE_TAG, String.format("Time Update Service listening to clock change events"));


        if(isAutomaticTimeRequested()){
            setUserTimeOffset(0L);
        }
        gpsTimeOffset = getUserTimeOffset();
        Log.i(TimeUpdateLog.SERVICE_TAG, "On Boot offset is: " + gpsTimeOffset);

        mHandler = new Handler();

        startLocationUpdates();

        // Vehicle HAL properties
        try {
            mVehicle = IVehicle.getService();
            if (mVehicle==null) {
                Log.e(TimeUpdateLog.SERVICE_TAG, "IVehicle.getService returned null");
                ExitServiceApp(); // terminate ourself to get re-spawned
            }
            mVehicle.linkToDeath(mTimeUpdateDeathRecipient, VHAL_COOKIE /* dummy cookie */);
            mInternalCallback = new VehicleCallback();
            ArrayList<SubscribeOptions> options = new ArrayList<>();
            SubscribeOptions opts = new SubscribeOptions();
            opts.propId = VehicleProperty.UNIX_TIME;
            opts.flags = SubscribeFlags.DEFAULT;
            options.add(opts);
            int subscribeResult = mVehicle.subscribe(mInternalCallback, options);
            if (subscribeResult != 0) {
                Log.e(TimeUpdateLog.SERVICE_TAG, "Subscribe failed with result:" + subscribeResult);
            }
        } catch(RemoteException ex) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "VHAL failure: " + ex.getMessage());
            ExitServiceApp(); // terminate ourself to get re-spawned
        } catch(NoSuchElementException ex) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "IVehicle not found: " + ex.getMessage());
            ExitServiceApp(); // terminate ourself to get re-spawned
        }

        mSettingsObserver = new SettingsObserver(mHandler, EVENT_AUTO_TIME_CHANGED, this);
        mSettingsObserver.observe(this);

        IntentFilter filter = new IntentFilter();
        filter.addAction(Intent.ACTION_TIME_CHANGED);
        filter.addAction(Intent.ACTION_TIMEZONE_CHANGED);

        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                Log.d(TimeUpdateLog.SERVICE_TAG, String.format("Intent Received: Time or Date Changed") );
                updateTimeOnSettingsChange(isAutomaticTimeRequested());
            }
        };

        this.registerReceiver(receiver, filter);
    }

    /**
     * Is called when user sets time manually or changes the clock mode
     * @param A boolean value false:Manual_Mode true:Auto_mode
     * @return None
     */
    public void updateTimeOnSettingsChange(boolean mode) {
        /* Logs Time and Date Mode */
        Log.d(TimeUpdateLog.SERVICE_TAG, mode ? AUTO_TIME_LOG:MANUAL_TIME_LOG);
        if (mode == MANUAL_MODE) {
            long userTime = getIHUTime();
            setUserTimeOffset(Long.MAX_VALUE);

            if (setCemTime(userTime))
                Log.i(TimeUpdateLog.SERVICE_TAG, "CEM time successfully set");
            else
                Log.e(TimeUpdateLog.SERVICE_TAG, "Failed to set CEM time on user change");

        } else {
            setUserTimeOffset(0L);
        }
    }



    private boolean setCemTime(long userTime) {
        try {
            // Update CEM time on change of usertime
            VehiclePropValue unixTimeProp = new VehiclePropValue();
            unixTimeProp.prop = VehicleProperty.UNIX_TIME;
            long userTimeSecs = (userTime + 500L) / 1000L;
            Log.i(TimeUpdateLog.SERVICE_TAG, "CET Time update :user time in secs: " + userTimeSecs);
            unixTimeProp.value.int64Values.add(0, userTimeSecs);
            if (mVehicle.set(unixTimeProp) == StatusCode.OK) {
                return true;
            } else {
                return false;
            }
        } catch (RemoteException re) {
            Log.e(TimeUpdateLog.SERVICE_TAG, re.getMessage());
            return false;
        }
    }

    private class GnssCallback extends ILocationCallback.Stub {

        @Override
        public void OnGnssLocationUpdate(GnssTimeLocinfo location) {
            gpsTimeMilis = location.utctime;
            long ihuTime = getIHUTime();
            long storedOffset = getUserTimeOffset();
            if (storedOffset == Long.MAX_VALUE) {
                setUserTimeOffset(ihuTime - gpsTimeMilis);
                locationCounter = 0;
            } else {

                if (Math.abs((ihuTime - gpsTimeMilis) - storedOffset) > maxDiffinIhu) {
                    locationCounter++;
                    if(locationCounter == 2){
                        setIHUTime(gpsTimeMilis + storedOffset);
                        if(setCemTime(gpsTimeMilis + storedOffset)) {
                            Log.i(TimeUpdateLog.SERVICE_TAG, "CEM time successfully set on GPS update");
                        } else {
                            Log.e(TimeUpdateLog.SERVICE_TAG, "Failed to set CEM time on GPS location update");
                        }
                    }
                }else{
                    locationCounter = 0;
                }
            }
        }
    }

    private static class VehicleCallback extends IVehicleCallback.Stub {
        @Override
        public void onPropertyEvent(ArrayList<VehiclePropValue> propValues) {
            for (VehiclePropValue prop : propValues) {
                if (prop.prop == VehicleProperty.UNIX_TIME) {
                    cemTime = prop.value.int64Values.get(0) * 1000L;

                    if (Math.abs(cemTime - getIHUTime()) > maxDiffinCem){
                        unixTimeCounter++;
                        if(unixTimeCounter == 2){
                            setIHUTime(cemTime);
                            Log.d(TimeUpdateLog.SERVICE_TAG, "cemTime in millis: " + cemTime);
                            unixTimeCounter = 0;
                        }

                    }else{
                        unixTimeCounter = 0;
                    }
                }

            }
        }

        @Override
        public void onPropertySet(VehiclePropValue propValue) {
            Log.d(TimeUpdateLog.SERVICE_TAG, "onPropertySet: " + propValue.toString());
        }

        @Override
        public void onPropertySetError(int errorCode, int propId, int areaId) {
            Log.d(TimeUpdateLog.SERVICE_TAG, "onPropertySetError: " + propId);
        }
    }

    public boolean isAutomaticTimeRequested() {
        return Settings.Global.getInt(this.getContentResolver(), Settings.Global.AUTO_TIME, 0) != 0;
    }

    private static long getIHUTime() {
        return System.currentTimeMillis();
    }

    private static boolean setIHUTime(long time) {
        return SystemClock.setCurrentTimeMillis(time);
    }

    private long getUserTimeOffset() {
        return Settings.Global.getLong(this.getContentResolver(), "TimeOffset",
                /*Default Value on Failure*/ Long.MAX_VALUE);
    }

    private boolean setUserTimeOffset(long offset) {
        if (!Settings.Global.putLong(this.getContentResolver(), "TimeOffset", offset)) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "Failed to update global settings database");
            return false;
        }

        return true;
    }

    private class TimeUpdateDeathRecipient implements DeathRecipient {

        @Override
        public void serviceDied(long cookie) {

            try {
                    if (cookie == VHAL_COOKIE ) {
                        Log.w(TimeUpdateLog.SERVICE_TAG, "Vehicle HAL died.");
                        mVehicle.unlinkToDeath(this);
                        mVehicle = null;
                    }

                    if(cookie == GPSHAL_COOKIE ) {
                         Log.w(TimeUpdateLog.SERVICE_TAG, "GPS HAL died.");
                         mGnsshal.unlinkToDeath(this);
                         mGnsshal = null;
                    }
                    ExitServiceApp();

                } catch (RemoteException e) {
                    Log.e(TimeUpdateLog.SERVICE_TAG, "Failed to unlinkToDeath", e);
                    ExitServiceApp();
                }
        }
    }

    private void ExitServiceApp() {
        System.exit(0);
    }

}
