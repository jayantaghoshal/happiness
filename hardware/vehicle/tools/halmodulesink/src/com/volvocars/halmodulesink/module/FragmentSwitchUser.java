/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module;

import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.UserInfo;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.UserHandle;
import android.os.UserManager;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.volvocars.halmodulesink.R;
import com.volvocars.test.lib.AModuleFragment;
import com.volvocars.userswitch.IUserSwitchCallBack;
import com.volvocars.userswitch.IUserSwitchService;
import com.volvocars.userswitch.UserSwitchResult;
import com.volvocars.userswitch.VolvoUser;
import com.volvocars.userswitch.ServiceInfo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;
import java.util.Optional;

import static android.content.Context.BIND_AUTO_CREATE;

/**
 * Template for creating a new model
 *
 */
public class FragmentSwitchUser extends AModuleFragment {

    public static final String TAG = FragmentSwitchUser.class.getSimpleName();
    public static final String TITLE = "Example";

    private IUserSwitchService mService;
    private Context context;
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

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        ViewGroup root = (ViewGroup) inflater.inflate(R.layout.fragment_switch_user, null);
        context = getActivity();
        userManager = context.getSystemService(UserManager.class);
        textResult = root.findViewById(R.id.textLog);
        textOwner = root.findViewById(R.id.textOwner);
        valueEditText = root.findViewById(R.id.editTextUserId);
        createButton = root.findViewById(R.id.pairUserButtonCreate);
        switchButton = root.findViewById(R.id.pairUserButtonSwitch);
        dumpUserButton = root.findViewById(R.id.dumpUserButton);
        deleteUserButton = root.findViewById(R.id.deleteUserButton);
        getUserIdButton = root.findViewById(R.id.getUserIdButton);

        // Remove it after TrustService
        textResult.setText("Starting service…\n");
//        startService(serviceIntent);
        activityManager = context.getSystemService(ActivityManager.class);
        userManager = context.getSystemService(UserManager.class);

        textResult.append("Binding service…\n");
//        bindService(serviceIntent, mConnection, BIND_AUTO_CREATE);

//        bindService(new Intent(MainActivityTest.this,
//                UserSwitchService.class), mConnection, Context.BIND_AUTO_CREATE);

        Intent intent = new Intent(IUserSwitchService.class.getName());

        /*this is service name*/
        intent.setAction(ServiceInfo.ACTION);

        /*From 5.0 annonymous intent calls are suspended so replacing with server app's package name*/
        intent.setPackage(ServiceInfo.PACKAGE_NAME);

        // binding to remote service
        context.bindServiceAsUser(intent, mConnection, BIND_AUTO_CREATE, UserHandle.SYSTEM);
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
        return root;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        getActivity().setTitle(TITLE);
    }

}