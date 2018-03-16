/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "hvacmodule.h"
#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>
#include "i_vehicle_hal_impl.h"

#include <cutils/log.h>
#include <functional>
#include <memory>

#include "common_factory.h"
#include "common_types_printers.h"
#include "first_row_printers.h"
#include "logging_context.h"
#undef LOG_TAG
#define LOG_TAG "HvacModule"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace v0::org::volvocars::climate;
using namespace android::hardware::automotive::vehicle::V2_0;
using namespace vendor::volvocars::hardware::vehiclehal::V1_0::impl;
using namespace std::placeholders;

using namespace android;

namespace {
vhal20::VehiclePropConfig boolConfig(VehicleProperty prop, int32_t zones) {
    vhal20::VehiclePropConfig c;
    c.prop = toInt(prop);
    c.access = VehiclePropertyAccess::READ_WRITE;
    c.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
    c.supportedAreas = zones;
    c.areaConfigs.resize(0);  // Important to not init this for bool properties!
    return c;
}

vhal20::VehiclePropConfig propconfig_temperature() {
    vhal20::VehiclePropConfig c;
    c.prop = toInt(VehicleProperty::HVAC_TEMPERATURE_SET);
    c.access = VehiclePropertyAccess::READ_WRITE;
    c.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
    c.supportedAreas = toInt(VehicleAreaZone::ROW_1_LEFT) | VehicleAreaZone::ROW_1_RIGHT;
    c.areaConfigs.resize(2);
    c.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1_LEFT);
    c.areaConfigs[0].minFloatValue = 17;  // TODO carconfig?
    c.areaConfigs[0].maxFloatValue = 27;  // TODO carconfig?
    c.areaConfigs[1].areaId = toInt(VehicleAreaZone::ROW_1_RIGHT);
    c.areaConfigs[1].minFloatValue = 17;  // TODO carconfig?
    c.areaConfigs[1].maxFloatValue = 27;  // TODO carconfig?
    return c;
}

vhal20::VehiclePropConfig propconfig_autoclimate() {
    return boolConfig(VehicleProperty::HVAC_AUTO_ON, toInt(VehicleAreaZone::WHOLE_CABIN));
}

vhal20::VehiclePropConfig propconfig_recirculation() {
    return boolConfig(VehicleProperty::HVAC_RECIRC_ON, toInt(VehicleAreaZone::WHOLE_CABIN));
}

vhal20::VehiclePropConfig propconfig_acon() {
    return boolConfig(VehicleProperty::HVAC_AC_ON, toInt(VehicleAreaZone::WHOLE_CABIN));
}

vhal20::VehiclePropConfig propconfig_defroster() {
    return boolConfig(VehicleProperty::HVAC_DEFROSTER,
                      toInt(VehicleAreaWindow::FRONT_WINDSHIELD) | toInt(VehicleAreaWindow::REAR_WINDSHIELD));
}
vhal20::VehiclePropConfig propconfig_maxdefrost() {
    return boolConfig(VehicleProperty::HVAC_MAX_DEFROST_ON, toInt(VehicleAreaWindow::FRONT_WINDSHIELD));
}

vhal20::VehiclePropConfig propconfig_fanlevelfront() {
    vhal20::VehiclePropConfig c;
    c.prop = toInt(VehicleProperty::HVAC_FAN_SPEED);
    c.access = VehiclePropertyAccess::READ_WRITE;
    c.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
    c.supportedAreas = toInt(VehicleAreaZone::ROW_1);
    c.areaConfigs.resize(1);
    c.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1);
    c.areaConfigs[0].minInt32Value = static_cast<int>(FirstRow::FanLevelFrontRequest::OFF);
    c.areaConfigs[0].maxInt32Value = static_cast<int>(FirstRow::FanLevelFrontRequest::MAX);
    return c;
}

