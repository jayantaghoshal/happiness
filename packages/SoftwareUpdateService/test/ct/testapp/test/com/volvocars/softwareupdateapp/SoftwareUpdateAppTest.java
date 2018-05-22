/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.support.test.filters.MediumTest;
import android.support.test.runner.AndroidJUnit4;
import android.support.test.uiautomator.By;
import android.support.test.uiautomator.BySelector;
import android.support.test.uiautomator.UiDevice;
import android.support.test.uiautomator.UiObjectNotFoundException;
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
import static org.junit.Assert.assertNotNull;

import java.lang.Thread;
import java.util.List;

@MediumTest
@RunWith(AndroidJUnit4.class)
public class SoftwareUpdateAppTest {

        private static final String SOFTWAREUPDATEAPP_PACKAGE = "com.volvocars.softwareupdateapp";
        private static final int LAUNCH_TIMEOUT = 5000;

        private UiDevice mDevice;

        private static final String CLASS_BUTTON = "android.widget.Button";
        private static final String CLASS_TEXT_VIEW = "android.widget.TextView";
        private static final String CLASS_RECYCLER_VIEW = "android.support.v7.widget.RecyclerView";
        private static final String CLASS_CHECKBOX = "android.widget.CheckBox";

        // Id:s of settings
        private static final String AUTO_DOWNLOAD_ID = "com.volvocars.softwareupdateapp:id/autoDownload";
        private static final String ENABLE_OTA_ID = "com.volvocars.softwareupdateapp:id/enableOta";

        // Names of assignments
        private static final String SPOTIFY = "Spotify";
        private static final String SECURITY_PATCH_FOR_IHU = "Security patch for IHU";
        private static final String ADAPTIVE_CRUISE_CONTROL = "Adaptive cruise control";

        // Menu items
        private static final String GET_AVAILABLE_UPDATES = "Get available updates";
        private static final String GET_AVAILABLE_ACCESSORIES = "Get available accessories";
        private static final String COMMISSION_ASSIGNMENT = "Commission assignment";

        // States
        private static final String COMMISSIONABLE = "COMMISSIONABLE";
        private static final String COMMISSIONED = "COMMISSIONED";
        private static final String DOWNLOADED = "DOWNLOADED";
        private static final String INSTALL_PENDING = "INSTALL PENDING";

        // Actions on installation popup
        private static final String INSTALL = "INSTALL";
        private static final String CANCEL = "CANCEL";
        private static final String POSTPONE = "POSTPONE";

        @Before
        public void prepare() {
                mDevice = UiDevice.getInstance(InstrumentationRegistry.getInstrumentation());
                assertThat(mDevice, notNullValue());
                mDevice.pressHome();

                Context context = InstrumentationRegistry.getContext();
                final Intent intent = context.getPackageManager().getLaunchIntentForPackage(SOFTWAREUPDATEAPP_PACKAGE);

                intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
                context.startActivity(intent);

                mDevice.wait(Until.hasObject(By.pkg(SOFTWAREUPDATEAPP_PACKAGE).depth(0)), LAUNCH_TIMEOUT);

                setSettingsToDefault();
        }

