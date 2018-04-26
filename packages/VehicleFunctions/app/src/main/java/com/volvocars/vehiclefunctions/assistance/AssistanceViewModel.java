/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.vehiclefunctions.assistance;

import android.arch.lifecycle.ViewModel;

import com.volvocars.vehiclefunctions.R;
import com.volvocars.vehiclefunctions.assistance.delegates.VhalOnOffDelegate;
import com.volvocars.vehiclefunctions.assistance.delegates.VhalOneButtonDelegate;
import com.volvocars.vehiclefunctions.assistance.delegates.VhalThreeStateDelegate;
import com.volvocars.vehiclefunctions.assistance.functions.OnOffFunction;
import com.volvocars.vehiclefunctions.assistance.functions.OneButtonFunction;
import com.volvocars.vehiclefunctions.assistance.functions.Section;
import com.volvocars.vehiclefunctions.assistance.functions.ThreeStateFunction;
import com.volvocars.vendorextension.VendorExtensionClient;

import java.util.ArrayList;
import java.util.List;

import vendor.volvocars.hardware.vehiclehal.V1_0.VehicleProperty;

public class AssistanceViewModel extends ViewModel {

    private final VendorExtensionClient mVendorExtensionClient;
    private List<Section> mSections;

    public AssistanceViewModel(VendorExtensionClient vendorExtensionClient) {
        mVendorExtensionClient = vendorExtensionClient;
    }

    public List<Section> getSections() {
        if (mSections == null) {
            mSections = createSections();
        }
        return mSections;
    }

    private List<Section> createSections() {

        Section driveAwayInformationSection = new Section.Builder()
                .setTitle("Drive Away Information")
                .addFunction(new ThreeStateFunction("Notification Control",
                        "No Info", "Visual Only", "Visual and Sound",
                        new VhalThreeStateDelegate(mVendorExtensionClient, VehicleProperty.DAI_SETTING),
                        R.id.dai_state_1_no_info_button, R.id.dai_state_2_visual_only_button, R.id.dai_state_3_visual_and_audio_button))
                .create();

        Section intellisafeSection = new Section.Builder()
                .setTitle("Intellisafe")
                .addFunction(new OnOffFunction("Connected Safety", new VhalOnOffDelegate(mVendorExtensionClient, VehicleProperty.CONNECTED_SAFETY_ON),
                        R.id.connected_safety_button_on, R.id.connected_safety_button_off))
                .create();

        Section driverSupportFunctionSection = new Section.Builder()
                .setTitle("Driver Support Function")
                .addFunction(new OneButtonFunction("CC", new VhalOneButtonDelegate(mVendorExtensionClient, VehicleProperty.CRUISE_CONTROL_ON),
                        R.id.cruise_control_on))
                .addFunction(new OneButtonFunction("ACC", new VhalOneButtonDelegate(mVendorExtensionClient, VehicleProperty.ADAPTIVE_CRUISE_CONTROL_ON),
                        R.id.adaptive_cruise_control_on))
                .addFunction(new OneButtonFunction("SL", new VhalOneButtonDelegate(mVendorExtensionClient, VehicleProperty.SPEED_LIMITER_ON),
                        R.id.speed_limiter_on))
                .create();

        Section laneKeepingAidSection = new Section.Builder()
                .setTitle("Lane Keeping Aid")
                .addFunction(new OnOffFunction("Lane Keeping Aid", new VhalOnOffDelegate(mVendorExtensionClient, VehicleProperty.LANE_KEEPING_AID_ON),
                        R.id.lane_keeping_aid_button_on, R.id.lane_keeping_aid_button_off))
                .addFunction(new ThreeStateFunction("Lane Keeping Aid mode",
                        "Both", "Steering", "Sound",
                        new VhalThreeStateDelegate(mVendorExtensionClient, VehicleProperty.LANE_KEEPING_AID_MODE),
                        R.id.lane_keeping_aid_button_mode_both, R.id.lane_keeping_aid_button_mode_steering, R.id.lane_keeping_aid_button_mode_sound))
                .addFunction(new OnOffFunction("Emergency Lane Keeping Aid", new VhalOnOffDelegate(mVendorExtensionClient, VehicleProperty.EMERGENCY_LANE_KEEPING_AID_ON),
                        R.id.emergency_lane_keeping_aid_button_on, R.id.emergency_lane_keeping_aid_button_off))
                .create();

          Section curveSpeedAdaptionSection = new Section.Builder()
                .setTitle("Curve Speed Adaption")
                .addFunction(new OnOffFunction("Curve Speed Adaption", new VhalOnOffDelegate(mVendorExtensionClient, VehicleProperty.CURVE_SPEED_ADAPTION_ON),
                        R.id.curve_speed_adaption_on, R.id.curve_speed_adaption_off))
                .create();

        ArrayList<Section> sections = new ArrayList<>();
        sections.add(driveAwayInformationSection);
        sections.add(driverSupportFunctionSection);
        sections.add(intellisafeSection);
        sections.add(laneKeepingAidSection);
        sections.add(curveSpeedAdaptionSection);

        return sections;
    }
}