vhal20::VehiclePropConfig propconfig_fandirection() {
    vhal20::VehiclePropConfig c;
    c.prop = toInt(VehicleProperty::HVAC_FAN_DIRECTION);
    c.access = VehiclePropertyAccess::READ_WRITE;
    c.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
    c.supportedAreas = toInt(VehicleAreaZone::ROW_1);
    c.areaConfigs.resize(1);
    c.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1);
    c.areaConfigs[0].minInt32Value = 0;
    c.areaConfigs[0].maxInt32Value = toInt(vhal20::VehicleHvacFanDirection::DEFROST_AND_FLOOR);
    return c;
}

}  // namespace

HvacModule::HvacModule(vhal20::impl::IVehicleHalImpl* vehicleHal,
                       FirstRowFactory& logicFactory,
                       common::daemon::Factory& commonFactory,
                       std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher)
    : logicFactory_{logicFactory},
      commonFactory_{commonFactory},
      prop_temperature(propconfig_temperature(), dispatcher, vehicleHal),
      prop_recirculation(propconfig_recirculation(), dispatcher, vehicleHal),
      prop_autoclimate(propconfig_autoclimate(), dispatcher, vehicleHal),
      prop_defroster(propconfig_defroster(), dispatcher, vehicleHal),
      prop_maxdefroster(propconfig_maxdefrost(), dispatcher, vehicleHal),
      prop_ac(propconfig_acon(), dispatcher, vehicleHal),
      prop_fanlevelfront(propconfig_fanlevelfront(), dispatcher, vehicleHal),
      prop_fandir(propconfig_fandirection(), dispatcher, vehicleHal) {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Temperature

    prop_temperature.registerToVehicleHal();
    prop_temperature.subscribe_set_prop([&](float temperature, int32_t areaId) {
        if (areaId == toInt(VehicleAreaZone::ROW_1_LEFT)) {
            ALOGD("Request temp left %f", temperature);
            logicFactory_.driverTemperatureLogic_.request(temperature);
        } else if (areaId == toInt(VehicleAreaZone::ROW_1_RIGHT)) {
            ALOGD("Request temp right %f", temperature);
            logicFactory_.passengerTemperatureLogic_.request(temperature);
        }
    });

    subs_.push_back(logicFactory_.driverConvertedTemp_.subscribe([this](const auto& value) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.driverTempState_.get(),
                                              logicFactory_.driverConvertedTemp_.get(),
                                              logicFactory_.driverTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        prop_temperature.PushProp(value, toInt(VehicleAreaZone::ROW_1_LEFT));
    }));
    subs_.push_back(logicFactory_.driverTempState_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.driverTempState_.get(),
                                              logicFactory_.driverConvertedTemp_.get(),
                                              logicFactory_.driverTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // TODO: PA Availability to VHAL
    }));

    subs_.push_back(logicFactory_.passengerConvertedTemp_.subscribe([this](const auto& value) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.passengerTempState_.get(),
                                              logicFactory_.passengerConvertedTemp_.get(),
                                              logicFactory_.passengerTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        prop_temperature.PushProp(value, toInt(VehicleAreaZone::ROW_1_RIGHT));
    }));

    subs_.push_back(logicFactory_.passengerTempState_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.passengerTempState_.get(),
                                              logicFactory_.passengerConvertedTemp_.get(),
                                              logicFactory_.passengerTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // TODO: PA Availability to VHAL
    }));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Fan level

    prop_fanlevelfront.registerToVehicleHal();
    prop_fanlevelfront.subscribe_set_prop([&](int32_t fan_speed, int32_t areaId) {
        (void)areaId;
        ALOGD("Fan speed request %d", fan_speed);
        // TODO: Proper range handling
        if (fan_speed < 0 || fan_speed > 6) {
            return;  // TODO: Return error
        }

        FirstRow::FanLevelFrontRequest req(static_cast<FirstRow::FanLevelFrontRequest::Literal>(fan_speed));
        logicFactory_.fanLevelFrontLogic_.requestFanLevel(req);
    });
    subs_.push_back(logicFactory_.fanLevelFront_.subscribe([this](const auto& state) {
        log_debug() << "fireFanLevelFrontAttributeChanged " << state;
        prop_fanlevelfront.PushProp(static_cast<int32_t>(state.value_));
    }));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Auto

    prop_autoclimate.registerToVehicleHal();
    prop_autoclimate.subscribe_set_prop([&](bool auto_on, int32_t areaId) {
        (void)areaId;
        ALOGD("Request auto climate %d", auto_on);
        logicFactory_.autoClimateLogic_.requestAutoClimate();
    });
    subs_.push_back(logicFactory_.autoClimate_.subscribe([this](const auto& state) {
        log_debug() << "fireAutoClimateAttributeChanged " << state;
        bool on = (state == FirstRowGen::AutoClimateState::ON);
        prop_autoclimate.PushProp(on);
    }));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Recirc

    prop_recirculation.registerToVehicleHal();
    prop_recirculation.subscribe_set_prop([&](bool recirculation_on, int32_t areaId) {
        (void)areaId;
        ALOGD("Request recirculation %d", recirculation_on);
        logicFactory_.manualRecircLogic_.requestManualRecirc(recirculation_on ? FirstRowGen::ManualRecircRequest::ON
                                                                              : FirstRowGen::ManualRecircRequest::OFF);
    });
    subs_.push_back(logicFactory_.manualRecirc_.subscribe([this](const auto& state) {
        log_debug() << "fireManualRecircAttributeChanged " << state;
        prop_recirculation.PushProp(state == FirstRowGen::ManualRecircState::ON);
    }));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // (Electric) Defrost

    prop_defroster.registerToVehicleHal();
    prop_defroster.subscribe_set_prop([&](bool defroster_on, int32_t areaId) {
        if (areaId == toInt(VehicleAreaWindow::REAR_WINDSHIELD)) {
            ALOGD("Request rear electric defroster %d", defroster_on);
            logicFactory_.electricDefrosterRearLogic_.request(defroster_on
                                                                      ? FirstRowGen::ElectricDefrosterRearRequest::ON
                                                                      : FirstRowGen::ElectricDefrosterRearRequest::OFF);
            logicFactory_.electricDefrosterRearPopupLogic_.requestElectricDefrosterRear(
                    defroster_on ? FirstRowGen::ElectricDefrosterRearRequest::ON
                                 : FirstRowGen::ElectricDefrosterRearRequest::OFF);
        } else if (areaId == toInt(VehicleAreaWindow::FRONT_WINDSHIELD)) {
            ALOGD("Request front electric defroster %d", defroster_on);
            logicFactory_.electricDefrosterWindscreenLogic_.request(
                    defroster_on ? FirstRowGen::ElectricDefrosterWindscreenRequest::ON
                                 : FirstRowGen::ElectricDefrosterWindscreenRequest::OFF);
            logicFactory_.electricDefrosterWindscreenPopupLogic_.requestElectricDefrosterWindscreen(
                    defroster_on ? FirstRowGen::ElectricDefrosterWindscreenRequest::ON
                                 : FirstRowGen::ElectricDefrosterWindscreenRequest::OFF);
        }
    });
    subs_.push_back(logicFactory_.electricDefrosterRear_.subscribe([this](const auto& state) {
        log_debug() << "fireAirConditionerAttributeChanged " << state;
        prop_defroster.PushProp(state == FirstRowGen::ElectricDefrosterRearState::ON,
                                toInt(VehicleAreaWindow::REAR_WINDSHIELD));
    }));
    subs_.push_back(logicFactory_.electricDefrosterWindscreen_.subscribe([this](const auto& state) {
        log_debug() << "fireElectricDefrosterWindscreenAttributeChanged " << state;
        prop_defroster.PushProp(state == FirstRowGen::ElectricDefrosterRearState::ON,
                                toInt(VehicleAreaWindow::FRONT_WINDSHIELD));
    }));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Max defrost

    prop_maxdefroster.registerToVehicleHal();
    prop_maxdefroster.subscribe_set_prop([&](bool maxdefroster_on, int32_t areaId) {
        (void)areaId;
        ALOGD("Request maxdefrost %d", maxdefroster_on);
        logicFactory_.maxDefrosterLogic_.request(maxdefroster_on ? FirstRow::MaxDefrosterRequest::ON
                                                                 : FirstRow::MaxDefrosterRequest::OFF);
    });
    subs_.push_back(commonFactory_.getMaxDefrosterProperty().subscribe([this](const auto& state) {
        log_debug() << "fireMaxDefrosterAttributeChanged " << state;
        prop_maxdefroster.PushProp(state == FirstRowGen::MaxDefrosterState::ON);
    }));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // AC

    prop_ac.registerToVehicleHal();
    prop_ac.subscribe_set_prop([&](bool ac_on, int32_t areaId) {
        (void)areaId;
        ALOGD("Request AC %d", ac_on);
        logicFactory_.airConditionerLogic_.requestAirConditioner(ac_on ? FirstRowGen::AirConditionerRequest::AUTO
                                                                       : FirstRowGen::AirConditionerRequest::OFF);
    });
    subs_.push_back(logicFactory_.airConditioner_.subscribe([this](const auto& state) {
        log_debug() << "fireAirConditionerAttributeChanged " << state;
        prop_ac.PushProp(state == FirstRowGen::AirConditionerState::AUTO);
    }));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Air distribution

    static const std::vector<std::pair<FirstRowGen::AirDistributionAngle, int32_t>> air_dist_conversions{
            {std::make_pair(FirstRowGen::AirDistributionAngle::FLOOR, toInt(VehicleHvacFanDirection::FLOOR)),
             std::make_pair(FirstRowGen::AirDistributionAngle::VENT, toInt(VehicleHvacFanDirection::FACE)),
             std::make_pair(FirstRowGen::AirDistributionAngle::DEFROST, toInt(VehicleHvacFanDirection::DEFROST)),
             std::make_pair(FirstRowGen::AirDistributionAngle::FLOOR_DEFROST,
                            toInt(VehicleHvacFanDirection::FLOOR) | toInt(VehicleHvacFanDirection::DEFROST)),
             std::make_pair(FirstRowGen::AirDistributionAngle::FLOOR_VENT,
                            toInt(VehicleHvacFanDirection::FLOOR) | toInt(VehicleHvacFanDirection::FACE)),
             std::make_pair(FirstRowGen::AirDistributionAngle::VENT_DEFROST,
                            toInt(VehicleHvacFanDirection::FACE) | toInt(VehicleHvacFanDirection::DEFROST)),
             std::make_pair(FirstRowGen::AirDistributionAngle::FLOOR_VENT_DEFROST,
                            toInt(VehicleHvacFanDirection::FLOOR) | toInt(VehicleHvacFanDirection::FACE) |
                                    toInt(VehicleHvacFanDirection::DEFROST)),
             // TODO: Auto=All OK???
             std::make_pair(FirstRowGen::AirDistributionAngle::AUTO,
                            toInt(VehicleHvacFanDirection::FLOOR) | toInt(VehicleHvacFanDirection::FACE) |
                                    toInt(VehicleHvacFanDirection::DEFROST)),
             std::make_pair(FirstRowGen::AirDistributionAngle::DISABLED, 0)}};

    prop_fandir.registerToVehicleHal();
    prop_fandir.subscribe_set_prop([this](int32_t fan_dir, int32_t areaId) {
        (void)areaId;
        for (const auto& c : air_dist_conversions) {
            if (c.second == fan_dir) {
                // TODO: Remove static_cast, ok for now as the enums are identical
                logicFactory_.airDistributionLogic_.request(
                        static_cast<FirstRowGen::AirDistributionAngleRequest::Literal>(c.first.value_));
                return;
            }
        }
        // TODO: Return/log error
    });
    subs_.push_back(logicFactory_.airDistribution_.subscribe([this](const auto& state) {
        log_debug() << "fireAirDistributionAttributeChanged " << state;

        for (const auto& c : air_dist_conversions) {
            if (c.first == state) {
                prop_fandir.PushProp(c.second);
                return;
            }
        }

        // TODO: Return/log error
    }));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Old not yet ported

    subs_.push_back(logicFactory_.cleanZone_.subscribe([](const auto& state) {
        log_debug() << "fireCleanzoneAttributeChanged " << state;
        // getStubAdapter()->fireCleanzoneAttributeChanged(state);
    }));

    subs_.push_back(commonFactory_.getTemperatureSyncProperty().subscribe([](const auto& syncAttribute) {
        log_info() << "fireTemperatureSyncAttributeChanged, " << syncAttribute;
        // getStubAdapter()->fireTemperatureSyncAttributeChanged(syncAttribute);
    }));

    subs_.push_back(logicFactory_.getSeatHeatDriverProperty().subscribe([](const auto& heatAttribute) {
        log_debug() << "fireSeatHeatDriverAttributeChanged, " << heatAttribute;
        // getStubAdapter()->fireSeatHeatDriverAttributeChanged(heatAttribute);
    }));

    subs_.push_back(logicFactory_.getSeatHeatPassengerProperty().subscribe([](const auto& heatAttribute) {
        log_debug() << "fireSeatHeatPassengerAttributeChanged, " << heatAttribute;
        // getStubAdapter()->fireSeatHeatPassengerAttributeChanged(heatAttribute);
    }));

    subs_.push_back(logicFactory_.electricDefrosterRearPopup_.subscribe([](const auto& state) {
        log_debug() << "fireElectricDefrosterRearPopupAttributeChanged " << state;
        // getStubAdapter()->fireElectricDefrosterRearPopupAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.getElectricDefrosterWindscreenPopupProperty().subscribe([](const auto& state) {
        log_debug() << "fireElectricDefrosterWindscreenPopupAttributeChanged " << state;
        // getStubAdapter()->fireElectricDefrosterWindscreenPopupAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.getSteeringWheelHeatProperty().subscribe([](auto heatAttribute) {
        log_debug() << "fireSteeringWheelHeatAttributeChanged, " << heatAttribute;
        // getStubAdapter()->fireSteeringWheelHeatAttributeChanged(heatAttribute);
    }));

    subs_.push_back(logicFactory_.getSeatVentDriverProperty().subscribe([](auto ventAttribute) {
        log_debug() << "fireSeatVentDriverAttributeChanged, " << ventAttribute;
        // getStubAdapter()->fireSeatVentDriverAttributeChanged(ventAttribute);
    }));

    subs_.push_back(logicFactory_.getSeatVentPassengerProperty().subscribe([](auto ventAttribute) {
        log_debug() << "fireSeatVentPassengerAttributeChanged, " << ventAttribute;
        // getStubAdapter()->fireSeatVentPassengerAttributeChanged(ventAttribute);
    }));

    subs_.push_back(logicFactory_.getIonizerProperty().subscribe([](auto ionizerAttribute) {
        log_debug() << "fireIonizerAttributeChanged" << ionizerAttribute;
        // getStubAdapter()->fireIonizerAttributeChanged(ionizerAttribute);
    }));

    ALOGD("Hvac initialized");
}

