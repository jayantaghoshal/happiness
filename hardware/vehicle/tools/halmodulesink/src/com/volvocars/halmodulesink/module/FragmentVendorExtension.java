/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.halmodulesink.module;

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
import com.volvocars.vendorextension.VendorExtensionClient;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

public class FragmentVendorExtension extends AModuleFragment {
    public static final String TAG = FragmentVendorExtension.class.getSimpleName();
    public static final String TITLE = "VendorExtension";
    private VendorExtensionClient vendorExtensionClient;
    private EditText testTextView;
    private StringBuilder text = new StringBuilder();

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        ViewGroup root = (ViewGroup) inflater.inflate(R.layout.fragment_vendor_extension, null);
        vendorExtensionClient = VendorExtensionClient.getVolvoExtension(getContext());
        Button settingDaiOff = root.findViewById(R.id.buttonSettingOff);
        Button settingDaiVisual = root.findViewById(R.id.buttonSettingVisual);
        Button settingDaiVisualSound = root.findViewById(R.id.buttonSettingVisualAndSound);
        Button settingCsaButtonOff = root.findViewById(R.id.buttonCsaOff);
        Button settingCsaButtonOn = root.findViewById(R.id.buttonCsaOn);
        Button settingConnSafetyOff = root.findViewById(R.id.buttonConnSafetyOff);
        Button settingConnSafetyOn = root.findViewById(R.id.buttonConnSafetyOn);

        testTextView = root.findViewById(R.id.testLog);
        settingDaiOff.setVisibility(View.INVISIBLE);
        settingDaiVisual.setVisibility(View.INVISIBLE);
        settingDaiVisualSound.setVisibility(View.INVISIBLE);
        settingCsaButtonOff.setVisibility(View.INVISIBLE);
        settingCsaButtonOn.setVisibility(View.INVISIBLE);
        settingConnSafetyOff.setVisibility(View.INVISIBLE);
        settingConnSafetyOn.setVisibility(View.INVISIBLE);

        // Dummy

        runBackgroundAndUpdate(() -> {
            try {
                if (vendorExtensionClient.isFeatureAvailable(VehicleProperty.DAI_SETTING)) {
                    updateUI(() -> {
                        settingDaiOff.setVisibility(View.VISIBLE);
                        settingDaiVisual.setVisibility(View.VISIBLE);
                        settingDaiVisualSound.setVisibility(View.VISIBLE);
                    });
                }
                if (vendorExtensionClient.isFeatureAvailable(VehicleProperty.CURVE_SPEED_ADAPTION_ON)) {
                    updateUI(() -> {
                        settingCsaButtonOff.setVisibility(View.VISIBLE);
                        settingCsaButtonOn.setVisibility(View.VISIBLE);
                    });
                }
                if (vendorExtensionClient.isFeatureAvailable(VehicleProperty.CONNECTED_SAFETY_ON)) {
                    updateUI(() -> {
                        settingConnSafetyOff.setVisibility(View.VISIBLE);
                        settingConnSafetyOn.setVisibility(View.VISIBLE);
                    });
                }
            } catch (Exception e) {
                Log.e(TAG, "Error", e);
            }
        });

        settingDaiOff.setOnClickListener(v -> {
            text.append("Setting value to 0\n");
            testTextView.setText(text);
            Log.d(TAG, "setOnClickListener()");
            runBackgroundAndUpdate(() -> {
                try {
                    vendorExtensionClient.set(
                            VehicleProperty.DAI_SETTING,
                            0);
                    text.append("Hal setting returned:  " + vendorExtensionClient.get(
                            VehicleProperty.DAI_SETTING) + "\n\n");
                } catch (Exception e) {
                    Log.e(TAG, "Error", e);
                }
                updateUI(() -> {
                    testTextView.setText(text);
                    testTextView.invalidate();
                });
            });
        });

