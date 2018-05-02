/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(Tags.LOG_TAG, "EVS service status: " + NdkManager.initEvsCameraStream());

        setContentView(R.layout.glsurfaceview);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(Tags.LOG_TAG, NdkManager.closeCameraStream());
    }
}