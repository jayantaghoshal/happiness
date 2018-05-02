/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdate;

import android.util.Log;
import com.volvocars.settingsstorageservice.ISettingsStorageManagerCallback;

public class SettingsStorageManagerCallback extends ISettingsStorageManagerCallback.Stub {
    private static final String LOG_TAG = "SettingsStorageManagerCallback";

    SoftwareUpdateService service = null;

    public SettingsStorageManagerCallback(SoftwareUpdateService service) {
        this.service = service;
    }

    @Override
    public void keyValueByString(String key, String value) {
        Log.v(LOG_TAG, "keyValueByString: [key: " + key + ", value: " + value + "]");
        service.UpdateSetting(key, value);
    }

    @Override
    public void keyValueByInt(int key, String value) {
        Log.v(LOG_TAG, "keyValueByInt: [key: " + key + ", value: " + value + "]");
    }

}
