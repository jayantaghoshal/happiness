/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
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
import java.util.Optional;

/**
 * Test Application where it starts the service and provides testing for sending commands
 * TODO: move it to the test folder and create an another app that creates an Android app CTS
 */

public class MainActivityTest extends Activity {

    public static final String TAG = MainActivityTest.class.getSimpleName();
    private IUserSwitchService mService;
    private TextView textResult;
    private TextView textOwner;
    private EditText valueEditText;
    private Button createButton;
    private Button switchButton;
    private Button pairButton;
    private Button dumpUserButton;
    private Button deleteUserButton;
    private Button getUserIdButton;
    private ActivityManager activityManager;
    private Context context;
    private UserManager userManager;
    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className, IBinder service) {
            textResult.append("Service binded!\n");
            mService = IUserSwitchService.Stub.asInterface(service);
            textResult.append("Calling switch user!\n");
        }

        @Override
        public void onServiceDisconnected(ComponentName className) {
            mService = null;
            // This method is only invoked when the service quits from the other end or gets killed
            // Invoking exit() from the AIDL interface makes the Service kill itself, thus
            // invoking this.
            textResult.append("Service disconnected.\n");
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_test);
        context = this;
        userManager = getBaseContext().getSystemService(UserManager.class);
        textResult = findViewById(R.id.textLog);
        textOwner = findViewById(R.id.textOwner);
        valueEditText = findViewById(R.id.editTextUserId);
        createButton = findViewById(R.id.pairUserButtonCreate);
        switchButton = findViewById(R.id.pairUserButtonSwitch);
        dumpUserButton = findViewById(R.id.dumpUserButton);
        deleteUserButton = findViewById(R.id.deleteUserButton);
        getUserIdButton = findViewById(R.id.getUserIdButton);

        Intent serviceIntent = new Intent(this, UserSwitchService.class);

        // Remove it after TrustService
        textResult.setText("Starting service…\n");
        startService(serviceIntent);
        activityManager = getBaseContext().getSystemService(ActivityManager.class);
        userManager = getBaseContext().getSystemService(UserManager.class);

        textResult.append("Binding service…\n");
        bindService(serviceIntent, mConnection, BIND_AUTO_CREATE);

        textOwner.setText(String.valueOf(ActivityManager.getCurrentUser()));
        createButton.setOnClickListener(event -> createUser(valueEditText.getText().toString()));
        switchButton.setOnClickListener(event -> switchUserButton(valueEditText.getText().toString()));

        dumpUserButton.setOnClickListener(event -> {
            List<UserInfo> listUser = userManager.getUsers(false);
            Log.d(TAG, listUser.toString());
            textResult.append("users: " + listUser.toString());
        });

        deleteUserButton.setOnClickListener(event -> deleteUser(valueEditText.getText().toString()));

        getUserIdButton.setOnClickListener(event -> {
            List<UserInfo> listUser = userManager.getUsers(false);
            Optional<UserInfo> result = listUser.stream().filter(
                    u -> u.name.equals(valueEditText.getText().toString()))
                    .findAny();
            if (result.isPresent()) {
                textResult.setText(result.get().id + "");
            } else {
                textResult.setText("ERROR");
            }
        });
    }

    private void createUser(String userName) {
        if (userName == null || userName.isEmpty()) {
            userName = "TestUser";
        }
        String volvoUserName = userName;
        textResult.append("Requesting createUser listing… " + userName + "\n");
        try {
            VolvoUser user = new VolvoUser(-1, volvoUserName, VolvoUser.FLAG_ADMIN);
            mService.createUser(user, new IUserSwitchCallBack.Stub() {
                @Override
                public void handleResponse(UserSwitchResult result) throws RemoteException {
                    // check also if name is created
                    boolean isNameExist = userManager.getUsers().stream().anyMatch(
                            u -> u.name.equals(volvoUserName));
                    if (isNameExist && result.equals(UserSwitchResult.SUCCESS)) {
                        showMessage(result.name());
                    } else {
                        showMessage(UserSwitchResult.ERROR.name());
                    }
                }
            });
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private void switchUserButton(String userName) {
        textResult.append("Requesting user switch.…\n");

        try {
            mService.switchUser(userName, new IUserSwitchCallBack.Stub() {
                @Override
                public void handleResponse(UserSwitchResult result) throws RemoteException {
                    showMessage(result.name());
                }
            });
        } catch (RemoteException e) {
            e.printStackTrace();
        }

    }

    private void deleteUser(String testNameOrId) {
        if (testNameOrId == null || testNameOrId.isEmpty()) {
            showMessage("Name or ID is required!");
            return;
        }
        textResult.append("Requesting deleteUser listing… " + testNameOrId + "\n");
        try {
            int checkInt = Integer.valueOf(testNameOrId);
        } catch (Exception e) {
            List<UserInfo> listUser = userManager.getUsers(false);
            String findAndroidUserId = testNameOrId;
            Optional<UserInfo> result = listUser.stream().filter(
                    u -> u.name.equals(findAndroidUserId)).findAny();
            if (result.isPresent()) {
                testNameOrId = result.get().id + "";
            } else {
                showMessage("No user found: " + testNameOrId);
                return;
            }
        }
        try {
            mService.deleteUser(testNameOrId, new IUserSwitchCallBack.Stub() {
                @Override
                public void handleResponse(UserSwitchResult result) throws RemoteException {
                    showMessage(result.name());
                }
            });
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private void showMessage(String message) {
        textResult.clearComposingText();
        textResult.setText(message);
    }
}
