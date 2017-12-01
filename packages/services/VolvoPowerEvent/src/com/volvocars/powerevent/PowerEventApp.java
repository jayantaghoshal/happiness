
package com.volvocars.powerevent;

import android.app.Application;
import android.content.Intent;
import android.util.Log;

public class PowerEventApp extends Application {
    static final String TAG = "Volvocars.Powerevent";
    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "onCreate()");
    }
}