        settingDaiVisual.setOnClickListener(v -> {

            text.append("Setting value to 1\n");
            testTextView.setText(text);
            Log.d(TAG, "setOnClickListener()");
            runBackgroundAndUpdate(() -> {
                try {
                    vendorExtensionClient.set(
                            VehicleProperty.DAI_SETTING,
                            1);

                    text.append("Hal setting returned:  " + vendorExtensionClient.get(
                            VehicleProperty.DAI_SETTING) + "\n\n");

                } catch (Exception e) {
                    Log.e(TAG, "Error", e);
                }
                updateUI(() -> {
                    testTextView.setText(text);
                    testTextView.invalidate();
                });
            });
        });
        settingDaiVisualSound.setOnClickListener(v -> {

            text.append("Setting value to 2\n\n");
            testTextView.setText(text);
            Log.d(TAG, "setOnClickListener()");
            runBackgroundAndUpdate(() -> {
                try {

                    vendorExtensionClient.set(
                            VehicleProperty.DAI_SETTING,
                            2);
                    text.append("Hal setting returned:  " + vendorExtensionClient.get(
                            VehicleProperty.DAI_SETTING) + "\n\n");

                } catch (Exception e) {
                    Log.e(TAG, "Error", e);
                }
                updateUI(() -> {
                    testTextView.setText(text);
                    testTextView.invalidate();
                });
            });
        });
        settingCsaButtonOff.setOnClickListener(v -> {

            text.append("Setting value to false\n");
            testTextView.setText(text);
            Log.d(TAG, "setOnClickListener()");
            runBackgroundAndUpdate(() -> {
                try {

                    vendorExtensionClient.set(
                            VehicleProperty.CURVE_SPEED_ADAPTION_ON,
                            false);

                    text.append("Hal setting returned:  " + vendorExtensionClient.get(
                            VehicleProperty.CURVE_SPEED_ADAPTION_ON) + "\n\n");

                } catch (Exception e) {
                    Log.e(TAG, "Error", e);
                }
                updateUI(() -> {
                    testTextView.setText(text);
                    testTextView.invalidate();
                });
            });
        });
        settingCsaButtonOn.setOnClickListener(v -> {

            text.append("Setting value to true\n");
            testTextView.setText(text);
            Log.d(TAG, "setOnClickListener()");
            runBackgroundAndUpdate(() -> {
                try {

                    vendorExtensionClient.set(
                            VehicleProperty.CURVE_SPEED_ADAPTION_ON,
                            true);

                    text.append("Hal setting returned:  " + vendorExtensionClient.get(
                            VehicleProperty.CURVE_SPEED_ADAPTION_ON) + "\n\n");

                } catch (Exception e) {
                    Log.e(TAG, "Error", e);
                }
                updateUI(() -> {
                    testTextView.setText(text);
                    testTextView.invalidate();
                });
            });
        });
        settingConnSafetyOff.setOnClickListener(v -> {

            text.append("Setting value set to 0\n");
            testTextView.setText(text);
            Log.d(TAG, "setOnClickListener()");
            runBackgroundAndUpdate(() -> {
                try {

                    vendorExtensionClient.set(
                            VehicleProperty.CONNECTED_SAFETY_ON,
                            0);

                    text.append("Hal setting returned:  " + vendorExtensionClient.get(
                            VehicleProperty.CONNECTED_SAFETY_ON) + "\n\n");

                } catch (Exception e) {
                    Log.e(TAG, "Error", e);
                }
                updateUI(() -> {
                    testTextView.setText(text);
                    testTextView.invalidate();
                });
            });
        });
        settingConnSafetyOn.setOnClickListener(v -> {

            text.append("Setting value set to 1\n");
            testTextView.setText(text);
            Log.d(TAG, "setOnClickListener()");
            runBackgroundAndUpdate(() -> {
                try {
                    vendorExtensionClient.set(
                            VehicleProperty.CONNECTED_SAFETY_ON,
                            1);

                    text.append("Hal setting returned:  " + vendorExtensionClient.get(
                            VehicleProperty.CONNECTED_SAFETY_ON) + "\n\n");

                } catch (Exception e) {
                    Log.e(TAG, "Error", e);
                }
                updateUI(() -> {
                    testTextView.setText(text);
                    testTextView.invalidate();
                });
            });
        });
        return root;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        getActivity().setTitle(TITLE);
    }

}