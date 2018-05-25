/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdatesample;

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

import org.junit.After;
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
import java.util.List;

@MediumTest
@RunWith(AndroidJUnit4.class)
public class softwareupdatesampleTest {

        private static final String softwareupdatesample_PACKAGE = "com.volvocars.softwareupdatesample";
        private static final int LAUNCH_TIMEOUT = 5000;

        private UiDevice mDevice;

        private static final String CLASS_BUTTON = "android.widget.Button";
        private static final String CLASS_TEXT_VIEW = "android.widget.TextView";
        private static final String CLASS_RECYCLER_VIEW = "android.support.v7.widget.RecyclerView";
        private static final String CLASS_CHECKBOX = "android.widget.CheckBox";

        @Before
        public void prepare() {
                mDevice = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation());
                assertThat(mDevice, notNullValue());
                mDevice.pressHome();
        }

        @Test
        public void happyGetSoftwareAssignmentsList() {
                Context context = InstrumentationRegistry.getContext();
                final Intent intent = context.getPackageManager().getLaunchIntentForPackage(softwareupdatesample_PACKAGE);

                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
                context.startActivity(intent);

                mDevice.wait(Until.hasObject(By.pkg(softwareupdatesample_PACKAGE).depth(0)), LAUNCH_TIMEOUT);

                // Find toolbar
                BySelector toolbar = By.clazz("android.view.ViewGroup")
                                .res("com.volvocars.softwareupdatesample:id/toolbar").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(toolbar), LAUNCH_TIMEOUT));

                // Reset settings to default value
                /*****************************************************************************************************/
                // Find settings icon in toolbar
                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/settings");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on settings icon to display menu
                mDevice.findObject(textSelector).click();

                // Find root layout of new activity (SettingsActivity) to verify that the
                // activity has started and is in foreground
                BySelector frame = By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdatesample:id/settingsRootLayout").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(frame), LAUNCH_TIMEOUT));

                UiObject2 autoDownloadSwitch = mDevice.findObject(By.clazz("android.widget.Switch")
                                .res("com.volvocars.softwareupdatesample:id/autoDownload"));
                if (autoDownloadSwitch.isChecked()) {
                        autoDownloadSwitch.click();
                }

                UiObject2 enableOtaSwitch = mDevice.findObject(
                                By.clazz("android.widget.Switch").res("com.volvocars.softwareupdatesample:id/enableOta"));
                if (!enableOtaSwitch.isChecked()) {
                        enableOtaSwitch.click();
                }

                assertTrue(enableOtaSwitch.isChecked());
                assertFalse(autoDownloadSwitch.isChecked());

                // Find back button in toolbar
                UiObject2 toolbarObj = mDevice.findObject(
                                By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdatesample:id/toolbar"));
                List<UiObject2> children = toolbarObj.getChildren();
                assertTrue(children.get(0).getClassName().equals("android.widget.ImageButton"));

                // Click on back button
                UiObject2 home = children.get(0);

                home.click();

                /*****************************************************************************************************/

                // Find cloud icon in toolbar
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/simSignals");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on cloud icon to display menu
                mDevice.findObject(textSelector).click();

                // Find menu item "Get available assignments"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text("Get available updates");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item "Get available updates"
                mDevice.findObject(textSelector).click();

                // Find assignments view (to verify that main activity is in foreground)
                BySelector recycleSelector = By.clazz(CLASS_RECYCLER_VIEW)
                                .res("com.volvocars.softwareupdatesample:id/recycler_view");
                assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

                // Find assignment with name "Spotify"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/name").text("Spotify");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Find assignment with name "Security patch for IHU"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/name")
                                .text("Security patch for IHU");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Find assignment with name "Spotify"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/name")
                                .text("Adaptive cruise control");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));
        }

        @Test
        public void happyCommissionAssignment() {
                Context context = InstrumentationRegistry.getContext();
                final Intent intent = context.getPackageManager().getLaunchIntentForPackage(softwareupdatesample_PACKAGE);

                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
                context.startActivity(intent);

                mDevice.wait(Until.hasObject(By.pkg(softwareupdatesample_PACKAGE).depth(0)), LAUNCH_TIMEOUT);

                // Find toolbar
                BySelector toolbar = By.clazz("android.view.ViewGroup")
                                .res("com.volvocars.softwareupdatesample:id/toolbar").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(toolbar), LAUNCH_TIMEOUT));

                // Reset settings to default value
                /*****************************************************************************************************/
                // Find settings icon in toolbar
                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/settings");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on settings icon to display menu
                mDevice.findObject(textSelector).click();

                // Find root layout of new activity (SettingsActivity) to verify that the
                // activity has started and is in foreground
                BySelector frame = By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdatesample:id/settingsRootLayout").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(frame), LAUNCH_TIMEOUT));

                UiObject2 autoDownloadSwitch = mDevice.findObject(By.clazz("android.widget.Switch")
                                .res("com.volvocars.softwareupdatesample:id/autoDownload"));
                if (autoDownloadSwitch.isChecked()) {
                        autoDownloadSwitch.click();
                }

                UiObject2 enableOtaSwitch = mDevice.findObject(
                                By.clazz("android.widget.Switch").res("com.volvocars.softwareupdatesample:id/enableOta"));
                if (!enableOtaSwitch.isChecked()) {
                        enableOtaSwitch.click();
                }

                assertTrue(enableOtaSwitch.isChecked());
                assertFalse(autoDownloadSwitch.isChecked());

                // Find back button in toolbar
                UiObject2 toolbarObj = mDevice.findObject(
                                By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdatesample:id/toolbar"));
                List<UiObject2> children = toolbarObj.getChildren();
                assertTrue(children.get(0).getClassName().equals("android.widget.ImageButton"));

                // Click on back button
                UiObject2 home = children.get(0);

                home.click();

                /*****************************************************************************************************/

                // Find cloud icon in toolbar
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/simSignals");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on cloud icon to display menu
                mDevice.findObject(textSelector).click();

                // Find menu item "Get available assignments"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text("Get available updates");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item "Get available assignments"
                mDevice.findObject(textSelector).click();

                // Find assignments view (to verify that main activity is in foreground)
                BySelector recycleSelector = By.clazz(CLASS_RECYCLER_VIEW)
                                .res("com.volvocars.softwareupdatesample:id/recycler_view");
                assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

                // Find assignment with name "Spotify"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/name").text("Spotify");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Find corresponding card view to the assignment with name "Spotify"
                UiObject2 card = mDevice.findObject(By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdatesample:id/assignmentCV").hasDescendant(textSelector));
                // Find the overflow icon in the card
                UiObject2 button = card.findObject(By.clazz("android.widget.ImageView")
                                .res("com.volvocars.softwareupdatesample:id/overflow"));
                // Click the overflow icon to display menu
                button.click();

                // Find menu item "Commission assignment"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text("Commission assignment");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item
                mDevice.findObject(textSelector).click();

                // Find toolbar
                toolbar = By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdatesample:id/toolbar")
                                .enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(toolbar), LAUNCH_TIMEOUT));

                // Find cloud icon in toolbar
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/simSignals");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on cloud icon to display menu
                mDevice.findObject(textSelector).click();

                // Find menu item "Get available assignments"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text("Get available updates");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item "Get available assignments"
                mDevice.findObject(textSelector).click();

                // Find assignment with name "Spotify"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/name").text("Spotify");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Verify that "Spotify" assignment is in expected state "COMMISSIONED"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/state")
                                .text("COMMISSIONED");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));
        }

        @Test
        public void happyAutoCommissionAssignment() {
                Context context = InstrumentationRegistry.getContext();
                final Intent intent = context.getPackageManager().getLaunchIntentForPackage(softwareupdatesample_PACKAGE);

                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
                context.startActivity(intent);

                mDevice.wait(Until.hasObject(By.pkg(softwareupdatesample_PACKAGE).depth(0)), LAUNCH_TIMEOUT);

                // Find toolbar
                BySelector toolbar = By.clazz("android.view.ViewGroup")
                                .res("com.volvocars.softwareupdatesample:id/toolbar").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(toolbar), LAUNCH_TIMEOUT));

                // Reset settings to default value
                /*****************************************************************************************************/
                // Find settings icon in toolbar
                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/settings");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on settings icon to display menu
                mDevice.findObject(textSelector).click();

                // Find root layout of new activity (SettingsActivity) to verify that the
                // activity has started and is in foreground
                BySelector frame = By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdatesample:id/settingsRootLayout").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(frame), LAUNCH_TIMEOUT));

                UiObject2 autoDownloadSwitch = mDevice.findObject(By.clazz("android.widget.Switch")
                                .res("com.volvocars.softwareupdatesample:id/autoDownload"));
                if (autoDownloadSwitch.isChecked()) {
                        autoDownloadSwitch.click();
                }

                UiObject2 enableOtaSwitch = mDevice.findObject(
                                By.clazz("android.widget.Switch").res("com.volvocars.softwareupdatesample:id/enableOta"));
                if (!enableOtaSwitch.isChecked()) {
                        enableOtaSwitch.click();
                }

                assertTrue(enableOtaSwitch.isChecked());
                assertFalse(autoDownloadSwitch.isChecked());

                // Find back button in toolbar
                UiObject2 toolbarObj = mDevice.findObject(
                                By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdatesample:id/toolbar"));
                List<UiObject2> children = toolbarObj.getChildren();
                assertTrue(children.get(0).getClassName().equals("android.widget.ImageButton"));

                // Click on back button
                UiObject2 home = children.get(0);

                home.click();

                /*****************************************************************************************************/

                // Find cloud icon in toolbar
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/simSignals");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on cloud icon to display menu
                mDevice.findObject(textSelector).click();

                // Find menu item "Get available assignments"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text("Get available updates");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item "Get available assignments"
                mDevice.findObject(textSelector).click();

                // Find assignments view (to verify that main activity is in foreground)
                BySelector recycleSelector = By.clazz(CLASS_RECYCLER_VIEW)
                                .res("com.volvocars.softwareupdatesample:id/recycler_view");
                assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

                // Find assignment with name "Security patch for IHU"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/name")
                                .text("Security patch for IHU");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Verify that "Security patch for IHU" assignment is in expected state
                // "COMMISSIONABLE"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/state")
                                .text("COMMISSIONABLE");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Find toolbar
                toolbar = By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdatesample:id/toolbar")
                                .enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(toolbar), LAUNCH_TIMEOUT));

                // Find settings icon in toolbar
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/settings");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on settings icon to display menu
                mDevice.findObject(textSelector).click();

                // Find root layout of new activity (SettingsActivity) to verify that the
                // activity has started and is in foreground
                frame = By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdatesample:id/settingsRootLayout").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(frame), LAUNCH_TIMEOUT));

                autoDownloadSwitch = mDevice.findObject(By.clazz("android.widget.Switch")
                                .res("com.volvocars.softwareupdatesample:id/autoDownload").checked(false));
                autoDownloadSwitch.click();

                // Find back button in toolbar
                toolbarObj = mDevice.findObject(
                                By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdatesample:id/toolbar"));
                children = toolbarObj.getChildren();
                assertTrue(children.get(0).getClassName().equals("android.widget.ImageButton"));

                // Click on back button
                home = children.get(0);

                home.click();

                // Find toolbar
                toolbar = By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdatesample:id/toolbar")
                                .enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(toolbar), LAUNCH_TIMEOUT));

                // Find cloud icon in toolbar
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/simSignals");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on cloud icon to display menu
                mDevice.findObject(textSelector).click();

                // Find menu item "Get available assignments"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text("Get available updates");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item "Get available assignments"
                mDevice.findObject(textSelector).click();

                // Find assignments view (to verify that main activity is in foreground)
                recycleSelector = By.clazz(CLASS_RECYCLER_VIEW).res("com.volvocars.softwareupdatesample:id/recycler_view");
                assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

                // Find assignments view (to verify that main activity is in foreground)
                recycleSelector = By.clazz(CLASS_RECYCLER_VIEW).res("com.volvocars.softwareupdatesample:id/recycler_view");
                assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

                // Find assignment with name "Security patch for IHU"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/name")
                                .text("Security patch for IHU");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Verify that "Security patch for IHU" assignment is in expected state
                // "COMMISSIONED"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/state")
                                .text("COMMISSIONED");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

        }

        @Test
        public void happyGetDownloads() {
                Context context = InstrumentationRegistry.getContext();
                final Intent intent = context.getPackageManager().getLaunchIntentForPackage(softwareupdatesample_PACKAGE);

                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
                context.startActivity(intent);

                mDevice.wait(Until.hasObject(By.pkg(softwareupdatesample_PACKAGE).depth(0)), LAUNCH_TIMEOUT);

                // Find toolbar
                BySelector toolbar = By.clazz("android.view.ViewGroup")
                                .res("com.volvocars.softwareupdatesample:id/toolbar").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(toolbar), LAUNCH_TIMEOUT));

                // Reset settings to default value
                /*****************************************************************************************************/
                // Find settings icon in toolbar
                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/settings");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on settings icon to display menu
                mDevice.findObject(textSelector).click();

                // Find root layout of new activity (SettingsActivity) to verify that the
                // activity has started and is in foreground
                BySelector frame = By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdatesample:id/settingsRootLayout").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(frame), LAUNCH_TIMEOUT));

                UiObject2 autoDownloadSwitch = mDevice.findObject(By.clazz("android.widget.Switch")
                                .res("com.volvocars.softwareupdatesample:id/autoDownload"));
                if (autoDownloadSwitch.isChecked()) {
                        autoDownloadSwitch.click();
                }

                UiObject2 enableOtaSwitch = mDevice.findObject(
                                By.clazz("android.widget.Switch").res("com.volvocars.softwareupdatesample:id/enableOta"));
                if (!enableOtaSwitch.isChecked()) {
                        enableOtaSwitch.click();
                }

                assertTrue(enableOtaSwitch.isChecked());
                assertFalse(autoDownloadSwitch.isChecked());

                // Find back button in toolbar
                UiObject2 toolbarObj = mDevice.findObject(
                                By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdatesample:id/toolbar"));
                List<UiObject2> children = toolbarObj.getChildren();
                assertTrue(children.get(0).getClassName().equals("android.widget.ImageButton"));

                // Click on back button
                UiObject2 home = children.get(0);

                home.click();

                /*****************************************************************************************************/

                // Find cloud icon in toolbar
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/simSignals");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on cloud icon to display menu
                mDevice.findObject(textSelector).click();

                // Find menu item "Get available assignments"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text("Get available updates");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item "Get available assignments"
                mDevice.findObject(textSelector).click();

                BySelector recycleSelector = By.clazz(CLASS_RECYCLER_VIEW)
                                .res("com.volvocars.softwareupdatesample:id/recycler_view");
                assertTrue(mDevice.wait(Until.hasObject(recycleSelector), LAUNCH_TIMEOUT));

                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/name")
                                .text("Security patch for IHU");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdatesample:id/state")
                                .text("DOWNLOADED");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));
        }
}