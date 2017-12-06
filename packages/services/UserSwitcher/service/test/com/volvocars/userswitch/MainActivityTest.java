/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

/**
 * Test Application where it starts the service and provides testing for sending commands
 * TODO: move it to the test folder and create an another app that creates an Android app CTS
 *
 */

public class MainActivityTest extends Activity {

    private ISwitchUserService mService;
    private TextView mLog;
    private EditText editText;
    private Button button1;
    private Button button2;
    private Button button3;
    public static final String TAG = MainActivityTest.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mLog = (TextView) findViewById(R.id.log);
        editText= (EditText) findViewById(R.id.editTextUserId);
        button1 = (Button) findViewById(R.id.button1);
        button2 = (Button) findViewById(R.id.button2);
        button3 = (Button) findViewById(R.id.button3);
        Intent serviceIntent = new Intent(this,UserSwitchService.class);

        // Remove it after TrustService
        mLog.setText("Starting service…\n");
        startService(serviceIntent);

        mLog.append("Binding service…\n");
        bindService(serviceIntent, mConnection, BIND_AUTO_CREATE);

        button1.setOnClickListener((event)->{
            switchUserButton(1);
        });

        button2.setOnClickListener((event)->{
            switchUserButton(2);
        });

        button3.setOnClickListener((event)->{
            if (!editText.getText().toString().isEmpty()){
                try{
                    switchUserButton(Integer.valueOf(editText.getText().toString()));
                }catch (Exception e){
                    mLog.append("Correct value needed!\n");
                }
            }
        });

    }

    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className, IBinder service) {
            mLog.append("Service binded!\n");
            mService = ISwitchUserService.Stub.asInterface(service);
            mLog.append("Calling switch user!\n");
            switchUser();
        }

        @Override
        public void onServiceDisconnected(ComponentName className) {
            mService = null;
            // This method is only invoked when the service quits from the other end or gets killed
            // Invoking exit() from the AIDL interface makes the Service kill itself, thus invoking this.
            mLog.append("Service disconnected.\n");
        }
    };

    private void switchUser() {
        mLog.append("Requesting file listing…\n");
        long start = System.currentTimeMillis();
        long end = 0;
        try {
            VolvoUser user = new VolvoUser("test", 1,1);
            mService.switchUser(user);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        mLog.append("File listing took " + (((double) end - (double) start) / 1000d) + " seconds, or " + (end - start) + " milliseconds.\n");
    }
    private void switchUserButton(int id) {
        mLog.append("Requesting user switch.…\n");
        long start = System.currentTimeMillis();
        long end = 0;
        try {
            VolvoUser user = new VolvoUser("test", id,1);
            mService.switchUser(user);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        mLog.append("File listing took " + (((double) end - (double) start) / 1000d) + " seconds, or " + (end - start) + " milliseconds.\n");
    }
}
