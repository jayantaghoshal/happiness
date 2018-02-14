/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;


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

import 	java.lang.Thread;

@MediumTest
@RunWith(AndroidJUnit4.class)
public class SoftwareUpdateAppTest {

    private static final String SOFTWAREUPDATEAPP_PACKAGE = "com.volvocars.softwareupdateapp";
    private static final int LAUNCH_TIMEOUT = 5000;

    private UiDevice mDevice;


    private static final String CLASS_BUTTON = "android.widget.Button";
    private static final String CLASS_TEXT_VIEW = "android.widget.TextView";
    private static final String CLASS_FLOATING_ACTION_BUTTON = "android.widget.ImageButton";
    private static final String CLASS_RECYCLER_VIEW = "android.support.v7.widget.RecyclerView";


    @Before
    public void prepare() {
        mDevice = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation());
        assertThat(mDevice, notNullValue());
        mDevice.pressHome();
    }

    @Test
    public void happyGetSoftwareAssignmentsList() {
        Context context = InstrumentationRegistry.getContext();
        final Intent intent = context.getPackageManager().getLaunchIntentForPackage(SOFTWAREUPDATEAPP_PACKAGE);

        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
        context.startActivity(intent);

        mDevice.wait(Until.hasObject(By.pkg(SOFTWAREUPDATEAPP_PACKAGE).depth(0)), LAUNCH_TIMEOUT);

        BySelector actionFab = By.clazz(CLASS_FLOATING_ACTION_BUTTON).res("com.volvocars.softwareupdateapp:id/actionFab").enabled(true);
        assertTrue(mDevice.wait(Until.hasObject(actionFab), LAUNCH_TIMEOUT));

        mDevice.findObject(actionFab).click();

        BySelector getAvailableFab = By.clazz(CLASS_FLOATING_ACTION_BUTTON).res("com.volvocars.softwareupdateapp:id/getAvailableFab").enabled(true);
        assertTrue(mDevice.wait(Until.hasObject(getAvailableFab), LAUNCH_TIMEOUT));

        mDevice.findObject(getAvailableFab).click();

        BySelector recycleSelector = By.clazz(CLASS_RECYCLER_VIEW).res("com.volvocars.softwareupdateapp:id/recycler_view");
        assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

        BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/name").text("Spotify");
        assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

        textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/name").text("Security patch for IHU");
        assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));
    }

    @Test
    public void happyCommissionAssignment() {
        Context context = InstrumentationRegistry.getContext();
        final Intent intent = context.getPackageManager().getLaunchIntentForPackage(SOFTWAREUPDATEAPP_PACKAGE);

        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
        context.startActivity(intent);

        mDevice.wait(Until.hasObject(By.pkg(SOFTWAREUPDATEAPP_PACKAGE).depth(0)), LAUNCH_TIMEOUT);

        BySelector actionFab = By.clazz(CLASS_FLOATING_ACTION_BUTTON).res("com.volvocars.softwareupdateapp:id/actionFab").enabled(true);
        assertTrue(mDevice.wait(Until.hasObject(actionFab), LAUNCH_TIMEOUT));

        mDevice.findObject(actionFab).click();

        BySelector getAvailableFab = By.clazz(CLASS_FLOATING_ACTION_BUTTON).res("com.volvocars.softwareupdateapp:id/getAvailableFab").enabled(true);
        assertTrue(mDevice.wait(Until.hasObject(getAvailableFab), LAUNCH_TIMEOUT));

        mDevice.findObject(getAvailableFab).click();

        BySelector recycleSelector = By.clazz(CLASS_RECYCLER_VIEW).res("com.volvocars.softwareupdateapp:id/recycler_view");
        assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

        BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/name").text("Spotify");
        assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));


        UiObject2 card = mDevice.findObject(By.clazz("android.widget.FrameLayout").res("com.volvocars.softwareupdateapp:id/assignmentCV").hasDescendant(textSelector));
        UiObject2 button = card.findObject(By.clazz("android.widget.ImageView").res("com.volvocars.softwareupdateapp:id/overflow"));

        button.click();

        textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text("Commission assignment");
        assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

        mDevice.findObject(textSelector).click();

        assertTrue(mDevice.wait(Until.hasObject(getAvailableFab), LAUNCH_TIMEOUT));

        mDevice.findObject(getAvailableFab).click();

        recycleSelector = By.clazz(CLASS_RECYCLER_VIEW).res("com.volvocars.softwareupdateapp:id/recycler_view");
        assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

        textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/name").text("Spotify");
        assertFalse(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));
    }

    @Test
    public void happyGetPendingInstallations() {
        Context context = InstrumentationRegistry.getContext();
        final Intent intent = context.getPackageManager().getLaunchIntentForPackage(SOFTWAREUPDATEAPP_PACKAGE);

        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
        context.startActivity(intent);

        mDevice.wait(Until.hasObject(By.pkg(SOFTWAREUPDATEAPP_PACKAGE).depth(0)), LAUNCH_TIMEOUT);

        BySelector actionFab = By.clazz(CLASS_FLOATING_ACTION_BUTTON).res("com.volvocars.softwareupdateapp:id/actionFab").enabled(true);
        assertTrue(mDevice.wait(Until.hasObject(actionFab), LAUNCH_TIMEOUT));

        mDevice.findObject(actionFab).click();

        BySelector getAvailableFab = By.clazz(CLASS_FLOATING_ACTION_BUTTON).res("com.volvocars.softwareupdateapp:id/getAvailableFab").enabled(true);
        assertTrue(mDevice.wait(Until.hasObject(getAvailableFab), LAUNCH_TIMEOUT));

        mDevice.findObject(getAvailableFab).click();

        BySelector getCommissionedFab = By.clazz(CLASS_FLOATING_ACTION_BUTTON).res("com.volvocars.softwareupdateapp:id/getCommissionedFab").enabled(true);
        assertTrue(mDevice.wait(Until.hasObject(getCommissionedFab), LAUNCH_TIMEOUT));
        mDevice.findObject(getCommissionedFab).click();

        BySelector recycleSelector = By.clazz(CLASS_RECYCLER_VIEW).res("com.volvocars.softwareupdateapp:id/recycler_view");
        assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

        BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/name").text("Spotify");
        assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

        textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/state").text("DOWNLOAD_PENDING");
        assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));
    }

}