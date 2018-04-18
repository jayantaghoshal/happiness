/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.connectivitymanager.relay;

import android.support.test.filters.MediumTest;
import android.support.test.runner.AndroidJUnit4;
import android.support.test.uiautomator.By;
import android.support.test.uiautomator.BySelector;
import android.support.test.uiautomator.UiDevice;
import android.support.test.uiautomator.UiObject;
import android.support.test.uiautomator.UiObject2;
import android.support.test.uiautomator.UiSelector;
import android.support.test.uiautomator.Until;

import android.support.test.InstrumentationRegistry;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import android.content.Context;
import android.content.Intent;

import static org.hamcrest.Matchers.notNullValue;

import static org.junit.Assert.assertThat;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

import java.lang.Thread;

@MediumTest
@RunWith(AndroidJUnit4.class)
public class ConnectivityManagerRelayTest {
    private static final String CMR_PACKAGE = "com.volvocars.connectivitymanager.relay";
    private static final int LAUNCH_TIMEOUT = 5000;
    private static final int SIGNAL_TIMEOUT = 5000;
    private UiDevice mDevice;

    private static final String CLASS_BUTTON = "android.widget.Button";

    private static final String WIFI_STATION_SWITCH_ID =
            "com.volvocars.connectivitymanager.relay:id/wifiStationSwitch";

    private static final String SWITCH_CLASS = "android.widget.Switch";

    @Before
    public void prepare() {
        mDevice = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation());
        assertThat(mDevice, notNullValue());
        mDevice.pressHome();
    }

    @Test
    public void GetWifiStationModeAtStartup() {
        Context context = InstrumentationRegistry.getContext();
        final Intent intent = context.getPackageManager().getLaunchIntentForPackage(CMR_PACKAGE);

        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
        context.startActivity(intent);

        assertTrue(mDevice.wait(Until.hasObject(By.pkg(CMR_PACKAGE).depth(0)), LAUNCH_TIMEOUT));

        BySelector wifiSwitch =
                By.clazz(SWITCH_CLASS).res(WIFI_STATION_SWITCH_ID).checked(true).clickable(true);
        assertTrue(mDevice.wait(Until.hasObject(wifiSwitch), SIGNAL_TIMEOUT));
    }

    @Test
    public void ChangeWifiStationMode() {
        Context context = InstrumentationRegistry.getContext();
        final Intent intent = context.getPackageManager().getLaunchIntentForPackage(CMR_PACKAGE);

        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
        context.startActivity(intent);

        assertTrue(mDevice.wait(Until.hasObject(By.pkg(CMR_PACKAGE).depth(0)), LAUNCH_TIMEOUT));

        BySelector wifiSwitch =
                By.clazz(SWITCH_CLASS).res(WIFI_STATION_SWITCH_ID).checked(true).clickable(true);
        assertTrue(mDevice.wait(Until.hasObject(wifiSwitch), SIGNAL_TIMEOUT));

        // Startup stuff done

        mDevice.findObject(wifiSwitch).click();

        wifiSwitch =
                By.clazz(SWITCH_CLASS).res(WIFI_STATION_SWITCH_ID).checked(false).clickable(true);
        assertTrue(mDevice.wait(Until.hasObject(wifiSwitch), SIGNAL_TIMEOUT));
    }
}