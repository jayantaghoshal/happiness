/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.content.Intent;
import android.os.RemoteException;
import android.util.Log;

import vendor.volvocars.hardware.profiles.V1_0.ICarProfileManager;
import vendor.volvocars.hardware.profiles.V1_0.IKeyPairResponseHandler;
import vendor.volvocars.hardware.profiles.V1_0.IProfileChangedHandler;
import vendor.volvocars.hardware.profiles.V1_0.KeyLocation;


/**
 * UserSwitchManagerHMI connects HMI client to Hal server
 */
public class UserSwitchManagerHMI extends IUserSwitchService.Stub {


    public static final String TAG = UserSwitchManagerHMI.class.getSimpleName();
    private UserSwitchService context;

    private ICarProfileManager carProfileManager;
    private boolean debug = false; // remove it after 100% done

    IKeyPairResponseHandler pairUserTokeyResponseHandler = new IKeyPairResponseHandler.Stub() {
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
    IProfileChangedHandler userChangedCallback = new IProfileChangedHandler.Stub() {
        @Override
        public void profileChanged(String androidUserId, short profileId) {
            Log.d(TAG, String.format("Received profileChanged - %s", androidUserId));
            int value = 1;
            try {
                value = Integer.valueOf(androidUserId);
            } catch (Exception e) {
                Log.e(TAG, "androidUserId is not Integer", e);
            }
            context.switchUser(new VolvoUser("", value, profileId));
            if (debug) {
                Intent mIntent = new Intent(context, RemoveMeAlertActivity.class);
                mIntent.putExtra("action", "Received profileChanged: " + androidUserId);
                context.startActivity(mIntent);
            }
        }

    };

    /**
     * Init
     * @param service
     * @param carProfileManager
     */
    public void init(UserSwitchService service, ICarProfileManager carProfileManager) {
        this.context = service;
        this.carProfileManager = carProfileManager;
        try {
            carProfileManager.subscribeUserChange(userChangedCallback);
        } catch (RemoteException e) {
            Log.d(TAG, "Problem with subscribeUserChange: ", e);
        }
    }

    /**
     * Clears after servers died
     */
    public void clear(){
        this.context = null;
        this.carProfileManager = null;
    }

    @Override
    public void switchUser(VolvoUser volvoUser) throws RemoteException {
        Log.d(TAG, String.format("Received switchUser - %s", volvoUser));
        carProfileManager.requestSwitchUser(String.valueOf(volvoUser.getId()));

        // Remove it after testing or add debugging option
        if (volvoUser.getId() == -1) {
            context.switchUser(volvoUser);
        }
    }

    @Override
    public void createUser(VolvoUser volvoUser) throws RemoteException {
        Log.d(TAG, String.format("Received createUser - %s", volvoUser));

        // Check if profile availability
        int profileNumber = carProfileManager.getNrOfUnusedProfiles();
        if (profileNumber < 1) {
            Log.d(TAG, "No available profiles: " + profileNumber);
            return;
        }
        context.createUser(volvoUser);

        if (debug) {
            Intent mIntent = new Intent(context, RemoveMeAlertActivity.class);
            mIntent.putExtra("action", "CreateUser");
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
        carProfileManager.pairCurrentProfileToKey(KeyLocation.AllZones, pairUserTokeyResponseHandler);
    }

    // TODO: should have a logic for result of the deletion to HMI?
    @Override
    public void deleteUser(VolvoUser volvoUser) throws RemoteException {
        Log.d(TAG, String.format("Received deleteUser - %s", volvoUser));
        carProfileManager.deleteUser(String.valueOf(volvoUser.getId()));
        Log.d(TAG, String.format("Received pairUserTokey - %s", "todo"));
        if (debug) {
            Intent mIntent = new Intent(context, RemoveMeAlertActivity.class);
            mIntent.putExtra("action", "deleteUser");
            mIntent.putExtra("info", volvoUser.toString());
            context.startActivity(mIntent);
        }
    }

    public void setCarProfileManager(
            ICarProfileManager carProfileManager) {
        this.carProfileManager = carProfileManager;
    }
}
