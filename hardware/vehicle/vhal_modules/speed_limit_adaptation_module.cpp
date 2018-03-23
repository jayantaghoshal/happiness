/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "speed_limit_adaptation_module.h"
#include "carconfig.h"
#include "cc_parameterlist.h"

#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "SpeedLimitAdaptation"

using namespace autosar;
using namespace CarConfigParams;

SpeedLimitAdaptationModule::SpeedLimitAdaptationModule() {
    auto car_config_23 = carconfig::getValue<CC23_CruiseControlType>();
    auto car_config_36 = carconfig::getValue<CC36_AdjustableSpeedLimiterType>();
    auto car_config_149 = carconfig::getValue<CC149_RoadSignInformationType>();

    // A list of all the Car Config values with TSI and SLA
    CC149_RoadSignInformationType TSI_with_SLA[] = {CC149_RoadSignInformationType::SWEDEN_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::IRELAND_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::ENGLAND_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::MID_EU_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::EAST_EUROPE_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::SOUTH_EU_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::USA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::MEXICO_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::CANADA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::SINGAPORE_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::KOREA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::LATIN_AMERICA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::ARABIA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::HONGKONG_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::TAIWAN_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::AUSTRALIA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::SOUTH_AFRICA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::TURKEY_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::JAPAN_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::CHINA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::INDIA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::BRAZIL_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::THAILAND_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::MALAYSIA_TSI_with_SLA,
                                                    CC149_RoadSignInformationType::OTHERS_TSI_with_SLA};

    // Check if the car has TSI and SLA
    const bool is_TSIandSLA =
            (std::find(std::begin(TSI_with_SLA), std::end(TSI_with_SLA), car_config_149) != std::end(TSI_with_SLA));

    // Check if enabled
    const bool is_enabled_ =
            ((car_config_149 !=
                      CC149_RoadSignInformationType::Without_TSI_Traffic_Sign_Information_SLA_Speed_limit_Adaption_ &&
              car_config_36 > CC36_AdjustableSpeedLimiterType::Adjustable_speed_limiter) ||
             (car_config_23 > CC23_CruiseControlType::Cruise_control && is_TSIandSLA));

    if (!is_enabled_) {
        ALOGV("Function disabled due to CarConfig.");
        speed_limit_adaptation_on = false;
        Update();
        return;
    }

    // Initialize the Flexray signal
    Update();

    // Read the input from the Driver Information Module
    drvrhmi_spdlimadpn_sts_flexray_receiver_.subscribe([&]() {
        const auto signal = drvrhmi_spdlimadpn_sts_flexray_receiver_.get();
        if (signal.isOk()) {
            auto drvrhmi_spdlimadpn_sts_value = signal.value();

            speed_limit_adaptation_on = (drvrhmi_spdlimadpn_sts_value == OnOff1::On);
            ALOGV("speed_limit_adaptation_on is changed: %d", speed_limit_adaptation_on);
            Update();
        }

    });
}

void SpeedLimitAdaptationModule::Update() {
    OnOffPen flexray_value;
    flexray_value.Sts = speed_limit_adaptation_on ? OnOff1::On : OnOff1::Off;
    flexray_value.Pen = IdPen::ProfAll;  // Since the setting does not exist anymore, Profile All is sent
    accadpr_spdlim_actv_flexray_sender_.send(flexray_value);
}
