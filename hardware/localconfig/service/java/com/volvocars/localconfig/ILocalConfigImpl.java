/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.localconfig;

import android.volvocars.localconfig.LocalConfig;
import android.volvocars.localconfig.ILocalConfig;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.IBinder;
import android.util.Log;

import com.android.internal.annotations.GuardedBy;


public class ILocalConfigImpl extends ILocalConfig.Stub {

    @GuardedBy("ILocalConfigImpl.class")
    private static ILocalConfigImpl sInstance = null;

    static {
        /*
         * Load the JNI shared library
         */
        System.loadLibrary("localconfig_jni");
    }

    private final Context mContext;

    public synchronized static ILocalConfigImpl getInstance(Context serviceContext) {
        if (sInstance == null) {
            sInstance = new ILocalConfigImpl(serviceContext);
        }
        return sInstance;
    }

    public synchronized static void releaseInstance() {
        if (sInstance == null) {
            return;
        }
        sInstance = null;
    }

    public ILocalConfigImpl(Context serviceContext) {

        mContext = serviceContext;
    }

    @Override
    public int getLocalConfigInteger(String key){
        int lcfval = 0;
        assertLcfgPermission(mContext);
        try {
            lcfval = getNativeInt(key);
        }
        catch(RuntimeException e) {
            throw new IllegalArgumentException(e.getMessage());
        }

        return lcfval;
    }

    @Override
    public String getLocalConfigString(String key)  {
        String lcfval = "";
        assertLcfgPermission(mContext);
        try {
             lcfval = getNativeString(key);
        }
        catch(RuntimeException e) {
            throw new IllegalArgumentException(e.getMessage());
        }
        return lcfval;
    }

    @Override
    public boolean getLocalConfigBoolean(String key) throws RuntimeException {
        /* Invalid value should be returned */
        boolean lcfval = false;
        assertLcfgPermission(mContext);
        try {
            lcfval = getNativeBool(key);
        }
        catch(RuntimeException e) {
            throw new IllegalArgumentException(e.getMessage());
        }
        return lcfval;
    }

    @Override
    public double getLocalConfigDouble(String key) throws RuntimeException {

        double lcfval = 0;
        assertLcfgPermission(mContext);
        try {
            lcfval = getNativeDouble(key);
        }
        catch(RuntimeException e) {
            throw new IllegalArgumentException(e.getMessage());
        }
        return lcfval;
    }

    public static void assertLcfgPermission(Context context) {
        if (context.checkCallingOrSelfPermission(LocalConfig.PERMISSION_LCFG)
                != PackageManager.PERMISSION_GRANTED) {
            throw new SecurityException(
                    "requires " + LocalConfig.PERMISSION_LCFG);
        }
    }

    native private int getNativeInt(String inputValue);
    native private String getNativeString(String inputValue);
    native private boolean getNativeBool(String inputValue);
    native private double getNativeDouble(String inputValue);
}
