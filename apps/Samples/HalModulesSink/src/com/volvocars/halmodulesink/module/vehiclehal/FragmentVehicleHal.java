/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module.vehiclehal;

import android.app.Activity;
import android.hardware.automotive.vehicle.V2_0.IVehicle;
import android.hardware.automotive.vehicle.V2_0.VehiclePropConfig;
import android.hardware.automotive.vehicle.V2_0.VehiclePropValue;
import android.hardware.automotive.vehicle.V2_0.VehicleProperty;
import android.os.Bundle;
import android.os.RemoteException;
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
import com.volvocars.test.lib.vehiclehal.PropertyTimeoutException;
import com.volvocars.test.lib.vehiclehal.VehicleHal;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.NoSuchElementException;

import static java.util.Objects.nonNull;

public class FragmentVehicleHal extends AModuleFragment {
    public static final String TAG = FragmentVehicleHal.class.getSimpleName();
    public static final String TITLE = "VehicleHal";
    public static final String logCatregex = "logcat -d com.volvocars.halmodulesink:ERROR *:S";
    private IVehicle vehicleManager;
    private String vehicleManagerInterfaceName;
    private EditText editPropId;
    private Button setPropButton;
    private EditText editData;
    private volatile TextView terminalLog;
    private Activity activity;
    private VehicleHal vehicleHal;

    @Nullable
    private static IVehicle getVehicle() {
        try {
            return IVehicle.getService();
        } catch (RemoteException e) {
            Log.e(TAG, "Failed to get IVehicle service", e);
        } catch (NoSuchElementException e) {
            Log.e(TAG, "IVehicle service not registered yet");
        }
        return null;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        ViewGroup root = (ViewGroup) inflater.inflate(R.layout.fragment_vehicle_hal, null);
        activity = getActivity();
        terminalLog = (TextView) root.findViewById(R.id.log);
        editPropId = (EditText) root.findViewById(R.id.vehicleHalPropIdEdit);
        editData = (EditText) root.findViewById(R.id.vehicleHalStringData);
        setPropButton = (Button) root.findViewById(R.id.vehicleHalSetButton);
        setPropButton.setOnClickListener(event -> {
            try {
                setProp(Integer.valueOf(editPropId.getText().toString()),
                        editPropId.getText().toString());
            } catch (NumberFormatException e){
                if (vehicleManager == null) {
                    logErrorWithToast(getActivity(), "Input should be integer", e);
                }
            }
        });
        terminalLog.setText("Getting Vehicle…\n");
        vehicleManager = getVehicle();

        if (vehicleManager == null) {
            Log.e(TAG, "Vehicle HAL service is not available.");
            terminalLog.setText("Vehicle HAL service is not available.");
            return root;
        }
        try {
            vehicleManagerInterfaceName = vehicleManager.interfaceDescriptor();
        } catch (RemoteException e) {
            throw new IllegalStateException("Unable to get Vehicle HAL interface descriptor", e);
        }
        vehicleHal = new VehicleHal(vehicleManager);

        Log.i(TAG, "Connected to " + vehicleManagerInterfaceName);

        runBackgroundAndUpdate(() -> {

            StringBuilder logBuilder = new StringBuilder();
            ArrayList<VehiclePropConfig> allPropConfig = null;
            try {
                allPropConfig = getAllPropConfigs();
                allPropConfig.forEach(vehiclePropConfig -> {
                    logBuilder.append("\n");
                    logBuilder.append(vehiclePropConfig.toString());
                    logBuilder.append("\n");
                });
            } catch (RemoteException e) {
                Log.e(TAG, "Problems with getting all propConfig", e);
            }

            try {
                logBuilder.append("\n\n");
                logBuilder.append("VIN NUMBER: ");
                logBuilder.append(getVIN());
                logBuilder.append("\n");
            } catch (Exception e) {
                logBuilder.append("Problems with getting all VIN Number, exception:");
                logBuilder.append(e.toString());
                logBuilder.append("\n\n");
                Log.e(TAG, "Problems with getting all VIN Number", e);
                printLogCat();
            }
            activity.runOnUiThread(() -> {
                terminalLog.setText(terminalLog.getText().toString() + logBuilder);
                terminalLog.invalidate();
            });
        });

        runBackgroundAndUpdate(() -> {
            StringBuilder logBuilder = new StringBuilder();
            ArrayList<VehiclePropConfig> allPropConfig = null;
            try {
                allPropConfig = getAllPropConfigs();
            } catch (RemoteException e) {
                Log.e(TAG, "Problems with getting all propConfig", e);
            }

            logBuilder.append("\n\n");
            logBuilder.append("PropValue NUMBER: ");

            if (nonNull(allPropConfig)) {
                allPropConfig.forEach(vehiclePropConfig -> {
                    try {
                        VehiclePropValue value = vehicleHal.get(vehiclePropConfig.prop);
                        logBuilder.append("\n");
                        logBuilder.append((nonNull(value)) ? value.toString()
                                : "got Null using prop value:" + vehiclePropConfig.prop);
                        logBuilder.append("\n");
                    } catch (Exception e) {
                        logBuilder.append("Problems with getting all value Number, exception:");
                        logBuilder.append(e.toString());
                        logBuilder.append("\n\n");
                        Log.e(TAG, "Problems with getting all value Number", e);
                    }
                });
            }
            Log.d(TAG, "done executing for the prop values");
            printLogCat();
            activity.runOnUiThread(() -> {
                terminalLog.setText(terminalLog.getText().toString() + logBuilder);
                terminalLog.invalidate();
            });
        });

        return root;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
//        super.onViewCreated(view, savedInstanceState);
        //you can set the title for your toolbar here for different fragments different titles
        getActivity().setTitle(TITLE);
    }

    ArrayList<VehiclePropConfig> getAllPropConfigs() throws RemoteException {
        return vehicleManager.getAllPropConfigs();
    }

    public String getVIN() throws RemoteException {
        String value = "";
        try {
            value = vehicleHal.get(String.class, VehicleProperty.INFO_VIN);
        } catch (PropertyTimeoutException e) {
            logErrorWithToast(getActivity(), "Unable to read property", e);
        }
        return value;
    }

    public void setProp(int propId, String data) {
        Log.d(TAG, "setProp called");
        VehiclePropValue value = new VehiclePropValue();
        value.prop = propId;
        value.value.stringValue = data;
        try {
            vehicleHal.set(value);
        } catch (PropertyTimeoutException e) {
            Log.e(TAG, "Cannot set value, exception: ", e);
        }
    }


    /**
     * Print out log cat
     */
    public void printLogCat() {
        try {
            Process process = Runtime.getRuntime().exec(logCatregex);
            BufferedReader bufferedReader = new BufferedReader(
                    new InputStreamReader(process.getInputStream()));

            StringBuilder log = new StringBuilder();
            log.append("Log Cat:\n");
            String line = "";
            while ((line = bufferedReader.readLine()) != null) {
                log.append(line);
            }
            updateUI(() -> {
                terminalLog.append(log);
            });
        } catch (IOException e) {
        }
    }
}
