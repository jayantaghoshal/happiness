/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.UserInfo;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.UserManager;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.List;

/**
 * Test Application where it starts the service and provides testing for sending commands
 * TODO: move it to the test folder and create an another app that creates an Android app CTS
 */

public class MainActivityTest extends Activity {

    public static final String TAG = MainActivityTest.class.getSimpleName();
    private IUserSwitchService mService;
    private TextView mLog;
    private EditText valueEditText;
    private Button createButton;
    private Button switchButton;
    private Button pairButton;
    private Button dumpUserButton;
    private Button deleteUserButton;
    private ActivityManager activityManager;
    private UserManager userM;
    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className, IBinder service) {
            mLog.append("Service binded!\n");
            mService = IUserSwitchService.Stub.asInterface(service);
            mLog.append("Calling switch user!\n");
        }

        @Override
        public void onServiceDisconnected(ComponentName className) {
            mService = null;
            // This method is only invoked when the service quits from the other end or gets killed
            // Invoking exit() from the AIDL interface makes the Service kill itself, thus
            // invoking this.
            mLog.append("Service disconnected.\n");
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mLog = findViewById(R.id.log);
        valueEditText = findViewById(R.id.editTextUserId);
        createButton = findViewById(R.id.pairUserButtonCreate);
        switchButton = findViewById(R.id.pairUserButtonSwitch);
        pairButton = findViewById(R.id.pairUserButtonPair);
        dumpUserButton = findViewById(R.id.dumpUserButton);
        deleteUserButton = findViewById(R.id.deleteUserButton);

        Intent serviceIntent = new Intent(this, UserSwitchService.class);

        // Remove it after TrustService
        mLog.setText("Starting service…\n");
        startService(serviceIntent);
        activityManager = getBaseContext().getSystemService(ActivityManager.class);
        userM = getBaseContext().getSystemService(UserManager.class);

        mLog.append("Binding service…\n");
        bindService(serviceIntent, mConnection, BIND_AUTO_CREATE);
        createButton.setOnClickListener((event) -> {
            createUser(valueEditText.getText().toString());
        });

        switchButton.setOnClickListener(event -> {
            switchUserButton(valueEditText.getText().toString());
        });

        pairButton.setOnClickListener(event -> {
            pairButton();
        });

        dumpUserButton.setOnClickListener(event -> {
            List<UserInfo> listUser = userM.getUsers(false);
            Log.d(TAG, listUser.toString());
            mLog.append("users: " + listUser.toString());
        });

        deleteUserButton.setOnClickListener(event -> {
            deleteUser(valueEditText.getText().toString());
        });

    }

    private void pairButton() {
        try {
            mService.pairUserTokey();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private void createUser(String userName) {
        if (userName == null || userName.isEmpty()) {
            userName = "TestUser";
        }
        mLog.append("Requesting createUser listing… " + userName + "\n");
        long start = System.currentTimeMillis();
        long end = 0;
        try {
            VolvoUser user = new VolvoUser(userName, -1, -1);
            mService.createUser(user);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        mLog.append(
                "File listing took " + (((double) end - (double) start) / 1000d) + " seconds, or "
                        + (end - start) + " milliseconds.\n");

    }

    private void switchUserButton(String userName) {
        mLog.append("Requesting user switch.…\n");
        long start = System.currentTimeMillis();
        long end = 0;
        int value = -1;
        try {
            value = Integer.valueOf(userName);

        } catch (Exception e) {

        }

        try {
            VolvoUser user = new VolvoUser(userName, value, -1);
            mService.switchUser(user);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        mLog.append(
                "File listing took " + (((double) end - (double) start) / 1000d) + " seconds, or "
                        + (end - start) + " milliseconds.\n");
    }

    private void deleteUser(String userName) {
        if (userName == null || userName.isEmpty()) {
            userName = "TestUser";
        }
        mLog.append("Requesting deleteUser listing… " + userName + "\n");
        long start = System.currentTimeMillis();
        long end = 0;
        try {
            VolvoUser user = new VolvoUser(userName, -1, -1);
            mService.deleteUser(user);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        mLog.append(
                "File listing took " + (((double) end - (double) start) / 1000d) + " seconds, or "
                        + (end - start) + " milliseconds.\n");

    }
}
