/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance;

import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;
import android.arch.lifecycle.ViewModel;
import android.util.Log;

import com.volvocars.carconfig.CarConfigApi;
import com.volvocars.carconfig.CarConfigEnums;
import com.volvocars.vehiclefunctions.R;
import com.volvocars.vehiclefunctions.assistance.delegates.VhalIntegerDelegate;
import com.volvocars.vehiclefunctions.assistance.delegates.VhalOnOffDelegate;
import com.volvocars.vehiclefunctions.assistance.delegates.VhalThreeStateDelegate;
import com.volvocars.vehiclefunctions.assistance.functions.Function;
import com.volvocars.vehiclefunctions.assistance.functions.IntegerFunction;
import com.volvocars.vehiclefunctions.assistance.functions.OnOffFunction;
import com.volvocars.vehiclefunctions.assistance.functions.Section;
import com.volvocars.vehiclefunctions.assistance.functions.ThreeStateFunction;
import com.volvocars.vendorextension.VendorExtensionClient;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

public class AssistanceViewModel extends ViewModel {
    private static final String TAG = AssistanceViewModel.class.getSimpleName();

    private final VendorExtensionClient mVendorExtensionClient;
    private final MutableLiveData<List<Section>> mSections = new MutableLiveData<List<Section>>();

    public AssistanceViewModel(VendorExtensionClient vendorExtensionClient) {
        mVendorExtensionClient = vendorExtensionClient;
        CompletableFuture.runAsync(() -> {
            try {
                mSections.postValue(createSections());
            } catch (Exception e) {
                Log.e(TAG, "Exception in createsections: " + Log.getStackTraceString(e));
            }
        });
    }

    public LiveData<List<Section>> getSections() {
        return mSections;
    }

