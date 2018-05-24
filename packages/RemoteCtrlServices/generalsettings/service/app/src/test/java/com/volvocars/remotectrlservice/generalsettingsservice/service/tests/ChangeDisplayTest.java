/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.remotectrlservice.generalsettingsservice.service.tests;

import static org.junit.Assert.fail;

import android.app.Instrumentation;
import android.content.Context;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.test.InstrumentationRegistry;
import android.support.test.filters.SmallTest;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;

import com.volvocars.remotectrlservice.generalsettingsservice.service.DisplayOverlay;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

@SmallTest
@RunWith(AndroidJUnit4.class)
public class ChangeDisplayTest {
    private Context mTargetContext;
    private String TAG = "ChangeDisplayTest";
    private DisplayOverlay mDisplayOverlay = null;
    private int brightnessvalue = -1; // Wrong brighness value

    @Before
    public void setUp() throws Exception {
        // Arrange
        Instrumentation instrumentation = InstrumentationRegistry.getInstrumentation();
        mTargetContext = instrumentation.getTargetContext();
        mDisplayOverlay = new DisplayOverlay(mTargetContext);
    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    @Test
    public void DisplayOffTest() {
        Log.v(TAG, "Start ApPowerChangeTest");
        try {
            // Act
            mDisplayOverlay.dispOff();
            brightnessvalue =
                    android.provider.Settings.System.getInt(mTargetContext.getContentResolver(),
                            android.provider.Settings.System.SCREEN_BRIGHTNESS);
            // Assert
            assert(mDisplayOverlay.mWindowManager.getDefaultDisplay().isValid());
            assert brightnessvalue == 0;
        } catch (Exception e) {
            fail();
            Log.e(TAG, e.toString());
        }
        Log.v(TAG, "End testDisplayoff");
    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    @Test
    public void DisplayOnTest() {
        Log.v(TAG, "Start ApPowerChangeTest");
        try {
            // Act
            mDisplayOverlay.dispOn();
            brightnessvalue =
                    android.provider.Settings.System.getInt(mTargetContext.getContentResolver(),
                            android.provider.Settings.System.SCREEN_BRIGHTNESS);
            // Assert
            assert(!mDisplayOverlay.mWindowManager.getDefaultDisplay().isValid());
        } catch (Exception e) {
            fail();
            Log.e(TAG, e.toString());
        }
        Log.v(TAG, "End testDisplayon");
    }
}