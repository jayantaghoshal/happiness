/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.brightnesstest;

import android.support.test.filters.SmallTest;
import android.support.test.rule.ActivityTestRule;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import java.io.FileReader;
import java.io.BufferedReader;
import java.lang.Integer;


@SmallTest
@RunWith(AndroidJUnit4.class)
public class IlluminationControlTestDimming extends ActivityTestRule<IlluminationControl> {
    @Rule
    public ActivityTestRule<IlluminationControl> mActivityRule = new ActivityTestRule<>(IlluminationControl.class);
    private String TAG = "IlluminationControlTestDimming";
    private CountDownLatch mLatch = null;
    private static final long TIMEOUT_S = 5;

    public IlluminationControlTestDimming() {
        super(IlluminationControl.class);
    }

    public class OnBrightnessChangeImpl implements IlluminationControl.OnBrightnessChange {
        @Override
        public synchronized void onChange(int brightness) {
            Log.d(TAG,"OnBrightnessChangeImpl onChange:"+brightness);
            brightnessChanged = true;
            if(brightness==expectedBrightness) {
                Log.d(TAG,"release latch OnBrightnessChangeImpl");
                if (mLatch!=null) mLatch.countDown();
            }
        }

        public synchronized CountDownLatch start(int expectedBr, boolean expectBrightnessCallbackBeforeEvaluating) {
            expectedBrightness = expectedBr;
            brightnessChanged = false;
            mLatch = new CountDownLatch(1);
            if (!expectBrightnessCallbackBeforeEvaluating &&
                mActivityRule.getActivity().getBrightness()==expectedBrightness) {
                   Log.d(TAG,"release latch on start "+expectedBrightness);
                   mLatch.countDown();
            }
            return mLatch;
        }

        public boolean brightnessChanged;
        private int expectedBrightness;
        private CountDownLatch mLatch;
    }

