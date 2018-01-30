/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import android.widget.LinearLayout;
import com.volvocars.softwareupdate.*;
import com.volvocars.cloudservice.*;
import java.util.*;

public class SoftwareUpdateApp extends Activity {
    private static final String LOG_TAG = "SwUpdApp";
    private SoftwareUpdateManager softwareUpdateManager = null;

    private Button getAssignmentsButton;
    private TextView assignmentsTv;
    private LinearLayout layout;

    private Context context = this;

    private ArrayList<TextView> sws = new ArrayList<TextView>();
    private ISoftwareUpdateManagerCallback callback = new ISoftwareUpdateManagerCallback.Stub() {
        public void UpdateState(int state) {
            Log.v(LOG_TAG, "UpdateState");
        }

        public void UpdateSoftwareAssignmentList(List<SoftwareAssignment> software_list) {
            Log.v(LOG_TAG, "UpdateSoftwareAssignmentList");

            updateTv(assignmentsTv, "Software assignments:", 20);
            for (SoftwareAssignment sw : software_list) {
                TextView name = new TextView(context);
                sws.add(name);
                updateTv(name, sw.name, 30);
                addToLayout(name);

                TextView desc = new TextView(context);
                sws.add(desc);
                updateTv(desc, sw.description, 20);
                addToLayout(desc);
            }
        }

        public void UpdateSoftwareState(String uuid, int state) {
            Log.v(LOG_TAG, "UpdateSoftwareState");
        }

        public void ProvideErrorMessage(int code, String message) {
            Log.d(LOG_TAG, "ProvideErrorMessage: [ code: " + code + ", message: " + message + "]");

            for(TextView tv : sws) {
                deleteFromLayout(tv);
            }

            updateTv(assignmentsTv, "Error message: [ code: " + code + ", message: " + message + "]", 20);
        }
    };

    private void updateTv(TextView tv, String s, int size) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tv.setText(s);
                tv.setTextSize(size);
            }
        });
    }
    private void addToLayout(TextView tv) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                layout.addView(tv);
            }
        });
    }

    private void deleteFromLayout(TextView tv)
    {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                layout.removeView(tv);
            }
        });
    }
    private SoftwareUpdateManagerCallback softwareUpdateManagerCallback = new SoftwareUpdateManagerCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, "onServiceConnected app");
            getAssignmentsButton.setEnabled(true);
        }

        @Override
        public void onServiceDisconnected() {
            getAssignmentsButton.setEnabled(false);
        }
    };

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.v(LOG_TAG, "onCreate");

        setContentView(R.layout.activity_main);
        getAssignmentsButton = (Button) findViewById(R.id.assignmentsButton);
        getAssignmentsButton.setEnabled(false);

        assignmentsTv = (TextView) findViewById(R.id.assignmentsTv);

        layout = (LinearLayout) findViewById(R.id.rootlinearlayout);

        getAssignmentsButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    softwareUpdateManager.GetSoftwareAssignments(callback);
                } catch (RemoteException e) {
                    Log.d(LOG_TAG, e.getMessage());
                }
            }
        });

    };

    @Override
    protected void onStart() {
        super.onStart();
        Log.v(LOG_TAG, "OnStart");
        softwareUpdateManager = new SoftwareUpdateManager(this, softwareUpdateManagerCallback);
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.v(LOG_TAG, "InBackground");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.v(LOG_TAG, "InForeground");
    }
}
