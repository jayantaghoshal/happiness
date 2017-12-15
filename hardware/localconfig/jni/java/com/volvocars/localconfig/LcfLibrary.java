package com.volvocars.localconfig;

import android.util.Log;

public final class LcfLibrary {

    private static final String TAG_LCCFG_LIB = "LocalConfigJavaJniLib";

    static {
        /*
         * Load the JNI shared library
         */
        System.loadLibrary("localconfig_library_jni");
    }

    private int mJniInt = -1;

    public LcfLibrary() { }

    public int getInt(String str) throws RuntimeException
    {
            int result = getNativeInt(str);
            Log.v(TAG_LCCFG_LIB, "getInt: " + result );
            return result;
    }

    public String getString(String str) throws RuntimeException
    {

            String result = getNativeString(str);
            Log.v(TAG_LCCFG_LIB, "getString: " + result );
            return result;
    }

    public boolean getBoolean(String str) throws RuntimeException
    {
            boolean result = getNativeBool(str);
            Log.v(TAG_LCCFG_LIB, "getBoolean: " + result );
            return result;
    }

    public double getDouble(String str) throws RuntimeException
    {
            double result = getNativeDouble(str);
            Log.v(TAG_LCCFG_LIB, "getDouble: " + result );
            return result;
    }

    native private int getNativeInt(String inputValue);
    native private String getNativeString(String inputValue);
    native private boolean getNativeBool(String inputValue);
    native private double getNativeDouble(String inputValue);
}
