/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.settingsstorageservice;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;

import org.json.*;

public class SettingsReader {
    private static final String LOG_TAG = "SettingsStorageService";
    private Context context;

    public void init(Context context) {
        this.context = context;
    }

    public HashMap<String, Setting> readJson() {
        Log.v(LOG_TAG, "[Settings] readJson");
        HashMap<String, Setting> settingsMap = new HashMap();
        try {
            InputStream input = context.getResources().openRawResource(R.raw.settings);

            byte[] buffer = new byte[input.available()];
            input.read(buffer);
            input.close();
            String json = new String(buffer, "UTF-8");

            JSONObject jsonReader = new JSONObject(json);
            JSONArray jsonSettingsType = jsonReader.getJSONArray("app_settings");

            for (int i = 0; i < jsonSettingsType.length(); i++) {
                Setting setting = new Setting();
                JSONObject s = jsonSettingsType.getJSONObject(i);
                String key = s.getString("app_id");
                setting.offset = s.getInt("app_settings_offset");
                if (s.has("cloud_settings_package")) {
                    JSONObject jsonPackage = s.getJSONObject("cloud_settings_package");
                    setting.cloudPackage.name = jsonPackage.getString("name");
                    JSONArray jsonSettings = jsonPackage.getJSONArray("settings");
                    ArrayList<String> settings = new ArrayList();
                    for (int j = 0; j < jsonSettings.length(); j++) {
                        settings.add(jsonSettings.getString(j));
                    }
                    setting.cloudPackage.settings = settings;
                }
                else Log.d(LOG_TAG, "WORLD");

                settingsMap.put(key, setting);
            }
        } catch (IOException | JSONException e) {
            Log.v(LOG_TAG, "[Settings] Error reading/parsing jsonfile [" + e.getMessage() + "]");
        }

        return settingsMap;
    }


    public class Setting {

        public class Package {
            public String name = "";
            public ArrayList<String> settings = new ArrayList();
        }

        public Package cloudPackage = new Package();
        public int offset = -1;
    }
}