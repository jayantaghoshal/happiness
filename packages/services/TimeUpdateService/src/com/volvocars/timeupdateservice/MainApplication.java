/*
 * Copyright 2017 Volvo Car Corporation
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

import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemClock;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;

import android.util.Log;

import android.support.v4.content.ContextCompat;

import android.provider.Settings;

import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;

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

public class MainApplication extends Application implements LocationListener {
    private BroadcastReceiver receiver;
    private SettingsObserver mSettingsObserver;
    private Handler mHandler;
    private static final int EVENT_AUTO_TIME_CHANGED = 1;
    /*Make this common to all classes*/
    private static final Boolean MANUAL_MODE = false;
    private static final Boolean AUTO_MODE = true;

    private static final int REQUEST_PERMISSIONS_REQUEST_CODE = 34;
    private static final int LOCATION_INTERVAL = 1000;  //30000;
    private static final float LOCATION_DISTANCE = 0f;

    private static long gpsTimeMilis;
    private static long gpsTimeOffset;
    private static long cemTime;

    private IVehicle mVehicle = null;
    private VehicleCallback mInternalCallback;


    private LocationManager mLocationManager = null;

    private static int locationCounter = 0;
    private static int unixTimeCounter = 0;


    private final static long maxDiffinCem = 2000L;
    private final static long maxDiffinIhu = 5000L;

    /**
     * Return the current state of the permissions need
     * @param None
     * @return true if all permissions are matched false if any one check fails
    */
    private boolean checkPermissions() {
        if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            Log.d(TimeUpdateLog.SERVICE_TAG, "Incorrect 'uses-permission', requires 'ACCESS_FINE_LOCATION'");
            return false;
        }

        if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            Log.d(TimeUpdateLog.SERVICE_TAG, "Incorrect 'uses-permission', requires 'ACCESS_COARSE_LOCATION'");
            return false;
        }

        if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.SET_TIME) != PackageManager.PERMISSION_GRANTED) {
            Log.d(TimeUpdateLog.SERVICE_TAG, "Incorrect 'uses-permission', requires 'SET_TIME'");
            return false;
        }

        return true;
    }

    /**
     * Implement virtual methods of the Location Listener to extract time from GPS
     * @param Location, last received location.
     * @return None
     */

    @Override
    public void onLocationChanged(Location location) {

        gpsTimeMilis = location.getTime();
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
                        setCemTime(gpsTimeMilis + storedOffset);
                    }
                }else{
                    locationCounter = 0;
                }
        }

    }

    @Override
    public void onProviderDisabled(String provider) {
        Log.e(TimeUpdateLog.SERVICE_TAG, "onProviderDisabled: " + provider);
    }

    @Override
    public void onProviderEnabled(String provider) {
        Log.e(TimeUpdateLog.SERVICE_TAG, "onProviderEnabled: " + provider);
    }

    @Override
    public void onStatusChanged(String provider, int status, Bundle extras) {
        Log.e(TimeUpdateLog.SERVICE_TAG, "onStatusChanged: " + provider);
    }

    private void startLocationUpdates() {
        try {
            List<String> providers = mLocationManager.getAllProviders();
            Log.i(TimeUpdateLog.SERVICE_TAG, "providers: " + providers);
        } catch (java.lang.SecurityException ex) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "fail to get location, ignore", ex);
        } catch (IllegalArgumentException ex) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "get gps provider does not exist " + ex.getMessage());
        }

        try {
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, LOCATION_INTERVAL, LOCATION_DISTANCE,
                    this);
        } catch (java.lang.SecurityException ex) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "fail to request location update, ignore", ex);
        } catch (IllegalArgumentException ex) {
            Log.e(TimeUpdateLog.SERVICE_TAG, "gps provider does not exist " + ex.getMessage());
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
        mLocationManager = (LocationManager) getApplicationContext().getSystemService(Context.LOCATION_SERVICE);

        if (!checkPermissions()) {
            Log.i(TimeUpdateLog.SERVICE_TAG, "Incorrect Permissions!");
            System.runFinalizersOnExit(true);
            System.exit(0);
        } else {
            startLocationUpdates();
        }
        // Vehicle HAL properties
        try {
            mVehicle = IVehicle.getService();
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
        } catch (RemoteException re) {
            Log.e(TimeUpdateLog.SERVICE_TAG, re.getMessage());
        }

        mSettingsObserver = new SettingsObserver(mHandler, EVENT_AUTO_TIME_CHANGED, this);
        mSettingsObserver.observe(this);

        IntentFilter filter = new IntentFilter();
        filter.addAction(Intent.ACTION_TIME_CHANGED);
        filter.addAction(Intent.ACTION_TIMEZONE_CHANGED);

        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                Log.d(TimeUpdateLog.SERVICE_TAG, String.format("Intent Received: Time Changed Manually"));
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
        if (mode == MANUAL_MODE) {

            long userTime = getIHUTime();
            Log.e(TimeUpdateLog.SERVICE_TAG, "user set time: " + String.valueOf(userTime));

            setUserTimeOffset(Long.MAX_VALUE);

            if (setCemTime(userTime))
                Log.i(TimeUpdateLog.SERVICE_TAG, "CEM time successfully set");
            else
                Log.i(TimeUpdateLog.SERVICE_TAG, "Failed to set CEM time on user change");

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
            Log.i(TimeUpdateLog.SERVICE_TAG, "user time in secs: " + userTimeSecs);
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

}