        /*
         * Reset settings to default value
         */
        private void setSettingsToDefault() {
                // Find settings icon in toolbar
                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/settings");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on settings icon to display menu
                mDevice.findObject(textSelector).click();

                // Find root layout of new activity (SettingsActivity) to verify that the
                // activity has started and is in foreground
                BySelector frame = By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdateapp:id/settingsRootLayout").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(frame), LAUNCH_TIMEOUT));

                UiObject2 autoDownloadSwitch = mDevice
                                .findObject(By.clazz("android.widget.Switch").res(AUTO_DOWNLOAD_ID));
                if (autoDownloadSwitch.isChecked()) {
                        autoDownloadSwitch.click();
                }

                UiObject2 enableOtaSwitch = mDevice.findObject(By.clazz("android.widget.Switch").res(ENABLE_OTA_ID));
                if (!enableOtaSwitch.isChecked()) {
                        enableOtaSwitch.click();
                }

                assertTrue(enableOtaSwitch.isChecked());
                assertFalse(autoDownloadSwitch.isChecked());

                // Find back button in toolbar
                UiObject2 toolbarObj = mDevice.findObject(
                                By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdateapp:id/toolbar"));
                List<UiObject2> children = toolbarObj.getChildren();
                assertTrue(children.get(0).getClassName().equals("android.widget.ImageButton"));

                // Click on back button
                UiObject2 home = children.get(0);

                home.click();
        }

        /**
         * Set specified setting
         *
         * @param settingId id of setting to set
         * @param value     value to set
         */
        private void setSetting(String settingId, boolean value) {
                // Find settings icon in toolbar
                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/settings");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on settings icon to display menu
                mDevice.findObject(textSelector).click();

                // Find root layout of new activity (SettingsActivity) to verify that the
                // activity has started and is in foreground
                BySelector frame = By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdateapp:id/settingsRootLayout").enabled(true);
                assertTrue(mDevice.wait(Until.hasObject(frame), LAUNCH_TIMEOUT));

                UiObject2 settingSwitch = mDevice.findObject(By.clazz("android.widget.Switch").res(settingId));
                if (settingSwitch.isChecked() != value) {
                        settingSwitch.click();
                }

                // Find back button in toolbar
                UiObject2 toolbarObj = mDevice.findObject(
                                By.clazz("android.view.ViewGroup").res("com.volvocars.softwareupdateapp:id/toolbar"));
                List<UiObject2> children = toolbarObj.getChildren();
                assertTrue(children.get(0).getClassName().equals("android.widget.ImageButton"));

                // Click on back button
                UiObject2 home = children.get(0);

                home.click();
        }

        /**
         * Find cloud menu item by text and click on it
         *
         * @param text text to identify the menu item
         */
        private void clickOnCloudMenuItem(String text) {
                // Find cloud icon in toolbar
                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW)
                                .res("com.volvocars.softwareupdateapp:id/simSignals");
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on cloud icon to display menu
                mDevice.findObject(textSelector).click();

                // Find menu item "Get available updates"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text(text);
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item
                mDevice.findObject(textSelector).click();
        }

        /**
         * Find assignment menu item by text and click on it
         *
         * @param assignmentName name of assignment
         * @param text           text to identify the menu item
         */
        private void clickOnAssignmentMenuItem(String assignmentName, String text) {

                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/name")
                                .text(assignmentName);
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Find corresponding card view to the assignment
                UiObject2 card = mDevice.findObject(By.clazz("android.widget.FrameLayout")
                                .res("com.volvocars.softwareupdateapp:id/assignmentCV").hasDescendant(textSelector));
                // Find the overflow icon in the card
                UiObject2 button = card.findObject(By.clazz("android.widget.ImageView")
                                .res("com.volvocars.softwareupdateapp:id/overflow"));
                // Click the overflow icon to display menu
                button.click();

                // Find menu item "Commission assignment"
                textSelector = By.clazz(CLASS_TEXT_VIEW).res("android:id/title").text(text);
                assertTrue(mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT));

