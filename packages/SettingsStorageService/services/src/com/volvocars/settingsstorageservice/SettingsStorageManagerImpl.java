/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.settingsstorageservice;

import android.os.RemoteException;
import android.util.Log;

import com.volvocars.settingsstorageservice.ISettingsStorageManager;
import com.volvocars.settingsstorageservice.ISettingsStorageManagerCallback;

import java.util.Arrays;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SettingsStorageManagerImpl extends ISettingsStorageManager.Stub {
    private static final String LOG_TAG = "SettingsStorageService";

    private SettingsStorage settingsStorage;

    private SettingsReader settingsReader;
    private HashMap<String, SettingsReader.Setting> settings;

    public enum UpdateReason {
        UNKNOWN; /* CLOUD, PROFILE_CHANGE? */
    }

    public SettingsStorageManagerImpl() {
    }

    public void init(SettingsStorage settingsStorage, SettingsReader settingsReader) {
        this.settingsStorage = settingsStorage;
        this.settingsReader = settingsReader;

        settings = settingsReader.readJson();
    }

    /**
     * Set value to specified key
     *
     * @param appId Identification of application
     * @param key   Key to set value to
     * @param value Value to set to key
     */
    @Override
    public void setByString(String appId, String key, String value)
            throws IdNotFoundException, InvalidValueException, RemoteException {
        Log.v(LOG_TAG, "[ManagerImpl] setByString: [appid: " + appId + ", key: " + key + ", value: " + value + "]");

        if (settings.get(appId) != null) {
            int keyWithOffset = getKeyWithOffset(appId, settings.get(appId).cloudPackage.settings.indexOf(key));
            if (keyWithOffset != -1) {
                settingsStorage.set(keyWithOffset, value);
            } else {
                Log.w(LOG_TAG, "[ManagerImpl] could not set key " + key + "due to invalid value of new key");
                throw new InvalidValueException(
                        "Could not compute new key, " + (-1 == settings.get(appId).cloudPackage.settings.indexOf(key)
                                ? "specified key is invalid (not maching any in settings.json"
                                : "offset value specified in settings.json is invalid"));
            }
        } else {
            throw new IdNotFoundException("Application id; " + appId + "doesn't match any id in json file");
        }
    }

    /**
     * Set value to specified key
     *
     * @param appId Identification of application
     * @param key   Key to set value to
     * @param value Value to set to key
     */
    @Override
    public void setByInt(String appId, int key, String value)
            throws IdNotFoundException, InvalidValueException, RemoteException {
        Log.v(LOG_TAG, "[ManagerImpl] setByInt: [appid: " + appId + ", key: " + key + ", value: " + value + "]");

        if (settings.get(appId) != null) {
            int keyWithOffset = getKeyWithOffset(appId, key);
            if (keyWithOffset != -1) {
                settingsStorage.set(keyWithOffset, value);
            } else {
                Log.w(LOG_TAG, "[ManagerImpl] could not set key " + key + "due to invalid value of new key");
                throw new InvalidValueException(
                        "Could not compute new key, offset value specified in settings.json is invalid");
            }
        } else {
            throw new IdNotFoundException("Application id; " + appId + "doesn't match any id in json file");
        }
    }

    /**
     * Get value of specified key (value returned in ISettingsStorageManager
     * keyValue)
     *
     * @param appId    Identification of application
     * @param key      Key to get value of
     * @param callback Callback
     */
    @Override
    public void getAsyncByString(String appId, String key, ISettingsStorageManagerCallback callback)
            throws IdNotFoundException, InvalidValueException {
        Log.v(LOG_TAG, "[ManagerImpl] getAsyncByString: [appid: " + appId + ", key: " + key + "]");

        if (settings.get(appId) != null) {
            int keyWithOffset = getKeyWithOffset(appId, settings.get(appId).cloudPackage.settings.indexOf(key));
            if (keyWithOffset != -1) {
                try {
                    callback.keyValueByString(key, settingsStorage.get(keyWithOffset));
                } catch (RemoteException e) {
                    Log.w(LOG_TAG, "[ManagerImpl] callback.keyValue RemoteException: [" + e.getMessage() + "]");
                }
            } else {
                Log.w(LOG_TAG,
                        "[ManagerImpl] Invalid offset registered for app " + appId + ", returning default offset -1");
                throw new InvalidValueException(
                        "Could not compute new key, " + (-1 == settings.get(appId).cloudPackage.settings.indexOf(key)
                                ? "specified key is invalid (not maching any in settings.json"
                                : "offset value specified in settings.json is invalid"));
            }
        } else {
            throw new IdNotFoundException("Application id; " + appId + "doesn't match any id in json file");
        }
    }

    /**
     * Get value of specified key (value returned in ISettingsStorageManager
     * keyValue)
     *
     * @param appId    Identification of application
     * @param key      Key to get value of
     * @param callback Callback
     */
    @Override
    public void getAsyncByInt(String appId, int key, ISettingsStorageManagerCallback callback)
            throws IdNotFoundException, InvalidValueException {
        Log.v(LOG_TAG, "[ManagerImpl] getAsyncByInt: [appid: " + appId + ", key: " + key + "]");

        if (settings.get(appId) != null) {
            int keyWithOffset = getKeyWithOffset(appId, key);
            if (keyWithOffset != -1) {
                try {
                    callback.keyValueByInt(key, settingsStorage.get(keyWithOffset));
                } catch (RemoteException e) {
                    Log.w(LOG_TAG, "[ManagerImpl] callback.keyValue RemoteException: [" + e.getMessage() + "]");
                }
            } else {
                Log.w(LOG_TAG,
                        "[ManagerImpl] Invalid offset registered for app " + appId + ", returning default offset -1");
                throw new InvalidValueException(
                        "Could not compute new key, offset value specified in settings.json is invalid");
            }
        } else {
            throw new IdNotFoundException("Application id; " + appId + "doesn't match any id in json file");
        }
    }

    /**
     * Calculate new key (with offset to get unique keys)
     */
    private int getKeyWithOffset(String appId, int key) {

        int offset = -1;

        if (settings.get(appId) != null) {
            offset = settings.get(appId).offset;
        }

        if (offset != -1) {
            int keyWithOffset = offset << 16 | key;
            return keyWithOffset;
        } else {
            Log.w(LOG_TAG, "[ManagerImpl] Offset value " + offset + "not valid!");
        }

        return -1;
    }

    public class IdNotFoundException extends RemoteException {
        public IdNotFoundException(String message) {
            super(message);
        }
    }

    public class InvalidValueException extends RemoteException {
        public InvalidValueException(String message) {
            super(message);
        }
    }

    public class NoSuchClientRegisteredException extends RemoteException {
        public NoSuchClientRegisteredException(String message) {
            super(message);
        }
    }
}