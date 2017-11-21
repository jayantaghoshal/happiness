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

    private static LcfLibrary sLcfinstance = null;

    private final Context mContext;

    public synchronized static ILocalConfigImpl getInstance(Context serviceContext) {
        if (sInstance == null) {
            sInstance = new ILocalConfigImpl(serviceContext);
            sInstance.init();
        }
        return sInstance;
    }

    public synchronized static void releaseInstance() {
        if (sInstance == null) {
            return;
        }
        sInstance.release();
        sInstance = null;
    }

    public ILocalConfigImpl(Context serviceContext) {

        mContext = serviceContext;
    }

    private void init() {
        sLcfinstance = new LcfLibrary();
    }

    private void release() {
    }

    @Override
    public int getLocalConfigInteger(String key) throws RuntimeException {
        /* TODO Exception handling from JNI */
        int lcfval = 0;
        if(sLcfinstance != null) {
            assertLcfgPermission(mContext);
            lcfval = sLcfinstance.getInt(key);
        }
        return lcfval;
    }

    @Override
    public String getLocalConfigString(String key) throws RuntimeException {
        /* TODO Exception handling from JNI */
        String lcfval = " ";
        if(sLcfinstance != null) {
            assertLcfgPermission(mContext);
            lcfval = sLcfinstance.getString(key);
        }
        return lcfval;
    }

    @Override
    public boolean getLocalConfigBoolean(String key) throws RuntimeException {
        /* TODO Exception handling from JNI */
        /* Invalid value should be returned */
        boolean lcfval = false;
        if(sLcfinstance != null) {
            assertLcfgPermission(mContext);
            lcfval = sLcfinstance.getBoolean(key);
        }
        return lcfval;
    }

    @Override
    public double getLocalConfigDouble(String key) throws RuntimeException {
        /* TODO Exception handling from JNI */
        double lcfval = 0;
        if(sLcfinstance != null) {
            assertLcfgPermission(mContext);
            lcfval = sLcfinstance.getDouble(key);
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
}
