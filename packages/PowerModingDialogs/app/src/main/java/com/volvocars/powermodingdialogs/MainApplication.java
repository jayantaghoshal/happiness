/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.powermodingdialogs;

import android.app.Application;
import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.SubscribeFlags;
import android.hardware.automotive.vehicle.V2_0.SubscribeOptions;
import android.os.HwBinder;
import android.os.RemoteException;
import android.util.Log;

import java.util.ArrayList;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

public class MainApplication extends Application implements HwBinder.DeathRecipient {
    private String TAG = "PowerModingDialogs";
    private IVehicle mVehicle;
    private DialogHandler mDialogHandler;

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG,"Started");

        try {
            // Connect to VHAL
            mVehicle = IVehicle.getService();
            if (mVehicle==null) {
                Log.e(TAG, "IVehicle.getService returned null, re-spawning");
                stopSelf(); // terminate ourself to get re-spawned
                return;
            }
            mVehicle.linkToDeath(this, 1010 /* dummy cookie */);

            // Setup vhal property subscriptions
            ArrayList<SubscribeOptions> options = new ArrayList<>();
            SubscribeOptions opts = new SubscribeOptions();
            opts.propId = VehicleProperty.POWERMODE_USER_INTERACTION;
            opts.flags = SubscribeFlags.DEFAULT;
            options.add(opts);
            mDialogHandler = new DialogHandler();
            mVehicle.subscribe(mDialogHandler, options);
        } catch (RemoteException e) {
            Log.e(TAG,"Failed to setup ("+e.getMessage()+"), re-spawning");
            e.printStackTrace();
            stopSelf();
        }
    }

    @Override
    public void serviceDied(long l) {
        Log.e(TAG,"VHAL died, re-spawning");
        stopSelf();
    }

    private void stopSelf() {
        System.exit(0);
    }
}
