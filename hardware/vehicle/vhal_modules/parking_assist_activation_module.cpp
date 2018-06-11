/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "parking_assist_activation_module.h"
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "interface_mapping.h"
#include "vcc/localconfig.h"

#undef LOG_TAG
#define LOG_TAG "ParkingAssistActivationModule"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;
using namespace ApplicationDataElement;

vhal20::VehiclePropConfig propconfig_parking_assistance_activation() {
    auto prop = vccvhal10::VehicleProperty::PARKING_ASSISTANCE_ACTIVATION_ON;
    return PaPropHandlerHelper::BoolConfig(prop);
}

ParkingAssistActivationModule::ParkingAssistActivationModule(gsl::not_null<VFContext*> ctx)
    : PA_prop_pas_activation_(propconfig_parking_assistance_activation(),
                                vccvhal10::VehicleProperty::PARKING_ASSIST_ACTIVATION_STATUS,
                                ctx->dispatcher,
                                &(ctx->vhal)),
        timerDispatcher_(void),
        vehmod_flexray_receiver_{ctx->dataelements},
        pas_activation_flexray_receiver_{ctx->dataelements},
        active_state_AP_flexray_receiver_{ctx->dataelements},
        is_system_error_(false),
        is_active_(true),
        pas_activation_on_(false),
        is_force_on_(false),
        is_pas_activation_alivetimeout_(false),
        is_active_state_AP_alivetimeout_(false) {

    ALOGV("Starting Module ");

    // A list of all the 
    AutnmsPrkgActvSt listAutnmsPrkgActvSt[] = {autosar::AutnmsPrkgActvSt::Pause,
                                               autosar::AutnmsPrkgActvSt::ParkInScan,
                                               autosar::AutnmsPrkgActvSt::ForwardMove,
                                               autosar::AutnmsPrkgActvSt::AutoBrakeInfo,
                                               autosar::AutnmsPrkgActvSt::RearwardMove,
                                               autosar::AutnmsPrkgActvSt::ParkInHighSpeed,
                                               autosar::AutnmsPrkgActvSt::ParkInStopToPark,
                                               autosar::AutnmsPrkgActvSt::ReleaseBrakePedal,
                                               autosar::AutnmsPrkgActvSt::ChangeGearForward,
                                               autosar::AutnmsPrkgActvSt::StandStillAndConfirm,
                                               autosar::AutnmsPrkgActvSt::ChangeGearRearward,
                                               autosar::AutnmsPrkgActvSt::MoveAwayFromObject,
                                               autosar::AutnmsPrkgActvSt::DirectParkInInstruction,
                                               autosar::AutnmsPrkgActvSt::SideAndDirectionSelection,
                                               autosar::AutnmsPrkgActvSt::StopAndChangeGearForward,
                                               autosar::AutnmsPrkgActvSt::StopAndChangeGearRearward};

    CC142_ParkingAssistanceType listCarConfig[] = {CC142_ParkingAssistanceType::Parking_assistance_Full_Automatic_Parking,
                                                   CC142_ParkingAssistanceType::Parking_assistance_Semi_Automatic_Parking_Gen3_SAP_,
                                                   CC142_ParkingAssistanceType::Parking_assistance_Gen3_rear,
                                                   CC142_ParkingAssistanceType::Parking_assistance_Gen3_front_and_rear,
                                                   CC142_ParkingAssistanceType::Parking_assistance_Remote_Full_Automatic_Parking_RFAP_};

    const bool is_enabled_by_carconfig = (std::find( std::begin(listCarConfig), 
                                                     std::end(listCarConfig), 
                                                     carconfig::getValue<CC142_ParkingAssistanceType>() ) != std::end(listCarConfig));



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
        is_pas_activation_alivetimeout_ = signal.isOk() ? false : true ;
        is_system_error_ = (is_pas_activation_alivetimeout_ && is_active_state_AP_alivetimeout_) ? true : false;
        ALOGV("changed: is_system_error_ : %d ", is_system_error_);
        Update();
    });

    // Check for system errors from active_state_AP_flexray_receiver_
    active_state_AP_flexray_receiver_.subscribe([&]() {
        const auto signal = active_state_AP_flexray_receiver_.get();
        is_active_state_AP_alivetimeout_ = signal.isOk() ? false : true ;
        if(!is_active_state_AP_alivetimeout_){

            //forceOn
            is_force_on_ = std::find( std::begin(listAutnmsPrkgActvSt), 
                                      std::end(listAutnmsPrkgActvSt), 
                                      signal.value().APActiveState ) != std::end(listAutnmsPrkgActvSt);
        }
        is_system_error_ = (is_pas_activation_alivetimeout_ && is_active_state_AP_alivetimeout_) ? true : false;
        ALOGV("changed: is_system_error_ : %d ", is_system_error_);
        Update();
    });

    // Check if function is active
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            is_active_ = (signal.value().UsgModSts == autosar::UsgModSts1::UsgModDrvg &&
                          signal.value().CarModSts1_ == autosar::CarModSts1::CarModNorm);

            ALOGV("changed: is_active_ : %d ", is_active_);
            Update();
        }
    });
}


void ParkingAssistActivationModule::Update() {

    if (is_system_error_) {
        ALOGV("State: SYSTEM_ERROR");
        SetFlexray(false);
        PA_prop_pas_activation_.PushProp(false, vccvhal10::PAStatus::SystemError);
    } else {
        ALOGV("State: SYSTEM_OK");
        if (!is_active_) {
            ALOGV("State: SYSTEM_OK : NOT_ACTIVE");
            SetFlexray(false);
            PA_prop_pas_activation_.PushProp(false, vccvhal10::PAStatus::Disabled);
        } else {
            ALOGV("State: SYSTEM_OK : ACTIVE");
            if(is_force_on_){
                pas_activation_on_ = true;
            }
            ALOGV("State: SYSTEM_OK : ACTIVE : forceOn : %d", is_force_on_);
            SetFlexray(pas_activation_on_);
            PA_prop_pas_activation_.PushProp(pas_activation_on_, vccvhal10::PAStatus::Active); 
        }
    }
}

void ParkingAssistActivationModule::SetFlexray(bool value) {

    OnOff1 flexray_value;
    flexray_value = value ? OnOff1::On : OnOff1::Off;
    pas_activation_flexray_sender_.send(flexray_value);
}