/* Climate Reset */
void HvacModule::requestClimateReset(GenericReply _reply) {
    logicFactory_.climateResetLogic_.request();
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Temperature sync

const FirstRow::TemperatureSyncState& HvacModule::getTemperatureSyncAttribute() {
    static FirstRow::TemperatureSyncState val;
    val = commonFactory_.getTemperatureSyncProperty().get();
    return val;
}

void HvacModule::requestTemperatureSync(FirstRow::TemperatureSyncRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        _reply(logicFactory_.temperatureSyncLogic_.request(_value));
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

// Driver Seat Heat

const FirstRow::HeatAttribute& HvacModule::getSeatHeatDriverAttribute() {
    static FirstRow::HeatAttribute val;
    val = logicFactory_.getSeatHeatDriverProperty().get();
    return val;
}

void HvacModule::requestSeatHeatDriver(FirstRow::HeatLevel _level, GenericReply _reply) {
    log_debug() << "requestSeatHeatDriver";
    logicFactory_.seatHeatDriverLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Passenger Seat Heat

const FirstRow::HeatAttribute& HvacModule::getSeatHeatPassengerAttribute() {
    static FirstRow::HeatAttribute val;
    val = logicFactory_.getSeatHeatPassengerProperty().get();
    return val;
}

void HvacModule::requestSeatHeatPassenger(FirstRow::HeatLevel _level, GenericReply _reply) {
    log_debug() << "requestSeatHeatPassenger";
    logicFactory_.seatHeatPassengerLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

/* Electric Defroster Windscreen Popup */

const FirstRow::ElectricDefrosterPopupState& HvacModule::getElectricDefrosterWindscreenPopupAttribute() {
    static FirstRow::ElectricDefrosterPopupState val;
    val = logicFactory_.getElectricDefrosterWindscreenPopupProperty().get();
    return val;
}

void HvacModule::requestElectricDefrosterWindscreenPopup(FirstRow::ElectricDefrosterPopupRequest _state,
                                                         GenericReply _reply) {
    log_debug() << "requestElectricDefrosterWindscreenPopUpStatus";
    if (_state.validate()) {
        logicFactory_.electricDefrosterWindscreenPopupLogic_.requestElectricDefrosterWindscreenPopup(_state);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

const FirstRow::ElectricDefrosterPopupState& HvacModule::getElectricDefrosterRearPopupAttribute() {
    static FirstRow::ElectricDefrosterPopupState val;
    val = logicFactory_.electricDefrosterRearPopup_.get();
    return val;
}

void HvacModule::requestElectricDefrosterRearPopup(FirstRow::ElectricDefrosterPopupRequest _value,
                                                   GenericReply _reply) {
    log_debug() << "requestElectricDefrosterRearPopup";
    if (_value.validate()) {
        logicFactory_.electricDefrosterRearPopupLogic_.request(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

// Steering Wheel Heat

const FirstRow::HeatAttribute& HvacModule::getSteeringWheelHeatAttribute() {
    static FirstRow::HeatAttribute val;
    val = logicFactory_.getSteeringWheelHeatProperty().get();
    return val;
}

void HvacModule::requestSteeringWheelHeat(FirstRow::HeatLevel _level, GenericReply _reply) {
    log_debug() << "Request Steering Wheel Heat level";
    logicFactory_.steeringWheelHeatLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Driver Seat Vent

const FirstRow::VentAttribute& HvacModule::getSeatVentDriverAttribute() {
    static FirstRow::VentAttribute val;
    val = logicFactory_.getSeatVentDriverProperty().get();
    return val;
}

void HvacModule::requestSeatVentDriver(FirstRow::VentLevel _level, GenericReply _reply) {
    log_debug() << "requestSeatVentDriver";
    logicFactory_.seatVentDriverLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Passenger Seat Vent

const FirstRow::VentAttribute& HvacModule::getSeatVentPassengerAttribute() {
    static FirstRow::VentAttribute val;
    val = logicFactory_.getSeatVentPassengerProperty().get();
    return val;
}

void HvacModule::requestSeatVentPassenger(FirstRow::VentLevel _level, GenericReply _reply) {
    log_debug() << "requestSeatVentPassenger";
    logicFactory_.seatVentPassengerLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Ionizer

const FirstRow::IonizerState& HvacModule::getIonizerAttribute() {
    static FirstRow::IonizerState val;
    val = logicFactory_.getIonizerProperty().get();
    return val;
}

void HvacModule::requestIonizer(FirstRow::IonizerRequest _value, GenericReply _reply) {
    log_debug() << "requestIonizer";

    if (_value.validate()) {
        _reply(logicFactory_.ionizerLogic_.request(_value));
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}
