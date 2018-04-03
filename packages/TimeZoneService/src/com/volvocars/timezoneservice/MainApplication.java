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
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Handler;

import android.provider.Settings;
import android.support.v4.content.ContextCompat;
import android.util.Log;

import java.io.File;

import java.util.List;
import java.util.Objects;
import java.util.TimeZone;

import jsqlite.Callback;
import jsqlite.Database;
import jsqlite.Exception;



public class MainApplication extends Application implements LocationListener, Callback {
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
    private static LocationManager mLocationManager = null;
    private static final String LOG_TAG = "TimeZoneService";


    /**
     * Return the current state of the permissions need
     * @return true if all permissions are matched false if any one check fails
    */
    private boolean checkPermissions() {
        if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            Log.d(LOG_TAG, "Incorrect 'uses-permission', requires 'ACCESS_FINE_LOCATION'");
            return false;
        }

        if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            Log.d(LOG_TAG, "Incorrect 'uses-permission', requires 'ACCESS_COARSE_LOCATION'");
            return false;
        }

        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.SET_TIME_ZONE) != PackageManager.PERMISSION_GRANTED) {
            Log.d(LOG_TAG, "Incorrect 'uses-permission', requires 'SET_TIME_ZONE'");
            return false;
        }
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            Log.d(LOG_TAG, "Incorrect 'uses-permission', requires 'READ_EXTERNAL_STORAGE'");
            return false;
        }

        return true;
    }

    /**
     * Implement virtual methods of the Location Listener to extract time from GPS
     * @param location, last received location.
     * @return None
     */

    @Override
    public void onLocationChanged(Location location) {

        if(isAutomaticTimeZoneRequested()) {
            double Latitude = location.getLatitude();
            double Longitude = location.getLongitude();
            String Query = queryBuilder(Latitude, Longitude);
            try {
                db.exec(Query, this);
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

    @Override
    public void onProviderDisabled(String provider) {
        Log.e(LOG_TAG, "onProviderDisabled: " + provider);
    }

    @Override
    public void onProviderEnabled(String provider) {
        Log.e(LOG_TAG, "onProviderEnabled: " + provider);
    }

    @Override
    public void onStatusChanged(String provider, int status, Bundle extras) {
        Log.e(LOG_TAG, "onStatusChanged: " + provider);
    }

    private void startLocationUpdates() {
        try {
            List<String> providers = mLocationManager.getAllProviders();
            Log.i(LOG_TAG, "providers: " + providers);
        } catch (java.lang.SecurityException ex) {
            Log.e(LOG_TAG, "fail to get location, ignore", ex);
        } catch (IllegalArgumentException ex) {
            Log.e(LOG_TAG, "get gps provider does not exist " + ex.getMessage());
        }

        try {
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, LOCATION_INTERVAL,
                    LOCATION_DISTANCE, this);
        } catch (java.lang.SecurityException ex) {
            Log.e(LOG_TAG, "fail to request location update, ignore", ex);
        } catch (IllegalArgumentException ex) {
            Log.e(LOG_TAG, "gps provider does not exist " + ex.getMessage());
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


        //Register for gps location Updates
        mHandler = new Handler();
        mLocationManager = (LocationManager) getApplicationContext().getSystemService(Context.LOCATION_SERVICE);

        if (!checkPermissions()) {
            Log.e(LOG_TAG, "Incorrect Permissions!");
            System.runFinalizersOnExit(true);
            System.exit(0);
        } else {
            startLocationUpdates();
        }


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
}
