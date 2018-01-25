/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module;

import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.VehiclePropConfig;
import android.hidl.manager.V1_0.IServiceManager;
import android.hidl.manager.V1_0.IServiceNotification;
import android.os.Bundle;
import android.os.IHwBinder;
import android.os.RemoteException;
import android.support.annotation.Nullable;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.volvocars.halmodulesink.R;
import com.volvocars.test.lib.AModuleFragment;
import com.volvocars.test.lib.vehiclehal.VehicleHal;

import java.util.List;
import java.util.NoSuchElementException;
import java.util.Vector;

public class FragmentVehicleHalGeneric extends AModuleFragment {
    public static final String TAG = FragmentVehicleHalGeneric.class.getSimpleName();
    public static final String TITLE = "VehicleHal";
    private Object mLock = new Object();
    private VehicleHalDeathRecipient mVehicleDeathRecipient = new VehicleHalDeathRecipient();
    private IVehicle mVehicle;
    private VehicleHal mHal;
    private RecyclerView mRecyclerView;
    private MyAdapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;
    private Button vehicelHalButton;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        ViewGroup root = (ViewGroup) inflater.inflate(R.layout.fragment_vehicle_hal_generic, null);
        mRecyclerView = (RecyclerView) root.findViewById(R.id.my_recycler_view);
        vehicelHalButton = root.findViewById(R.id.vehicelHalButton);
        // RecycleView
        // use this setting to improve performance if you know that changes
        // in content do not change the layout size of the RecyclerView
        mRecyclerView.setHasFixedSize(true);

        // use a linear layout manager
        mLayoutManager = new LinearLayoutManager(getActivity());
        mRecyclerView.setLayoutManager(mLayoutManager);
        // specify an adapter

        Log.v(TAG, "Registering for the serviceNotification service.");
        try {
            ServiceNotification serviceNotification = new ServiceNotification();

            boolean ret = IServiceManager.getService()
                    .registerForNotifications(IVehicle.kInterfaceName,
                            "", serviceNotification);
            if (!ret) {
                Log.d(TAG, "Failed to register service start notification");
            }
        } catch (RemoteException e) {
            Log.d(TAG, "Failed to register service start notification", e);
        }
        connectToVehicleHal();
        vehicelHalButton.setOnClickListener(event -> {
            mAdapter.changeItem();
        });
        return root;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
//        super.onViewCreated(view, savedInstanceState);
        //you can set the title for your toolbar here for different fragments different titles
        getActivity().setTitle(TITLE);
    }

    private void connectToVehicleHal() {
        synchronized (mLock) {
            if (mVehicle != null) {
                return;
            }
            try {
                mVehicle = IVehicle.getService();
                if (mVehicle != null) {
                    mVehicle.linkToDeath(mVehicleDeathRecipient, 0);
                }
                updateUI(() -> {
                    mHal = new VehicleHal(mVehicle);
                    mAdapter = new MyAdapter(mHal, mRecyclerView, this);
                    mRecyclerView.setAdapter(mAdapter);
                });
            } catch (RemoteException e) {
                Log.e(TAG, "CarProfileManager service not responding", e);
            } catch (NoSuchElementException e) {
                Log.e(TAG, "CarProfileManager service not registered yet");
            }

            if (mVehicle == null) {
                throw new IllegalStateException("Profile Manager HAL service is not available.");
            }
            List<VehiclePropConfig> list = new Vector<>(mHal.getAllPropConfigsDirect());

            mAdapter.addDataList(list);
            Log.d(TAG, "connectToVehicleHal is successful");


        }
    }

    /**
     * Validation and crash handling
     */

    private class VehicleHalDeathRecipient implements IHwBinder.DeathRecipient {
        private int deathCount = 0;

        @Override
        public void serviceDied(long cookie) {
            Log.w(TAG, "Profile Manager HAL died.");

            // Remove a previously registered death notification. The recipient will no longer be
            // called if this object dies.
            try {
                mVehicle.unlinkToDeath(this);
            } catch (RemoteException e) {
                Log.e(TAG, "Failed to unlinkToDeath", e);  // TAG and continue.
            }
            mVehicle = null;
            Log.i(TAG, "Notifying car service Profile Manager HAL reconnected...");
        }
    }

    final class ServiceNotification extends IServiceNotification.Stub {
        @Override
        public void onRegistration(String fqName, String name, boolean preexisting) {
            synchronized (mLock) {
                Log.d(TAG, "IServiceNotification service started " + fqName + " " + name);
                connectToVehicleHal();
            }
        }
    }


}
