/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module.vehiclehal;

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
import android.widget.Toast;

import com.volvocars.halmodulesink.R;
import com.volvocars.test.lib.AModuleFragment;
import com.volvocars.test.lib.vehiclehal.VehicleHal;

import java.util.List;
import java.util.NoSuchElementException;
import java.util.Vector;

public class FragmentVehicleHalGeneric extends AModuleFragment {
    public static final String TAG = FragmentVehicleHalGeneric.class.getSimpleName();
    public static final String TITLE = "VehicleHalGeneric";
    private Object mLock = new Object();
    private VehicleHalDeathRecipient vehicleManagerDeathRecipient = new VehicleHalDeathRecipient();
    private IVehicle vehicleManager;
    private VehicleHal vehicleHal;
    private RecyclerView mRecyclerView;
    private VehiclePropListAdapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;
    private Button vehicelHalButton;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        ViewGroup root = (ViewGroup)inflater.inflate(R.layout.fragment_vehicle_hal_generic, null);
        mRecyclerView = root.findViewById(R.id.my_recycler_view);
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
                logErrorWithToast(getActivity(),"Failed to register service start notification", null);
            }
        } catch (RemoteException e) {
            Log.d(TAG, "Failed to register service start notification", e);
        }
        connectToVehicleHal();
        vehicelHalButton.setOnClickListener(event -> {
            if (vehicleHal != null && mAdapter != null){
                mAdapter.changeItem();
            }
        });
        return root;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        //you can set the title for your toolbar here for different fragments different titles
        getActivity().setTitle(TITLE);
    }

    private void connectToVehicleHal() {
        synchronized (mLock) {
            if (vehicleManager != null) {
                return;
            }
            try {
                vehicleManager = IVehicle.getService();
                if (vehicleManager != null) {
                    vehicleManager.linkToDeath(vehicleManagerDeathRecipient, 0);
                } else {
                    logErrorWithToast(getActivity(),"VehicleHal Manager HAL service is not available.", null);
                    return;
                }
                updateUI(() -> {
                    vehicleHal = new VehicleHal(vehicleManager);
                    mAdapter = new VehiclePropListAdapter(vehicleHal, mRecyclerView, this);
                    mRecyclerView.setAdapter(mAdapter);
                });
                List<VehiclePropConfig> list = new Vector<>(vehicleHal.getAllPropConfigsDirect());

                mAdapter.addDataList(list);
            } catch (RemoteException e) {
                logErrorWithToast(getActivity(),"VehicleHal service not responding", e);
            } catch (NoSuchElementException e) {
                logErrorWithToast(getActivity(), "VehicleHal service not registered yet",e);
            } catch (Exception e){
                logErrorWithToast(getActivity(), "Something happened",e);
            }
        }
    }

    /**
     * Validation and crash handling
     */

    private class VehicleHalDeathRecipient implements IHwBinder.DeathRecipient {
        private int deathCount = 0;

        @Override
        public void serviceDied(long cookie) {
            Log.w(TAG, "vehicleHal Manager HAL died.");

            // Remove a previously registered death notification. The recipient will no longer be
            // called if this object dies.
            try {
                vehicleManager.unlinkToDeath(this);
            } catch (RemoteException e) {
                logErrorWithToast(getActivity(),"Failed to unlinkToDeath", e);  // TAG and continue.
            }
            vehicleManager = null;
            Log.i(TAG, "Notifying vehicleHal Manager HAL reconnected...");
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
