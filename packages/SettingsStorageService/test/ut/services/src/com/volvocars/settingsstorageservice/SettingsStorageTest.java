/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.settingsstorageservice;

import android.support.test.filters.SmallTest;
import android.support.test.runner.AndroidJUnit4;

import android.content.Context;
import android.util.Log;

import org.json.*;
import org.junit.Test;
import org.junit.Rule;

import org.junit.runner.RunWith;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;

import static org.mockito.Mockito.*;
import org.mockito.Matchers.*;
import org.mockito.Mock;
import org.mockito.Spy;
import org.mockito.junit.MockitoRule;
import org.mockito.junit.MockitoJUnit;
import org.mockito.runners.MockitoJUnitRunner;

import java.lang.AssertionError;
import java.io.ByteArrayInputStream;;
import java.io.IOException;
import java.io.InputStream;;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

@SmallTest
@RunWith(AndroidJUnit4.class)
public class SettingsStorageTest {

    private static final String LOG_TAG = "SettingsStorageService";
    private static final String SUB_LOG_TAG = "[Test]";

    private HashMap<String, SettingsReader.Setting> settings = new HashMap();
    private SettingsStorageManagerImpl settingsStorageManagerImpl = new SettingsStorageManagerImpl();

    @Mock
    SettingsReader mockSettingsReader;

    @Mock
    SettingsStorage mockSettingsStorage;

    @Mock
    ISettingsStorageManagerCallback mockSettingsStorageManagerCallback;

    @Mock
    Context mockContext;

    @Mock
    InputStream mockInputStream;

    @Mock
    JSONObject mockJsonReader;

    @Rule
    public MockitoRule mockitoRule = MockitoJUnit.rule();

    /** Test SettingsStorageManagerImpl */

    @Test
    public void testSetValueByIntSuccess() {
        int offset = 10000;
        int key = 0;
        String appId = "Test";

        prepareTestCase(appId, offset);

        try {
            settingsStorageManagerImpl.setByInt(appId, key, "1");
            verify(mockSettingsStorage, times(1)).set(expectedKeyWithOffset(key, offset), "1");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            fail(e.getMessage());
        }
    }

    @Test
    public void testSetValueByStringSuccess() {
        int offset = 10000;
        int keyInt = 0;
        String keyString = "settingNrOne";
        String appId = "Test";

        ArrayList<String> s = new ArrayList<>();
        s.add(keyString);

        prepareTestCase(appId, s, offset);

        try {
            settingsStorageManagerImpl.setByString(appId, keyString, "1");
            verify(mockSettingsStorage, times(1)).set(expectedKeyWithOffset(keyInt, offset), "1");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            fail(e.getMessage());
        }
    }

    @Test
    public void testSetValueByIntFailInvalidOffset() {
        int offset = -1;
        int key = 0;
        String appId = "Test";

        prepareTestCase(appId, offset);

        try {
            settingsStorageManagerImpl.setByInt(appId, key, "1");
            fail("expect InvalidValueException to be thrown");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            assertTrue(e.getClass().getName().contains("InvalidValueException"));
        }
    }

    @Test
    public void testSetValueByStringFailInvalidOffset() {
        int offset = -1;
        String key = "settingNrOne";
        String appId = "Test";

        ArrayList<String> s = new ArrayList<>();
        s.add(key);

        prepareTestCase(appId, s, offset);

        try {
            settingsStorageManagerImpl.setByString(appId, key, "1");
            fail("expect InvalidValueException to be thrown");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            assertTrue(e.getClass().getName().contains("InvalidValueException"));
        }
    }

    @Test
    public void testSetValueByIntFailInvalidAppId() {
        int offset = 10000;
        int key = 0;
        String appId = "Test";

        prepareTestCase(appId, offset);

        try {
            settingsStorageManagerImpl.setByInt("Hello World", key, "1");
            fail("expect IdNotFoundException to be thrown");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            assertTrue(e.getClass().getName().contains("IdNotFoundException"));
        }
    }

    @Test
    public void testSetValueByStringFailInvalidAppId() {
        int offset = 10000;
        String key = "settingNrOne";
        String appId = "Test";

        ArrayList<String> s = new ArrayList<>();
        s.add(key);

        prepareTestCase(appId, s, offset);

        try {
            settingsStorageManagerImpl.setByString("Hello World", key, "1");
            fail("expect IdNotFoundException to be thrown");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            assertTrue(e.getClass().getName().contains("IdNotFoundException"));
        }
    }

