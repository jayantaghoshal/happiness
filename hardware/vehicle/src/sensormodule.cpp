/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include "sensormodule.h"

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "SensorModule"

SensorModule::SensorModule(vhal20::impl::IVehicleHalImpl* vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal),
      sensor_prop_config_{{.prop = toInt(vhal20::VehicleProperty::DRIVING_STATUS),
                           .access = vhal20::VehiclePropertyAccess::READ,
                           .changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE,
                           .areaConfigs = {{.areaId = 0, .minInt32Value = 0x00, .maxInt32Value = 0x1F}}},
                          {.prop = toInt(vhal20::VehicleProperty::IGNITION_STATE),
                           .access = vhal20::VehiclePropertyAccess::READ,
                           .changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE,
                           .areaConfigs = {{.areaId = 0, .minInt32Value = 0, .maxInt32Value = 5}}},
                          {// Fix for preventing evs_app to continously restart
                           .prop = toInt(vhal20::VehicleProperty::GEAR_SELECTION),
                           .access = vhal20::VehiclePropertyAccess::READ,
                           .changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE,
                           .areaConfigs = {{.areaId = 0, .minInt32Value = 1, .maxInt32Value = 0x1FFF}}}} {
    StartFlexraySubscribers();
}

bool SensorModule::GetDrivingStatus(vhal20::VehiclePropValue& driving_status) {
    driving_status.timestamp = android::elapsedRealtimeNano();
    driving_status.areaId = static_cast<int32_t>(vhal20::VehicleArea::GLOBAL);
    driving_status.prop = toInt(vhal20::VehicleProperty::DRIVING_STATUS);
    driving_status.value.int32Values.resize(1);
    driving_status.value.int32Values[0] = toInt(vhal20::VehicleDrivingStatus::UNRESTRICTED);
    return true;
}

bool SensorModule::GetGearSelection(vhal20::VehiclePropValue& gear_selection) {
    gear_selection.timestamp = android::elapsedRealtimeNano();
    gear_selection.areaId = static_cast<int32_t>(vhal20::VehicleArea::GLOBAL);
    gear_selection.prop = toInt(vhal20::VehicleProperty::GEAR_SELECTION);
    gear_selection.value.int32Values.resize(1);
    gear_selection.value.int32Values[0] = 0x0001;  // GEAR_NEUTRAL
    return true;
}

std::vector<vhal20::VehiclePropValue> SensorModule::getAllPropValues() {
    vhal20::VehiclePropValue drivingstatus;
    vhal20::VehiclePropValue ignitionstate;
    vhal20::VehiclePropValue gearselection;

    GetDrivingStatus(drivingstatus);
    GetIgnitionState(ignitionstate);
    GetGearSelection(gearselection);

    return {drivingstatus, ignitionstate, gearselection};
}

std::vector<vhal20::VehiclePropConfig> SensorModule::listProperties() {
    ALOGV("[%s] Vehicle property configs returned.", __FUNCTION__);
    return sensor_prop_config_;
}

std::unique_ptr<vhal20::VehiclePropValue> SensorModule::getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                                vhal20::impl::Status& status) {
    ALOGV("[%s] Property ID: %d", __func__, requestedPropValue.prop);
    vhal20::VehiclePropValue prop_value;
    switch (requestedPropValue.prop) {
        case toInt(vhal20::VehicleProperty::DRIVING_STATUS):
            if (GetDrivingStatus(prop_value)) {
                status = vhal20::impl::Status::SUCCESS;
            } else {
                status = vhal20::impl::Status::TRY_AGAIN;
            }
            break;

        case toInt(vhal20::VehicleProperty::IGNITION_STATE):
            if (GetIgnitionState(prop_value)) {
                status = vhal20::impl::Status::SUCCESS;
            } else {
                status = vhal20::impl::Status::TRY_AGAIN;
            }
            break;
        case toInt(vhal20::VehicleProperty::GEAR_SELECTION):
            // Fix for preventing evs_app to continously restart
            if (GetGearSelection(prop_value)) {
                status = vhal20::impl::Status::SUCCESS;
            } else {
                status = vhal20::impl::Status::TRY_AGAIN;
            }
            break;
        default:
            ALOGW("Unknown getProp: 0x%0x", static_cast<int>(requestedPropValue.prop));
            status = vhal20::impl::Status::PERMISSION_ERROR;
            return nullptr;
            break;
    }
    return std::make_unique<vhal20::VehiclePropValue>(prop_value);
}

bool SensorModule::GetIgnitionState(vhal20::VehiclePropValue& prop_value) {
    prop_value.timestamp = android::elapsedRealtimeNano();
    prop_value.areaId = 0;
    prop_value.prop = toInt(vhal20::VehicleProperty::IGNITION_STATE);
    prop_value.value.int32Values.resize(1);
    const auto signal_state_value = veh_mod_receiver_.get();
    if (signal_state_value.isOk()) {
        switch (signal_state_value.value().UsgModSts) {
            case autosar::UsgModSts1::UsgModDrvg:
                prop_value.value.int32Values[0] = toInt(vhal20::VehicleIgnitionState::ON);
                break;

            case autosar::UsgModSts1::UsgModAbdnd:  // This is a usage mode we will normally never see in the MP
                prop_value.value.int32Values[0] = toInt(vhal20::VehicleIgnitionState::OFF);
                break;

            default:
                prop_value.value.int32Values[0] = toInt(vhal20::VehicleIgnitionState::ACC);
                break;
        }
        return true;
    } else {
        ALOGV("%s signal VehMod is in error.", __FUNCTION__);
        // default==OFF if ignition state cannot be determined
        prop_value.value.int32Values[0] = toInt(vhal20::VehicleIgnitionState::OFF);
        return false;
    }
}

void SensorModule::StartFlexraySubscribers() {
    veh_mod_receiver_.subscribe([this]() {
        vhal20::VehiclePropValue prop_value;
        if (GetIgnitionState(prop_value)) {
            ALOGD("Usage Mode changed to: %d", prop_value.value.int32Values[0]);
            pushProp(prop_value);
        }
    });
}
