/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.softwareupdateapp;

import android.content.Context;
import android.os.Bundle;
import android.os.RemoteException;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.NumberPicker;
import android.widget.Toast;
import com.volvocars.softwareupdate.*;
import com.volvocars.cloudservice.Query;
import java.util.*;

public class AvailableAssignmentsActivity extends AppCompatActivity {
    private final String LOG_TAG = "SwUpdApp.AssignmentsActivity";
    private SoftwareUpdateManager softwareUpdateManager = null;
    private Context context = this;

    private NumberPicker installationOrderPicker;
    private NumberPicker softwarePicker;
    private Button sendButton;
    private CheckBox installationOrderBox;
    private CheckBox softwareBox;

    private ISoftwareUpdateManagerCallback callback = new ISoftwareUpdateManagerCallback.Stub() {
        @Override
        public void UpdateState(int state) {
        }

        @Override
        public void UpdateSoftwareList(List<SoftwareInformation> software_list) {
        }

        @Override
        public void UpdateSoftware(SoftwareInformation software) {
        }

        @Override
        public void UpdateSettings(List<Setting> settings) {
        }

        @Override
        public void ProvideErrorMessage(int code, String message) {
            Log.d(LOG_TAG, "ProvideErrorMessage: [ code: " + code + ", message: " + message + "]");
        }
    };

    private SoftwareUpdateManagerCallback softwareUpdateManagerCallback = new SoftwareUpdateManagerCallback() {
        @Override
        public void onServiceConnected() {
            Log.v(LOG_TAG, "onServiceConnected app");
            try {
                softwareUpdateManager.GetState(callback);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Error GetState");
            }
        }

        @Override
        public void onServiceDisconnected() {
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.v(LOG_TAG, "onCreate");
        softwareUpdateManager = new SoftwareUpdateManager(this, softwareUpdateManagerCallback);

        setContentView(R.layout.layout_getassignments);

        Toolbar myChildToolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(myChildToolbar);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        installationOrderPicker = (NumberPicker) findViewById(R.id.installationOrderIdNP);
        installationOrderPicker.setMinValue(0);
        installationOrderPicker.setMaxValue(20);

        softwarePicker = (NumberPicker) findViewById(R.id.softwareIdNP);
        softwarePicker.setMinValue(0);
        softwarePicker.setMaxValue(20);

        installationOrderBox = (CheckBox) findViewById(R.id.installationOrderIdCB);
        softwareBox = (CheckBox) findViewById(R.id.softwareIdCB);

        sendButton = (Button) findViewById(R.id.sendButton);
        sendButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Query query = new Query();
                if (softwareBox.isChecked()) query.id = "" + softwarePicker.getValue();
                if (installationOrderBox.isChecked()) query.installationOrderId = "" + installationOrderPicker.getValue();

                Log.d(LOG_TAG, "QUERY: " + query.buildQuery());

                try {
                    softwareUpdateManager.GetSoftwareAssignment(query);
                    Toast.makeText(context, "Sending GetSoftwareAssignments", Toast.LENGTH_SHORT).show();
                } catch (Exception e) {
                    //TODO: handle exception
                }
            }
        });


    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.v(LOG_TAG, "onDestroy");
        softwareUpdateManager.disconnect();
    }
}