package com.volvocars.localconfig;

import android.volvocars.localconfig.LocalConfig;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.UserHandle;
import android.util.Log;

/**
 *  When system boots up, start Lcfg service.
 */
public class BootReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.v(LcfLog.TAG_SERVICE, "Starting Local Config Service...");

        Intent lcfgServiceintent = new Intent();
        lcfgServiceintent.setPackage(context.getPackageName());
        lcfgServiceintent.setAction(LocalConfig.LOCAL_CONFIG_SERVICE_INTERFACE_NAME);
        context.startServiceAsUser(lcfgServiceintent, UserHandle.SYSTEM);
    }
}
