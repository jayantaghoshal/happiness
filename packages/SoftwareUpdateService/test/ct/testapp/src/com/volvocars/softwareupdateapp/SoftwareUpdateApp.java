/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.app.Activity;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import com.volvocars.softwareupdate.*;
import com.volvocars.cloudservice.*;
import java.util.*;

public class SoftwareUpdateApp extends Activity {
    private static final String LOG_TAG = "SwUpdApp";
    private SoftwareUpdateManager softwareUpdateManager = null;

    private Button getAssignmentsButton;
    private TextView assignmentsTv;

    private ISoftwareUpdateManagerCallback callback = new ISoftwareUpdateManagerCallback.Stub() {
        public void UpdateState(int state) {
            Log.v(LOG_TAG, "UpdateState");
        }

        public void UpdateSoftwareAssignmentList(List<SoftwareAssignment> software_list) {
            Log.v(LOG_TAG, "UpdateSoftwareAssignmentList");
            String str = "**********";
            for (SoftwareAssignment sw : software_list) {
                str += sw.toString();
                str += "**********";
            }

            updateTv(assignmentsTv, str);
        }

        public void UpdateSoftwareState(String uuid, int state) {
            Log.v(LOG_TAG, "UpdateSoftwareState");
        }

        public void ProvideErrorMessage(int code, String message) {
            Log.d(LOG_TAG, "ProvideErrorMessage: [ code: " + code + ", message: " + message + "]");
            updateTv(assignmentsTv, "Error message: [ code: " + code + ", message: " + message + "]");
        }
    };

    private void updateTv(TextView tv, String s) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tv.setText(s);
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
