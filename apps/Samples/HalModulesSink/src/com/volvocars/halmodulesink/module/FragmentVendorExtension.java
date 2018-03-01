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
        Button settingDaiOff = (Button) root.findViewById(R.id.buttonSettingOff);
        Button settingDaiVisual = (Button) root.findViewById(R.id.buttonSettingVisual);
        Button settingDaiVisualSound = (Button) root.findViewById(R.id.buttonSettingVisualAndSound);
        Button settingCsaButtonOff = (Button) root.findViewById(R.id.buttonCsaOff);
        Button settingCsaButtonOn = (Button) root.findViewById(R.id.buttonCsaOn);
        Button settingConnSafetyOff = (Button) root.findViewById(R.id.buttonConnSafetyOff);
        Button settingConnSafetyOn = (Button) root.findViewById(R.id.buttonConnSafetyOn);

        testTextView = (EditText) root.findViewById(R.id.testLog);
        settingDaiOff.setVisibility(View.INVISIBLE);
        settingDaiVisual.setVisibility(View.INVISIBLE);
        settingDaiVisualSound.setVisibility(View.INVISIBLE);
        settingCsaButtonOff.setVisibility(View.INVISIBLE);
        settingCsaButtonOn.setVisibility(View.INVISIBLE);
        settingConnSafetyOff.setVisibility(View.INVISIBLE);
        settingConnSafetyOn.setVisibility(View.INVISIBLE);

        runBackgroundAndUpdate(() -> {
            try {
                CarVendorExtensionManager carVEManager = moduleVendorExtensionM.getCarVEManager();
                if (moduleVendorExtensionM.isFeatureAvailable(Integer.class, VehicleProperty.DAI_SETTING, VehicleArea.GLOBAL)){
                    updateUI(()-> {
                        settingDaiOff.setVisibility(View.VISIBLE);
                        settingDaiVisual.setVisibility(View.VISIBLE);
                        settingDaiVisualSound.setVisibility(View.VISIBLE);
                    });
                }
                if (moduleVendorExtensionM.isFeatureAvailable(Boolean.class, VehicleProperty.CURVE_SPEED_ADAPTION_ON, VehicleArea.GLOBAL)){
                    updateUI(()-> {
                        settingCsaButtonOff.setVisibility(View.VISIBLE);
                        settingCsaButtonOn.setVisibility(View.VISIBLE);
                    });
                }
                if (moduleVendorExtensionM.isFeatureAvailable(Integer.class, VehicleProperty.CONNECTED_SAFETY_ON, VehicleArea.GLOBAL)){
                    updateUI(()-> {
                        settingConnSafetyOff.setVisibility(View.VISIBLE);
                        settingConnSafetyOn.setVisibility(View.VISIBLE);
                    });
                }
            } catch (Exception e) {
                Log.e(TAG, "Error", e);
            }
        });

        settingDaiOff.setOnClickListener(new View.OnClickListener() {
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
        settingDaiVisual.setOnClickListener(new View.OnClickListener() {
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
        settingDaiVisualSound.setOnClickListener(new View.OnClickListener() {
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

                        carVEManager.setProperty(Boolean.class,
                                VehicleProperty.CURVE_SPEED_ADAPTION_ON,
                                VehicleArea.GLOBAL,
                                false);

                        text.append("Value set to: " + carVEManager.getProperty(Boolean.class,
                                VehicleProperty.CURVE_SPEED_ADAPTION_ON, VehicleArea.GLOBAL) + "\n\n");

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

                        carVEManager.setProperty(Boolean.class,
                                VehicleProperty.CURVE_SPEED_ADAPTION_ON,
                                VehicleArea.GLOBAL,
                                true);

                        text.append("Value set to: " + carVEManager.getProperty(Boolean.class,
                                VehicleProperty.CURVE_SPEED_ADAPTION_ON, VehicleArea.GLOBAL) + "\n\n");

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
        settingConnSafetyOff.setOnClickListener(new View.OnClickListener() {
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
                                VehicleProperty.CONNECTED_SAFETY_ON,
                                VehicleArea.GLOBAL,
                                0);

                        text.append("Value set to: " + carVEManager.getProperty(Integer.class,
                                VehicleProperty.CONNECTED_SAFETY_ON, VehicleArea.GLOBAL) + "\n\n");

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
        settingConnSafetyOn.setOnClickListener(new View.OnClickListener() {
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
                                VehicleProperty.CONNECTED_SAFETY_ON,
                                VehicleArea.GLOBAL,
                                1);

                        text.append("Value set to: " + carVEManager.getProperty(Integer.class,
                                VehicleProperty.CONNECTED_SAFETY_ON, VehicleArea.GLOBAL) + "\n\n");

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