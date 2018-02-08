/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.localconfigtest;

import android.os.Bundle;
import android.util.Log;
import android.content.Context;
import android.content.Intent;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.Map;
import java.util.HashMap;
import java.io.File;
import java.util.Collections;
import android.content.ComponentName;
import android.system.Os;
import android.system.ErrnoException;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;

import android.volvocars.localconfig.LocalConfig;
import android.volvocars.localconfig.LocalConfigServiceListener;
import android.volvocars.localconfig.LcfgNotConnectedException;

import android.app.Activity;

public class MainActivity extends Activity
{
    private String TAG_LCLCFG_CTS;
    private static final long DEFAULT_WAIT_TIMEOUT_MS = 3000;
    private final Semaphore mConnectionWait = new Semaphore(0);
    private static final String mockedLocalConfigPath = "/data/local/tmp/localconfig.json";
    // Valid Key Integer type
    private static final String LCFINT_IIPM_LMN_NoofNodesRG1 = "IIPS_LM_NofNodesRG1";
    // Valid Key Double type
    private static final String LCFDB_CONN_TMR_WFA = "CONN_increaseTimerValueWFA";
    // Valid Key String type
    private static final String LCF_IIPS_IPADDR_LOCAL = "IIPS_IpAddress_LOCAL";
    // Local Values
    private static final int LCF_INT_IIPS_LM_NofNodesRG1_VAL = 3;
    private static final double LCF_DB_CONN_TMR_WFA_VAL = 1.5;
    private static final String LCF_STR_IIPS_IPADDR_LOCAL = "198.18.34.1";

    private static final double DELTA = 1e-15;


    private void waitForConnection(long timeoutMs) throws InterruptedException {
        mConnectionWait.tryAcquire(timeoutMs, TimeUnit.MILLISECONDS);
    }

    private final LocalConfigServiceListener mServiceConnectionListener =
            new LocalConfigServiceListener () {
                @Override
                public void onServiceConnected(ComponentName componentName) {

                    mConnectionWait.release();
                }

                @Override
                public void onServiceDisconnected(ComponentName componentName) {

                }
        };

