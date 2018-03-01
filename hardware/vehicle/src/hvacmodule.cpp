/*
 * Copyright 2017 Volvo Car Corporation
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

HvacModule::HvacModule(vhal20::impl::IVehicleHalImpl* vehicleHal, FirstRowFactory& logicFactory,
                       common::daemon::Factory& commonFactory)
    : vccvhal10::impl::ModulePropertyHandler{vehicleHal}, logicFactory_{logicFactory}, commonFactory_{commonFactory} {
    {  // Temperature, HVAC_TEMPERATURE_SET
        PropertyAndConfig temperature_property;

        // Config for all zones
        temperature_property.prop_config.prop = toInt(VehicleProperty::HVAC_TEMPERATURE_SET);
        temperature_property.prop_config.access = VehiclePropertyAccess::READ_WRITE;
        temperature_property.prop_config.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        temperature_property.prop_config.supportedAreas =
                toInt(VehicleAreaZone::ROW_1_LEFT) | VehicleAreaZone::ROW_1_RIGHT;
        temperature_property.prop_config.areaConfigs.resize(2);
        temperature_property.prop_config.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1_LEFT);
        temperature_property.prop_config.areaConfigs[0].minFloatValue = 17;  // TODO carconfig?
        temperature_property.prop_config.areaConfigs[0].maxFloatValue = 27;  // TODO carconfig?
        temperature_property.prop_config.areaConfigs[1].areaId = toInt(VehicleAreaZone::ROW_1_RIGHT);
        temperature_property.prop_config.areaConfigs[1].minFloatValue = 17;  // TODO carconfig?
        temperature_property.prop_config.areaConfigs[1].maxFloatValue = 27;  // TODO carconfig?

        // TODO(climateport): Map left/right zone to passenger/driver

        // Temp common
        VehiclePropValue prop_value;
        prop_value.prop = toInt(VehicleProperty::HVAC_TEMPERATURE_SET);

        {  // Left temperature
            prop_value.areaId = toInt(VehicleAreaZone::ROW_1_LEFT);

            // Put Helper for tempLeft in map
            temperature_property.property_per_zone_handlers[static_cast<VehicleAreaZone>(prop_value.areaId)] =
                    std::make_unique<PropertyHandlerFloat>(
                            [this](float left_temp) {
                                ALOGD("Temp request %f", left_temp);
                                logicFactory_.driverTemperatureLogic_.request(left_temp);
                                return 0;
                            },
                            [this]() { return &dummy1; }, [this](VehiclePropValue prop_value) { pushProp(prop_value); },
                            prop_value);
        }
        {  // Right temperature
            prop_value.areaId = toInt(VehicleAreaZone::ROW_1_RIGHT);

            // Put Helper for tempRight in map
            temperature_property.property_per_zone_handlers[static_cast<VehicleAreaZone>(prop_value.areaId)] =
                    std::make_unique<PropertyHandlerFloat>(
                            [this](float right_temp) {
                                ALOGD("Temp request %f", right_temp);
                                logicFactory_.passengerTemperatureLogic_.request(right_temp);
                                return 0;
                            },
                            [this]() { return &dummy2; }, [this](VehiclePropValue prop_value) { pushProp(prop_value); },
                            prop_value);
        }

        // Property handler configured, put it in map
        propertyhandlers_[static_cast<vccvhal10::VehicleProperty>(prop_value.prop)] = std::move(temperature_property);
    }
    {  // Fan level, HVAC_FAN_SPEED
        PropertyAndConfig fan_property;

        // Config for all zones
        fan_property.prop_config.prop = toInt(VehicleProperty::HVAC_FAN_SPEED);
        fan_property.prop_config.access = VehiclePropertyAccess::READ_WRITE;
        fan_property.prop_config.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
        fan_property.prop_config.supportedAreas = toInt(VehicleAreaZone::ROW_1);
        fan_property.prop_config.areaConfigs.resize(1);
        fan_property.prop_config.areaConfigs[0].areaId = toInt(VehicleAreaZone::ROW_1);
        fan_property.prop_config.areaConfigs[0].minInt32Value = static_cast<int>(FirstRow::FanLevelFrontRequest::OFF);
        fan_property.prop_config.areaConfigs[0].maxInt32Value = static_cast<int>(FirstRow::FanLevelFrontRequest::MAX);

        // Fan
        vhal20::VehiclePropValue prop_value;
        prop_value.prop = fan_property.prop_config.prop;
        prop_value.areaId = toInt(VehicleAreaZone::ROW_1);

        // Put Helper for fanLevel in map
        fan_property.property_per_zone_handlers[static_cast<VehicleAreaZone>(prop_value.areaId)] =
                std::make_unique<PropertyHandlerInt32>(
                        [this](int fan_speed) {
                            ALOGD("Fan speed request %d", fan_speed);
                            // TODO: Proper range handling
                            if (fan_speed < 0 || fan_speed > 6) {
                                return 0;
                            }

                            FirstRow::FanLevelFrontRequest req(
                                    static_cast<FirstRow::FanLevelFrontRequest::Literal>(fan_speed));
                            logicFactory_.fanLevelFrontLogic_.requestFanLevel(req);
                            return 0;
                        },
                        [this]() { return &dummy3; },
                        [this](vhal20::VehiclePropValue prop_value) { pushProp(prop_value); }, prop_value);

        // Property handler configured, put it in map
        propertyhandlers_[static_cast<vccvhal10::VehicleProperty>(prop_value.prop)] = std::move(fan_property);
    }

    subs_.push_back(logicFactory_.airConditioner_.subscribe([](const auto& state) {
        log_debug() << "fireAirConditionerAttributeChanged " << state;
        // getStubAdapter()->fireAirConditionerAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.airDistribution_.subscribe([](const auto& state) {
        log_debug() << "fireAirDistributionAttributeChanged " << state;
        // getStubAdapter()->fireAirDistributionAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.autoClimate_.subscribe([](const auto& state) {
        log_debug() << "fireAutoClimateAttributeChanged " << state;
        // getStubAdapter()->fireAutoClimateAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.cleanZone_.subscribe([](const auto& state) {
        log_debug() << "fireCleanzoneAttributeChanged " << state;
        // getStubAdapter()->fireCleanzoneAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.fanLevelFront_.subscribe([](const auto& state) {
        log_debug() << "fireFanLevelFrontAttributeChanged " << state;
        // getStubAdapter()->fireFanLevelFrontAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.manualRecirc_.subscribe([](const auto& state) {
        log_debug() << "fireManualRecircAttributeChanged " << state;
        // getStubAdapter()->fireManualRecircAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.electricDefrosterWindscreen_.subscribe([](const auto& state) {
        log_debug() << "fireElectricDefrosterWindscreenAttributeChanged " << state;
        // getStubAdapter()->fireElectricDefrosterWindscreenAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.electricDefrosterRear_.subscribe([](const auto& state) {
        log_debug() << "fireElectricDefrosterRearAttributeChanged " << state;
        // getStubAdapter()->fireElectricDefrosterRearAttributeChanged(state);
    }));

    subs_.push_back(commonFactory_.getMaxDefrosterProperty().subscribe([](const auto& state) {
        log_debug() << "fireMaxDefrosterAttributeChanged " << state;
        // getStubAdapter()->fireMaxDefrosterAttributeChanged(state);
    }));

    subs_.push_back(logicFactory_.driverConvertedTemp_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.driverTempState_.get(),
                                              logicFactory_.driverConvertedTemp_.get(),
                                              logicFactory_.driverTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // getStubAdapter()->fireDriverTemperatureAttributeChanged(pubVal);
    }));

    subs_.push_back(logicFactory_.driverTempState_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.driverTempState_.get(),
                                              logicFactory_.driverConvertedTemp_.get(),
                                              logicFactory_.driverTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // getStubAdapter()->fireDriverTemperatureAttributeChanged(pubVal);
    }));

    subs_.push_back(logicFactory_.passengerConvertedTemp_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.passengerTempState_.get(),
                                              logicFactory_.passengerConvertedTemp_.get(),
                                              logicFactory_.passengerTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // getStubAdapter()->firePassengerTemperatureAttributeChanged(pubVal);
    }));

    subs_.push_back(logicFactory_.passengerTempState_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.passengerTempState_.get(),
                                              logicFactory_.passengerConvertedTemp_.get(),
                                              logicFactory_.passengerTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // getStubAdapter()->firePassengerTemperatureAttributeChanged(pubVal);
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

/* Cleanzone */
const FirstRow::CleanzoneState& HvacModule::getCleanzoneAttribute() {
    static FirstRow::CleanzoneState val;
    val = logicFactory_.cleanZone_.get();
    return val;
}

