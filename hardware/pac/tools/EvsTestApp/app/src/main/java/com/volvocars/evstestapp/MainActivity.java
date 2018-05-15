/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.evstestapp;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.d(LogTags.EVSTEST, "EVS service status: " + NdkManager.initEvsCameraStream());

        setContentView(R.layout.glsurfaceview);

        Button test = findViewById(R.id.disappearBtn);

        test.setOnClickListener(new TestBtnListener());
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(LogTags.EVSTEST, NdkManager.closeCameraStream());
    }

    private class TestBtnListener implements View.OnClickListener {

        @Override
        public void onClick(View view) {
            MyGLRenderer.testingIfWeCanMakeTheCarInvisible = !MyGLRenderer.testingIfWeCanMakeTheCarInvisible;
        }
    }
}