    @Test
    public void testGetValueByIntSuccess() {
        int offset = 10000;
        int key = 0;
        String appId = "Test";

        prepareTestCase(appId, offset);

        try {
            settingsStorageManagerImpl.getAsyncByInt(appId, key, mockSettingsStorageManagerCallback);
            verify(mockSettingsStorage, times(1)).get(expectedKeyWithOffset(key, offset));
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            fail(e.getMessage());
        }
    }

    @Test
    public void testGetValueByStringSuccess() {
        int offset = 10000;
        int keyInt = 0;
        String keyString = "settingNrOne";
        String appId = "Test";

        ArrayList<String> s = new ArrayList<>();
        s.add(keyString);

        prepareTestCase(appId, s, offset);

        try {
            settingsStorageManagerImpl.getAsyncByString(appId, keyString, mockSettingsStorageManagerCallback);
            verify(mockSettingsStorage, times(1)).get(expectedKeyWithOffset(keyInt, offset));
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            fail(e.getMessage());
        }
    }

    @Test
    public void testGetValueByIntFailInvalidOffset() {
        int offset = -1;
        int key = 0;
        String appId = "Test";

        prepareTestCase(appId, offset);

        try {
            settingsStorageManagerImpl.getAsyncByInt(appId, key, mockSettingsStorageManagerCallback);
            fail("expect InvalidValueException to be thrown");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            assertTrue(e.getClass().getName().contains("InvalidValueException"));
        }
    }

    @Test
    public void testGetValueByStringFailInvalidOffset() {
        int offset = -1;
        String key = "settingNrOne";
        String appId = "Test";

        ArrayList<String> s = new ArrayList<>();
        s.add(key);

        prepareTestCase(appId, s, offset);

        try {
            settingsStorageManagerImpl.getAsyncByString(appId, key, mockSettingsStorageManagerCallback);
            fail("expect InvalidValueException to be thrown");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            assertTrue(e.getClass().getName().contains("InvalidValueException"));
        }
    }

    @Test
    public void testGetValueByIntFailInvalidAppId() {
        int offset = 10000;
        int key = 0;
        String appId = "Test";

        prepareTestCase(appId, offset);

        try {
            settingsStorageManagerImpl.getAsyncByInt("Hello World", key, mockSettingsStorageManagerCallback);
            fail("expect IdNotFoundException to be thrown");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            assertTrue(e.getClass().getName().contains("IdNotFoundException"));
        }
    }

    @Test
    public void testGetValueByStringFailInvalidAppId() {
        int offset = 10000;
        String key = "settingNrOne";
        String appId = "Test";

        ArrayList<String> s = new ArrayList<>();
        s.add(key);

        prepareTestCase(appId, s, offset);

        try {
            settingsStorageManagerImpl.getAsyncByString("Hello World", key, mockSettingsStorageManagerCallback);
            fail("expect IdNotFoundException to be thrown");
        } catch (Exception e) {
            Log.v(LOG_TAG, "Exception: " + e.getMessage());
            assertTrue(e.getClass().getName().contains("IdNotFoundException"));
        }
    }

    @Test
    public void testReadJsonWithCloudSettingsPackageSuccess() {
        SettingsReader settingsReader = new SettingsReader();
        HashMap<String, SettingsReader.Setting> expectedSettingsMap = new HashMap();
        SettingsReader.Setting setting = mockSettingsReader.new Setting();
        setting.offset = 10000;
        ArrayList<String> settingsList = new ArrayList<>();
        settingsList.add("setting1");
        settingsList.add("setting2");
        setting.cloudPackage.settings = settingsList;
        setting.cloudPackage.name = "test_package";
        expectedSettingsMap.put("Test", setting);

        String json = "{app_settings: [{app_id: Test, cloud_settings_package: {name: test_package,"
                + "settings: [setting1,setting2]}, app_settings_offset: 10000}]}";

        InputStream stream = new ByteArrayInputStream(json.getBytes());
        settingsReader.init(mockContext, stream);

        try {
            HashMap<String, SettingsReader.Setting> actualSettingsMap = settingsReader.readJson();
            assertTrue(compareSettingMaps(expectedSettingsMap, actualSettingsMap));
        } catch (IOException | JSONException e) {
            Log.v(LOG_TAG,
                    SUB_LOG_TAG + "Error reading/parsing jsonfile [" + e.getClass() + "," + e.getMessage() + "]");
            fail("Exception");
        }
    }

