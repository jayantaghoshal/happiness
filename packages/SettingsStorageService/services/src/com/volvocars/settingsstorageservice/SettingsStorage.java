/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.settingsstorageservice;

import android.util.Log;
import android.os.HwBinder;
import android.os.RemoteException;

import java.util.NoSuchElementException;
import java.util.Timer;
import java.util.TimerTask;

import vendor.volvocars.hardware.settings.V1_0.*;
import vendor.volvocars.hardware.profiles.V1_0.ProfileIdentifier;

public class SettingsStorage extends ISettingsListener.Stub {
    private static final String LOG_TAG = "SettingsStorageService";
    private ISettingsStorage settingsStorage;
    private SettingsStorageService service;

    private DeathRecipient death = new DeathRecipient(this);

    public SettingsStorage(SettingsStorageService service) {
        this.service = service;
    }

    final class DeathRecipient implements HwBinder.DeathRecipient {
        SettingsStorage settingsStorage;

        DeathRecipient(SettingsStorage settingsStorage) {
            this.settingsStorage = settingsStorage;
        }

        @Override
        public void serviceDied(long cookie) {
            Log.e(LOG_TAG, "[SettingsStorage] Lost Connection to SettingsStorageDaemon");
            settingsStorage.daemonDied();
        }
    }

    public void init() {
        retryService();
    }

    private void retryService() {
        try {
            connectToService();
        } catch (NoSuchElementException e) {
            Log.e(LOG_TAG, "[SettingsStorage] SettingsStorageDaemon not up yet.. Scheduling retry.");
            new Timer().schedule(new TimerTask() {
                @Override
                public void run() {
                    Log.d(LOG_TAG, "[SettingsStorage] Retrying to connect to SettingsStorageDaemon");
                    retryService();
                }
            }, 2000);
        }
    }

    private void connectToService() {
        try {
            settingsStorage = ISettingsStorage.getService();
            settingsStorage.linkToDeath(death, 1010);
        } catch (RemoteException e) {
            Log.e(LOG_TAG,
                    "[SettingsStorage] Cannot connect to ISettingsStorage: RemoteException [" + e.getMessage() + "]");
        }

    }

    public void daemonDied() {
        settingsStorage = null;
        retryService();
    }

    @Override
    public void settingsForCurrentUserChanged(int key, short reason, short currentProfile) {
        Log.d(LOG_TAG, "[SettingsStorage] settingsForCurrentUserChanged called but not implemented! [key: " + key
                + ", reason: " + reason + ", currentProfile: " + currentProfile);

        // TODO: Will only be called on profile changed in
        // SettingsStorage::onProfileChange?
    }

    public void set(int key, String data) throws RemoteException {
        short profileId = 0;
        Log.w(LOG_TAG, "TODO: Implement a way to retrieve current profile! Just setting 0 now as default... ");
        Log.v(LOG_TAG,
                "[SettingsStorage] set [key: " + key + ", ProfileIdentifier: " + profileId + ", data: " + data + "]");
        if (settingsStorage != null) {
            settingsStorage.set(key, profileId, data);
        } else {
             Log.w(LOG_TAG, "[SettingsStorage] no connection to daemon, can't do anything...");
        }
    }

    public String get(int key) throws RemoteException {
        short profileId = 0;
        Log.w(LOG_TAG, "TODO: Implement a way to retrieve current profile! Just setting 0 now as default... ");
        Log.v(LOG_TAG, "[SettingsStorage] get [key: " + key + ", ProfileIdentifier: " + profileId + "]");
        if (settingsStorage != null) {

            return settingsStorage.get(key, (short) profileId);
        }
        else {
            Log.w(LOG_TAG, "[SettingsStorage] no connection to daemon, can't do anything...");
       }
        throw new RemoteException("No connection to daemon");
    }

    public void subscribe(int key, short userScope) throws RemoteException {
        Log.v(LOG_TAG, "[SettingsStorage] subscribe [key: " + key + ", UserScope: " + userScope + "]");
        if (settingsStorage != null) {

            settingsStorage.subscribe(key, userScope, this);
        }
        else {
            Log.w(LOG_TAG, "[SettingsStorage] no connection to daemon, can't do anything...");
       }
    }

    public void unsubscribe(int key) throws RemoteException {
        Log.v(LOG_TAG, "[SettingsStorage] unsubscribe [key: " + key + "]");
        if (settingsStorage != null) {
            settingsStorage.unsubscribe(key, this);
        }
        else {
            Log.w(LOG_TAG, "[SettingsStorage] no connection to daemon, can't do anything...");
       }
    }
}