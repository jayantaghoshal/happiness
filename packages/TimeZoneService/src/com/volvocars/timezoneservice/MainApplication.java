/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.timezoneservice;

import android.Manifest;
import android.app.AlarmManager;
import android.app.Application;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;

import android.provider.Settings;
import android.support.v4.content.ContextCompat;
import android.util.Log;

import android.os.RemoteException;
import android.util.Log;
import android.os.IHwBinder.DeathRecipient;

import java.io.File;

import java.util.List;
import java.util.Objects;
import java.util.TimeZone;

import jsqlite.Callback;
import jsqlite.Database;
import jsqlite.Exception;

import vendor.volvocars.hardware.gps.V1_0.ILocationCallback;
import vendor.volvocars.hardware.gps.V1_0.GnssTimeLocinfo;
import vendor.volvocars.hardware.gps.V1_0.ILocationUpdate;



public class MainApplication extends Application implements Callback {
    private BroadcastReceiver receiver;

    private Handler mHandler;


    private static final int LOCATION_INTERVAL = 30000;
    private static final float LOCATION_DISTANCE = 0f;
    private static String timeZoneStringFromDatabase;

    private static int timeZoneChangeCounter = 0;
    private static final int noOfChangesDetected = 3;
    private static String prevTzFromDb;

    private static AlarmManager am;
    private static  Database db;
    private GnssCallback mGnssCallback;
    private ILocationUpdate mGnsshal = null;

    private final TimeZoneDeathRecipient mTimeZoneDeathRecipient = new TimeZoneDeathRecipient();

    private static final String LOG_TAG = "TimeZoneService";


    private void startLocationUpdates() {
        int retry = 0;
        boolean IsLocServiceOk = false;
        try
        {
            while(retry <= 10) {
                mGnsshal = ILocationUpdate.getService();
                if (mGnsshal == null) {
                    Log.i(LOG_TAG, "GPS-HAL ILocationUpdate::getService: failed !!");
                    retry++;
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        Log.e(LOG_TAG, "Sleep was interrupted: " + e.getMessage());
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
            mGnsshal.linkToDeath(mTimeZoneDeathRecipient, 1221 /* dummy cookie */);
            mGnssCallback = new GnssCallback(this);
            // Register GNSS Location update
            mGnsshal.requestGNSSLocationUpdates(mGnssCallback);
        } catch (RemoteException ex) {
            Log.e(LOG_TAG, "GNSS Location HAL failure: " + ex.getMessage());
            ExitServiceApp(); // terminate ourself to get re-spawne
        }
    }

    private class GnssCallback extends ILocationCallback.Stub {

        private MainApplication minstance;

        public GnssCallback(MainApplication instance) {
            minstance = instance;
        }
        @Override
        public void OnGnssLocationUpdate(GnssTimeLocinfo location) {
            if(isAutomaticTimeZoneRequested()) {
                double Latitude = location.latitude;
                double Longitude = location.longitude;
                String Query = queryBuilder(Latitude, Longitude);
                try {
                    db.exec(Query, minstance);
                    String systemTimeZone = TimeZone.getDefault().getID();

                    /**
                    * The TimeZone will be checked every 30 seconds and if
                    * the new timezone is consistent over 3 tries (90 secs)
                    * System timezone will be updated.
                    */

                    if (timeZoneStringFromDatabase.equals(prevTzFromDb) &&
                            !systemTimeZone.equals(timeZoneStringFromDatabase)) {
                        timeZoneChangeCounter++;
                        if(timeZoneChangeCounter == noOfChangesDetected) {
                            am.setTimeZone(timeZoneStringFromDatabase);
                            timeZoneChangeCounter = 0;
                        }
                    } else {
                        timeZoneChangeCounter=0;
                    }
                    prevTzFromDb = timeZoneStringFromDatabase;

                } catch (Exception e) {
                    Log.e(LOG_TAG, "Query Failed: " + e.getMessage());
                }
            }
        }
    }

    public String queryBuilder(double lat, double lon){
        String query;

        query = "SELECT tzid from timezones where within(GeomFromText('POINT(";
        query = query + String.valueOf(lon) + " " + String.valueOf(lat);
        query = query + ")'), timezones.Geometry);";

        Log.v(LOG_TAG, "Query:" + query);
        return query;
    }



    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(LOG_TAG, String.format("Time Zone Service Started!"));
         am = (AlarmManager) this.getApplicationContext().getSystemService(Context.ALARM_SERVICE);
        System.loadLibrary("jsqlite");

        //Open the spatialite database
        try {
            db = new jsqlite.Database();
            db.open("/vendor/etc/timezone/timezones.db", jsqlite.Constants.SQLITE_OPEN_READONLY);
        } catch (jsqlite.Exception e) {
            Log.e(LOG_TAG, "onCreate Exception: " + e.getMessage());
        } catch (java.lang.OutOfMemoryError me) {
            Log.e(LOG_TAG, "onCreate OutOfMemory: " + me.getMessage());
        }

        mHandler = new Handler();
        // Register for gps location Updates
        startLocationUpdates();
    }



    @Override
    public void columns(String[] coldata) {
    }

    @Override
    public void types(String[] types) {

    }

    //The result of db.exec is received here in the callback
    @Override
    public boolean newrow(String[] rowdata) {
        for (String s: rowdata ) {
            Log.v(LOG_TAG,"rowdata " + s);
            timeZoneStringFromDatabase = s;
        }
        return false;
    }

    public boolean isAutomaticTimeZoneRequested() {
        return Settings.Global.getInt(this.getContentResolver(), Settings.Global.AUTO_TIME_ZONE, 0) != 0;
    }

    private class TimeZoneDeathRecipient implements DeathRecipient {

        @Override
        public void serviceDied(long cookie) {

            try {
                Log.w(LOG_TAG, "GPS HAL died.");
                mGnsshal.unlinkToDeath(this);
                mGnsshal = null;
                ExitServiceApp();
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Failed to unlinkToDeath", e);
                ExitServiceApp();
            }
        }
    }

    private void ExitServiceApp() {
        System.exit(0);
    }
}
