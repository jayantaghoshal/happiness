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
    private String TAG;
    private static final long DEFAULT_WAIT_TIMEOUT_MS = 3000;
    private final Semaphore mConnectionWait = new Semaphore(0);
    private static final String mockedLocalConfigPath = "/data/local/tmp/localconfig.json";
    private static final String LCFINT_IIPM_LMN_NoofNodesRG1 = "IIPS_LM_NofNodesRG1";
    private static final int LCF_INT_IIPS_LM_NofNodesRG1_VAL = 3;
    private static final String LCFDB_CONN_TMR_WFA = "CONN_increaseTimerValueWFA";
    private static final double LCF_DB_CONN_TMR_WFA_VAL = 1.5;
    private static final String LCF_IIPS_IPADDR_LOCAL = "IIPS_IpAddress_LOCAL";
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

        TAG = "LocalConfigTest";

        File file = new File(mockedLocalConfigPath);
        if(file.exists() && !file.isDirectory()) {
            try {
                Os.setenv("VCC_LOCALCONFIG_PATH",mockedLocalConfigPath,true);
            }
            catch (ErrnoException e) {
                Log.e(TAG,e.getMessage());
            }
        }
    }


    public void LocalConfigEnvPath()
    {
           String path = Os.getenv("VCC_LOCALCONFIG_PATH");
           assertEquals(path,mockedLocalConfigPath);
    }

    public void LocalConfigMockedPathExists()
    {
        File file = new File(mockedLocalConfigPath);
        assertTrue(file.exists() && !file.isDirectory());
    }

    public void LocalConfigConnection()
    {
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);

        mlocalcfgapi.connectlocalConfig();

        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {

        }

        assertTrue(mlocalcfgapi.isConnectedLocalConfig());

        mlocalcfgapi.disconnectlocalConfig();

        assertFalse(mlocalcfgapi.isConnectedLocalConfig());

    }

    public void LocalConfigDoubleConnection()
    {
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        assertFalse(mlocalcfgapi.isConnectedLocalConfig());
        mlocalcfgapi.connectlocalConfig();

        try {
            mlocalcfgapi.connectlocalConfig();
            fail("dobule connect should throw");
        } catch (IllegalStateException e) {
            // expected
        }
        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetInteger()
    {
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {

        }

        int lcf = 0;

        try {
            lcf = mlocalcfgapi.getLocalConfigInteger(LCFINT_IIPM_LMN_NoofNodesRG1);
        } catch (LcfgNotConnectedException e)
        {

        }
        assertEquals(LCF_INT_IIPS_LM_NofNodesRG1_VAL,lcf);

        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetDouble()
    {
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {

        }
        double lcf = 0.0;

        try {
            lcf = mlocalcfgapi.getLocalConfigDouble(LCFDB_CONN_TMR_WFA);
        } catch (LcfgNotConnectedException e)
        {

        }

        assertEquals(LCF_DB_CONN_TMR_WFA_VAL,lcf,DELTA);

        mlocalcfgapi.disconnectlocalConfig();
    }

    public void LocalConfigGetString()
    {
        LocalConfig mlocalcfgapi = LocalConfig.createLocalConfig(getApplicationContext(),mServiceConnectionListener);
        mlocalcfgapi.connectlocalConfig();
        try {
            waitForConnection(DEFAULT_WAIT_TIMEOUT_MS);
        } catch (InterruptedException e) {

        }
        String lcf = "";

        try {
            lcf = mlocalcfgapi.getLocalConfigString(LCF_IIPS_IPADDR_LOCAL);
        } catch (LcfgNotConnectedException e)
        {

        }

        assertEquals(LCF_STR_IIPS_IPADDR_LOCAL,lcf);

        mlocalcfgapi.disconnectlocalConfig();
    }
}