/* Air Conditioner */
const FirstRow::AirConditionerState& HvacModule::getAirConditionerAttribute() {
    static FirstRow::AirConditionerState val;
    val = logicFactory_.airConditioner_.get();
    return val;
}

void HvacModule::requestAirConditioner(FirstRow::AirConditionerRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.airConditionerLogic_.requestAirConditioner(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Air Distribution */

const FirstRow::AirDistributionAngle& HvacModule::getAirDistributionAttribute() {
    static FirstRow::AirDistributionAngle val;
    val = logicFactory_.airDistribution_.get();
    return val;
}

void HvacModule::requestAirDistribution(FirstRow::AirDistributionAngleRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.airDistributionLogic_.request(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Climate Reset */
void HvacModule::requestClimateReset(GenericReply _reply) {
    logicFactory_.climateResetLogic_.request();
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

/* Fan level front */
const FirstRow::FanLevelFrontValue& HvacModule::getFanLevelFrontAttribute() {
    static FirstRow::FanLevelFrontValue val;
    val = logicFactory_.fanLevelFront_.get();
    return val;
}

void HvacModule::requestFanLevelFront(FirstRow::FanLevelFrontRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.fanLevelFrontLogic_.requestFanLevel(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Max Defroster */
const FirstRow::MaxDefrosterState& HvacModule::getMaxDefrosterAttribute() {
    static FirstRow::MaxDefrosterState val;
    val = commonFactory_.getMaxDefrosterProperty().get();
    return val;
}

void HvacModule::requestMaxDefroster(FirstRow::MaxDefrosterRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.maxDefrosterLogic_.request(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Auto Climate */
const FirstRow::AutoClimateState& HvacModule::getAutoClimateAttribute() {
    static FirstRow::AutoClimateState val;
    val = logicFactory_.autoClimate_.get();
    return val;
}

void HvacModule::requestAutoClimate(GenericReply _reply) {
    logicFactory_.autoClimateLogic_.requestAutoClimate();
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Manual Recirc

const FirstRow::ManualRecircState& HvacModule::getManualRecircAttribute() {
    static FirstRow::ManualRecircState val;
    val = logicFactory_.manualRecirc_.get();
    return val;
}

void HvacModule::requestManualRecirc(FirstRow::ManualRecircRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.manualRecircLogic_.requestManualRecirc(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Electric Defroster Windscreen */

const FirstRow::ElectricDefrosterWindscreenState& HvacModule::getElectricDefrosterWindscreenAttribute() {
    static FirstRow::ElectricDefrosterWindscreenState val;
    val = logicFactory_.getElectricDefrosterWindscreenProperty().get();
    return val;
}

void HvacModule::requestElectricDefrosterWindscreen(FirstRow::ElectricDefrosterWindscreenRequest _value,
                                                    GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.electricDefrosterWindscreenLogic_.request(_value);
        logicFactory_.electricDefrosterWindscreenPopupLogic_.requestElectricDefrosterWindscreen(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Electric Defroster Rear */

const FirstRow::ElectricDefrosterRearState& HvacModule::getElectricDefrosterRearAttribute() {
    static FirstRow::ElectricDefrosterRearState val;
    val = logicFactory_.getElectricDefrosterRearProperty().get();
    return val;
}

void HvacModule::requestElectricDefrosterRear(FirstRow::ElectricDefrosterRearRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.electricDefrosterRearLogic_.request(_value);
        logicFactory_.electricDefrosterRearPopupLogic_.requestElectricDefrosterRear(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

// Driver temperature

const FirstRow::TemperatureAttribute& HvacModule::getDriverTemperatureAttribute() {
    static FirstRow::TemperatureAttribute val;
    val.setValue(logicFactory_.driverConvertedTemp_.get());
    val.setCurrState(logicFactory_.driverTempState_.get());
    val.setRange(logicFactory_.driverTemperatureLogic_.range());
    return val;
}

// Passenger temperature

const FirstRow::TemperatureAttribute& HvacModule::getPassengerTemperatureAttribute() {
    static FirstRow::TemperatureAttribute val;
    val.setValue(logicFactory_.passengerConvertedTemp_.get());
    val.setCurrState(logicFactory_.passengerTempState_.get());
    val.setRange(logicFactory_.passengerTemperatureLogic_.range());
    return val;
}

void HvacModule::requestPassengerTemperature(double _value, GenericReply _reply) {
    logicFactory_.passengerTemperatureLogic_.request(_value);
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