    @Override protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        TAG_LCLCFG_CTS = "LocalConfigCTSTest";
    }

    public void LocalConfigConnection()
    {
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        boolean blContout_flag = false;

        mlocalcfgapi.connectlocalConfig();

        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigConnection + | " + e.getMessage());
            blContout_flag = true;
        }

        assertFalse(blContout_flag);

        assertTrue(mlocalcfgapi.isConnectedLocalConfig());

        mlocalcfgapi.disconnectlocalConfig();

        assertFalse(mlocalcfgapi.isConnectedLocalConfig());

    }

    public void LocalConfigDoubleConnection()
    {
        boolean bldoubleconnect_flag = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        assertFalse(mlocalcfgapi.isConnectedLocalConfig());
        mlocalcfgapi.connectlocalConfig();

        try {
            mlocalcfgapi.connectlocalConfig();
            fail("double connect should throw");
        } catch (IllegalStateException e) {
            // expected
            bldoubleconnect_flag = true;
        }
        assertTrue(bldoubleconnect_flag);
        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetInteger()
    {
        boolean blContout_flag = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetInteger + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);

        int lcf = 0;

        try {
            lcf = mlocalcfgapi.getLocalConfigInteger(LCFINT_IIPM_LMN_NoofNodesRG1);
        } catch (LcfgNotConnectedException | RuntimeException e) {

            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetInteger + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);
        assertEquals(LCF_INT_IIPS_LM_NofNodesRG1_VAL,lcf);

        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetDouble()
    {
        boolean blContout_flag = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetDouble + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);

        double lcf = 0.0;

        try {
            lcf = mlocalcfgapi.getLocalConfigDouble(LCFDB_CONN_TMR_WFA);
        } catch (LcfgNotConnectedException | RuntimeException e) {

            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetDouble + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);

        assertEquals(LCF_DB_CONN_TMR_WFA_VAL,lcf,DELTA);

        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetString()
    {
        boolean blContout_flag = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetString + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);
        String lcf = "";

        try {
            lcf = mlocalcfgapi.getLocalConfigString(LCF_IIPS_IPADDR_LOCAL);
        } catch (LcfgNotConnectedException | RuntimeException e) {

            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetString + | " + e.getMessage());
            blContout_flag = true;
        }

        assertFalse(blContout_flag);

        assertEquals(LCF_STR_IIPS_IPADDR_LOCAL,lcf);

        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetWrongString()
    {
        boolean blContout_flag = false;
        boolean isRuntimeException = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongString + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);
        String lcf = "";

        try {
            lcf = mlocalcfgapi.getLocalConfigString("Invalid_String");
        }
        catch (LcfgNotConnectedException e)
        {
             Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongString + | " + e.getMessage());
        }
        catch(RuntimeException e)
        {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongString + | " + e.getMessage());
            isRuntimeException = true;
        }

        assertTrue(isRuntimeException);
        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetWrongInteger()
    {
        boolean blContout_flag = false;
        boolean isRuntimeException = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongInteger +" + e.getMessage());
            blContout_flag = true;
        }
         assertFalse(blContout_flag);
        try {
            int lcf = mlocalcfgapi.getLocalConfigInteger("Invalid_Int");
        } catch (LcfgNotConnectedException e) {
             Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongInteger + | " + e.getMessage());
        }
        catch(RuntimeException e)
        {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongInteger + | " + e.getMessage());
            isRuntimeException = true;
        }
        assertTrue(isRuntimeException);

        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetWrongDouble()
    {
        boolean blContout_flag = false;
        boolean isRuntimeException = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongDouble + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);
        try {
            double lcf = mlocalcfgapi.getLocalConfigDouble("Invalid_Double");
        }
        catch (LcfgNotConnectedException e)
        {
             Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongDouble + | " + e.getMessage());
        }
        catch(RuntimeException e)
        {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetWrongDouble + | " + e.getMessage());
            isRuntimeException = true;
        }

        assertTrue(isRuntimeException);
        mlocalcfgapi.disconnectlocalConfig();
    }
    public void LocalConfigGetBoolean()
    {
        boolean blContout_flag = false;
        boolean isRuntimeException = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetBool + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);
        boolean lcf = false;
        // Note : At the time of this testing LocalConfig doesn't support boolean  key type
        // Assuming this Invaild key exception this test is evaluated as Success
        try {
            lcf = mlocalcfgapi.getLocalConfigBoolean("Boolean_type");
        } catch (LcfgNotConnectedException e) {

            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetBoolean + | " + e.getMessage());
            blContout_flag = true;
        } catch (RuntimeException e) {
             Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetBoolean + | " + e.getMessage());
             isRuntimeException = true;
        }

        assertFalse(blContout_flag);

        assertTrue(isRuntimeException);

        assertEquals(false,lcf);

        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetString_MismatchKey()
    {
        boolean blContout_flag = false;
        boolean isRuntimeException = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetString_MismatchKey + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);
        String lcf = "";
        // Input key of integer type
        try {
            lcf = mlocalcfgapi.getLocalConfigString(LCFINT_IIPM_LMN_NoofNodesRG1);
        }
        catch (LcfgNotConnectedException e)
        {
             Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetString_MismatchKey + | " + e.getMessage());
        }
        catch(RuntimeException e)
        {   // expected
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetString_MismatchKey + | " + e.getMessage());
            isRuntimeException = true;
        }

        assertTrue(isRuntimeException);
        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetDouble_Mismatchtype()
    {
        boolean blContout_flag = false;
        boolean isRuntimeException = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetDouble_Mismatchtype + | " + e.getMessage());
            blContout_flag = true;
        }
        assertFalse(blContout_flag);
        // Input key of String type
        try {
            double lcf = mlocalcfgapi.getLocalConfigDouble(LCF_IIPS_IPADDR_LOCAL);
        }
        catch (LcfgNotConnectedException e)
        {
             Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetDouble_Mismatchtype + | " + e.getMessage());
        }
        catch(RuntimeException e)
        {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetDouble_Mismatchtype + | " + e.getMessage());
            isRuntimeException = true;
        }

        assertTrue(isRuntimeException);
        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetInteger_Mismatchtype()
    {
        boolean blContout_flag = false;
        boolean isRuntimeException = false;
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetInteger_Mismatchtype +" + e.getMessage());
            blContout_flag = true;
        }
         assertFalse(blContout_flag);
        try {
            int lcf = mlocalcfgapi.getLocalConfigInteger(LCFDB_CONN_TMR_WFA);
        } catch (LcfgNotConnectedException e) {
             Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetInteger_Mismatchtype + | " + e.getMessage());
        }
        catch(RuntimeException e)
        {
            Log.v(TAG_LCLCFG_CTS,"+ LocalConfigGetInteger_Mismatchtype + | " + e.getMessage());
            isRuntimeException = true;
        }
        assertTrue(isRuntimeException);

        mlocalcfgapi.disconnectlocalConfig();
    }



}