    @Test
    public void testReadJsonWithoutCloudSettingsPackageSuccess() {
        SettingsReader settingsReader = new SettingsReader();
        HashMap<String, SettingsReader.Setting> expectedSettingsMap = new HashMap();
        SettingsReader.Setting setting = mockSettingsReader.new Setting();
        setting.offset = 10000;
        expectedSettingsMap.put("Test", setting);

        String json = "{app_settings: [{app_id: Test, app_settings_offset: 10000}]}";

        InputStream stream = new ByteArrayInputStream(json.getBytes());
        settingsReader.init(mockContext, stream);

        try {
            HashMap<String, SettingsReader.Setting> actualSettingsMap = settingsReader.readJson();
            assertTrue(compareSettingMaps(expectedSettingsMap, actualSettingsMap));
        } catch (IOException | JSONException e) {
            Log.v(LOG_TAG,
                    SUB_LOG_TAG + "Error reading/parsing jsonfile [" + e.getClass() + "," + e.getMessage() + "]");
            fail("Exception");
        }
    }

    public void testReadJsonMissingOffset() {
        SettingsReader settingsReader = new SettingsReader();
        HashMap<String, SettingsReader.Setting> expectedSettingsMap = new HashMap();
        SettingsReader.Setting setting = mockSettingsReader.new Setting();
        setting.offset = 10000;
        expectedSettingsMap.put("Test", setting);

        String json = "{app_settings: [{app_id: Test}]}";

        InputStream stream = new ByteArrayInputStream(json.getBytes());
        settingsReader.init(mockContext, stream);

        try {
            HashMap<String, SettingsReader.Setting> actualSettingsMap = settingsReader.readJson();
        } catch (IOException | JSONException e) {
            Log.v(LOG_TAG,
                    SUB_LOG_TAG + "Error reading/parsing jsonfile [" + e.getClass() + "," + e.getMessage() + "]");
            assertTrue(e.getClass().getName().contains("JSONException"));
        }
    }

    private void prepareTestCase(String appId, int offset) {
        SettingsReader.Setting setting = mockSettingsReader.new Setting();

        setting.offset = offset;
        settings.put(appId, setting);

        try {
            when(mockSettingsReader.readJson()).thenReturn(settings);
            settingsStorageManagerImpl.init(mockSettingsStorage, mockSettingsReader);
        } catch (IOException | JSONException e) {
            Log.v(LOG_TAG,
                    SUB_LOG_TAG + "Error reading/parsing jsonfile [" + e.getClass() + "," + e.getMessage() + "]");
        }
    }

    private void prepareTestCase(String appId, ArrayList<String> appSettings, int offset) {
        SettingsReader.Setting setting = mockSettingsReader.new Setting();
        setting.offset = offset;
        setting.cloudPackage.settings = appSettings;
        settings.put(appId, setting);

        try {
            when(mockSettingsReader.readJson()).thenReturn(settings);
            settingsStorageManagerImpl.init(mockSettingsStorage, mockSettingsReader);
        } catch (IOException | JSONException e) {
            Log.v(LOG_TAG,
                    SUB_LOG_TAG + "Error reading/parsing jsonfile [" + e.getClass() + "," + e.getMessage() + "]");
        }
    }

    private int expectedKeyWithOffset(int key, int offset) {
        return offset << 16 | key;
    }

    private boolean compareSettingMaps(HashMap<String, SettingsReader.Setting> expected,
            HashMap<String, SettingsReader.Setting> actual) {
        if (!expected.keySet().equals(actual.keySet()))
            return false;

        Set<Map.Entry<String, SettingsReader.Setting>> expectedSet = expected.entrySet();

        for (Map.Entry<String, SettingsReader.Setting> expectedEntry : expectedSet) {
            SettingsReader.Setting actualSetting = actual.get(expectedEntry.getKey());

            if (actualSetting.offset != expectedEntry.getValue().offset)
                return false;

            if (!actualSetting.cloudPackage.name.equals(expectedEntry.getValue().cloudPackage.name))
                return false;

            for (String setting : expectedEntry.getValue().cloudPackage.settings) {
                if (!actualSetting.cloudPackage.settings.contains(setting)) {
                    return false;
                }
            }
        }

        return true;
    }
}