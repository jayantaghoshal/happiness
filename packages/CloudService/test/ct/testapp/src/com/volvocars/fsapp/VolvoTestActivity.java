/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.fsapp;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import vendor.volvocars.cloudservice.*;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;

public class VolvoTestActivity extends Activity {
    private static final String LOG_TAG = "FsApiTestApp";
    private FoundationServicesApi fsapi = null;
    private boolean avail = false;
    private Button makeRequestBtn;
    private TextView requestTv;
    private int counter = 0;
    private FoundationServicesApiConnectionCallback fsApiConnectionCallback = new FoundationServicesApiConnectionCallback(){
        @Override
        public void onServiceConnected(){
            Log.d(LOG_TAG, "onServiceConnected app");
            makeRequestBtn.setEnabled(true);
            requestTv.setText("Button enabled onServiceConnected");
        }
        @Override
        public void onServiceDisconnected(){
            makeRequestBtn.setEnabled(false);
            requestTv.setText("Button disabled onServiceDisconnected");
        }
    };

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        makeRequestBtn = (Button) findViewById(R.id.makeRequestBtn);
        makeRequestBtn.setEnabled(false);
        requestTv = (TextView) findViewById(R.id.request_text);

        Log.d(LOG_TAG,"onCreate");
        makeRequestBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try{
                    counter++;
                    avail=fsapi.IsFeatureAvailable("carsharing");
                    requestTv.setText("request nr:"+ counter+" IsFeatureAvailable: " + avail);
                }
                catch(Exception ex){
                    ex.printStackTrace();
                    Log.d(LOG_TAG, ex.getMessage());
                }
            }
        });

    };
    @Override
    protected void onStart() {
        super.onStart();
        fsapi = new FoundationServicesApi(this,fsApiConnectionCallback);
    }
    @Override
    protected void onPause() {
        super.onPause();
        Log.d(LOG_TAG,"InBackground");
    }
    @Override
    protected void onResume() {
        super.onResume();
        Log.d(LOG_TAG,"InForeground");
    }
}


