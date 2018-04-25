package com.volvocars.settingsstorageservice;

import com.volvocars.settingsstorageservice.ISettingsStorageManagerCallback;

/**
* Interface to SettingsStorageService
*/
oneway interface ISettingsStorageManager {

    /**
    * Set value to specified key
    * @param appId Identification of application
    * @param key Key to set value to
    * @param value Value to set to key
    */
    void setByString(in String appId, in String key, in String value);

     /**
    * Set value to specified key
    * @param appId Identification of application
    * @param key Key to set value to
    * @param value Value to set to key
    */
    void setByInt(in String appId, in int key, in String value);

    /**
    * Get value of specified (value returned in ISettingsStorageManager keyValue)
    * @param appId Identification of application
    * @param key Key to get value of
    * @param callback Callback
    */
    void getAsyncByString(in String appId, in String key, in ISettingsStorageManagerCallback callback);

    /**
    * Get value of specified key (value returned in ISettingsStorageManager keyValue)
    * @param appId Identification of application
    * @param key Key to get value of
    * @param callback Callback
    */
    void getAsyncByInt(in String appId, in int key, in ISettingsStorageManagerCallback callback);
 }