    @Before
    public void init() {
        Log.d(TAG,"init ");
    }
    @After
    public void tearDownTest() {
        Log.d(TAG,"tearDownTest ");

        try {
            mActivityRule.getActivity().unRegisterChangeCallback();
            runCommand("killall InjectDEForBrightness");
        }
        catch (InterruptedException ex){
            Log.d(TAG,ex.getMessage());
        }
    }
    /**
     * Helper function for running command on target
     * */
    private void runCommand(String command){
        try {
            Runtime.getRuntime().exec(command);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
    /**
     * Test verifies that the brightness is
     * settable via FlexRay signal IntrBriSts = (rheostat wheel)
     * AND also that the CSD brightness device is updated accordingly.
     * TwliBriSts = 0 (Night)
     *
     * Arrange : TwliBriSts = 0 (Night) IntrBriSts 0-15
     * Act: Change brightnesslevel via IntrBriSts
     * Assert : BrightnessLevel has changed
     * lets try the rheo values 1,5,15 and expect brightness to be 7,34,101 (device values:28,136,406)
     */
    @Test
    public void TestBrightnessIsSettable() throws Exception {
        Log.d(TAG,"TestBrightnessIsSettable Test Starting");

        runCommand("/data/local/tmp/InjectDEForBrightness night");

        OnBrightnessChangeImpl impl = new OnBrightnessChangeImpl();
        mActivityRule.getActivity().registerChangeCallback(impl);
        CountDownLatch latch;

        Log.d(TAG,"rheo=1, exp 7");
        latch = impl.start(7, false);
        runCommand("/data/local/tmp/InjectDEForBrightness ill 1");
        assertTrue(latch.await(TIMEOUT_S, TimeUnit.SECONDS));
        AssertDeviceBrightness(28);

        Log.d(TAG,"rheo=5, exp 34");
        latch = impl.start(34, true);
        runCommand("/data/local/tmp/InjectDEForBrightness ill 5");
        assertTrue(latch.await(TIMEOUT_S, TimeUnit.SECONDS));
        AssertDeviceBrightness(136);

        Log.d(TAG,"rheo=15, exp 101");
        latch = impl.start(101, true);
        runCommand("/data/local/tmp/InjectDEForBrightness ill 15");
        assertTrue(latch.await(TIMEOUT_S, TimeUnit.SECONDS));
        AssertDeviceBrightness(406);

        runCommand("/data/local/tmp/InjectDEForBrightness day"); //go back to full brightness
        Thread.sleep(1000);
        Log.d(TAG,"TestBrightnessIsSettable Test Done");
    }

    /**
     * Test verifies that the brightness is NOT settable
     * via FlexRay signal IntrBriSts = (rheostat wheel)
     * When TwliBriSts = 1 (Day)
     * Then maximum brightness should in this case always be set.
     * Also the value written to the CSD brightness device is verified.
     *
     * Arrange : TwliBriSts = 1 (Day) IntrBriSts 0-15
     * Act: Change brightness level via IntrBriSts
     * Assert : Brightness level has not changed, Should be 255.
     */
    @Test
    public void TestBrightnessFull() throws Exception {
        Log.d(TAG,"TestBrightnessFull Test Starting");

        runCommand("/data/local/tmp/InjectDEForBrightness night");

        OnBrightnessChangeImpl impl = new OnBrightnessChangeImpl();
        mActivityRule.getActivity().registerChangeCallback(impl);
        CountDownLatch latch;

        Log.d(TAG,"rheo=0, exp 0");
        latch = impl.start(0, false);
        runCommand("/data/local/tmp/InjectDEForBrightness ill 0");
        assertTrue(latch.await(TIMEOUT_S, TimeUnit.SECONDS));

        // Now switch to day and expect 255 in brightness
        Log.d(TAG,"day, rheo=0, exp 255");
        latch = impl.start(255, true);
        runCommand("/data/local/tmp/InjectDEForBrightness day");
        assertTrue(latch.await(TIMEOUT_S, TimeUnit.SECONDS));
        AssertDeviceBrightness(1023);

        // Set another rheo value -> brightness still 255
        // Either nothing has happened at all (brightnessChanged==false)
        // or we received a "new" value that is still 255.
        Log.d(TAG,"day, rheo=15, exp 255");
        latch = impl.start(255, true);
        runCommand("/data/local/tmp/InjectDEForBrightness ill 15");
        assertTrue(latch.await(TIMEOUT_S, TimeUnit.SECONDS) || !impl.brightnessChanged);
        AssertDeviceBrightness(1023);

        Log.d(TAG,"TestBrightnessFull Test Done");
    }

    // The reason this is needed is that when the brightness is set to the Android
    // brightness setting the change is "animated" towards the device so the
    // value isn't updated immediately. It is ramping up/down for a period of time.
    public int GetStableDeviceBrightness(int waitMillisecs) {
        final long endTime = System.currentTimeMillis()+waitMillisecs;
        try {
            Thread.sleep(500);
        } catch(Exception e) {}
        int lastbr = -10; // just an impossible value
        int br = GetBrightnessFromDevice();
        // wait for value to stabilize
        while(lastbr!=br && endTime>System.currentTimeMillis()) {
            lastbr = br;
            try {
                Thread.sleep(200);
            } catch(Exception e) {}
            br = GetBrightnessFromDevice();
        }
        Log.d(TAG,"Device says: "+lastbr);
        return lastbr;
    }

    public int GetBrightnessFromDevice() {
        String line=null;
        try
        {
            BufferedReader reader = new BufferedReader(new FileReader("/sys/class/csd_display/csd_display0/csd/brightness"));
            line = reader.readLine();
            if (line == null) {
                Log.d(TAG,"Failed to read brightness (null)");
                return -1;
            }
            return Integer.parseInt(line);
        } catch (Exception e) {
            Log.d(TAG,"Failed to read brightness: "+line);
            return -1;
        }
    }

    public void AssertDeviceBrightness(int expectedBrightness) {
        // Since we don't have exactly control over how Android maps the setting 0..255
        // to the device values 0..1023 we do allow that it is not exactly what we expect.
        final int currentDeviceBrightness = GetStableDeviceBrightness(5000);
        final int maxdiff=5; // the largest diff in expected device value we allow
        Log.d(TAG,"Asserting "+expectedBrightness+"=="+currentDeviceBrightness+" (+/-"+maxdiff+")");
        assertTrue(currentDeviceBrightness>=0); // <0 means some sort of error
        final int diff = Math.abs(expectedBrightness-currentDeviceBrightness);
        assertTrue(diff<maxdiff);
    }
}