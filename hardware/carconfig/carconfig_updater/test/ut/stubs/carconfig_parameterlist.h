/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

namespace CarConfigParamList {
/* Do not edit by hand!
The values below is extracted from the file /home/johnny/shared/ihu_cedric/core/docs/carconfigspec.ods
on 2016-07-04 10:48:31 with the tool parse_ccdb.py
*/

const std::map<int, std::vector<int>> paramList = {
        {1,  // VehicleType
         {
                 0x01,  // L541
                 0x02,  // V542
                 0x03,  // V543
                 0x04,  // V526
                 0x05,  // V426
                 0x06,  // V541
                 0x07,  // V316
                 0x08,  // V320
                 0x09,  // V323
                 0x0A,  // V431
                 0x0B,  // V432
                 0x0C,  // L431
                 0x0D,  // V433
                 0x80,  // CX11
                 0x81,  // CS11
                 0x82,  // CH11
                 0x83,  // CC11
                 0x84,  // CS12
                 0xE0,  // HRE1
         }},
        {3,  // TransmissionDriveline
         {
                 0x01,  // Two_wheel_drive
                 0x02,  // AWD_All_wheel_drive
         }},
        {5,  // Engine
         {
                 0x01,  // VED4_2_0L_225_470
                 0x02,  // VEP4_2_0L_190_300
                 0x03,  // VED4_2_0L_190_400
                 0x04,  // VEP4_2_0L_249_350
                 0x05,  // VEP4_2_0L_254_350
                 0x06,  // VEP4_2_0L_260_350_PZEV
                 0x07,  // VEP4_2_0L_233_350
                 0x08,  // VEP4_2_0L_230_350_PZEV
                 0x09,  // VED4_2_0L_150_320
                 0x0A,  // VEP4_2_0L_320_400
                 0x0B,  // VEP4_2_0L_317_400_H_PZEV
                 0x0C,  // VEP4_2_0L_266_380
                 0x0D,  // VEP4_2_0L_266_380_PZEV
                 0x0E,  // VED4_2_0L_190_420
                 0x0F,  // VEP4_2_0L_197_330
                 0x10,  // VED4_2_0L_231_480
                 0x11,  // VED4_2_0L_150_350
                 0x12,  // VEP4_2_0L_156_280
                 0x13,  // VEP4_2_0L_320_400_SULEV
                 0x14,  // VEP4_2_0L_320_400_H
                 0x15,  // VED4_2_0L_190_420_X
                 0x16,  // VED4_2_0L_235_480
                 0x17,  // VEP4_2_0L_254_350_H
                 0x18,  // GEP3_MP_1_5L_156_265
                 0x19,  // GEP3_MP_1_5L_150_265
                 0x1A,  // GEP3_MP_1_5L_180_265_H
                 0x1B,  // VEP4_2_0L_190_300_X
                 0x1C,  // VED4_2_0L_238_480
                 0x1D,  // VED4_2_0L_156_330
                 0x1E,  // VED4_2_0L_197_420
                 0x1F,  // GEP3_LP_1_5L_129_245
                 0xE0,  // GEP_180_265_Non_VCC
         }},
        {6,  // Fuel
         {
                 0x01,  // Petrol
                 0x02,  // Diesel
         }},
        {7,  // EngineGenerationVea2
         {
                 0x01,  // VEA_generation_1
                 0x02,  // VEA_generation_2
                 0x03,  // VEA_generation_3
         }},
        {8,  // SteeringWheelPosition
         {
                 0x01,  // Left_Hand_Drive
                 0x02,  // Right_Hand_Drive
         }},
        {9,  // Gearbox
         {
                 0x01,  // AWF22FWD_Automatic_8_speed_FWD
                 0x02,  // M66FWD_Manual_6_speed_FWD
                 0x03,  // C6FWD_Manual_6_speed_FWD
                 0x04,  // AWF22AWD_Automatic_8_speed_AWD
                 0x05,  // M66AWD_Manual_6_speed_AWD
                 0x06,  // AWF21FWD_Automatic_6_speed_FWD
                 0x07,  // SevenDCTFWD_Dual_Clutch_7_speed_FWD
                 0x08,  // M76FWD_Manual_6_speed_FWD
                 0x09,  // SevenDCTAWD_Dual_Clutch_7_speed_AWD
                 0x0A,  // AW8GFWD_Automatic_8_speed_FWD
                 0x0B,  // AW8GAWD_Automatic_8_speed_AWD
         }},
        {10,  // GearboxType
         {
                 0x01,  // Manual_gearbox
                 0x02,  // Automatic_gearbox
         }},
        {11,  // FuelTankVolume
         {
                 0x01,  // Volume_71_Litre
                 0x02,  // Volume_60_Litre
                 0x03,  // Volume_50_Litre
                 0x04,  // Volume_55_Litre
                 0x05,  // Volume_40_Litre
                 0x06,  // Volume_47_Litre
         }},
        {13,  // PropulsionType
         {
                 0x01,  // Combustion_engine_only
                 0x02,  // Hybrid_vehicle
                 0x03,  // Plugin_hybrid_vehicle
         }},
        {15,  // TurboConfiguration
         {
                 0x01,  // Medium_performance
                 0x02,  // High_performance
                 0x03,  // Low_performance
                 0x04,  // Entry_performance
         }},
        {16,  // PowerSteeringPersonalisation
         {
                 0x01,  // Without_Electro_Power_Steering_Personalisation
                 0x02,  // With_Electro_Power_Steering_Personalisation
         }},
        {18,  // TyreRollingCircumference
         {
                 0x01,  // Added for unit test
                 0x28,  // Rolling_circumference_1900mm
                 0x29,  // Rolling_circumference_1905mm
                 0x2A,  // Rolling_circumference_1910mm
                 0x2B,  // Rolling_circumference_1915mm
                 0x2C,  // Rolling_circumference_1920mm
                 0x2D,  // Rolling_circumference_1925mm
                 0x2E,  // Rolling_circumference_1930mm
                 0x2F,  // Rolling_circumference_1935mm
                 0x30,  // Rolling_circumference_1940mm
                 0x31,  // Rolling_circumference_1945mm
                 0x32,  // Rolling_circumference_1950mm
                 0x33,  // Rolling_circumference_1955mm
                 0x34,  // Rolling_circumference_1960mm
                 0x35,  // Rolling_circumference_1965mm
                 0x36,  // Rolling_circumference_1970mm
                 0x37,  // Rolling_circumference_1975mm
                 0x38,  // Rolling_circumference_1980mm
                 0x39,  // Rolling_circumference_1985mm
                 0x3A,  // Rolling_circumference_1990mm
                 0x3B,  // Rolling_circumference_1995mm
                 0x3C,  // Rolling_circumference_2000mm
                 0x3D,  // Rolling_circumference_2005mm
                 0x3E,  // Rolling_circumference_2010mm
                 0x3F,  // Rolling_circumference_2015mm
                 0x40,  // Rolling_circumference_2020mm
                 0x41,  // Rolling_circumference_2025mm
                 0x42,  // Rolling_circumference_2030mm
                 0x43,  // Rolling_circumference_2035mm
                 0x44,  // Rolling_circumference_2040mm
                 0x45,  // Rolling_circumference_2045mm
                 0x46,  // Rolling_circumference_2050mm
                 0x47,  // Rolling_circumference_2055mm
                 0x48,  // Rolling_circumference_2060mm
                 0x49,  // Rolling_circumference_2065mm
                 0x4A,  // Rolling_circumference_2070mm
                 0x4B,  // Rolling_circumference_2075mm
                 0x4C,  // Rolling_circumference_2080mm
                 0x4D,  // Rolling_circumference_2085mm
                 0x4E,  // Rolling_circumference_2090mm
                 0x4F,  // Rolling_circumference_2095mm
                 0x50,  // Rolling_circumference_2100mm
                 0x51,  // Rolling_circumference_2105mm
                 0x52,  // Rolling_circumference_2110mm
                 0x53,  // Rolling_circumference_2115mm
                 0x54,  // Rolling_circumference_2120mm
                 0x55,  // Rolling_circumference_2125mm
                 0x56,  // Rolling_circumference_2130mm
                 0x57,  // Rolling_circumference_2135mm
                 0x58,  // Rolling_circumference_2140mm
                 0x59,  // Rolling_circumference_2145mm
                 0x5A,  // Rolling_circumference_2150mm
                 0x5B,  // Rolling_circumference_2155mm
                 0x5C,  // Rolling_circumference_2160mm
                 0x5D,  // Rolling_circumference_2165mm
                 0x5E,  // Rolling_circumference_2170mm
                 0x5F,  // Rolling_circumference_2175mm
                 0x60,  // Rolling_circumference_2180mm
                 0x61,  // Rolling_circumference_2185mm
                 0x62,  // Rolling_circumference_2190mm
                 0x63,  // Rolling_circumference_2195mm
                 0x64,  // Rolling_circumference_2200mm
                 0x65,  // Rolling_circumference_2205mm
                 0x66,  // Rolling_circumference_2210mm
                 0x67,  // Rolling_circumference_2215mm
                 0x68,  // Rolling_circumference_2220mm
                 0x69,  // Rolling_circumference_2225mm
                 0x6A,  // Rolling_circumference_2230mm
                 0x6B,  // Rolling_circumference_2235mm
                 0x6C,  // Rolling_circumference_2240mm
                 0x6D,  // Rolling_circumference_2245mm
                 0x6E,  // Rolling_circumference_2250mm
                 0x6F,  // Rolling_circumference_2255mm
                 0x70,  // Rolling_circumference_2260mm
                 0x71,  // Rolling_circumference_2265mm
                 0x72,  // Rolling_circumference_2270mm
                 0x73,  // Rolling_circumference_2275mm
                 0x74,  // Rolling_circumference_2280mm
                 0x75,  // Rolling_circumference_2285mm
                 0x76,  // Rolling_circumference_2290mm
                 0x77,  // Rolling_circumference_2295mm
                 0x78,  // Rolling_circumference_2300mm
                 0x79,  // Rolling_circumference_2305mm
                 0x7A,  // Rolling_circumference_2310mm
                 0x7B,  // Rolling_circumference_2315mm
                 0x7C,  // Rolling_circumference_2320mm
                 0x7D,  // Rolling_circumference_2325mm
                 0x7E,  // Rolling_circumference_2330mm
                 0x7F,  // Rolling_circumference_2335mm
                 0x80,  // Rolling_circumference_2340mm
                 0x81,  // Rolling_circumference_2345mm
                 0x82,  // Rolling_circumference_2350mm
                 0x83,  // Rolling_circumference_2355mm
                 0x84,  // Rolling_circumference_2360mm
                 0x85,  // Rolling_circumference_2365mm
                 0x86,  // Rolling_circumference_2370mm
                 0x87,  // Rolling_circumference_2375mm
                 0x88,  // Rolling_circumference_2380mm
                 0x89,  // Rolling_circumference_2385mm
                 0x8A,  // Rolling_circumference_2390mm
                 0x8B,  // Rolling_circumference_2395mm
                 0x8C,  // Rolling_circumference_2400mm
         }},
        {19,  // TyrePressureMonitoringSystem
         {
                 0x01,  // Without_Monitoring_system_TPMS
                 0x02,  // TPMS_WBA_US_algorithm
                 0x03,  // TPMS_WBL_US_algorithm
                 0x04,  // TPMS_WBA_EU_algorithm
                 0x05,  // TPMS_WBL_EU_algorithm
         }},
        {20,  // TpmsMenuFunctionsInIhu
         {
                 0x01,  // No_TPMS
                 0x02,  // TPMS_Reset_menu
                 0x03,  // TPMS_ShutOff_menu
                 0x04,  // TPMS_Show_Pressures_menu
                 0x05,  // TPMS_Reset_and_Show_Pressures_menu
                 0x06,  // TPMS_Reset_and_ShutOff_menu
                 0x07,  // TPMS_ShutOff_and_Show_Pressures_menu
                 0x08,  // TPMS_ShutOff_Show_Pressures_and_Reset_Menu
         }},
        {23,  // CruiseControl
         {
                 0x01,  // No_Cruise_control
                 0x02,  // Cruise_control
                 0x03,  // Adaptive_Cruise_Control_Level1
                 0x04,  // Adaptive_Cruise_Control_Stop_Go_Level1
                 0x05,  // Not_in_use
                 0x06,  // Adaptive_Cruise_Control_Level2
                 0x07,  // Adaptive_Cruise_Control_Stop_Go_Level2
                 0x08,  // Adaptive_Cruise_Control_Level3
                 0x09,  // Adaptive_Cruise_Control_Stop_Go_Level3
         }},
        {24,  // Seats
         {
                 0x01,  // Not_applicable_variant_no_parts_allowed
                 0x02,  // Fiveseat
                 0x03,  // Sevenseat
                 0x04,  // Fourseat
         }},
        {28,  // RadioAmpSpeakerSystem
         {
                 0x01,  // Performance_audio
                 0x02,  // High_Performance_audio
                 0x03,  // Premium_Sound_audio
                 0x04,  // High_Performance_Plus_audio
         }},
        {36,  // AdjustableSpeedLimiter
         {
                 0x01,  // Without_Adjustable_speed_limiter
                 0x02,  // Adjustable_speed_limiter
                 0x03,  // ASL_and_AVSL
                 0x04,  // AVSL
         }},
        {39,  // ShiftByWire
         {
                 0x01,  // Without_Shift_By_Wire
                 0x02,  // With_Shift_By_Wire
                 0x03,  // SHIFT_BY_WIRE_pattern_type_2_
         }},
        {43,  // MaxVehicleSpeedLimitation
         {
                 0x01,  // Without_speed_limit
                 0x02,  // Speed_limit_100_km_h
                 0x03,  // Speed_limit_105_km_h
                 0x04,  // Speed_limit_110_km_h
                 0x05,  // Speed_limit_115_km_h
                 0x06,  // Speed_limit_120_km_h
                 0x07,  // Speed_limit_125_km_h
                 0x08,  // Speed_limit_130_km_h
                 0x09,  // Speed_limit_135_km_h
                 0x0A,  // Speed_limit_140_km_h
                 0x0B,  // Speed_limit_145_km_h
                 0x0C,  // Speed_limit_150_km_h
                 0x0D,  // Speed_limit_155_km_h
                 0x0E,  // Speed_limit_160_km_h
                 0x0F,  // Speed_limit_165_km_h
                 0x10,  // Speed_limit_170_km_h
                 0x11,  // Speed_limit_175_km_h
                 0x12,  // Speed_limit_180_km_h
                 0x13,  // Speed_limit_185_km_h
                 0x14,  // Speed_limit_190_km_h
                 0x15,  // Speed_limit_195_km_h
                 0x16,  // Speed_limit_200_km_h
                 0x17,  // Speed_limit_205_km_h
                 0x18,  // Speed_limit_210_km_h
                 0x19,  // Speed_limit_215_km_h
                 0x1A,  // Speed_limit_220_km_h
                 0x1B,  // Speed_limit_225_km_h
                 0x1C,  // Speed_limit_230_km_h
                 0x1D,  // Speed_limit_235_km_h
                 0x1E,  // Speed_limit_240_km_h
                 0x1F,  // Speed_limit_245_km_h
                 0x20,  // Speed_limit_250_km_h
                 0x21,  // Speed_limit_255_km_h
                 0x22,  // Speed_limit_260_km_h
                 0x23,  // Speed_limit_265_km_h
                 0x24,  // Speed_limit_270_km_h
                 0x25,  // Speed_limit_275_km_h
                 0x26,  // Speed_limit_280_km_h
                 0x27,  // Speed_limit_285_km_h
                 0x28,  // Speed_limit_290_km_h
                 0x29,  // Speed_limit_295_km_h
                 0x2A,  // Speed_limit_300_km_h
                 0x2B,  // Speed_limit_60_mph
                 0x2C,  // Speed_limit_65_mph
                 0x2D,  // Speed_limit_70_mph
                 0x2E,  // Speed_limit_75_mph
                 0x2F,  // Speed_limit_80_mph
                 0x30,  // Speed_limit_85_mph
                 0x31,  // Speed_limit_90_mph
                 0x32,  // Speed_limit_95_mph
                 0x33,  // Speed_limit_100_mph
                 0x34,  // Speed_limit_105_mph
                 0x35,  // Speed_limit_110_mph
                 0x36,  // Speed_limit_115_mph
                 0x37,  // Speed_limit_120_mph
                 0x38,  // Speed_limit_125_mph
                 0x39,  // Speed_limit_130_mph
                 0x3A,  // Speed_limit_135_mph
                 0x3B,  // Speed_limit_140_mph
                 0x3C,  // Speed_limit_145_mph
                 0x3D,  // Speed_limit_150_mph
                 0x3E,  // Speed_limit_155_mph
                 0x3F,  // Speed_limit_160_mph
                 0x40,  // Speed_limit_165_mph
                 0x41,  // Speed_limit_170_mph
                 0x42,  // Speed_limit_175_mph
                 0x43,  // Speed_limit_180_mph
                 0x44,  // Speed_limit_185_mph
                 0x45,  // Speed_limit_190_mph
         }},
        {47,  // SelectiveCatalyticReduction
         {
                 0x01,  // Without_UREA
                 0x02,  // With_UREA
         }},
        {48,  // OilLevelSensor
         {
                 0x01,  // Without_oil_level_sensor
                 0x02,  // Oil_level_sensor
         }},
        {50,  // ChassisType
         {
                 0x01,  // Chassis_STD
                 0x02,  // Chassis_SPORT_std_height
                 0x03,  // Chassis_DYNAMIC
                 0x04,  // Chassis_CCD
                 0x05,  // Chassi_P_Police
                 0x06,  // Chassi_T_Taxi
                 0x07,  // Chassis_TOURING
                 0x08,  // Chassis_SPORT_low
         }},
        {51,  // TrailerStabilityAssist
         {
                 0x01,  // Without_TRaiLer_STaBility_Assist
                 0x02,  // TRaiLer_STaBility_Assist
         }},
        {53,  // HillDescentControl
         {
                 0x01,  // Without_Hill_Descent_Control
                 0x02,  // Hill_Descent_Control
         }},
        {57,  // DriveMode
         {
                 0x01,  // Without_drive_mode_switch
                 0x02,  // With_drive_mode_switch
         }},
        {58,  // AirSuspension
         {
                 0x01,  // Without_air_suspension
                 0x02,  // Air_suspension_2_corner
                 0x03,  // Air_suspension_4_corner
         }},
        {59,  // ActiveSuspension
         {
                 0x01,  // Without_Continious_Controlled_Chassi
                 0x02,  // Continious_Controlled_Chassi
         }},
        {65,  // InteriorMotionSensor
         {
                 0x01,  // Without_Interior_Motion_Sensor
                 0x02,  // Interior_Motion_Sensor
         }},
        {66,  // InclinationSensor
         {
                 0x01,  // Without_inclination_sensor
                 0x02,  // With_inclination_sensor
         }},
        {70,  // PassiveArming
         {
                 0x01,  // Without_passive_arming
                 0x02,  // With_passive_arming
         }},
        {71,  // ForeignComponentDetection
         {
                 0x01,  // Without_Foreign_Component_Detection
                 0x02,  // Foreign_Component_Detection
         }},
        {85,  // CentralLockType
         {
                 0x01,  // Without_blocked_lock_mode
                 0x02,  // Blocked_lock_mode
         }},
        {88,  // PrivateLockingLuggageComp
         {
                 0x01,  // Private_locking_disabled
                 0x02,  // Private_locking_enabled
         }},
        {90,  // AutomaticLockingBySpeed
         {
                 0x01,  // Automatic_Locking_by_speed_always_ON_disabled_in_menu
                 0x02,  // Automatic_Locking_by_speed_enabled_default_OFF
                 0x03,  // Automatic_Locking_by_speed_enabled_default_ON
         }},
        {91,  // TwoStepUnlocking
         {
                 0x01,  // Two_step_unlocking_default_OFF
                 0x02,  // Two_step_unlocking_default_ON
         }},
        {94,  // KeylessEntry
         {
                 0x01,  // Without_Keyless_entry
                 0x02,  // Passive_entry_Personal_Car_Communicator_incl_remote_tag
         }},
        {95,  // VisibleLockingunlocking
         {
                 0x01,  // Visible_locking_and_unlocking_feedback_default_OFF
                 0x02,  // Visible_locking_and_unlocking_feedback_default_ON
         }},
        {96,  // AudibleLockingFeedback
         {
                 0x01,  // Audible_locking_feedback_disabled
                 0x02,  // Audible_locking_feedback_enabled_default_OFF
                 0x03,  // Audible_locking_feedback_enabled_default_ON
         }},
        {106,  // HeadlightsType
         {
                 0x01,  // Halogen_lights_without_adjuster
                 0x02,  // Halogen_lights_with_adjuster
                 0x03,  // Xenon_Automatic_Bending_Light_type_A
                 0x04,  // Xenon_Automatic_Bending_Light_type_B
                 0x05,  // LED_With_Bending_Light_type_A_Top_Range
                 0x06,  // LED_With_Bending_Light_type_B_Top_Range
                 0x07,  // LED_Without_Bending_Light_type_A_Mid_Range
                 0x08,  // LED_Without_Bending_Light_type_B_Mid_Range
                 0x09,  // LED_type_A_with_HCM_US_CND_Mid_Range
                 0x0A,  // LED_type_B_with_HCM_ROW_Mid_Range
                 0x0B,  // LED_type_A_with_HCM_US_CND_High_Range
                 0x0C,  // LED_type_B_with_HCM_ROW_High_Range
                 0x0D,  // HALO_B_Halogen_lights_with_adjuster_LED_Supplement_Lamp_for_DRL_Pos_TI
                 0x0E,  // LED_Type_B_with_HCM_LED_Supplement_Lamp_for_DRL_Pos_TI
         }},
        {107,  // ExteriorLightLogic
         {
                 0x01,  // DDRL_not_together_with_position_light
                 0x02,  // Without_DDRL
                 0x03,  // DDRL_in_pos_0_user_configurable
                 0x04,  // DDRL_in_pos_Auto_user_configurable
                 0x05,  // DDRL_in_all_positions
                 0x06,  // Without_DDRL_with_position_light_after_market
         }},
        {109,  // ActiveHighBeam
         {
                 0x01,  // Without_AHB
                 0x02,  // With_AHB1_On_Off_Low_Beam_in_Village
                 0x03,  // With_AHB1_On_Off_High_Beam_in_Small_Village
                 0x04,  // With_AHB2_Adaptive_Low_Beam_in_Village
                 0x05,  // With_AHB2_Adaptive_High_Beam_in_Small_Village
         }},
        {111,  // TouristLight
         {
                 0x01,  // Without_configurable_Tourist_Light
                 0x02,  // Left_asymmetry_with_configurable_Tourist_Light
                 0x03,  // Right_asymmetry_with_configurable_Tourist_Light
         }},
        {112,  // CorneringLight
         {
                 0x01,  // Without_cornering_light
                 0x02,  // With_cornering_light
         }},
        {119,  // AuxiliaryLights
         {
                 0x01,  // Without_auxiliary_lights
                 0x02,  // With_Auxiliary_lights
         }},
        {122,  // HeatedWindscreen
         {
                 0x01,  // No_Heated_Frontscreen
                 0x02,  // Heated_Frontscreen
         }},
        {124,  // RearWindowWiper
         {
                 0x01,  // No_rear_window_wiper
                 0x02,  // Rear_Window_wiper_with_washer
         }},
        {132,  // CutoffSwitchPassengerAirbag
         {
                 0x01,  // Without_Cut_Off_Switch_Passenger_Airbag
                 0x02,  // Cut_Off_Switch_Passenger_AirBag
         }},
        {141,  // NightVision
         {
                 0x01,  // Without_Night_Vision
                 0x02,  // With_Night_Vision
         }},
        {142,  // ParkingAssistance
         {
                 0x01,  // Without_parking_assistance
                 0x02,  // Parking_assistance_rear
                 0x03,  // Parking_assistance_front_and_rear
                 0x04,  // Parking_assistance_Semi_Automatic_Parking
                 0x05,  // Parking_assistance_Full_Automatic_Parking
         }},
        {143,  // DriverImpairmentMonitor
         {
                 0x01,  // Without_Driver_Impairment_Monitor
                 0x02,  // With_Driver_Impairment_Monitor
         }},
        {144,  // DistanceAlert
         {
                 0x01,  // Without_Distance_Alert
                 0x02,  // With_Distance_Alert
         }},
        {145,  // TrafficJamAssist
         {
                 0x01,  // Without_Traffic_Jam_Assist
                 0x02,  // With_Traffic_Jam_Assist_1
                 0x03,  // With_Traffic_Jam_Assist_2
         }},
        {146,  // CollisionMitigationSupportFront
         {
                 0x01,  // Without_Collision_Mitigation_Support_Front
                 0x02,  // With_Collision_Mitigation_Support_Front_ver_1
                 0x03,  // With_Collision_Mitigation_Support_Front_ver_2
         }},
        {147,  // RoadFrictionInformation
         {
                 0x01,  // Without_Road_friction_indication
                 0x02,  // Road_friction_indication
         }},
        {148,  // CurveSpeedSupport
         {
                 0x01,  // Without_Curve_Speed_Warning_without_Curve_Speed_Assist
                 0x02,  // With_Curve_Speed_Warning_without_Curve_Speed_Assist
                 0x03,  // Without_Curve_Speed_Warning_with_Curve_Speed_Assist
                 0x04,  // With_Curve_Speed_Warning_with_Curve_Speed_Assist
         }},
        {149,  // RoadSignInformation
         {
                 0x01,  // Without_TSI_Traffic_Sign_Information_SLA_Speed_limit_Adaption_
                 0x02,  // SWEDEN_TSI_without_SLA
                 0x03,  // SWEDEN_TSI_with_SLA
                 0x04,  // IRELAND_TSI_without_SLA
                 0x05,  // IRELAND_TSI_with_SLA
                 0x06,  // ENGLAND_TSI_without_SLA
                 0x07,  // ENGLAND_TSI_with_SLA
                 0x08,  // MID_EU_TSI_without_SLA
                 0x09,  // MID_EU_TSI_with_SLA
                 0x0A,  // EAST_EUROPE_TSI_without_SLA
                 0x0B,  // EAST_EUROPE_TSI_with_SLA
                 0x0C,  // SOUTH_EU_TSI_without_SLA
                 0x0D,  // SOUTH_EU_TSI_with_SLA
                 0x0E,  // USA_TSI_without_SLA
                 0x0F,  // USA_TSI_with_SLA
                 0x10,  // MEXICO_TSI_without_SLA
                 0x11,  // MEXICO_TSI_with_SLA
                 0x12,  // CANADA_TSI_without_SLA
                 0x13,  // CANADA_TSI_with_SLA
                 0x14,  // SINGAPORE_TSI_without_SLA
                 0x15,  // SINGAPORE_TSI_with_SLA
                 0x16,  // KOREA_TSI_without_SLA
                 0x17,  // KOREA_TSI_with_SLA
                 0x18,  // LATIN_AMERICA_TSI_without_SLA
                 0x19,  // LATIN_AMERICA_TSI_with_SLA
                 0x1A,  // ARABIA_TSI_without_SLA
                 0x1B,  // ARABIA_TSI_with_SLA
                 0x1C,  // HONGKONG_TSI_without_SLA
                 0x1D,  // HONGKONG_TSI_with_SLA
                 0x1E,  // TAIWAN_TSI_without_SLA
                 0x1F,  // TAIWAN_TSI_with_SLA
                 0x20,  // AUSTRALIA_TSI_without_SLA
                 0x21,  // AUSTRALIA_TSI_with_SLA
                 0x22,  // SOUTH_AFRICA_TSI_without_SLA
                 0x23,  // SOUTH_AFRICA_TSI_with_SLA
                 0x24,  // TURKEY_TSI_without_SLA
                 0x25,  // TURKEY_TSI_with_SLA
                 0x26,  // JAPAN_TSI_without_SLA
                 0x27,  // JAPAN_TSI_with_SLA
                 0x28,  // CHINA_TSI_without_SLA
                 0x29,  // CHINA_TSI_with_SLA
                 0x2A,  // INDIA_TSI_without_SLA
                 0x2B,  // INDIA_TSI_with_SLA
                 0x2C,  // BRAZIL_TSI_without_SLA
                 0x2D,  // BRAZIL_TSI_with_SLA
                 0x2E,  // THAILAND_TSI_without_SLA
                 0x2F,  // THAILAND_TSI_with_SLA
                 0x30,  // MALAYSIA_TSI_without_SLA
                 0x31,  // MALAYSIA_TSI_with_SLA
                 0x32,  // OTHERS_TSI_without_SLA
                 0x33,  // OTHERS_TSI_with_SLA
                 0xF0,  // Not_allowed_varaint
         }},
        {150,  // LaneKeepingAid
         {
                 0x01,  // Without_Lane_Keeping_Aid_Lane_Departure_Warning
                 0x02,  // Lane_Keeping_Aid
                 0x05,  // Lane_Departure_Warning
         }},
        {151,  // ElectronicHorizon
         {
                 0x01,  // Without_Electronic_Horizon
                 0x02,  // With_Electronic_Horizon
         }},
        {152,  // BlindSpotInformationSystem
         {
                 0x01,  // Without_Blind_spot_Information_System
                 0x02,  // Blind_spot_Information_System_with_CTA
                 0x03,  // Without_Blind_spot_Information_System_sensors_fitted
                 0x04,  // BLIS_System_with_audio_and_CTA_for_Geely
         }},
        {153,  // CollisionMitigWarnRear
         {
                 0x01,  // Without_Rear_Collision_Warning_and_Mitigation
                 0x02,  // With_Rear_Collision_Warning_and_Mitigation
                 0x03,  // With_Rear_Collision_Mitigation
                 0x04,  // With_Rear_Collision_Warning
         }},
        {154,  // ParkingCamera
         {
                 0x01,  // Without_Parking_Camera
                 0x02,  // Rear_Parking_Camera
                 0x03,  // Parking_camera_360
                 0x04,  // Front_Parking_Camera_accessory_
                 0x05,  // Rear_Parking_Camera_always_activated_during_reverse
                 0x06,  // Parking_camera_360_always_activated_during_reverse
                 0x07,  // Parking_Camera_preparation_front_rear
                 0x08,  // Rear_Parking_Camera_GEN_2
                 0x09,  // Rear_Parking_Camera_GEN_2_always_activated_during_reverse
                 0x0A,  // Rear_Parking_Camera_Basic_standard_US
                 0x0B,  // Parking_camera_360_GEN_2
                 0x0C,  // Parking_camera_360_GEN_2_always_activated_during
         }},
        {156,  // RearWiewMirrorsouter
         {
                 0x01,  // Without_retractable_mirrors
                 0x02,  // Retractable_mirrors
         }},
        {157,  // CombinedInstrument
         {
                 0x01,  // Drivers_Information_Module_with_8_inch_screen
                 0x02,  // Drivers_Information_Module_with_12_3_inch_screen
                 0x03,  // Drivers_Information_Module_with_4_2_inch_screen_KMH
                 0x04,  // Drivers_Information_Module_with_4_2_inch_screen_MPH
                 0x80,  // Drivers_Information_Module_with_7_inch_screen_Non_VCC
         }},
        {158,  // HeadUpDisplay
         {
                 0x01,  // With_Head_Up_Display_FCW
                 0x02,  // With_Head_Up_Display_graphic
                 0x03,  // Without_Head_Up_Display
                 0x04,  // With_Head_Up_Display_graphic_2
         }},
        {161,  // UnitSettings
         {
                 0x01,  // Temp_C_Clock_24_Distance_KM_Fuel_Liter_KM
                 0x02,  // Temp_C_Clock_24_Distance_KM_Fuel_KM_Liter
                 0x03,  // Temp_C_Clock_24_Distance_Km_Fuel_MPG_US_
                 0x04,  // Temp_C_Clock_24_Distance_Km_Fuel_MPG_UK_
                 0x05,  // Temp_C_Clock_24_Distance_Miles_Fuel_L_Km
                 0x06,  // Temp_C_Clock_24_Distance_Miles_Fuel_Km_L
                 0x07,  // Temp_C_Clock_24_Distance_Miles_Fuel_MPG_US_
                 0x08,  // Temp_C_Clock_24_Distance_Miles_Fuel_UK_Gallon
                 0x09,  // Temp_C_Clock_12_Distance_KM_Fuel_Liter_KM
                 0x0A,  // Temp_C_Clock_12_Distance_Km_Fuel_Km_L
                 0x0B,  // Temp_C_Clock_12_Distance_Km_Fuel_MPG_US_
                 0x0C,  // Temp_C_Clock_12_Distance_Km_Fuel_MPG_UK_
                 0x0D,  // Temp_C_Clock_12_Distance_Miles_Fuel_L_Km
                 0x0E,  // Temp_C_Clock_12_Distance_Miles_Fuel_Km_L
                 0x0F,  // Temp_C_Clock_12_Distance_Miles_Fuel_MPG_US_
                 0x10,  // Temp_C_Clock_12_Distance_Miles_Fuel_MPG_UK_
                 0x11,  // Temp_F_Clock_24_Distance_Km_Fuel_L_Km
                 0x12,  // Temp_F_Clock_24_Distance_Km_Fuel_Km_L
                 0x13,  // Temp_F_Clock_24_Distance_Km_Fuel_MPG_US_
                 0x14,  // Temp_F_Clock_24_Distance_Km_Fuel_MPG_UK_
                 0x15,  // Temp_F_Clock_24_Distance_Miles_Fuel_L_Km
                 0x16,  // Temp_F_Clock_24_Distance_Miles_Fuel_Km_L
                 0x17,  // Temp_F_Clock_24_Distance_Miles_Fuel_MPG_US_
                 0x18,  // Temp_F_Clock_24_Distance_Miles_Fuel_MPG_UK_
                 0x19,  // Temp_F_Clock_12_Distance_Km_Fuel_L_Km
                 0x1A,  // Temp_F_Clock_12_Distance_Km_Fuel_Km_L
                 0x1B,  // Temp_F_Clock_12_Distance_Km_Fuel_MPG_US_
                 0x1C,  // Temp_F_Clock_12_Distance_Km_Fuel_MPG_UK_
                 0x1D,  // Temp_F_Clock_12_Distance_Miles_Fuel_L_Km
                 0x1E,  // Temp_F_Clock_12_Distance_Miles_Fuel_Km_L
                 0x1F,  // Temp_F_Clock_12_Distance_Miles_Fuel_US_Gallon
                 0x20,  // Temp_F_Clock_12_Distance_Miles_Fuel_MPG_UK_
         }},
        {167,  // TripComputerAndStatistics
         {
                 0x01,  // Without_trip_comp_and_trip_stat
                 0x02,  // With_trip_comp_and_without_trip_stat
                 0x03,  // Without_trip_comp_and_with_trip_stat
                 0x04,  // With_trip_comp_and_with_trip_stat
         }},
        {168,  // ScreenThemesskins
         {
                 0x01,  // Screen_Skins_Standard_1_
                 0x02,  // Screen_Skins_R_Design_
                 0x03,  // Screen_Skins_Inscription_
                 0x04,  // Screen_Skins_VOR_
         }},
        {170,  // HvacRear
         {
                 0x01,  // Without_rear_VAC
                 0x02,  // With_rear_VAC_without_PTCs
                 0x03,  // With_rear_VAC_with_PTCs
         }},
        {171,  // AdditionalHeater
         {
                 0x01,  // Without_Additional_Heater
                 0x02,  // With_Additional_Heater
         }},
        {174,  // AirQualitySystem
         {
                 0x01,  // No_Air_Quality_System_Pollenfilter
                 0x02,  // Air_Quality_System_Multifilter
                 0x03,  // No_Air_Quality_System_Multifilter
                 0x04,  // Air_Quality_System_Multifilter2
         }},
        {175,  // HvacVariants
         {
                 0x01,  // Electronic_Climate_Control_2_zone
                 0x02,  // Electronic_Climate_Control_4_zone
                 0x03,  // HVAC_Small_1_zone
                 0x04,  // HVAC_Small_2_zone
                 0x05,  // HVAC_Small_3_zone
                 0x06,  // Manual_AC
         }},
        {176,  // RearViewMirrorType
         {
                 0x01,  // Manual_dimming
                 0x02,  // Automatic_dimming_inner
                 0x03,  // Automatic_dimming_inner_outer
         }},
        {177,  // RoofHatch
         {
                 0x01,  // No_roof_hatch_panorama_roof
                 0x02,  // Openable_panorama_roof
                 0x03,  // Hatch
         }},
        {179,  // FrontSeatVentilation
         {
                 0x01,  // Without_ventilated_seats
                 0x02,  // Ventilated_seats
                 0x03,  // Ventilated_seat_driver_seat
         }},
        {180,  // HeatedFrontSeats
         {
                 0x01,  // Without_heated_front_seats
                 0x02,  // Heated_front_seats
         }},
        {181,  // HeatedRearSeat
         {
                 0x01,  // Without_heated_rear_seat
                 0x02,  // Heated_rear_seat
         }},
        {182,  // ParkingClimateLevel
         {
                 0x01,  // No_parking_climate
                 0x02,  // Afterrun_parking_climate
                 0x03,  // Heating_parking_climate
                 0x04,  // Full_parking_climate
         }},
        {183,  // InteriorIllumination
         {
                 0x01,  // Interior_lighting_base_version
                 0x02,  // Interior_lighting_high_version
                 0x03,  // Interior_lighting_mid_version
                 0x11,  // Interior_lighting_base_version_not_for_Volvo
                 0x12,  // Interior_lighting_mid_version_not_for_Volvo
                 0x13,  // Interior_lighting_high_version_not_for_Volvo
         }},
        {184,  // PowerSeats
         {
                 0x01,  // No_Memory_Seats
                 0x02,  // Driver_Memory_seat_and_Mechanical_passenger_seat
                 0x03,  // Driver_Memory_seat_and_Passenger_seat_without_memory
                 0x04,  // Driver_and_Passenger_Memory_Seats
         }},
        {185,  // PowerFoldRise3dRow
         {
                 0x01,  // No_fold_rise_3_d_row_rear_seat
                 0x02,  // Mecanical_fold_rise_3_d_row_rear_seat
                 0x03,  // Power_fold_rise_3_d_row_rear_seat
         }},
        {186,  // HeatedSteeringWheel
         {
                 0x01,  // Without_heated_steering_wheel
                 0x02,  // Heated_steering_wheel
         }},
        {187,  // Headrests2ndRowFolding
         {
                 0x01,  // Manually_foldable_rear_headrests
                 0x02,  // El_folding_rear_headrests
                 0x03,  // Fixed_rear_headrests
                 0x04,  // Rear_headsrests_Comfort
         }},
        {189,  // RearSeatVentilation
         {
                 0x01,  // Without_rear_seat_ventilation
                 0x02,  // With_rear_seat_ventilation
         }},
        {190,  // GlobalNavigationSatSystemReceiver
         {
                 0x01,  // Without_Global_Navigation_Satellite_System_receiver
                 0x02,  // With_Global_Navigation_Satellite_System_receiver
         }},
        {191,  // TelematicModule
         {
                 0x01,  // Without_Telematic_module
                 0x02,  // With_Telematic_module
                 0x03,  // With_Telematic_module_Brazil
                 0x04,  // With_Telematic_module_USA_CDN
                 0x05,  // With_Telematic_module_China
                 0x06,  // With_Telematic_module_ROW_LTE
                 0x07,  // With_Telematic_module_Russia
         }},
        {192,  // IhuLevel
         {
                 0x01,  // IHU_low
                 0x02,  // IHU_high
                 0x03,  // IHU_mid
         }},
        {193,  // DigitalRadioReceiver
         {
                 0x01,  // Without_digital_radio_system
                 0x02,  // DAB_radio_system
                 0x03,  // HD_SDARS_radio_system_US
                 0x04,  // Not_in_use
                 0x05,  // HD_SDARS_radio_system_Can
         }},
        {194,  // RearSeatEntertainment
         {
                 0x01,  // Without_Rear_Seat_Entertainment
                 0x02,  // Rear_Seat_Entertainment
         }},
        {195,  // RegionIfDvd
         {
                 0x01,  // DVD_region_0
                 0x02,  // DVD_region_1
                 0x03,  // DVD_region_2
                 0x04,  // DVD_region_3
                 0x05,  // DVD_region_4
                 0x06,  // DVD_region_5
                 0x07,  // DVD_region_6
                 0x08,  // DVD_region_7
                 0x09,  // DVD_region_8
                 0x0A,  // DVD_region_9
                 0x0B,  // DVD_region_10
         }},
        {196,  // WifiFrequencyBand
         {
                 0x01,  // Two_4_GHz
                 0x02,  // Five_GHz
                 0x03,  // Two_4_GHz_and_5_GHz
         }},
        {197,  // SystemLanguage
         {
                 0x01,  // Arabic
                 0x02,  // Chinese_Simp_Man
                 0x03,  // Chinese_Trad_Can
                 0x04,  // Chinese_Trad_Man
                 0x05,  // Czech
                 0x06,  // Danish
                 0x07,  // Dutch
                 0x08,  // Australien_English
                 0x09,  // English
                 0x0A,  // American_English
                 0x0B,  // Finnish
                 0x0C,  // Flemmish
                 0x0D,  // Canadian_French
                 0x0E,  // French
                 0x0F,  // German
                 0x10,  // Greek
                 0x11,  // Hungarian
                 0x12,  // Italian
                 0x13,  // Japanese
                 0x14,  // Korean_Han_Gul_
                 0x15,  // Norwegian
                 0x16,  // Polish
                 0x17,  // Brazilian_Portuguese
                 0x18,  // Portuguese
                 0x19,  // Russian
                 0x1A,  // Spanish
                 0x1B,  // American_Spanish
                 0x1C,  // Swedish
                 0x1D,  // Thai
                 0x1E,  // Turkish
                 0x1F,  // Bulgarian
                 0x20,  // Estonian
                 0x21,  // Latvian
                 0x22,  // Lithuanian
                 0x23,  // Romanian
                 0x24,  // Slovak
                 0x25,  // Slovene
         }},
        {198,  // Speech
         {
                 0x01,  // No_speech
                 0x02,  // Speech
         }},
        {199,  // SpeechLanguage
         {
                 0x01,  // Added for unit test
                 0x07,  // English
         }},
        {200,  // WifiFunctionalityControl
         {
                 0x01,  // N_STA_N_AP_No_Wi_Fi_Station_No_Wi_Fi_AP
                 0x02,  // STA_N_AP_Wi_Fi_Station_No_Wi_Fi_AP
                 0x03,  // N_STA_AP_No_Wi_Fi_Station_Wi_Fi_AP
                 0x04,  // STA_AP_Wi_Fi_Station_Wi_Fi_AP
         }},
        {201,  // Connectivity
         {
                 0x01,  // No_Connectivity
                 0x02,  // Connectivity
         }},
        {202,  // OwnersManual
         {
                 0x01,  // Without_digital_owners_manual
                 0x02,  // Digital_owners_manual
                 0x03,  // Canadian_French
                 0x04,  // Chinese_simplified
                 0x05,  // Chinese_traditional
                 0x06,  // Czech
                 0x07,  // Danish
                 0x08,  // Dutch
                 0x09,  // English_International
                 0x0A,  // English_UK
                 0x0B,  // Estonian
                 0x0C,  // Finnish
                 0x0D,  // French
                 0x0E,  // German
                 0x0F,  // Italian
                 0x10,  // Japanese
                 0x11,  // Latvian
                 0x12,  // Lithuanian
                 0x13,  // Norska
                 0x14,  // Polish
                 0x15,  // Portuguese
                 0x16,  // Russian
                 0x17,  // Spanish
                 0x18,  // Swedish
                 0x19,  // Thai
                 0x1A,  // Turkish
                 0x1B,  // Ukrainian
                 0x1C,  // Us_English
                 0x1D,  // Greek
                 0x1E,  // Hungarian
         }},
        {203,  // Messaging
         {
                 0x01,  // Messaging_disabled
                 0x02,  // Messaging_enabled
         }},
        {204,  // TvTuner
         {
                 0x01,  // Without_TV_tuner
                 0x02,  // With_DVB_T_tuner_8Mhz
                 0x03,  // With_ISDB_T_tuner
                 0x04,  // With_DTMB_T_tuner
                 0x05,  // With_SBTVD_tuner
                 0x06,  // With_T_DMB_tuner
                 0x07,  // With_DVB_T_tuner_6Mhz
         }},
        {205,  // DsrcUnit
         {
                 0x01,  // Without_DSRC_unit
                 0x02,  // DSRC_unit
         }},
        {206,  // DualMediaVicsAntenna
         {
                 0x01,  // Without_VICS
                 0x02,  // With_VICS
         }},
        {207,  // VideoAllowed
         {
                 0x01,  // Never
                 0x02,  // Always_All_speeds_
                 0x03,  // Speed_dependent
         }},
        {209,  // RoadAndTrafficInfoRti
         {
                 0x01,  // Without_RTI_map
                 0x02,  // Map_region_for_Europe
                 0x03,  // Map_region_for_North_America
                 0x04,  // Map_region_for_South_America
                 0x05,  // Map_region_for_Africa_and_Middle_East
                 0x06,  // Map_region_for_South_East_Asia
                 0x07,  // Map_region_for_Pacific_Australia_New_Zeeland_
                 0x08,  // Map_region_for_Russia
                 0x09,  // Map_region_for_Korea
                 0x0A,  // Map_region_for_China
                 0x0B,  // Map_region_for_Taiwan
                 0x0C,  // Map_region_for_Japan
                 0x0D,  // Map_region_for_India
                 0x0E,  // Map_region_for_Israel
                 0x0F,  // Map_region_for_Turkey
         }},
        {210,  // Subwoofer
         {
                 0x01,  // Without_subwoofer
                 0x02,  // Subwoofer
         }},
        {211,  // TrailerModuleAftermarket
         {
                 0x01,  // Without_TRailer_Module
                 0x02,  // TRailer_Module_13_pos_socket_Fixed_towbar
                 0x03,  // TRailer_Module_4_7_pos_socket_Fixed_towbar
                 0x04,  // TRailer_Module_13_pos_socket_Semi_el_retractable_towbar
                 0x05,  // TRailer_Module_4_7_pos_socket_Semi_el_retractable_towbar
         }},
        {212,  // Alcolock
         {
                 0x01,  // Without_Alcolock
                 0x02,  // Alcolock_with_bypass
                 0x03,  // Alcolock_without_bypass
         }},
        {213,  // RunningboardAccessories
         {
                 0x01,  // Without_Runningboard
                 0x02,  // Fixed_Runningboard
                 0x03,  // Retractable_Runningboard
                 0x04,  // Retractable_Runningboard_Special
         }},
        {214,  // RestrictedKeyFunction
         {
                 0x01,  // Without_restricted_key
                 0x02,  // Restricted_key
         }},
        {216,  // AccessoryGatewayUnit
         {
                 0x01,  // Without_AGU
                 0x02,  // With_AGU
                 0x03,  // With_Traffic_Incident_Recorder
                 0x04,  // With_CAM
                 0x05,  // With_TIR_CAM
                 0x06,  // Not_used
                 0x07,  // Not_used_X
                 0x08,  // Not_used_X_X
         }},
        {218,  // MarketTelephoneNetwork
         {
                 0x01,  // Telephone_system_GSM
                 0x02,  // Telephone_system_CDMA2000
                 0x03,  // Telephone_system_TD_SCDMA
                 0x04,  // Telephone_system_OTHER_1_
                 0x05,  // Telephone_system_OTHER_2_
         }},
        {219,  // MicrophoneSetup
         {
                 0x01,  // No_microphone_mounted_in_car
                 0x02,  // One_handsfree_microphone_is_available
                 0x03,  // Two_handsfree_microphones_are_available
                 0x04,  // Three_handsfree_microphones
                 0x05,  // Two_handsfree_microphones_and_two_ANC_microphone_are_available
                 0x06,  // Three_handsfree_microphones_and_two_ANC_microphone_are_available
         }},
        {220,  // LanguageRegion
         {
                 0x01,  // Language_Region_for_World
                 0x02,  // Language_Region_for_Europe
                 0x03,  // Language_Region_for_North_America
                 0x04,  // Language_Region_for_South_America
                 0x05,  // Language_Region_for_Middle_East_Africa
                 0x06,  // Language_Region_for_South_East_Asia
                 0x07,  // Language_Region_for_Pacific
                 0x08,  // Language_Region_for_Russia
                 0x09,  // Language_Region_for_Korea
                 0x0A,  // Language_Region_for_China
                 0x0B,  // Language_Region_for_Taiwan
                 0x0C,  // Language_Region_for_Japan
                 0x0D,  // Language_Region_for_India
                 0x0E,  // Language_Region_for_Israel
                 0x0F,  // Language_Region_for_Turkey
         }},
        {221,  // ConnectedServiceBookingIfConnectivity
         {
                 0x01,  // Without_Connected_Service_Booking
                 0x02,  // Connected_Service_Booking
         }},
        {222,  // Mediaplayer
         {
                 0x01,  // Without_media_player
                 0x02,  // Media_player
                 0x03,  // Media_player_BD
         }},
        {223,  // NavigationSystemAndInternetMap
         {
                 0x01,  // Without_Connected_Navigation_enabled_without_Internet_map_support
                 0x02,  // Without_Connected_Navigation_enabled_with_Internet_map_support
                 0x03,  // With_Connected_Navigation_enabled_without_Internet_map_support
                 0x04,  // With_Connected_Navigation_enabled_with_Internet_map_support
         }},
        {224,  // DeviceMirroring
         {
                 0x01,  // Without_device_mirroring
                 0x02,  // With_device_mirroring_IOS
                 0x03,  // With_device_mirroring_GPM
                 0x04,  // With_device_mirroring_IOS_GPM
         }},
        {239,  // MassageFrontSeat
         {
                 0x01,  // Without_Massage_front_seat
                 0x02,  // Massage_front_seat
         }},
        {240,  // LumbarSupport
         {
                 0x01,  // Fixed_lumbar_support
                 0x02,  // Four_way_lumbar_support
                 0x03,  // Two_way_lumbar_support
         }},
        {241,  // CushionExtension
         {
                 0x01,  // Without_Cushion_Extension
                 0x02,  // With_Cushion_Extension_Driver
                 0x03,  // With_Cushion_Extension_Driver_Passenger
         }},
        {242,  // HeadrestsFront
         {
                 0x01,  // Two_way_headsrests_Front_manual
                 0x02,  // Two_way_headsrests_Front_Power
                 0x03,  // Headsrests_Front_Fixed
         }},
        {243,  // PowerAdjustableSideSupport
         {
                 0x01,  // Without_power_adjustable_side_support
                 0x02,  // Power_adjustable_side_support
         }},
        {244,  // RemoteControlOfPassengerSeatFunctions
         {
                 0x01,  // Without_Remote_control_of_Passengers_Seat_Functions
                 0x02,  // Remote_control_of_Passengers_Seat_Functions
         }},
        {245,  // EasyIngressEgress
         {
                 0x01,  // Without_Easy_Ingress_Egress
                 0x02,  // With_Easy_Ingress_Egress_Down
                 0x03,  // With_Easy_Ingress_Egress_Retracting
         }},
        {256,  // BendingLight
         {
                 0x01,  // Without_Bending_Light
                 0x02,  // With_Bending_Light
         }},
        {267,  // IndirectTpmsSystem
         {
                 0x01,  // Without_Indirect_Tyre_Pressure_Monitoring_System
                 0x02,  // Indirect_TPMS_Basic_Algorithm
                 0x03,  // Indirect_TPMS_US_Algorithm
                 0x04,  // Indirect_TPMS_EU_Algorithm
         }},
        {316,  // EmergencyLaneKeepingAid
         {
                 0x01,  // Without_Emergency_Lane_Keeping_Aid_eLKA_
                 0x02,  // eLKA_level_1
                 0x03,  // eLKA_level_2
                 0x04,  // eLKA_level_3
         }},
        {337,  // CenterConsoleSwitchModuleccsm
         {
                 0x01,  // Without_LIN_communication
                 0x02,  // With_Dummy_button
                 0x03,  // With_Drive_Mode_button
         }},
        {338,  // Pre_clean
         {
                 0x01,  // without_Pre_Clean
                 0x02,  // with_Pre_Clean
         }},
        {346,  // SunCurtainRearWindow
         {
                 0x01,  // Without_Electric_Rear_Sun_Curtain
                 0x02,  // With_Electric_Rear_Sun_Curtain
         }},
        {347,  // HeaterType
         {
                 0x01,  // Without_heater
                 0x02,  // FOH_Fuel_Operated_Heater
                 0x03,  // HVCH_High_Voltage_Coolant_heater
                 0x04,  // EPH_Electrical_Parking_Heater_accessory
         }},
        {349,  // IonicInternalAirCleaner
         {
                 0x01,  // Without_Ionic_internal_air_cleaner
                 0x02,  // With_Ionic_internal_air_cleaner
         }},
        {358,  // ActiveNoiseControl
         {
                 0x01,  // Without_Active_Noise_Control
                 0x02,  // Active_Noise_Control_EOE_EOR_
                 0x03,  // Active_Noise_Control_EOE_Engine_Order_Enhancement_
                 0x04,  // Active_Noise_Control_EOR_Engine_Order_Reduction_
         }},
        {359,  // ExteriorSoundHmiOption
         {
                 0x01,  // Exterior_Sound_HMI_Not_Available
                 0x02,  // Exterior_Sound_HMI_Available
         }},
        {360,  // FrequencyBandStepAmfmHdRadio
         {
                 0x01,  // Frequency_band_and_step_for_EU
                 0x02,  // Frequency_band_and_step_for_EU_without_RDS
                 0x03,  // Frequency_band_and_step_for_USA
                 0x04,  // Frequency_band_and_step_for_Japan
                 0x05,  // Frequency_band_and_step_for_China
                 0x06,  // Frequency_band_and_step_for_Australia
                 0x07,  // Frequency_band_and_step_for_Gulf
                 0x08,  // Frequency_band_and_step_for_Thailand
         }},
        {361,  // SimCardForConnectivity
         {
                 0x01,  // Without_SIM
                 0x02,  // only_through_Personal_SIM
                 0x03,  // only_through_Telematic_SIM
                 0x04,  // T_SIM_and_P_SIM_Accessory_
                 0x05,  // Volvo_SIM_Accessory_
         }},
        {362,  // UsbConnection
         {
                 0x01,  // Without_USB_connection
                 0x02,  // With_1_USB_connecion
                 0x03,  // With_USB_hub
         }},
        {363,  // AudioAuxiliaryInput
         {
                 0x01,  // Without_Auxiliary_Audio_Input
                 0x02,  // With_Auxiliary_Audio_Input
         }},
        {365,  // WifiWorkshopModeFunctionalControl
         {
                 0x01,  // WLAN_Workshop_Mode_Disabled
                 0x02,  // WLAN_Workshop_Mode_Enabled
         }},
        {401,  // RainSensorFunction
         {
                 0x01,  // No_Rain_Sensor_Function
                 0x02,  // Rain_Sensor_Function_Activated_By_Button
                 0x03,  // Rain_Sensor_Function_Activated_By_Interval_Position
         }}};
const std::map<int, std::vector<int>> paramListBad = {
        {1,  // VehicleType
         {
                 0x01,  // L541
                 0x02,  // V542
                 0x03,  // V543
                 0x04,  // V526
                 0x05,  // V426
                 0x06,  // V541
                 0x07,  // V316
                 0x08,  // V320
                 0x09,  // V323
                 0x0A,  // V431
                 0x0B,  // V432
                 0x0C,  // L431
                 0x0D,  // V433
                 0x80,  // CX11
                 0x81,  // CS11
                 0x82,  // CH11
                 0x83,  // CC11
                 0x84,  // CS12
                 0xE0,  // HRE1
         }},
        {3,  // TransmissionDriveline
         {
                 0x01,  // Two_wheel_drive
                 0x02,  // AWD_All_wheel_drive
         }},
        {5,  // Engine
         {
                 0x01,  // VED4_2_0L_225_470
                 0x02,  // VEP4_2_0L_190_300
                 0x03,  // VED4_2_0L_190_400
                 0x04,  // VEP4_2_0L_249_350
                 0x05,  // VEP4_2_0L_254_350
                 0x06,  // VEP4_2_0L_260_350_PZEV
                 0x07,  // VEP4_2_0L_233_350
                 0x08,  // VEP4_2_0L_230_350_PZEV
                 0x09,  // VED4_2_0L_150_320
                 0x0A,  // VEP4_2_0L_320_400
                 0x0B,  // VEP4_2_0L_317_400_H_PZEV
                 0x0C,  // VEP4_2_0L_266_380
                 0x0D,  // VEP4_2_0L_266_380_PZEV
                 0x0E,  // VED4_2_0L_190_420
                 0x0F,  // VEP4_2_0L_197_330
                 0x10,  // VED4_2_0L_231_480
                 0x11,  // VED4_2_0L_150_350
                 0x12,  // VEP4_2_0L_156_280
                 0x13,  // VEP4_2_0L_320_400_SULEV
                 0x14,  // VEP4_2_0L_320_400_H
                 0x15,  // VED4_2_0L_190_420_X
                 0x16,  // VED4_2_0L_235_480
                 0x17,  // VEP4_2_0L_254_350_H
                 0x18,  // GEP3_MP_1_5L_156_265
                 0x19,  // GEP3_MP_1_5L_150_265
                 0x1A,  // GEP3_MP_1_5L_180_265_H
                 0x1B,  // VEP4_2_0L_190_300_X
                 0x1C,  // VED4_2_0L_238_480
                 0x1D,  // VED4_2_0L_156_330
                 0x1E,  // VED4_2_0L_197_420
                 0x1F,  // GEP3_LP_1_5L_129_245
                 0xE0,  // GEP_180_265_Non_VCC
         }},
        {6,  // Fuel
         {
                 0x01,  // Petrol
                 0x02,  // Diesel
         }},
        {7,  // EngineGenerationVea2
         {
                 0x01,  // VEA_generation_1
                 0x02,  // VEA_generation_2
                 0x03,  // VEA_generation_3
         }},
        {8,  // SteeringWheelPosition
         {
                 0x01,  // Left_Hand_Drive
                 0x02,  // Right_Hand_Drive
         }},
        {9,  // Gearbox
         {
                 0x01,  // AWF22FWD_Automatic_8_speed_FWD
                 0x02,  // M66FWD_Manual_6_speed_FWD
                 0x03,  // C6FWD_Manual_6_speed_FWD
                 0x04,  // AWF22AWD_Automatic_8_speed_AWD
                 0x05,  // M66AWD_Manual_6_speed_AWD
                 0x06,  // AWF21FWD_Automatic_6_speed_FWD
                 0x07,  // SevenDCTFWD_Dual_Clutch_7_speed_FWD
                 0x08,  // M76FWD_Manual_6_speed_FWD
                 0x09,  // SevenDCTAWD_Dual_Clutch_7_speed_AWD
                 0x0A,  // AW8GFWD_Automatic_8_speed_FWD
                 0x0B,  // AW8GAWD_Automatic_8_speed_AWD
         }},
        {10,  // GearboxType
         {
                 0x01,  // Manual_gearbox
                 0x02,  // Automatic_gearbox
         }},
        {11,  // FuelTankVolume
         {
                 0x01,  // Volume_71_Litre
                 0x02,  // Volume_60_Litre
                 0x03,  // Volume_50_Litre
                 0x04,  // Volume_55_Litre
                 0x05,  // Volume_40_Litre
                 0x06,  // Volume_47_Litre
         }},
        {13,  // PropulsionType
         {
                 0x01,  // Combustion_engine_only
                 0x02,  // Hybrid_vehicle
                 0x03,  // Plugin_hybrid_vehicle
         }},
        {15,  // TurboConfiguration
         {
                 0x01,  // Medium_performance
                 0x02,  // High_performance
                 0x03,  // Low_performance
                 0x04,  // Entry_performance
         }},
        {16,  // PowerSteeringPersonalisation
         {
                 0x01,  // Without_Electro_Power_Steering_Personalisation
                 0x02,  // With_Electro_Power_Steering_Personalisation
         }},
        {18,  // TyreRollingCircumference
         {
                 0x28,  // Rolling_circumference_1900mm
                 0x29,  // Rolling_circumference_1905mm
                 0x2A,  // Rolling_circumference_1910mm
                 0x2B,  // Rolling_circumference_1915mm
                 0x2C,  // Rolling_circumference_1920mm
                 0x2D,  // Rolling_circumference_1925mm
                 0x2E,  // Rolling_circumference_1930mm
                 0x2F,  // Rolling_circumference_1935mm
                 0x30,  // Rolling_circumference_1940mm
                 0x31,  // Rolling_circumference_1945mm
                 0x32,  // Rolling_circumference_1950mm
                 0x33,  // Rolling_circumference_1955mm
                 0x34,  // Rolling_circumference_1960mm
                 0x35,  // Rolling_circumference_1965mm
                 0x36,  // Rolling_circumference_1970mm
                 0x37,  // Rolling_circumference_1975mm
                 0x38,  // Rolling_circumference_1980mm
                 0x39,  // Rolling_circumference_1985mm
                 0x3A,  // Rolling_circumference_1990mm
                 0x3B,  // Rolling_circumference_1995mm
                 0x3C,  // Rolling_circumference_2000mm
                 0x3D,  // Rolling_circumference_2005mm
                 0x3E,  // Rolling_circumference_2010mm
                 0x3F,  // Rolling_circumference_2015mm
                 0x40,  // Rolling_circumference_2020mm
                 0x41,  // Rolling_circumference_2025mm
                 0x42,  // Rolling_circumference_2030mm
                 0x43,  // Rolling_circumference_2035mm
                 0x44,  // Rolling_circumference_2040mm
                 0x45,  // Rolling_circumference_2045mm
                 0x46,  // Rolling_circumference_2050mm
                 0x47,  // Rolling_circumference_2055mm
                 0x48,  // Rolling_circumference_2060mm
                 0x49,  // Rolling_circumference_2065mm
                 0x4A,  // Rolling_circumference_2070mm
                 0x4B,  // Rolling_circumference_2075mm
                 0x4C,  // Rolling_circumference_2080mm
                 0x4D,  // Rolling_circumference_2085mm
                 0x4E,  // Rolling_circumference_2090mm
                 0x4F,  // Rolling_circumference_2095mm
                 0x50,  // Rolling_circumference_2100mm
                 0x51,  // Rolling_circumference_2105mm
                 0x52,  // Rolling_circumference_2110mm
                 0x53,  // Rolling_circumference_2115mm
                 0x54,  // Rolling_circumference_2120mm
                 0x55,  // Rolling_circumference_2125mm
                 0x56,  // Rolling_circumference_2130mm
                 0x57,  // Rolling_circumference_2135mm
                 0x58,  // Rolling_circumference_2140mm
                 0x59,  // Rolling_circumference_2145mm
                 0x5A,  // Rolling_circumference_2150mm
                 0x5B,  // Rolling_circumference_2155mm
                 0x5C,  // Rolling_circumference_2160mm
                 0x5D,  // Rolling_circumference_2165mm
                 0x5E,  // Rolling_circumference_2170mm
                 0x5F,  // Rolling_circumference_2175mm
                 0x60,  // Rolling_circumference_2180mm
                 0x61,  // Rolling_circumference_2185mm
                 0x62,  // Rolling_circumference_2190mm
                 0x63,  // Rolling_circumference_2195mm
                 0x64,  // Rolling_circumference_2200mm
                 0x65,  // Rolling_circumference_2205mm
                 0x66,  // Rolling_circumference_2210mm
                 0x67,  // Rolling_circumference_2215mm
                 0x68,  // Rolling_circumference_2220mm
                 0x69,  // Rolling_circumference_2225mm
                 0x6A,  // Rolling_circumference_2230mm
                 0x6B,  // Rolling_circumference_2235mm
                 0x6C,  // Rolling_circumference_2240mm
                 0x6D,  // Rolling_circumference_2245mm
                 0x6E,  // Rolling_circumference_2250mm
                 0x6F,  // Rolling_circumference_2255mm
                 0x70,  // Rolling_circumference_2260mm
                 0x71,  // Rolling_circumference_2265mm
                 0x72,  // Rolling_circumference_2270mm
                 0x73,  // Rolling_circumference_2275mm
                 0x74,  // Rolling_circumference_2280mm
                 0x75,  // Rolling_circumference_2285mm
                 0x76,  // Rolling_circumference_2290mm
                 0x77,  // Rolling_circumference_2295mm
                 0x78,  // Rolling_circumference_2300mm
                 0x79,  // Rolling_circumference_2305mm
                 0x7A,  // Rolling_circumference_2310mm
                 0x7B,  // Rolling_circumference_2315mm
                 0x7C,  // Rolling_circumference_2320mm
                 0x7D,  // Rolling_circumference_2325mm
                 0x7E,  // Rolling_circumference_2330mm
                 0x7F,  // Rolling_circumference_2335mm
                 0x80,  // Rolling_circumference_2340mm
                 0x81,  // Rolling_circumference_2345mm
                 0x82,  // Rolling_circumference_2350mm
                 0x83,  // Rolling_circumference_2355mm
                 0x84,  // Rolling_circumference_2360mm
                 0x85,  // Rolling_circumference_2365mm
                 0x86,  // Rolling_circumference_2370mm
                 0x87,  // Rolling_circumference_2375mm
                 0x88,  // Rolling_circumference_2380mm
                 0x89,  // Rolling_circumference_2385mm
                 0x8A,  // Rolling_circumference_2390mm
                 0x8B,  // Rolling_circumference_2395mm
                 0x8C,  // Rolling_circumference_2400mm
         }},
        {19,  // TyrePressureMonitoringSystem
         {
                 0x01,  // Without_Monitoring_system_TPMS
                 0x02,  // TPMS_WBA_US_algorithm
                 0x03,  // TPMS_WBL_US_algorithm
                 0x04,  // TPMS_WBA_EU_algorithm
                 0x05,  // TPMS_WBL_EU_algorithm
         }},
        {20,  // TpmsMenuFunctionsInIhu
         {
                 0x01,  // No_TPMS
                 0x02,  // TPMS_Reset_menu
                 0x03,  // TPMS_ShutOff_menu
                 0x04,  // TPMS_Show_Pressures_menu
                 0x05,  // TPMS_Reset_and_Show_Pressures_menu
                 0x06,  // TPMS_Reset_and_ShutOff_menu
                 0x07,  // TPMS_ShutOff_and_Show_Pressures_menu
                 0x08,  // TPMS_ShutOff_Show_Pressures_and_Reset_Menu
         }},
        {23,  // CruiseControl
         {
                 0x01,  // No_Cruise_control
                 0x02,  // Cruise_control
                 0x03,  // Adaptive_Cruise_Control_Level1
                 0x04,  // Adaptive_Cruise_Control_Stop_Go_Level1
                 0x05,  // Not_in_use
                 0x06,  // Adaptive_Cruise_Control_Level2
                 0x07,  // Adaptive_Cruise_Control_Stop_Go_Level2
                 0x08,  // Adaptive_Cruise_Control_Level3
                 0x09,  // Adaptive_Cruise_Control_Stop_Go_Level3
         }},
        {24,  // Seats
         {
                 0x01,  // Not_applicable_variant_no_parts_allowed
                 0x02,  // Fiveseat
                 0x03,  // Sevenseat
                 0x04,  // Fourseat
         }},
        {28,  // RadioAmpSpeakerSystem
         {
                 0x01,  // Performance_audio
                 0x02,  // High_Performance_audio
                 0x03,  // Premium_Sound_audio
                 0x04,  // High_Performance_Plus_audio
         }},
        {36,  // AdjustableSpeedLimiter
         {
                 0x01,  // Without_Adjustable_speed_limiter
                 0x02,  // Adjustable_speed_limiter
                 0x03,  // ASL_and_AVSL
                 0x04,  // AVSL
         }},
        {39,  // ShiftByWire
         {
                 0x01,  // Without_Shift_By_Wire
                 0x02,  // With_Shift_By_Wire
                 0x03,  // SHIFT_BY_WIRE_pattern_type_2_
         }},
        {43,  // MaxVehicleSpeedLimitation
         {
                 0x01,  // Without_speed_limit
                 0x02,  // Speed_limit_100_km_h
                 0x03,  // Speed_limit_105_km_h
                 0x04,  // Speed_limit_110_km_h
                 0x05,  // Speed_limit_115_km_h
                 0x06,  // Speed_limit_120_km_h
                 0x07,  // Speed_limit_125_km_h
                 0x08,  // Speed_limit_130_km_h
                 0x09,  // Speed_limit_135_km_h
                 0x0A,  // Speed_limit_140_km_h
                 0x0B,  // Speed_limit_145_km_h
                 0x0C,  // Speed_limit_150_km_h
                 0x0D,  // Speed_limit_155_km_h
                 0x0E,  // Speed_limit_160_km_h
                 0x0F,  // Speed_limit_165_km_h
                 0x10,  // Speed_limit_170_km_h
                 0x11,  // Speed_limit_175_km_h
                 0x12,  // Speed_limit_180_km_h
                 0x13,  // Speed_limit_185_km_h
                 0x14,  // Speed_limit_190_km_h
                 0x15,  // Speed_limit_195_km_h
                 0x16,  // Speed_limit_200_km_h
                 0x17,  // Speed_limit_205_km_h
                 0x18,  // Speed_limit_210_km_h
                 0x19,  // Speed_limit_215_km_h
                 0x1A,  // Speed_limit_220_km_h
                 0x1B,  // Speed_limit_225_km_h
                 0x1C,  // Speed_limit_230_km_h
                 0x1D,  // Speed_limit_235_km_h
                 0x1E,  // Speed_limit_240_km_h
                 0x1F,  // Speed_limit_245_km_h
                 0x20,  // Speed_limit_250_km_h
                 0x21,  // Speed_limit_255_km_h
                 0x22,  // Speed_limit_260_km_h
                 0x23,  // Speed_limit_265_km_h
                 0x24,  // Speed_limit_270_km_h
                 0x25,  // Speed_limit_275_km_h
                 0x26,  // Speed_limit_280_km_h
                 0x27,  // Speed_limit_285_km_h
                 0x28,  // Speed_limit_290_km_h
                 0x29,  // Speed_limit_295_km_h
                 0x2A,  // Speed_limit_300_km_h
                 0x2B,  // Speed_limit_60_mph
                 0x2C,  // Speed_limit_65_mph
                 0x2D,  // Speed_limit_70_mph
                 0x2E,  // Speed_limit_75_mph
                 0x2F,  // Speed_limit_80_mph
                 0x30,  // Speed_limit_85_mph
                 0x31,  // Speed_limit_90_mph
                 0x32,  // Speed_limit_95_mph
                 0x33,  // Speed_limit_100_mph
                 0x34,  // Speed_limit_105_mph
                 0x35,  // Speed_limit_110_mph
                 0x36,  // Speed_limit_115_mph
                 0x37,  // Speed_limit_120_mph
                 0x38,  // Speed_limit_125_mph
                 0x39,  // Speed_limit_130_mph
                 0x3A,  // Speed_limit_135_mph
                 0x3B,  // Speed_limit_140_mph
                 0x3C,  // Speed_limit_145_mph
                 0x3D,  // Speed_limit_150_mph
                 0x3E,  // Speed_limit_155_mph
                 0x3F,  // Speed_limit_160_mph
                 0x40,  // Speed_limit_165_mph
                 0x41,  // Speed_limit_170_mph
                 0x42,  // Speed_limit_175_mph
                 0x43,  // Speed_limit_180_mph
                 0x44,  // Speed_limit_185_mph
                 0x45,  // Speed_limit_190_mph
         }},
        {47,  // SelectiveCatalyticReduction
         {
                 0x01,  // Without_UREA
                 0x02,  // With_UREA
         }},
        {48,  // OilLevelSensor
         {
                 0x01,  // Without_oil_level_sensor
                 0x02,  // Oil_level_sensor
         }},
        {50,  // ChassisType
         {
                 0x01,  // Chassis_STD
                 0x02,  // Chassis_SPORT_std_height
                 0x03,  // Chassis_DYNAMIC
                 0x04,  // Chassis_CCD
                 0x05,  // Chassi_P_Police
                 0x06,  // Chassi_T_Taxi
                 0x07,  // Chassis_TOURING
                 0x08,  // Chassis_SPORT_low
         }},
        {51,  // TrailerStabilityAssist
         {
                 0x01,  // Without_TRaiLer_STaBility_Assist
                 0x02,  // TRaiLer_STaBility_Assist
         }},
        {53,  // HillDescentControl
         {
                 0x01,  // Without_Hill_Descent_Control
                 0x02,  // Hill_Descent_Control
         }},
        {57,  // DriveMode
         {
                 0x01,  // Without_drive_mode_switch
                 0x02,  // With_drive_mode_switch
         }},
        {58,  // AirSuspension
         {
                 0x01,  // Without_air_suspension
                 0x02,  // Air_suspension_2_corner
                 0x03,  // Air_suspension_4_corner
         }},
        {59,  // ActiveSuspension
         {
                 0x01,  // Without_Continious_Controlled_Chassi
                 0x02,  // Continious_Controlled_Chassi
         }},
        {65,  // InteriorMotionSensor
         {
                 0x01,  // Without_Interior_Motion_Sensor
                 0x02,  // Interior_Motion_Sensor
         }},
        {66,  // InclinationSensor
         {
                 0x01,  // Without_inclination_sensor
                 0x02,  // With_inclination_sensor
         }},
        {70,  // PassiveArming
         {
                 0x01,  // Without_passive_arming
                 0x02,  // With_passive_arming
         }},
        {71,  // ForeignComponentDetection
         {
                 0x01,  // Without_Foreign_Component_Detection
                 0x02,  // Foreign_Component_Detection
         }},
        {85,  // CentralLockType
         {
                 0x01,  // Without_blocked_lock_mode
                 0x02,  // Blocked_lock_mode
         }},
        {88,  // PrivateLockingLuggageComp
         {
                 0x01,  // Private_locking_disabled
                 0x02,  // Private_locking_enabled
         }},
        {90,  // AutomaticLockingBySpeed
         {
                 0x01,  // Automatic_Locking_by_speed_always_ON_disabled_in_menu
                 0x02,  // Automatic_Locking_by_speed_enabled_default_OFF
                 0x03,  // Automatic_Locking_by_speed_enabled_default_ON
         }},
        {91,  // TwoStepUnlocking
         {
                 0x01,  // Two_step_unlocking_default_OFF
                 0x02,  // Two_step_unlocking_default_ON
         }},
        {94,  // KeylessEntry
         {
                 0x01,  // Without_Keyless_entry
                 0x02,  // Passive_entry_Personal_Car_Communicator_incl_remote_tag
         }},
        {95,  // VisibleLockingunlocking
         {
                 0x01,  // Visible_locking_and_unlocking_feedback_default_OFF
                 0x02,  // Visible_locking_and_unlocking_feedback_default_ON
         }},
        {96,  // AudibleLockingFeedback
         {
                 0x01,  // Audible_locking_feedback_disabled
                 0x02,  // Audible_locking_feedback_enabled_default_OFF
                 0x03,  // Audible_locking_feedback_enabled_default_ON
         }},
        {106,  // HeadlightsType
         {
                 0x01,  // Halogen_lights_without_adjuster
                 0x02,  // Halogen_lights_with_adjuster
                 0x03,  // Xenon_Automatic_Bending_Light_type_A
                 0x04,  // Xenon_Automatic_Bending_Light_type_B
                 0x05,  // LED_With_Bending_Light_type_A_Top_Range
                 0x06,  // LED_With_Bending_Light_type_B_Top_Range
                 0x07,  // LED_Without_Bending_Light_type_A_Mid_Range
                 0x08,  // LED_Without_Bending_Light_type_B_Mid_Range
                 0x09,  // LED_type_A_with_HCM_US_CND_Mid_Range
                 0x0A,  // LED_type_B_with_HCM_ROW_Mid_Range
                 0x0B,  // LED_type_A_with_HCM_US_CND_High_Range
                 0x0C,  // LED_type_B_with_HCM_ROW_High_Range
                 0x0D,  // HALO_B_Halogen_lights_with_adjuster_LED_Supplement_Lamp_for_DRL_Pos_TI
                 0x0E,  // LED_Type_B_with_HCM_LED_Supplement_Lamp_for_DRL_Pos_TI
         }},
        {107,  // ExteriorLightLogic
         {
                 0x01,  // DDRL_not_together_with_position_light
                 0x02,  // Without_DDRL
                 0x03,  // DDRL_in_pos_0_user_configurable
                 0x04,  // DDRL_in_pos_Auto_user_configurable
                 0x05,  // DDRL_in_all_positions
                 0x06,  // Without_DDRL_with_position_light_after_market
         }},
        {109,  // ActiveHighBeam
         {
                 0x01,  // Without_AHB
                 0x02,  // With_AHB1_On_Off_Low_Beam_in_Village
                 0x03,  // With_AHB1_On_Off_High_Beam_in_Small_Village
                 0x04,  // With_AHB2_Adaptive_Low_Beam_in_Village
                 0x05,  // With_AHB2_Adaptive_High_Beam_in_Small_Village
         }},
        {111,  // TouristLight
         {
                 0x01,  // Without_configurable_Tourist_Light
                 0x02,  // Left_asymmetry_with_configurable_Tourist_Light
                 0x03,  // Right_asymmetry_with_configurable_Tourist_Light
         }},
        {112,  // CorneringLight
         {
                 0x01,  // Without_cornering_light
                 0x02,  // With_cornering_light
         }},
        {119,  // AuxiliaryLights
         {
                 0x01,  // Without_auxiliary_lights
                 0x02,  // With_Auxiliary_lights
         }},
        {122,  // HeatedWindscreen
         {
                 0x01,  // No_Heated_Frontscreen
                 0x02,  // Heated_Frontscreen
         }},
        {124,  // RearWindowWiper
         {
                 0x01,  // No_rear_window_wiper
                 0x02,  // Rear_Window_wiper_with_washer
         }},
        {132,  // CutoffSwitchPassengerAirbag
         {
                 0x01,  // Without_Cut_Off_Switch_Passenger_Airbag
                 0x02,  // Cut_Off_Switch_Passenger_AirBag
         }},
        {141,  // NightVision
         {
                 0x01,  // Without_Night_Vision
                 0x02,  // With_Night_Vision
         }},
        {142,  // ParkingAssistance
         {
                 0x01,  // Without_parking_assistance
                 0x02,  // Parking_assistance_rear
                 0x03,  // Parking_assistance_front_and_rear
                 0x04,  // Parking_assistance_Semi_Automatic_Parking
                 0x05,  // Parking_assistance_Full_Automatic_Parking
         }},
        {143,  // DriverImpairmentMonitor
         {
                 0x01,  // Without_Driver_Impairment_Monitor
                 0x02,  // With_Driver_Impairment_Monitor
         }},
        {144,  // DistanceAlert
         {
                 0x01,  // Without_Distance_Alert
                 0x02,  // With_Distance_Alert
         }},
        {145,  // TrafficJamAssist
         {
                 0x01,  // Without_Traffic_Jam_Assist
                 0x02,  // With_Traffic_Jam_Assist_1
                 0x03,  // With_Traffic_Jam_Assist_2
         }},
        {146,  // CollisionMitigationSupportFront
         {
                 0x01,  // Without_Collision_Mitigation_Support_Front
                 0x02,  // With_Collision_Mitigation_Support_Front_ver_1
                 0x03,  // With_Collision_Mitigation_Support_Front_ver_2
         }},
        {147,  // RoadFrictionInformation
         {
                 0x01,  // Without_Road_friction_indication
                 0x02,  // Road_friction_indication
         }},
        {148,  // CurveSpeedSupport
         {
                 0x01,  // Without_Curve_Speed_Warning_without_Curve_Speed_Assist
                 0x02,  // With_Curve_Speed_Warning_without_Curve_Speed_Assist
                 0x03,  // Without_Curve_Speed_Warning_with_Curve_Speed_Assist
                 0x04,  // With_Curve_Speed_Warning_with_Curve_Speed_Assist
         }},
        {149,  // RoadSignInformation
         {
                 0x01,  // Without_TSI_Traffic_Sign_Information_SLA_Speed_limit_Adaption_
                 0x02,  // SWEDEN_TSI_without_SLA
                 0x03,  // SWEDEN_TSI_with_SLA
                 0x04,  // IRELAND_TSI_without_SLA
                 0x05,  // IRELAND_TSI_with_SLA
                 0x06,  // ENGLAND_TSI_without_SLA
                 0x07,  // ENGLAND_TSI_with_SLA
                 0x08,  // MID_EU_TSI_without_SLA
                 0x09,  // MID_EU_TSI_with_SLA
                 0x0A,  // EAST_EUROPE_TSI_without_SLA
                 0x0B,  // EAST_EUROPE_TSI_with_SLA
                 0x0C,  // SOUTH_EU_TSI_without_SLA
                 0x0D,  // SOUTH_EU_TSI_with_SLA
                 0x0E,  // USA_TSI_without_SLA
                 0x0F,  // USA_TSI_with_SLA
                 0x10,  // MEXICO_TSI_without_SLA
                 0x11,  // MEXICO_TSI_with_SLA
                 0x12,  // CANADA_TSI_without_SLA
                 0x13,  // CANADA_TSI_with_SLA
                 0x14,  // SINGAPORE_TSI_without_SLA
                 0x15,  // SINGAPORE_TSI_with_SLA
                 0x16,  // KOREA_TSI_without_SLA
                 0x17,  // KOREA_TSI_with_SLA
                 0x18,  // LATIN_AMERICA_TSI_without_SLA
                 0x19,  // LATIN_AMERICA_TSI_with_SLA
                 0x1A,  // ARABIA_TSI_without_SLA
                 0x1B,  // ARABIA_TSI_with_SLA
                 0x1C,  // HONGKONG_TSI_without_SLA
                 0x1D,  // HONGKONG_TSI_with_SLA
                 0x1E,  // TAIWAN_TSI_without_SLA
                 0x1F,  // TAIWAN_TSI_with_SLA
                 0x20,  // AUSTRALIA_TSI_without_SLA
                 0x21,  // AUSTRALIA_TSI_with_SLA
                 0x22,  // SOUTH_AFRICA_TSI_without_SLA
                 0x23,  // SOUTH_AFRICA_TSI_with_SLA
                 0x24,  // TURKEY_TSI_without_SLA
                 0x25,  // TURKEY_TSI_with_SLA
                 0x26,  // JAPAN_TSI_without_SLA
                 0x27,  // JAPAN_TSI_with_SLA
                 0x28,  // CHINA_TSI_without_SLA
                 0x29,  // CHINA_TSI_with_SLA
                 0x2A,  // INDIA_TSI_without_SLA
                 0x2B,  // INDIA_TSI_with_SLA
                 0x2C,  // BRAZIL_TSI_without_SLA
                 0x2D,  // BRAZIL_TSI_with_SLA
                 0x2E,  // THAILAND_TSI_without_SLA
                 0x2F,  // THAILAND_TSI_with_SLA
                 0x30,  // MALAYSIA_TSI_without_SLA
                 0x31,  // MALAYSIA_TSI_with_SLA
                 0x32,  // OTHERS_TSI_without_SLA
                 0x33,  // OTHERS_TSI_with_SLA
                 0xF0,  // Not_allowed_varaint
         }},
        {150,  // LaneKeepingAid
         {
                 0x01,  // Without_Lane_Keeping_Aid_Lane_Departure_Warning
                 0x02,  // Lane_Keeping_Aid
                 0x05,  // Lane_Departure_Warning
         }},
        {151,  // ElectronicHorizon
         {
                 0x01,  // Without_Electronic_Horizon
                 0x02,  // With_Electronic_Horizon
         }},
        {152,  // BlindSpotInformationSystem
         {
                 0x01,  // Without_Blind_spot_Information_System
                 0x02,  // Blind_spot_Information_System_with_CTA
                 0x03,  // Without_Blind_spot_Information_System_sensors_fitted
                 0x04,  // BLIS_System_with_audio_and_CTA_for_Geely
         }},
        {153,  // CollisionMitigWarnRear
         {
                 0x01,  // Without_Rear_Collision_Warning_and_Mitigation
                 0x02,  // With_Rear_Collision_Warning_and_Mitigation
                 0x03,  // With_Rear_Collision_Mitigation
                 0x04,  // With_Rear_Collision_Warning
         }},
        {154,  // ParkingCamera
         {
                 0x01,  // Without_Parking_Camera
                 0x02,  // Rear_Parking_Camera
                 0x03,  // Parking_camera_360
                 0x04,  // Front_Parking_Camera_accessory_
                 0x05,  // Rear_Parking_Camera_always_activated_during_reverse
                 0x06,  // Parking_camera_360_always_activated_during_reverse
                 0x07,  // Parking_Camera_preparation_front_rear
                 0x08,  // Rear_Parking_Camera_GEN_2
                 0x09,  // Rear_Parking_Camera_GEN_2_always_activated_during_reverse
                 0x0A,  // Rear_Parking_Camera_Basic_standard_US
                 0x0B,  // Parking_camera_360_GEN_2
                 0x0C,  // Parking_camera_360_GEN_2_always_activated_during
         }},
        {156,  // RearWiewMirrorsouter
         {
                 0x01,  // Without_retractable_mirrors
                 0x02,  // Retractable_mirrors
         }},
        {157,  // CombinedInstrument
         {
                 0x01,  // Drivers_Information_Module_with_8_inch_screen
                 0x02,  // Drivers_Information_Module_with_12_3_inch_screen
                 0x03,  // Drivers_Information_Module_with_4_2_inch_screen_KMH
                 0x04,  // Drivers_Information_Module_with_4_2_inch_screen_MPH
                 0x80,  // Drivers_Information_Module_with_7_inch_screen_Non_VCC
         }},
        {158,  // HeadUpDisplay
         {
                 0x01,  // With_Head_Up_Display_FCW
                 0x02,  // With_Head_Up_Display_graphic
                 0x03,  // Without_Head_Up_Display
                 0x04,  // With_Head_Up_Display_graphic_2
         }},
        {161,  // UnitSettings
         {
                 0x01,  // Temp_C_Clock_24_Distance_KM_Fuel_Liter_KM
                 0x02,  // Temp_C_Clock_24_Distance_KM_Fuel_KM_Liter
                 0x03,  // Temp_C_Clock_24_Distance_Km_Fuel_MPG_US_
                 0x04,  // Temp_C_Clock_24_Distance_Km_Fuel_MPG_UK_
                 0x05,  // Temp_C_Clock_24_Distance_Miles_Fuel_L_Km
                 0x06,  // Temp_C_Clock_24_Distance_Miles_Fuel_Km_L
                 0x07,  // Temp_C_Clock_24_Distance_Miles_Fuel_MPG_US_
                 0x08,  // Temp_C_Clock_24_Distance_Miles_Fuel_UK_Gallon
                 0x09,  // Temp_C_Clock_12_Distance_KM_Fuel_Liter_KM
                 0x0A,  // Temp_C_Clock_12_Distance_Km_Fuel_Km_L
                 0x0B,  // Temp_C_Clock_12_Distance_Km_Fuel_MPG_US_
                 0x0C,  // Temp_C_Clock_12_Distance_Km_Fuel_MPG_UK_
                 0x0D,  // Temp_C_Clock_12_Distance_Miles_Fuel_L_Km
                 0x0E,  // Temp_C_Clock_12_Distance_Miles_Fuel_Km_L
                 0x0F,  // Temp_C_Clock_12_Distance_Miles_Fuel_MPG_US_
                 0x10,  // Temp_C_Clock_12_Distance_Miles_Fuel_MPG_UK_
                 0x11,  // Temp_F_Clock_24_Distance_Km_Fuel_L_Km
                 0x12,  // Temp_F_Clock_24_Distance_Km_Fuel_Km_L
                 0x13,  // Temp_F_Clock_24_Distance_Km_Fuel_MPG_US_
                 0x14,  // Temp_F_Clock_24_Distance_Km_Fuel_MPG_UK_
                 0x15,  // Temp_F_Clock_24_Distance_Miles_Fuel_L_Km
                 0x16,  // Temp_F_Clock_24_Distance_Miles_Fuel_Km_L
                 0x17,  // Temp_F_Clock_24_Distance_Miles_Fuel_MPG_US_
                 0x18,  // Temp_F_Clock_24_Distance_Miles_Fuel_MPG_UK_
                 0x19,  // Temp_F_Clock_12_Distance_Km_Fuel_L_Km
                 0x1A,  // Temp_F_Clock_12_Distance_Km_Fuel_Km_L
                 0x1B,  // Temp_F_Clock_12_Distance_Km_Fuel_MPG_US_
                 0x1C,  // Temp_F_Clock_12_Distance_Km_Fuel_MPG_UK_
                 0x1D,  // Temp_F_Clock_12_Distance_Miles_Fuel_L_Km
                 0x1E,  // Temp_F_Clock_12_Distance_Miles_Fuel_Km_L
                 0x1F,  // Temp_F_Clock_12_Distance_Miles_Fuel_US_Gallon
                 0x20,  // Temp_F_Clock_12_Distance_Miles_Fuel_MPG_UK_
         }},
        {167,  // TripComputerAndStatistics
         {
                 0x01,  // Without_trip_comp_and_trip_stat
                 0x02,  // With_trip_comp_and_without_trip_stat
                 0x03,  // Without_trip_comp_and_with_trip_stat
                 0x04,  // With_trip_comp_and_with_trip_stat
         }},
        {168,  // ScreenThemesskins
         {
                 0x01,  // Screen_Skins_Standard_1_
                 0x02,  // Screen_Skins_R_Design_
                 0x03,  // Screen_Skins_Inscription_
                 0x04,  // Screen_Skins_VOR_
         }},
        {170,  // HvacRear
         {
                 0x01,  // Without_rear_VAC
                 0x02,  // With_rear_VAC_without_PTCs
                 0x03,  // With_rear_VAC_with_PTCs
         }},
        {171,  // AdditionalHeater
         {
                 0x01,  // Without_Additional_Heater
                 0x02,  // With_Additional_Heater
         }},
        {174,  // AirQualitySystem
         {
                 0x01,  // No_Air_Quality_System_Pollenfilter
                 0x02,  // Air_Quality_System_Multifilter
                 0x03,  // No_Air_Quality_System_Multifilter
                 0x04,  // Air_Quality_System_Multifilter2
         }},
        {175,  // HvacVariants
         {
                 0x01,  // Electronic_Climate_Control_2_zone
                 0x02,  // Electronic_Climate_Control_4_zone
                 0x03,  // HVAC_Small_1_zone
                 0x04,  // HVAC_Small_2_zone
                 0x05,  // HVAC_Small_3_zone
                 0x06,  // Manual_AC
         }},
        {176,  // RearViewMirrorType
         {
                 0x01,  // Manual_dimming
                 0x02,  // Automatic_dimming_inner
                 0x03,  // Automatic_dimming_inner_outer
         }},
        {177,  // RoofHatch
         {
                 0x01,  // No_roof_hatch_panorama_roof
                 0x02,  // Openable_panorama_roof
                 0x03,  // Hatch
         }},
        {179,  // FrontSeatVentilation
         {
                 0x01,  // Without_ventilated_seats
                 0x02,  // Ventilated_seats
                 0x03,  // Ventilated_seat_driver_seat
         }},
        {180,  // HeatedFrontSeats
         {
                 0x01,  // Without_heated_front_seats
                 0x02,  // Heated_front_seats
         }},
        {181,  // HeatedRearSeat
         {
                 0x01,  // Without_heated_rear_seat
                 0x02,  // Heated_rear_seat
         }},
        {182,  // ParkingClimateLevel
         {
                 0x01,  // No_parking_climate
                 0x02,  // Afterrun_parking_climate
                 0x03,  // Heating_parking_climate
                 0x04,  // Full_parking_climate
         }},
        {183,  // InteriorIllumination
         {
                 0x01,  // Interior_lighting_base_version
                 0x02,  // Interior_lighting_high_version
                 0x03,  // Interior_lighting_mid_version
                 0x11,  // Interior_lighting_base_version_not_for_Volvo
                 0x12,  // Interior_lighting_mid_version_not_for_Volvo
                 0x13,  // Interior_lighting_high_version_not_for_Volvo
         }},
        {184,  // PowerSeats
         {
                 0x01,  // No_Memory_Seats
                 0x02,  // Driver_Memory_seat_and_Mechanical_passenger_seat
                 0x03,  // Driver_Memory_seat_and_Passenger_seat_without_memory
                 0x04,  // Driver_and_Passenger_Memory_Seats
         }},
        {185,  // PowerFoldRise3dRow
         {
                 0x01,  // No_fold_rise_3_d_row_rear_seat
                 0x02,  // Mecanical_fold_rise_3_d_row_rear_seat
                 0x03,  // Power_fold_rise_3_d_row_rear_seat
         }},
        {186,  // HeatedSteeringWheel
         {
                 0x01,  // Without_heated_steering_wheel
                 0x02,  // Heated_steering_wheel
         }},
        {187,  // Headrests2ndRowFolding
         {
                 0x01,  // Manually_foldable_rear_headrests
                 0x02,  // El_folding_rear_headrests
                 0x03,  // Fixed_rear_headrests
                 0x04,  // Rear_headsrests_Comfort
         }},
        {189,  // RearSeatVentilation
         {
                 0x01,  // Without_rear_seat_ventilation
                 0x02,  // With_rear_seat_ventilation
         }},
        {190,  // GlobalNavigationSatSystemReceiver
         {
                 0x01,  // Without_Global_Navigation_Satellite_System_receiver
                 0x02,  // With_Global_Navigation_Satellite_System_receiver
         }},
        {191,  // TelematicModule
         {
                 0x01,  // Without_Telematic_module
                 0x02,  // With_Telematic_module
                 0x03,  // With_Telematic_module_Brazil
                 0x04,  // With_Telematic_module_USA_CDN
                 0x05,  // With_Telematic_module_China
                 0x06,  // With_Telematic_module_ROW_LTE
                 0x07,  // With_Telematic_module_Russia
         }},
        {192,  // IhuLevel
         {
                 0x01,  // IHU_low
                 0x02,  // IHU_high
                 0x03,  // IHU_mid
         }},
        {193,  // DigitalRadioReceiver
         {
                 0x01,  // Without_digital_radio_system
                 0x02,  // DAB_radio_system
                 0x03,  // HD_SDARS_radio_system_US
                 0x04,  // Not_in_use
                 0x05,  // HD_SDARS_radio_system_Can
         }},
        {194,  // RearSeatEntertainment
         {
                 0x01,  // Without_Rear_Seat_Entertainment
                 0x02,  // Rear_Seat_Entertainment
         }},
        {195,  // RegionIfDvd
         {
                 0x01,  // DVD_region_0
                 0x02,  // DVD_region_1
                 0x03,  // DVD_region_2
                 0x04,  // DVD_region_3
                 0x05,  // DVD_region_4
                 0x06,  // DVD_region_5
                 0x07,  // DVD_region_6
                 0x08,  // DVD_region_7
                 0x09,  // DVD_region_8
                 0x0A,  // DVD_region_9
                 0x0B,  // DVD_region_10
         }},
        {196,  // WifiFrequencyBand
         {
                 0x01,  // Two_4_GHz
                 0x02,  // Five_GHz
                 0x03,  // Two_4_GHz_and_5_GHz
         }},
        {197,  // SystemLanguage
         {
                 0x01,  // Arabic
                 0x02,  // Chinese_Simp_Man
                 0x03,  // Chinese_Trad_Can
                 0x04,  // Chinese_Trad_Man
                 0x05,  // Czech
                 0x06,  // Danish
                 0x07,  // Dutch
                 0x08,  // Australien_English
                 0x09,  // English
                 0x0A,  // American_English
                 0x0B,  // Finnish
                 0x0C,  // Flemmish
                 0x0D,  // Canadian_French
                 0x0E,  // French
                 0x0F,  // German
                 0x10,  // Greek
                 0x11,  // Hungarian
                 0x12,  // Italian
                 0x13,  // Japanese
                 0x14,  // Korean_Han_Gul_
                 0x15,  // Norwegian
                 0x16,  // Polish
                 0x17,  // Brazilian_Portuguese
                 0x18,  // Portuguese
                 0x19,  // Russian
                 0x1A,  // Spanish
                 0x1B,  // American_Spanish
                 0x1C,  // Swedish
                 0x1D,  // Thai
                 0x1E,  // Turkish
                 0x1F,  // Bulgarian
                 0x20,  // Estonian
                 0x21,  // Latvian
                 0x22,  // Lithuanian
                 0x23,  // Romanian
                 0x24,  // Slovak
                 0x25,  // Slovene
         }},
        {198,  // Speech
         {
                 0x01,  // No_speech
                 0x02,  // Speech
         }},
        {199,  // SpeechLanguage
         {
                 0x07,  // English
         }},
        {200,  // WifiFunctionalityControl
         {
                 0x01,  // N_STA_N_AP_No_Wi_Fi_Station_No_Wi_Fi_AP
                 0x02,  // STA_N_AP_Wi_Fi_Station_No_Wi_Fi_AP
                 0x03,  // N_STA_AP_No_Wi_Fi_Station_Wi_Fi_AP
                 0x04,  // STA_AP_Wi_Fi_Station_Wi_Fi_AP
         }},
        {201,  // Connectivity
         {
                 0x01,  // No_Connectivity
                 0x02,  // Connectivity
         }},
        {202,  // OwnersManual
         {
                 0x01,  // Without_digital_owners_manual
                 0x02,  // Digital_owners_manual
                 0x03,  // Canadian_French
                 0x04,  // Chinese_simplified
                 0x05,  // Chinese_traditional
                 0x06,  // Czech
                 0x07,  // Danish
                 0x08,  // Dutch
                 0x09,  // English_International
                 0x0A,  // English_UK
                 0x0B,  // Estonian
                 0x0C,  // Finnish
                 0x0D,  // French
                 0x0E,  // German
                 0x0F,  // Italian
                 0x10,  // Japanese
                 0x11,  // Latvian
                 0x12,  // Lithuanian
                 0x13,  // Norska
                 0x14,  // Polish
                 0x15,  // Portuguese
                 0x16,  // Russian
                 0x17,  // Spanish
                 0x18,  // Swedish
                 0x19,  // Thai
                 0x1A,  // Turkish
                 0x1B,  // Ukrainian
                 0x1C,  // Us_English
                 0x1D,  // Greek
                 0x1E,  // Hungarian
         }},
        {203,  // Messaging
         {
                 0x01,  // Messaging_disabled
                 0x02,  // Messaging_enabled
         }},
        {204,  // TvTuner
         {
                 0x01,  // Without_TV_tuner
                 0x02,  // With_DVB_T_tuner_8Mhz
                 0x03,  // With_ISDB_T_tuner
                 0x04,  // With_DTMB_T_tuner
                 0x05,  // With_SBTVD_tuner
                 0x06,  // With_T_DMB_tuner
                 0x07,  // With_DVB_T_tuner_6Mhz
         }},
        {205,  // DsrcUnit
         {
                 0x01,  // Without_DSRC_unit
                 0x02,  // DSRC_unit
         }},
        {206,  // DualMediaVicsAntenna
         {
                 0x01,  // Without_VICS
                 0x02,  // With_VICS
         }},
        {207,  // VideoAllowed
         {
                 0x01,  // Never
                 0x02,  // Always_All_speeds_
                 0x03,  // Speed_dependent
         }},
        {209,  // RoadAndTrafficInfoRti
         {
                 0x01,  // Without_RTI_map
                 0x02,  // Map_region_for_Europe
                 0x03,  // Map_region_for_North_America
                 0x04,  // Map_region_for_South_America
                 0x05,  // Map_region_for_Africa_and_Middle_East
                 0x06,  // Map_region_for_South_East_Asia
                 0x07,  // Map_region_for_Pacific_Australia_New_Zeeland_
                 0x08,  // Map_region_for_Russia
                 0x09,  // Map_region_for_Korea
                 0x0A,  // Map_region_for_China
                 0x0B,  // Map_region_for_Taiwan
                 0x0C,  // Map_region_for_Japan
                 0x0D,  // Map_region_for_India
                 0x0E,  // Map_region_for_Israel
                 0x0F,  // Map_region_for_Turkey
         }},
        {210,  // Subwoofer
         {
                 0x01,  // Without_subwoofer
                 0x02,  // Subwoofer
         }},
        {211,  // TrailerModuleAftermarket
         {
                 0x01,  // Without_TRailer_Module
                 0x02,  // TRailer_Module_13_pos_socket_Fixed_towbar
                 0x03,  // TRailer_Module_4_7_pos_socket_Fixed_towbar
                 0x04,  // TRailer_Module_13_pos_socket_Semi_el_retractable_towbar
                 0x05,  // TRailer_Module_4_7_pos_socket_Semi_el_retractable_towbar
         }},
        {212,  // Alcolock
         {
                 0x01,  // Without_Alcolock
                 0x02,  // Alcolock_with_bypass
                 0x03,  // Alcolock_without_bypass
         }},
        {213,  // RunningboardAccessories
         {
                 0x01,  // Without_Runningboard
                 0x02,  // Fixed_Runningboard
                 0x03,  // Retractable_Runningboard
                 0x04,  // Retractable_Runningboard_Special
         }},
        {214,  // RestrictedKeyFunction
         {
                 0x01,  // Without_restricted_key
                 0x02,  // Restricted_key
         }},
        {216,  // AccessoryGatewayUnit
         {
                 0x01,  // Without_AGU
                 0x02,  // With_AGU
                 0x03,  // With_Traffic_Incident_Recorder
                 0x04,  // With_CAM
                 0x05,  // With_TIR_CAM
                 0x06,  // Not_used
                 0x07,  // Not_used_X
                 0x08,  // Not_used_X_X
         }},
        {218,  // MarketTelephoneNetwork
         {
                 0x01,  // Telephone_system_GSM
                 0x02,  // Telephone_system_CDMA2000
                 0x03,  // Telephone_system_TD_SCDMA
                 0x04,  // Telephone_system_OTHER_1_
                 0x05,  // Telephone_system_OTHER_2_
         }},
        {219,  // MicrophoneSetup
         {
                 0x01,  // No_microphone_mounted_in_car
                 0x02,  // One_handsfree_microphone_is_available
                 0x03,  // Two_handsfree_microphones_are_available
                 0x04,  // Three_handsfree_microphones
                 0x05,  // Two_handsfree_microphones_and_two_ANC_microphone_are_available
                 0x06,  // Three_handsfree_microphones_and_two_ANC_microphone_are_available
         }},
        {220,  // LanguageRegion
         {
                 0x01,  // Language_Region_for_World
                 0x02,  // Language_Region_for_Europe
                 0x03,  // Language_Region_for_North_America
                 0x04,  // Language_Region_for_South_America
                 0x05,  // Language_Region_for_Middle_East_Africa
                 0x06,  // Language_Region_for_South_East_Asia
                 0x07,  // Language_Region_for_Pacific
                 0x08,  // Language_Region_for_Russia
                 0x09,  // Language_Region_for_Korea
                 0x0A,  // Language_Region_for_China
                 0x0B,  // Language_Region_for_Taiwan
                 0x0C,  // Language_Region_for_Japan
                 0x0D,  // Language_Region_for_India
                 0x0E,  // Language_Region_for_Israel
                 0x0F,  // Language_Region_for_Turkey
         }},
        {221,  // ConnectedServiceBookingIfConnectivity
         {
                 0x01,  // Without_Connected_Service_Booking
                 0x02,  // Connected_Service_Booking
         }},
        {222,  // Mediaplayer
         {
                 0x01,  // Without_media_player
                 0x02,  // Media_player
                 0x03,  // Media_player_BD
         }},
        {223,  // NavigationSystemAndInternetMap
         {
                 0x01,  // Without_Connected_Navigation_enabled_without_Internet_map_support
                 0x02,  // Without_Connected_Navigation_enabled_with_Internet_map_support
                 0x03,  // With_Connected_Navigation_enabled_without_Internet_map_support
                 0x04,  // With_Connected_Navigation_enabled_with_Internet_map_support
         }},
        {224,  // DeviceMirroring
         {
                 0x01,  // Without_device_mirroring
                 0x02,  // With_device_mirroring_IOS
                 0x03,  // With_device_mirroring_GPM
                 0x04,  // With_device_mirroring_IOS_GPM
         }},
        {239,  // MassageFrontSeat
         {
                 0x01,  // Without_Massage_front_seat
                 0x02,  // Massage_front_seat
         }},
        {240,  // LumbarSupport
         {
                 0x01,  // Fixed_lumbar_support
                 0x02,  // Four_way_lumbar_support
                 0x03,  // Two_way_lumbar_support
         }},
        {241,  // CushionExtension
         {
                 0x01,  // Without_Cushion_Extension
                 0x02,  // With_Cushion_Extension_Driver
                 0x03,  // With_Cushion_Extension_Driver_Passenger
         }},
        {242,  // HeadrestsFront
         {
                 0x01,  // Two_way_headsrests_Front_manual
                 0x02,  // Two_way_headsrests_Front_Power
                 0x03,  // Headsrests_Front_Fixed
         }},
        {243,  // PowerAdjustableSideSupport
         {
                 0x01,  // Without_power_adjustable_side_support
                 0x02,  // Power_adjustable_side_support
         }},
        {244,  // RemoteControlOfPassengerSeatFunctions
         {
                 0x01,  // Without_Remote_control_of_Passengers_Seat_Functions
                 0x02,  // Remote_control_of_Passengers_Seat_Functions
         }},
        {245,  // EasyIngressEgress
         {
                 0x01,  // Without_Easy_Ingress_Egress
                 0x02,  // With_Easy_Ingress_Egress_Down
                 0x03,  // With_Easy_Ingress_Egress_Retracting
         }},
        {256,  // BendingLight
         {
                 0x01,  // Without_Bending_Light
                 0x02,  // With_Bending_Light
         }},
        {267,  // IndirectTpmsSystem
         {
                 0x01,  // Without_Indirect_Tyre_Pressure_Monitoring_System
                 0x02,  // Indirect_TPMS_Basic_Algorithm
                 0x03,  // Indirect_TPMS_US_Algorithm
                 0x04,  // Indirect_TPMS_EU_Algorithm
         }},
        {316,  // EmergencyLaneKeepingAid
         {
                 0x01,  // Without_Emergency_Lane_Keeping_Aid_eLKA_
                 0x02,  // eLKA_level_1
                 0x03,  // eLKA_level_2
                 0x04,  // eLKA_level_3
         }},
        {337,  // CenterConsoleSwitchModuleccsm
         {
                 0x01,  // Without_LIN_communication
                 0x02,  // With_Dummy_button
                 0x03,  // With_Drive_Mode_button
         }},
        {338,  // Pre_clean
         {
                 0x01,  // without_Pre_Clean
                 0x02,  // with_Pre_Clean
         }},
        {346,  // SunCurtainRearWindow
         {
                 0x01,  // Without_Electric_Rear_Sun_Curtain
                 0x02,  // With_Electric_Rear_Sun_Curtain
         }},
        {347,  // HeaterType
         {
                 0x01,  // Without_heater
                 0x02,  // FOH_Fuel_Operated_Heater
                 0x03,  // HVCH_High_Voltage_Coolant_heater
                 0x04,  // EPH_Electrical_Parking_Heater_accessory
         }},
        {349,  // IonicInternalAirCleaner
         {
                 0x01,  // Without_Ionic_internal_air_cleaner
                 0x02,  // With_Ionic_internal_air_cleaner
         }},
        {358,  // ActiveNoiseControl
         {
                 0x01,  // Without_Active_Noise_Control
                 0x02,  // Active_Noise_Control_EOE_EOR_
                 0x03,  // Active_Noise_Control_EOE_Engine_Order_Enhancement_
                 0x04,  // Active_Noise_Control_EOR_Engine_Order_Reduction_
         }},
        {359,  // ExteriorSoundHmiOption
         {
                 0x01,  // Exterior_Sound_HMI_Not_Available
                 0x02,  // Exterior_Sound_HMI_Available
         }},
        {360,  // FrequencyBandStepAmfmHdRadio
         {
                 0x01,  // Frequency_band_and_step_for_EU
                 0x02,  // Frequency_band_and_step_for_EU_without_RDS
                 0x03,  // Frequency_band_and_step_for_USA
                 0x04,  // Frequency_band_and_step_for_Japan
                 0x05,  // Frequency_band_and_step_for_China
                 0x06,  // Frequency_band_and_step_for_Australia
                 0x07,  // Frequency_band_and_step_for_Gulf
                 0x08,  // Frequency_band_and_step_for_Thailand
         }},
        {361,  // SimCardForConnectivity
         {
                 0x01,  // Without_SIM
                 0x02,  // only_through_Personal_SIM
                 0x03,  // only_through_Telematic_SIM
                 0x04,  // T_SIM_and_P_SIM_Accessory_
                 0x05,  // Volvo_SIM_Accessory_
         }},
        {362,  // UsbConnection
         {
                 0x01,  // Without_USB_connection
                 0x02,  // With_1_USB_connecion
                 0x03,  // With_USB_hub
         }},
        {363,  // AudioAuxiliaryInput
         {
                 0x01,  // Without_Auxiliary_Audio_Input
                 0x02,  // With_Auxiliary_Audio_Input
         }},
        {365,  // WifiWorkshopModeFunctionalControl
         {
                 0x01,  // WLAN_Workshop_Mode_Disabled
                 0x02,  // WLAN_Workshop_Mode_Enabled
         }},
        {401,  // RainSensorFunction
         {
                 0x01,  // No_Rain_Sensor_Function
                 0x02,  // Rain_Sensor_Function_Activated_By_Button
                 0x03,  // Rain_Sensor_Function_Activated_By_Interval_Position
         }}};
}
