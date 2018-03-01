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


@SmallTest
@RunWith(AndroidJUnit4.class)
public class IlluminationControlTestDimming extends ActivityTestRule<IlluminationControl> {
    @Rule
    public ActivityTestRule<IlluminationControl> mActivityRule = new ActivityTestRule<>(IlluminationControl.class);
    private String TAG = "IlluminationControlTestDimming";
    private ArrayList<Integer> mAcceptedBrightnessValues;
    private CountDownLatch mLatch = null;
    private static final long TIMEOUT_S = 15;
    private static final long TIMEOUT_BRIGHTNESSFULL = 5;
    private Integer mExpectedCallCount = 15;
    private Integer mDimmingCallCount = 0;
    private boolean ignoredFirstCallback = false;

    public IlluminationControlTestDimming() {
        super(IlluminationControl.class);
        mAcceptedBrightnessValues = new ArrayList<>();
        mAcceptedBrightnessValues.add(0);
        mAcceptedBrightnessValues.add(7);
        mAcceptedBrightnessValues.add(13);
        mAcceptedBrightnessValues.add(20);
        mAcceptedBrightnessValues.add(27);
        mAcceptedBrightnessValues.add(33);
        mAcceptedBrightnessValues.add(40);
        mAcceptedBrightnessValues.add(48);
        mAcceptedBrightnessValues.add(54);
        mAcceptedBrightnessValues.add(61);
        mAcceptedBrightnessValues.add(68);
        mAcceptedBrightnessValues.add(74);
        mAcceptedBrightnessValues.add(81);
        mAcceptedBrightnessValues.add(88);
        mAcceptedBrightnessValues.add(94);
        mAcceptedBrightnessValues.add(101);
        mExpectedCallCount = mAcceptedBrightnessValues.size();
    }
    public class OnBrightnessChangeImpl implements IlluminationControl.OnBrightnessChange {
        @Override
        public void onChange() {
            if(!ignoredFirstCallback) {
                ignoredFirstCallback = true;
                return;
            }
            int brightness = mActivityRule.getActivity().getBrightness();
            Log.d(TAG,"" +brightness);
            if(mDimmingCallCount < mExpectedCallCount) {
                Assert.assertTrue(mAcceptedBrightnessValues.contains(brightness));
                mDimmingCallCount++;
            }
            else{
                Log.d(TAG,"release latch OnBrightnessChangeImpl");
                mLatch.countDown();
            }
        }
    }
    public class OnBrightnessChangeImplTestFull implements IlluminationControl.OnBrightnessChange {
        @Override
        public void onChange() {
            if(!ignoredFirstCallback){
                ignoredFirstCallback = true;
                int brightness = mActivityRule.getActivity().getBrightness();
                assertEquals(brightness,255);
                return;
            }
            Log.d(TAG,"Release latch OnBrightnessChangeImplTestFull");
            //Shouldn't be called..
            mLatch.countDown();

        }
    }
    @Before
    public void init() {
        Log.d(TAG,"init ");
        mLatch = new CountDownLatch(1);
        ignoredFirstCallback = false;
    }
    @After
    public void tearDownTest() {
        Log.d(TAG,"tearDownTest ");

        try {
            mActivityRule.getActivity().unRegisterChangeCallback();
            runCommand("killall InjectDEForBrightness &");
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
     * TwliBriSts = 0 (Night)
     *
     * Arrange : TwliBriSts = 0 (Night) IntrBriSts 0-15
     * Act: Change brightnesslevel via IntrBriSts
     * Assert : BrightnessLevel has changed, Should be any of values in mAcceptedBrightnessValues
     */
    @Test
    public void TestBrightnessIsSettable() throws Exception {
        runCommand("/data/local/tmp/InjectDEForBrightness illtest night &");
        OnBrightnessChangeImpl impl = new OnBrightnessChangeImpl();
        mActivityRule.getActivity().registerChangeCallback(impl);
        Log.d(TAG,"Lets wait ");
        assertTrue(mLatch.await(TIMEOUT_S, TimeUnit.SECONDS));
        mActivityRule.getActivity().unRegisterChangeCallback();
        Log.d(TAG,"TestBrightnessIsSettable Test Done");
        runCommand("/data/local/tmp/InjectDEForBrightness illtest day noloop &"); //just inject day signal
        Thread.sleep(500); //to be sure that its injected
    }
    /**
     * Test verifies that the brightness is NOT settable
     * via FlexRay signal IntrBriSts = (rheostat wheel)
     * When TwliBriSts = 1 (Day)
     * Then maximum brightness should be set instead.
     *
     * Arrange : TwliBriSts = 1 (Day) IntrBriSts 0-15
     * Act: Change brightness level via IntrBriSts
     * Assert : Brightness level has not changed, Should be 255.
     */
    @Test
    public void TestBrightnessFull() throws Exception {
        runCommand("/data/local/tmp/InjectDEForBrightness illtest day &");
        OnBrightnessChangeImplTestFull impl = new OnBrightnessChangeImplTestFull();
        mActivityRule.getActivity().registerChangeCallback(impl);
        assertFalse(mLatch.await(TIMEOUT_BRIGHTNESSFULL, TimeUnit.SECONDS)); //shouldn't be release before timeout.
        assertEquals(mActivityRule.getActivity().getBrightness(), 255);
    }
}