                // Click on menu item
                mDevice.findObject(textSelector).click();

        }

        /**
         * Verify that assignment exists
         *
         * @param assignmentName name of assignment
         * @return if assignment exists
         */
        private boolean verifyAssignment(String assignmentName) {
                // Find assignment with name "Spotify"
                BySelector textSelector = By.clazz(CLASS_TEXT_VIEW).res("com.volvocars.softwareupdateapp:id/name")
                                .text(assignmentName);
                return mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT);
        }

        /**
         * Verify that assignment exists with expected state
         *
         * @param assignmentName name of assignment
         * @param state          expected state of assignment
         * @return if assignment exists and is in expected state
         */
        private boolean verifyAssignment(String assignmentName, String state) {
                if (verifyAssignment(assignmentName)) {
                        BySelector textSelector = By.clazz(CLASS_TEXT_VIEW)
                                        .res("com.volvocars.softwareupdateapp:id/state").text(state);
                        return mDevice.wait(Until.hasObject(textSelector), LAUNCH_TIMEOUT);
                }
                return false;
        }

        /**
         * Verify that installation popup is shown, with correct assignment, and perform
         * action on it
         *
         * @param assignmentName name of assignment
         * @param action         action to perform
         */
        private void performActionOnInstallPopUp(String assignmentName, String action)
                        throws UiObjectNotFoundException {
                // Find install popup and verify that correct assignment is going to be
                // installed
                UiObject alertDialog = mDevice.findObject(
                                new UiSelector().className("android.widget.TextView").resourceId("android:id/message")
                                                .text(assignmentName + " is ready to be installed. Install now?"));
                assertTrue(alertDialog.exists());

                // Find button
                UiObject button = mDevice.findObject(new UiSelector().resourceId("android:id/button1").text(action));
                assertTrue(button.exists());

                // Click on button
                button.click();

                assertFalse(alertDialog.exists());
        }

        @Test
        public void happyGetSoftwareAssignmentsList() {

                // Call Get available updates/accessories
                clickOnCloudMenuItem(GET_AVAILABLE_UPDATES);
                clickOnCloudMenuItem(GET_AVAILABLE_ACCESSORIES);

                // Verify that all expected assignments are visible
                assertTrue(verifyAssignment(SPOTIFY));
                assertTrue(verifyAssignment(SECURITY_PATCH_FOR_IHU));
                assertTrue(verifyAssignment(ADAPTIVE_CRUISE_CONTROL));
        }

        @Test
        public void happyCommissionAssignment() {
                // Call Get available updates
                clickOnCloudMenuItem(GET_AVAILABLE_UPDATES);

                // Verify that expected assignment is visible
                assertTrue(verifyAssignment(SPOTIFY));

                // Commission assignment
                clickOnAssignmentMenuItem(SPOTIFY, COMMISSION_ASSIGNMENT);

                // Verify that expected assignment is visible and in state "COMMISSIONED"
                assertTrue(verifyAssignment(SPOTIFY, COMMISSIONED));
        }

        @Test
        public void happyAutoCommissionAssignment() {
                // Call Get available updates
                clickOnCloudMenuItem(GET_AVAILABLE_UPDATES);

                // Verify that expected assignment is visible and in state "COMMISSIONABLE"
                assertTrue(verifyAssignment(SECURITY_PATCH_FOR_IHU, COMMISSIONABLE));

                // Set setting "Automatic download" to true
                setSetting(AUTO_DOWNLOAD_ID, true);

                // Call Get available updates
                clickOnCloudMenuItem(GET_AVAILABLE_UPDATES);

                // Verify that expected assignment is visible and in state "COMMISSIONED"
                assertTrue(verifyAssignment(SECURITY_PATCH_FOR_IHU, COMMISSIONED));
        }

        @Test
        public void happyGetDownloads() {
                // Call Get available updates
                clickOnCloudMenuItem(GET_AVAILABLE_UPDATES);

                // Verify that expected assignment is visible and in state "DOWNLOADED"
                assertTrue(verifyAssignment(SECURITY_PATCH_FOR_IHU, DOWNLOADED));
        }

        @Test
        public void happyAssignInstallationUpdate() throws UiObjectNotFoundException {
                // Call Get available updates

                clickOnCloudMenuItem(GET_AVAILABLE_UPDATES);

                // Verify that expected assignment is visible

                verifyAssignment(SPOTIFY);

                // Commission assignment
                clickOnAssignmentMenuItem(SPOTIFY, COMMISSION_ASSIGNMENT);

                // Verify that expected assignment is visible and in state "COMMISSIONED"
                verifyAssignment(SPOTIFY, COMMISSIONED);

                // Call Get available updates
                clickOnCloudMenuItem(GET_AVAILABLE_UPDATES);

                // Verify that installation popup is visible for assignment and click on INSTALL
                // button
                performActionOnInstallPopUp(SPOTIFY, INSTALL);

                // Verify that expected assignment is visible and in state "COMMISSIONED"
                assertTrue(verifyAssignment(SPOTIFY, INSTALL_PENDING));

                // Call Get available updates
                clickOnCloudMenuItem(GET_AVAILABLE_UPDATES);

                // Verify that assignment is removed from list
                assertFalse(verifyAssignment(SPOTIFY));
        }

        @Test
        public void happyAssignInstallationAccessories() throws UiObjectNotFoundException {
                // Call Get available accessories

                clickOnCloudMenuItem(GET_AVAILABLE_ACCESSORIES);
                // Verify that expected assignment is visible

                verifyAssignment(ADAPTIVE_CRUISE_CONTROL);
                // Commission assignment

                clickOnAssignmentMenuItem(ADAPTIVE_CRUISE_CONTROL, COMMISSION_ASSIGNMENT);
                // Verify that expected assignment is visible and in state "COMMISSIONED"

                verifyAssignment(ADAPTIVE_CRUISE_CONTROL, COMMISSIONED);
                // Call Get available accessories

                clickOnCloudMenuItem(GET_AVAILABLE_ACCESSORIES);

                // Verify that installation popup is visible for assignment and click on INSTALL
                // button
                performActionOnInstallPopUp(ADAPTIVE_CRUISE_CONTROL, INSTALL);
                // Verify that expected assignment is visible and in state "COMMISSIONED"

                assertTrue(verifyAssignment(ADAPTIVE_CRUISE_CONTROL, INSTALL_PENDING));
                // Call Get available accessories

                clickOnCloudMenuItem(GET_AVAILABLE_ACCESSORIES);

                // Verify that assignment is removed from list
                assertFalse(verifyAssignment(ADAPTIVE_CRUISE_CONTROL));
        }

}