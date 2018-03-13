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
public class CsdManagerTests {
    private String TAG = "CsdManagerTests";

    public CsdManagerTests() {}

    @Before
    public void init() {
        Log.d(TAG,"init ");
    }
    @After
    public void tearDownTest() {
        Log.d(TAG,"tearDownTest ");
        runCommand("killall InjectDEForBrightness &",false);
    }
    /**
     * Helper function for running command on target
     * */
    private String runCommand(String command, Boolean getOutput){
        try {
            if(getOutput){
                java.util.Scanner s = new java.util.Scanner(Runtime.getRuntime().exec(command).getInputStream()).useDelimiter("\\A");
                return s.hasNext() ? s.next() : "";
            }
            Runtime.getRuntime().exec(command);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        return "";
    }
    /**
     * Test verifies that wakelock is set When UsageMode is UsgModDrvg:
     *
     * Arrange : Set UsageMode to UsgModDrv.
     * Act:  Check current wakelocks
     * Assert : That wakelock VehicleIgnitionOn is set when running command dumpsys power
     */
    @Test
    public void TestWakeLockIsSetWhenDriving() {
        try {
            runCommand("/data/local/tmp/InjectDEForBrightness csdmantest UsgModCnvinc &", false);
            Log.d(TAG, "After UsgModCnvinc lets wait 1 sec");
            Thread.sleep(1000);
            runCommand("/data/local/tmp/InjectDEForBrightness csdmantest UsgModDrvg &", false);
            Log.d(TAG, "After UsgModDrvg lets wait 1 sec");
            Thread.sleep(1000);
            String wakelocklog = runCommand("dumpsys power", true);
            Log.d(TAG, "After dumpsys power");
            String[] splittedString = wakelocklog.split("\n");
            Boolean haveFullWakeLock = false;
            Boolean haveVehicleIgnitionOnString = false;
            for (String line : splittedString) {
                if (line.contains("VehicleIgnitionOn")) {
                    // if dumpsys string contains VehicleIgnitionOn then it
                    // should also have the string FULL_WAKE_LOCK;
                    haveVehicleIgnitionOnString = true;
                    if (line.contains("FULL_WAKE_LOCK")) {
                        haveFullWakeLock = true;
                    }
                }
            }
            assertTrue(haveVehicleIgnitionOnString);
            assertTrue(haveFullWakeLock);
            runCommand("/data/local/tmp/InjectDEForBrightness csdmantest UsgModDrvg &", false); //return normal state
            Thread.sleep(1000);
        }
        catch (InterruptedException ex){
            Log.d(TAG,ex.getMessage());
        }
    }
    /**
     * Test verifies that wakelock is NOT set When UsageMode is != UsgModDrvg:
     *
     * Arrange : Set UsageMode to !UsgModDrv.
     * Act:  Check current wakelocks
     * Assert : That wakelock  VehicleIgnitionOn is NOT set  when running command dumpsys power
     */
    public void TestWakeLockIsNotSetWhenNotDriving() {
        try{
            runCommand("/data/local/tmp/InjectDEForBrightness csdmantest UsgModDrvg &",false);
            Log.d(TAG,"After UsgModDrvg lets wait 1 sec");
            Thread.sleep(1000);
            runCommand("/data/local/tmp/InjectDEForBrightness csdmantest UsgModCnvinc &",false);
            Log.d(TAG,"After UsgModAbdnd lets wait 1 sec");
            Thread.sleep(1000);
            String wakelocklog = runCommand("dumpsys power",true);
            Log.d(TAG,"After dumpsys power");
            String[] splittedString = wakelocklog.split("\n");
            Boolean haveFullWakeLock = false;
            Boolean haveVehicleIgnitionOnString = false;
            for(String line :splittedString){
                if(line.contains("VehicleIgnitionOn")){
                    haveVehicleIgnitionOnString = true;
                    if(line.contains("FULL_WAKE_LOCK")){
                        haveFullWakeLock = true;
                    }
                }
            }
            assertFalse(haveVehicleIgnitionOnString);
            assertFalse(haveFullWakeLock);
            runCommand("/data/local/tmp/InjectDEForBrightness csdmantest UsgModDrvg &",false); //return normal state
            Thread.sleep(1000);
        }
        catch (InterruptedException ex){
            Log.d(TAG,ex.getMessage());
        }
    }
}