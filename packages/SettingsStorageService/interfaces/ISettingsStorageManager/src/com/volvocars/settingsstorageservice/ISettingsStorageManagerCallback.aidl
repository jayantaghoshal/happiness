package com.volvocars.settingsstorageservice;

oneway interface ISettingsStorageManagerCallback {
    void keyValueByString(in String key, in String value);

    void keyValueByInt(in int key, in String value);
}