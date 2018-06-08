/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "parking_assistance_activation_module.h"
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "interface_mapping.h"
#include "vcc/localconfig.h"

#undef LOG_TAG
#define LOG_TAG "ParkingAssistanceActivationModule"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;
using namespace ApplicationDataElement;

vhal20::VehiclePropConfig propconfig_parking_assistance_activation() {
    auto prop = vccvhal10::VehicleProperty::PARKING_ASSISTANCE_ACTIVATION_ON;
    return PaPropHandlerHelper::BoolConfig(prop);
}

ParkingAssistanceActivationModule::ParkingAssistanceActivationModule(gsl::not_null<VFContext*> ctx)
    : PA_prop_pas_activation_(propconfig_parking_assistance_activation(),
                                 vccvhal10::VehicleProperty::PARKING_ASSISTANCE_ACTIVATION_STATUS,
                                 ctx->dispatcher,
                                 &(ctx->vhal)),
      vehmod_flexray_receiver_{ctx->dataelements},
      pas_activation_flexray_receiver_{ctx->dataelements},
      active_state_AP_flexray_receiver_{ctx->dataelements},
      is_system_error_(false),
      is_active_(true),
      is_pas_activation__alivetimeout_(false),
      is_active_state_AP_alivetimeout_(false) {

    ALOGV("Starting Module ");

    auto car_config_142 = carconfig::getValue<CC142_ParkingAssistanceType>();

    const bool is_enabled_by_carconfig =
            (car_config_142 == CC142_ParkingAssistanceType::Parking_assistance_Full_Automatic_Parking ||
            car_config_142 == CC142_ParkingAssistanceType::Parking_assistance_Semi_Automatic_Parking_Gen3_SAP_ ||
            car_config_142 == CC142_ParkingAssistanceType::Parking_assistance_Gen3_rear ||
            car_config_142 == CC142_ParkingAssistanceType::Parking_assistance_Gen3_front_and_rear ||
            car_config_142 == CC142_ParkingAssistanceType::Parking_assistance_Remote_Full_Automatic_Parking_RFAP_);



    // Check if enabled by carconfig
    if (!is_enabled_by_carconfig) {
        ALOGV("Function disabled due to CarConfig.");
        pas_activation_on_ = false;
        SetFlexray(pas_activation_on_);
        return;
    }


    // Listen for Vhal request
    PA_prop_pas_activation_.registerToVehicleHal();
    PA_prop_pas_activation_.subscribe_set_prop([&](bool requestedPAS_On, int32_t zone) {
        (void)zone;
        ALOGV("VHAL is changed, lets update (Setting->MainLoop->Vhal/Flexray): %d", requestedPAS_On);
        pas_activation_on_ = requestedPAS_On;
        Update();
    });

    // Check for system errors from pas_activation_flexray_receiver_
    pas_activation_flexray_receiver_.subscribe([&]() {
        const auto signal = pas_activation_flexray_receiver_.get();
        is_pas_activation__alivetimeout_ = signal.isOk() ? false : true ;
        is_system_error_ = (is_pas_activation__alivetimeout_ && is_active_state_AP_alivetimeout_) ? true : false;
        Update();
    });

    // Check for system errors from active_state_AP_flexray_receiver_
    active_state_AP_flexray_receiver_.subscribe([&]() {
        const auto signal = active_state_AP_flexray_receiver_.get();
        is_active_state_AP_alivetimeout_ = signal.isOk() ? false : true ;
        is_system_error_ = (is_pas_activation__alivetimeout_ && is_active_state_AP_alivetimeout_) ? true : false;
        Update();
    });

    // Check if function is active
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();
            is_active_ = (vehmod_value.UsgModSts == autosar::UsgModSts1::UsgModDrvg &&
                          vehmod_value.CarModSts1_ == autosar::CarModSts1::CarModNorm);

            ALOGV("is_active_ : %d ", is_active_);
            Update();
        }
    });
}


void ParkingAssistanceActivationModule::Update() {

    ALOGV(" Update() :: is_active_----------- : %d ", is_active_);
    ALOGV(" Update() :: is_system_error_----- : %d ", is_system_error_);

    if (is_system_error_) {
        SetFlexray(false);
        PA_prop_pas_activation_.PushProp(false, vccvhal10::PAStatus::SystemError);
    } else {
        if (!is_active_) {
            SetFlexray(false);
            PA_prop_pas_activation_.PushProp(false, vccvhal10::PAStatus::Disabled);
        } else {
            SetFlexray(pas_activation_on_);
            PA_prop_pas_activation_.PushProp(pas_activation_on_, vccvhal10::PAStatus::Active);
        }
    }
}

void ParkingAssistanceActivationModule::SetFlexray(bool value) {

    OnOff1 flexray_value;
    flexray_value = value ? OnOff1::On : OnOff1::Off;
    pas_activation_flexray_sender_.send(flexray_value);
}