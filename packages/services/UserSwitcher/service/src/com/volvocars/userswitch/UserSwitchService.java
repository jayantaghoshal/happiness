/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.KeyguardManager;
import android.app.Service;
import android.content.Intent;
import android.content.pm.UserInfo;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.UserManager;
import android.util.Log;

import vendor.volvocars.hardware.profiles.V1_0.ICarProfileManager;


/**
 * UserSwitchService starts the userswitch service that connects HMI client to Hal server
 * TODO: After TrustService is fixed, need to start the service as a TrustService
 */
public class UserSwitchService extends Service {

    public static final String LOG = "VccUserSwitchService";

    private UserSwitchHalClient userSwitchHalClient;
    private ICarProfileManager carProfileManager;
    private IBinder userSwitchManagerHMI;

    class DismissListener extends KeyguardManager.KeyguardDismissCallback {
        public void onDismissCancelled() {
            Log.i(LOG, "onDismissCancelled");
        }
        public void onDismissError() {
            Log.i(LOG, "onDismissError");
        }
        public void onDismissSucceeded() {
            Log.i(LOG, "onDismissSucceeded");
        }
    }

    @Override
    public void onCreate() {
        Log.i(LOG, "onCreate");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(LOG, "onStartCommand");
        try {
            Log.d(LOG, "Getting  ICarProfileManager service.");
            carProfileManager = vendor.volvocars.hardware.profiles.V1_0.ICarProfileManager.getService();
            if (carProfileManager == null){
                Log.e(LOG, "Couldn't get a service from CarProfileManager");
            }
        } catch (RemoteException e){
            Log.e(LOG, "ouldn't get a service from CarProfileManager", e);
        }
        userSwitchHalClient = new UserSwitchHalClient(carProfileManager);
        userSwitchManagerHMI = (IBinder) new UserSwitchManagerHMI(UserSwitchService.this, userSwitchHalClient);
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.i(LOG, "onDestroy");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return userSwitchManagerHMI;
    }

    public void switchUser(int androidUserID){
        final ActivityManager activityManager = getBaseContext().getSystemService(ActivityManager.class);
        final boolean switchStatus = activityManager.switchUser(androidUserID);
        Log.i(LOG, "Current user: "+  getBaseContext().getSystemService(UserManager.class).getUserName());
        Log.i(LOG, "Switch user status: "+ switchStatus);

        KeyguardManager keyguardManager = (KeyguardManager)getSystemService(Activity.KEYGUARD_SERVICE);
        KeyguardManager.KeyguardLock lock = keyguardManager.newKeyguardLock(Activity.KEYGUARD_SERVICE);
        lock.disableKeyguard();
        //keyguardManager.requestDismissKeyguard(new DismissListener());    //requires activity
    }

    public void createUser(){
        // TODO: handling a logic for creating a user after implentation is done in HAL
        getBaseContext().getSystemService(UserManager.class).createUser("test", UserInfo.FLAG_ADMIN);
    }
}
