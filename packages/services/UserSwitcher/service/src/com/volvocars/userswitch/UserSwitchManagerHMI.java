/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.content.Context;
import android.content.Intent;
import android.os.RemoteException;
import android.util.Log;

import vendor.volvocars.hardware.profiles.V1_0.IKeyPairResponseHandler;
import vendor.volvocars.hardware.profiles.V1_0.IProfileChangedHandler;


/**
 * UserSwitchManagerHMI connects HMI client to Hal server
 */
public class UserSwitchManagerHMI extends ISwitchUserService.Stub {


    public static final String TAG = UserSwitchManagerHMI.class.getSimpleName();
    private Context context;
    private UserSwitchHalClient userSwitchHalClient;
    private boolean debug = true; // remove it after 100% done
    IKeyPairResponseHandler pairUserTokeyRespnseHandler = new IKeyPairResponseHandler.Stub (){
        @Override
        public void keySearchCompleted(short result) throws RemoteException {
            Log.d(TAG, String.format("Received keySearchCompleted - %s", result));
            if (debug) {
                Intent mIntent = new Intent(context, RemoveMeAlertActivity.class);
                mIntent.putExtra("action", "Received keySearchCompleted: " + result);
                context.startActivity(mIntent);
            }
        }
    };
    IProfileChangedHandler userChangedCallback = new IProfileChangedHandler.Stub (){
        @Override
        public void profileChanged(String androidUserId, short profileId) throws RemoteException {
            Log.d(TAG, String.format("Received profileChanged - %s", androidUserId));
            if (debug) {
                Intent mIntent = new Intent(context, RemoveMeAlertActivity.class);
                mIntent.putExtra("action", "Received profileChanged: " + androidUserId);
                context.startActivity(mIntent);
            }
        }

    };

    public UserSwitchManagerHMI(UserSwitchService service, UserSwitchHalClient userSwitchHalClient) {
        this.context = service;
        this.userSwitchHalClient = userSwitchHalClient;
        try {
            userSwitchHalClient.subscribeUserChange(userChangedCallback);
        } catch (RemoteException e) {
            Log.d(TAG, "Problem with subscribeUserChange: ", e);
        }
    }

    @Override
    public void switchUser(VolvoUser volvoUser) throws RemoteException {
        Log.d(TAG, String.format("Received switchUser - %s", volvoUser));
        userSwitchHalClient.requestSwitchUser(String.valueOf(volvoUser.getId()));
        // enable it after setting manager is available
        // context.switchUser(volvoUser.getId());
    }

    @Override
    public void createUser(VolvoUser volvoUser) throws RemoteException {
        Log.d(TAG, String.format("Received createUser - %s", volvoUser));
        if (debug) {
            Intent mIntent = new Intent(context, RemoveMeAlertActivity.class);
            mIntent.putExtra("action", "switchUser");
            mIntent.putExtra("info", volvoUser.toString());
            context.startActivity(mIntent);
        }
    }

    @Override
    public void pairUserTokey() throws RemoteException {
        Log.d(TAG, String.format("Received pairUserTokey - %s", "todo"));
        if (debug) {
            Intent mIntent = new Intent(context, RemoveMeAlertActivity.class);
            mIntent.putExtra("action", "pairUserTokey");
            context.startActivity(mIntent);
        }
        userSwitchHalClient.pairCurrentProfileToKey((short) 1, pairUserTokeyRespnseHandler);
    }
}
