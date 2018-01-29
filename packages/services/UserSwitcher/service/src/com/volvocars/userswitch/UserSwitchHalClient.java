/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.userswitch;

import android.os.RemoteException;

import vendor.volvocars.hardware.profiles.V1_0.ICarProfileManager;
import vendor.volvocars.hardware.profiles.V1_0.IKeyPairResponseHandler;
import vendor.volvocars.hardware.profiles.V1_0.IProfileChangedHandler;
import vendor.volvocars.hardware.profiles.V1_0.KeyId;

/**
 * Implementation for the HIDL client
 * TODO: Handle properly deathTolink and handling exception
 */

public class UserSwitchHalClient {

    public static final String TAG = UserSwitchService.class.getSimpleName();
    ICarProfileManager carProfileManager;

    public UserSwitchHalClient(ICarProfileManager carProfileManager) {
        this.carProfileManager = carProfileManager;
    }

    public void subscribeUserChange(IProfileChangedHandler cb) throws RemoteException {
        carProfileManager.subscribeUserChange(cb);
    }

    public void requestSwitchUser(String androidUserId) throws RemoteException {
        carProfileManager.requestSwitchUser(androidUserId);
    }

    public short getUserProfileInformation(String androidUserId) throws RemoteException {
        return carProfileManager.getUserProfileInformation(androidUserId);
    }

    public short getNrOfUnusedProfiles() throws RemoteException {
        return carProfileManager.getNrOfUnusedProfiles();
    }

    public boolean pairAndroidUserToUnusedVehicleProfile(String androidUserId) throws RemoteException {
        return carProfileManager.pairAndroidUserToUnusedVehicleProfile(androidUserId);

    }

    public void deleteUser(String androidUserId) throws RemoteException {
        carProfileManager.deleteUser(androidUserId);
    }

    public void pairCurrentProfileToKey(short searchLocation, IKeyPairResponseHandler onKeySearchCompleted)
            throws RemoteException {
        carProfileManager.pairCurrentProfileToKey(searchLocation, onKeySearchCompleted);
    }

    public boolean disconnectCurrentProfileFromKey() throws RemoteException {
        return carProfileManager.disconnectCurrentProfileFromKey();
    }
}
