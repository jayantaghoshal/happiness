/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module;

import android.car.hardware.CarVendorExtensionManager;
import android.hardware.automotive.vehicle.V2_0.VehicleArea;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import com.volvocars.halmodulesink.R;
import com.volvocars.test.lib.AModuleFragment;
import com.volvocars.test.lib.ModuleVendorExtension;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

public class FragmentVendorExtension extends AModuleFragment {
    public static final String TAG = FragmentVendorExtension.class.getSimpleName();
    public static final String TITLE = "VendorExtension";
    private ModuleVendorExtension moduleVendorExtensionM;
    private EditText testTextView;
    private StringBuilder text = new StringBuilder();

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        ViewGroup root = (ViewGroup) inflater.inflate(R.layout.fragment_vendor_extension, null);
        moduleVendorExtensionM = new ModuleVendorExtension(getActivity());
        Button setting0Button = (Button) root.findViewById(R.id.buttonSettingOff);
        Button setting1Button = (Button) root.findViewById(R.id.buttonSettingVisual);
        Button setting2Button = (Button) root.findViewById(R.id.buttonSettingVisualAndSound);
        Button settingCsaButtonOff = (Button) root.findViewById(R.id.buttonCsaOff);
        Button settingCsaButtonOn = (Button) root.findViewById(R.id.buttonCsaOn);
        testTextView = (EditText) root.findViewById(R.id.testLog);

        setting0Button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                text.append("Connecting to the car Service...\n");
                testTextView.setText(text);
                Log.d(TAG, "setOnClickListener()");
                runBackgroundAndUpdate(() -> {
                    try {
                        CarVendorExtensionManager carVEManager =
                                moduleVendorExtensionM.getCarVEManager();

                        carVEManager.setProperty(Integer.class,
                                VehicleProperty.DAI_SETTING,
                                VehicleArea.GLOBAL,
                                0);
                        text.append("Value set to: " + carVEManager.getProperty(Integer.class,
                                VehicleProperty.DAI_SETTING, VehicleArea.GLOBAL) + "\n\n");
                    } catch (Exception e) {
                        Log.e(TAG, "Error", e);
                    }
                    updateUI(() -> {
                        testTextView.setText(text);
                        testTextView.invalidate();
                    });

                });
            }
        });
        setting1Button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                text.append("Connecting to the car Service...\n");
                testTextView.setText(text);
                Log.d(TAG, "setOnClickListener()");
                runBackgroundAndUpdate(() -> {
                    try {
                        CarVendorExtensionManager carVEManager =
                                moduleVendorExtensionM.getCarVEManager();

                        carVEManager.setProperty(Integer.class,
                                VehicleProperty.DAI_SETTING,
                                VehicleArea.GLOBAL,
                                1);

                        text.append("Value set to: " + carVEManager.getProperty(Integer.class,
                                VehicleProperty.DAI_SETTING, VehicleArea.GLOBAL) + "\n\n");

                    } catch (Exception e) {
                        Log.e(TAG, "Error", e);
                    }
                    updateUI(() -> {
                        testTextView.setText(text);
                        testTextView.invalidate();
                    });

                });

            }
        });
        setting2Button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                text.append("Connecting to the car Service...\n");
                testTextView.setText(text);
                Log.d(TAG, "setOnClickListener()");
                runBackgroundAndUpdate(() -> {
                    try {

                        CarVendorExtensionManager carVEManager =
                                moduleVendorExtensionM.getCarVEManager();

                        carVEManager.setProperty(Integer.class,
                                VehicleProperty.DAI_SETTING,
                                VehicleArea.GLOBAL,
                                2);
                        text.append("Value set to: " + carVEManager.getProperty(Integer.class,
                                VehicleProperty.DAI_SETTING, VehicleArea.GLOBAL) + "\n\n");

                    } catch (Exception e) {
                        Log.e(TAG, "Error", e);
                    }
                    updateUI(() -> {
                        testTextView.setText(text);
                        testTextView.invalidate();
                    });
                });


            }
        });
        settingCsaButtonOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                text.append("Connecting to the car Service...\n");
                testTextView.setText(text);
                Log.d(TAG, "setOnClickListener()");
                runBackgroundAndUpdate(() -> {
                    try {
                        CarVendorExtensionManager carVEManager =
                                moduleVendorExtensionM.getCarVEManager();

                        carVEManager.setProperty(Integer.class,
                                VehicleProperty.DAI_SETTING,
                                VehicleArea.GLOBAL,
                                0);

                        text.append("Value set to: " + carVEManager.getProperty(Integer.class,
                                VehicleProperty.DAI_SETTING, VehicleArea.GLOBAL) + "\n\n");

                    } catch (Exception e) {
                        Log.e(TAG, "Error", e);
                    }
                    updateUI(() -> {
                        testTextView.setText(text);
                        testTextView.invalidate();
                    });

                });

            }
        });
        settingCsaButtonOn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                text.append("Connecting to the car Service...\n");
                testTextView.setText(text);
                Log.d(TAG, "setOnClickListener()");
                runBackgroundAndUpdate(() -> {
                    try {
                        CarVendorExtensionManager carVEManager =
                                moduleVendorExtensionM.getCarVEManager();

                        carVEManager.setProperty(Integer.class,
                                VehicleProperty.DAI_SETTING,
                                VehicleArea.GLOBAL,
                                1);

                        text.append("Value set to: " + carVEManager.getProperty(Integer.class,
                                VehicleProperty.DAI_SETTING, VehicleArea.GLOBAL) + "\n\n");

                    } catch (Exception e) {
                        Log.e(TAG, "Error", e);
                    }
                    updateUI(() -> {
                        testTextView.setText(text);
                        testTextView.invalidate();
                    });

                });

            }
        });
        return root;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        getActivity().setTitle(TITLE);
    }


}