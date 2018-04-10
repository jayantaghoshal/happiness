/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.os.RemoteException;
import android.os.UserManager;
import android.util.Log;
import java.util.HashMap;
import vendor.volvocars.hardware.profiles.V1_0.ICarProfileManager;
import vendor.volvocars.hardware.profiles.V1_0.IKeyPairResponseHandler;
import vendor.volvocars.hardware.profiles.V1_0.IProfileChangedHandler;
import vendor.volvocars.hardware.profiles.V1_0.KeyLocation;


/**
 * UserSwitchManagerHMI connects HMI client to Hal server
 */
public class UserSwitchManagerHMI extends IUserSwitchService.Stub {


    public static final String TAG = UserSwitchManagerHMI.class.getSimpleName();
    public static final int PAIR_ID = -2;
    IKeyPairResponseHandler pairUserTokeyResponseHandler = new IKeyPairResponseHandler.Stub() {
        @Override
        public void keySearchCompleted(short result) throws RemoteException {
            Log.d(TAG, String.format("Received keySearchCompleted - %s", result));
        }
    };
    private UserSwitchService context;
    private HashMap<Integer, IUserSwitchCallBack> callBackMap;
    private Object lockCallBackMap;
    private UserManager userManager;
    private ICarProfileManager carProfileManager;
    IProfileChangedHandler userChangedCallback = new IProfileChangedHandler.Stub() {
        @Override
        public void profileChanged(String androidUserId, short profileId) {
            Log.d(TAG, String.format("Received profileChanged - %s", androidUserId));
            try {
                int value = Integer.valueOf(androidUserId);
                context.switchUser(Integer.valueOf(androidUserId));
            } catch (Exception e) {
                //TODO: This should never happened but current interface supports it
                callBackMap.clear();
                Log.e(TAG, "androidUserId is not Integer", e);
            }
        }

    };

    /**
     * Init
     */
    public void init(UserSwitchService service, ICarProfileManager carProfileManager,
            UserManager userManager) {
        this.context = service;
        this.carProfileManager = carProfileManager;
        callBackMap = new HashMap<>();
        lockCallBackMap = new Object();
        this.userManager = userManager;
        try {
            carProfileManager.subscribeUserChange(userChangedCallback);
        } catch (RemoteException e) {
            Log.d(TAG, "Problem with subscribeUserChange: ", e);
        }
    }

    /**
     * Clears after servers died
     */
    public void clear() {
        this.context = null;
        this.carProfileManager = null;
    }

    @Override
    public void switchUser(String androidUserId, IUserSwitchCallBack callback)
            throws RemoteException {
        Log.d(TAG, String.format("Received switchUser - %s", androidUserId));

        // Validation
        try {
            int isInteger = Integer.valueOf(androidUserId);
        } catch (NumberFormatException ex){
            callback.handleResponse(UserSwitchResult.ERROR);
            return;
        }

        putCallBackMap(Integer.valueOf(androidUserId), callback);
        carProfileManager.requestSwitchUser(androidUserId);
    }

    @Override
    public void createUser(VolvoUser volvoUser, IUserSwitchCallBack callback)
            throws RemoteException {
        Log.d(TAG, String.format("Received createUser - %s", volvoUser));

        // Check if profile availability
        int profileNumber = carProfileManager.getNrOfUnusedProfiles();
        if (profileNumber < 1) {
            Log.d(TAG, "No available profiles: " + profileNumber);
            callback.handleResponse(UserSwitchResult.ERROR);
            return;
        }
        putCallBackMap(volvoUser.id, callback);
        context.createUser(volvoUser);
    }

    @Override
    public void pairUserTokey(IUserSwitchCallBack callback) throws RemoteException {
        Log.d(TAG, String.format("Received pairUserTokey - %s", "todo"));
        putCallBackMap(PAIR_ID, callback);
        carProfileManager.pairCurrentProfileToKey(KeyLocation.AllZones,
                pairUserTokeyResponseHandler);
    }

    // TODO: should have a logic for result of the deletion to HMI?
    @Override
    public void deleteUser(String androidUserId, IUserSwitchCallBack callback)
            throws RemoteException {
        Log.d(TAG, String.format("Received deleteUser - %s", androidUserId));
        carProfileManager.deleteUser(String.valueOf(androidUserId));
        userManager.removeUser(Integer.valueOf(androidUserId));
        // TODO: handle the callback?
    }

    /***
     * Get a callback from the list map using androidUserId.
     * @param androidUserId
     * @return
     */
    public IUserSwitchCallBack getCallBackMap(int androidUserId) {
        synchronized (lockCallBackMap) {
            return callBackMap.get(androidUserId);
        }
    }

    /**
     * Put a callback to the list map using androidUserId.
     * @param androidUserId
     * @param callback
     */
    public void putCallBackMap(int androidUserId, IUserSwitchCallBack callback) {
        synchronized (lockCallBackMap) {
            callBackMap.put(androidUserId, callback);
        }
    }
}