    private List<Section> createSections() {
        List<Integer> supportedFeatures = mVendorExtensionClient.getSupportedFeatures();
        ArrayList<Section> sections = new ArrayList<>();

        {
            List<Function> driveAwayFunctions = new ArrayList<Function>();
            if (supportedFeatures.contains(VehicleProperty.DAI_SETTING)) {
                driveAwayFunctions.add(new ThreeStateFunction("Notification Control",
                        "No Info", "Visual Only", "Visual and Sound",
                        new VhalThreeStateDelegate(mVendorExtensionClient,
                                VehicleProperty.DAI_SETTING),
                        R.id.dai_state_1_no_info_button,
                        R.id.dai_state_2_visual_only_button,
                        R.id.dai_state_3_visual_and_audio_button));
            }
            sections.add(new Section("Drive Away Information", driveAwayFunctions));
        }

        {
            List<Function> intelliSafeFunctions = new ArrayList<Function>();
            if (supportedFeatures.contains(VehicleProperty.CONNECTED_SAFETY_ON)) {
                intelliSafeFunctions.add(new OnOffFunction("Connected Safety",
                        new VhalOnOffDelegate(mVendorExtensionClient,
                                VehicleProperty.CONNECTED_SAFETY_ON),
                        R.id.connected_safety_button_on, R.id.connected_safety_button_off));
            }
            sections.add(new Section("Intellisafe", intelliSafeFunctions));
        }

        {
            List<Function> driverSupportFunctions = new ArrayList<Function>();
            if (supportedFeatures.contains(VehicleProperty.DRIVER_SUPPORT_FUNCTION_ON)) {
                driverSupportFunctions.add(new ThreeStateFunction("Driver Support Functions",
                        "Cruise Control", "Adaptive Cruise Control", "Speed Limiter",
                        new VhalThreeStateDelegate(mVendorExtensionClient,
                                VehicleProperty.DRIVER_SUPPORT_FUNCTION_ON,
                                VehicleProperty.DRIVER_SUPPORT_FUNCTION_STATUS,
                                true),
                        R.id.cruise_control_on, R.id.adaptive_cruise_control_on, R.id.speed_limiter_on));
            }
            sections.add(new Section("Driver Support Function", driverSupportFunctions));
        }

        {
            List<Function> lkaFunctions = new ArrayList<Function>();
            if (supportedFeatures.contains(VehicleProperty.LANE_KEEPING_AID_ON)) {
                lkaFunctions.add(new OnOffFunction("Lane Keeping Aid",
                        new VhalOnOffDelegate(mVendorExtensionClient,
                                VehicleProperty.LANE_KEEPING_AID_ON, VehicleProperty.LANE_KEEPING_AID_ON_STATUS),
                        R.id.lane_keeping_aid_button_on, R.id.lane_keeping_aid_button_off));
            }
            if (supportedFeatures.contains(VehicleProperty.LANE_KEEPING_AID_MODE)) {
                lkaFunctions.add(new ThreeStateFunction("Lane Keeping Aid mode",
                        "Both", "Steering", "Sound",
                        new VhalThreeStateDelegate(mVendorExtensionClient,
                                VehicleProperty.LANE_KEEPING_AID_MODE, VehicleProperty.LANE_KEEPING_AID_MODE_STATUS),
                        R.id.lane_keeping_aid_button_mode_both,
                        R.id.lane_keeping_aid_button_mode_steering,
                        R.id.lane_keeping_aid_button_mode_sound));
            }
            if (supportedFeatures.contains(VehicleProperty.EMERGENCY_LANE_KEEPING_AID_ON)) {
                lkaFunctions.add(new OnOffFunction("Emergency Lane Keeping Aid",
                        new VhalOnOffDelegate(mVendorExtensionClient,
                                VehicleProperty.EMERGENCY_LANE_KEEPING_AID_ON, VehicleProperty.EMERGENCY_LANE_KEEPING_AID_STATUS),
                        R.id.emergency_lane_keeping_aid_button_on,
                        R.id.emergency_lane_keeping_aid_button_off));
            }
            sections.add(new Section("Lane Keeping Aid", lkaFunctions));
        }
        {
            List<Function> csaFunctions = new ArrayList<Function>();
            if (supportedFeatures.contains(VehicleProperty.CURVE_SPEED_ADAPTION_ON)) {
                csaFunctions.add(new OnOffFunction("Curve Speed Adaption",
                        new VhalOnOffDelegate(mVendorExtensionClient,
                                VehicleProperty.CURVE_SPEED_ADAPTION_ON, VehicleProperty.CURVE_SPEED_ADAPTION_STATUS),
                        R.id.curve_speed_adaption_on, R.id.curve_speed_adaption_off));
            }

            sections.add(new Section("Curve Speed Adaption", csaFunctions));
        }

        {
            int tsi_offset_max;
            int tsi_offset_min = 0;
            CarConfigEnums.CC_1_VehicleType carConfig_1 = CarConfigApi.getValue(
                    CarConfigEnums.CC_1_VehicleType.class);

            // Get the TSI Speet Alert Offset based on Car Config
            if (carConfig_1.value < CarConfigEnums.CC_1_VehicleType.V316.value) {
                tsi_offset_max = 20;
            } else {
                tsi_offset_max = 10;
            }

            List<Function> speedManagementFunctions = new ArrayList<Function>();
            if (supportedFeatures.contains(VehicleProperty.TSI_RSI_ON)) {
                speedManagementFunctions.add(new OnOffFunction("Road Sign Information",
                        new VhalOnOffDelegate(mVendorExtensionClient, VehicleProperty.TSI_RSI_ON, VehicleProperty.TSI_RSI_STATUS),
                        R.id.tsi_rsi_button_on, R.id.tsi_rsi_button_off));
            }
            if (supportedFeatures.contains(VehicleProperty.TSI_SPEEDCAM_AUDIO_WARN_ON)) {
                speedManagementFunctions.add(new OnOffFunction("Speed Camera Warning",
                        new VhalOnOffDelegate(mVendorExtensionClient,
                                VehicleProperty.TSI_SPEEDCAM_AUDIO_WARN_ON, VehicleProperty.TSI_SPEEDCAM_AUDIO_WARN_STATUS),
                        R.id.tsi_spdCamWarn_button_on, R.id.tsi_spdCamWarn_button_off));
            }
            if (supportedFeatures.contains(VehicleProperty.TSI_SPEED_VISUAL_WARN_ON)) {
                speedManagementFunctions.add(new OnOffFunction("Speed Alert Visual Warning",
                        new VhalOnOffDelegate(mVendorExtensionClient,
                                VehicleProperty.TSI_SPEED_VISUAL_WARN_ON, VehicleProperty.TSI_SPEED_VISUAL_WARN_STATUS),
                        R.id.tsi_spdAlrtVisWarn_button_on, R.id.tsi_spdAlrtVisWarn_button_off));
            }
            if (supportedFeatures.contains(VehicleProperty.TSI_SPEED_AUDIO_WARN_ON)) {
                speedManagementFunctions.add(new OnOffFunction("Speed Alert Sound Warning",
                        new VhalOnOffDelegate(mVendorExtensionClient,
                                VehicleProperty.TSI_SPEED_AUDIO_WARN_ON, VehicleProperty.TSI_SPEED_AUDIO_WARN_STATUS),
                        R.id.tsi_spdAlrtSndWarn_button_on, R.id.tsi_spdAlrtSndWarn_button_off));
            }
            if (supportedFeatures.contains(VehicleProperty.TSI_SPEED_WARN_OFFSET)) {
                speedManagementFunctions.add(new IntegerFunction("Speed Alert Offset",
                        new VhalIntegerDelegate(mVendorExtensionClient,
                                VehicleProperty.TSI_SPEED_WARN_OFFSET, VehicleProperty.TSI_SPEED_WARN_OFFSET_STATUS),
                        R.id.tsi_offset_button_plus, R.id.tsi_offset_text,
                        R.id.tsi_offset_button_minus, tsi_offset_min, tsi_offset_max, 5));
            }

            sections.add(new Section("Speed Management", speedManagementFunctions));
        }


        {
            List<Function> ldwFunctions = new ArrayList<Function>();
            if (supportedFeatures.contains(VehicleProperty.LANE_DEPARTURE_WARNING_ON)) {
                ldwFunctions.add(new OnOffFunction("Lane Departure Warning",
                        new VhalOnOffDelegate(mVendorExtensionClient,
                                VehicleProperty.LANE_DEPARTURE_WARNING_ON, VehicleProperty.LANE_DEPARTURE_WARNING_STATUS),
                        R.id.lane_departureWarning_button_on, R.id.lane_departureWarning_button_off));
            }

            sections.add(new Section("Lane Departure Warning", ldwFunctions));
        }


        return sections;

    }
}
