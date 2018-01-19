/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package vendor.volvocars.cloudservice;

import android.app.Service;
import android.app.Application;
import android.content.Intent;
import android.util.Log;

public class MainApplication extends Application {
    private static final String LOGTAG = "CloudService";
    @Override
    public void onCreate() {
        super.onCreate();
        Log.v(LOGTAG, "onCreate() Main");
        Intent intent = new Intent(this, CloudService.class);
        startService(intent);
    }
}