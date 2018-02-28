/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "first_row_stubimpl.hpp"

#include "common_factory.h"
#include "common_types_printers.h"
#include "first_row_printers.h"
#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace v0::org::volvocars::climate;

FirstRowStubImpl::FirstRowStubImpl(FirstRowFactory& logicFactory, common::daemon::Factory& commonFactory)
    : logicFactory_{logicFactory}, commonFactory_{commonFactory} {}

void FirstRowStubImpl::initStubAdapter() {
    SubscriptionHandle subscriptionHandle;

    subscriptionHandles_.push_back(logicFactory_.airConditioner_.subscribe([](const auto& state) {
        log_debug() << "fireAirConditionerAttributeChanged " << state;
        // getStubAdapter()->fireAirConditionerAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(logicFactory_.airDistribution_.subscribe([](const auto& state) {
        log_debug() << "fireAirDistributionAttributeChanged " << state;
        // getStubAdapter()->fireAirDistributionAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(logicFactory_.autoClimate_.subscribe([](const auto& state) {
        log_debug() << "fireAutoClimateAttributeChanged " << state;
        // getStubAdapter()->fireAutoClimateAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(logicFactory_.cleanZone_.subscribe([](const auto& state) {
        log_debug() << "fireCleanzoneAttributeChanged " << state;
        // getStubAdapter()->fireCleanzoneAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(logicFactory_.fanLevelFront_.subscribe([](const auto& state) {
        log_debug() << "fireFanLevelFrontAttributeChanged " << state;
        // getStubAdapter()->fireFanLevelFrontAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(logicFactory_.manualRecirc_.subscribe([](const auto& state) {
        log_debug() << "fireManualRecircAttributeChanged " << state;
        // getStubAdapter()->fireManualRecircAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(logicFactory_.electricDefrosterWindscreen_.subscribe([](const auto& state) {
        log_debug() << "fireElectricDefrosterWindscreenAttributeChanged " << state;
        // getStubAdapter()->fireElectricDefrosterWindscreenAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(logicFactory_.electricDefrosterRear_.subscribe([](const auto& state) {
        log_debug() << "fireElectricDefrosterRearAttributeChanged " << state;
        // getStubAdapter()->fireElectricDefrosterRearAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(commonFactory_.getMaxDefrosterProperty().subscribe([](const auto& state) {
        log_debug() << "fireMaxDefrosterAttributeChanged " << state;
        // getStubAdapter()->fireMaxDefrosterAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(logicFactory_.driverConvertedTemp_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.driverTempState_.get(),
                                              logicFactory_.driverConvertedTemp_.get(),
                                              logicFactory_.driverTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // getStubAdapter()->fireDriverTemperatureAttributeChanged(pubVal);
    }));

    subscriptionHandles_.push_back(logicFactory_.driverTempState_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.driverTempState_.get(),
                                              logicFactory_.driverConvertedTemp_.get(),
                                              logicFactory_.driverTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // getStubAdapter()->fireDriverTemperatureAttributeChanged(pubVal);
    }));

    subscriptionHandles_.push_back(logicFactory_.passengerConvertedTemp_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.passengerTempState_.get(),
                                              logicFactory_.passengerConvertedTemp_.get(),
                                              logicFactory_.passengerTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // getStubAdapter()->firePassengerTemperatureAttributeChanged(pubVal);
    }));

    subscriptionHandles_.push_back(logicFactory_.passengerTempState_.subscribe([this](const auto&) {
        FirstRow::TemperatureAttribute pubVal(logicFactory_.passengerTempState_.get(),
                                              logicFactory_.passengerConvertedTemp_.get(),
                                              logicFactory_.passengerTemperatureLogic_.range());
        log_debug() << "fireDriverTemperatureAttributeChanged, " << pubVal;
        // getStubAdapter()->firePassengerTemperatureAttributeChanged(pubVal);
    }));

    subscriptionHandles_.push_back(commonFactory_.getTemperatureSyncProperty().subscribe([](const auto& syncAttribute) {
        log_info() << "fireTemperatureSyncAttributeChanged, " << syncAttribute;
        // getStubAdapter()->fireTemperatureSyncAttributeChanged(syncAttribute);
    }));

    subscriptionHandles_.push_back(logicFactory_.getSeatHeatDriverProperty().subscribe([](const auto& heatAttribute) {
        log_debug() << "fireSeatHeatDriverAttributeChanged, " << heatAttribute;
        // getStubAdapter()->fireSeatHeatDriverAttributeChanged(heatAttribute);
    }));

    subscriptionHandles_.push_back(
            logicFactory_.getSeatHeatPassengerProperty().subscribe([](const auto& heatAttribute) {
                log_debug() << "fireSeatHeatPassengerAttributeChanged, " << heatAttribute;
                // getStubAdapter()->fireSeatHeatPassengerAttributeChanged(heatAttribute);
            }));

    subscriptionHandles_.push_back(logicFactory_.electricDefrosterRearPopup_.subscribe([](const auto& state) {
        log_debug() << "fireElectricDefrosterRearPopupAttributeChanged " << state;
        // getStubAdapter()->fireElectricDefrosterRearPopupAttributeChanged(state);
    }));

    subscriptionHandles_.push_back(
            logicFactory_.getElectricDefrosterWindscreenPopupProperty().subscribe([](const auto& state) {
                log_debug() << "fireElectricDefrosterWindscreenPopupAttributeChanged " << state;
                // getStubAdapter()->fireElectricDefrosterWindscreenPopupAttributeChanged(state);
            }));

    subscriptionHandles_.push_back(logicFactory_.getSteeringWheelHeatProperty().subscribe([](auto heatAttribute) {
        log_debug() << "fireSteeringWheelHeatAttributeChanged, " << heatAttribute;
        // getStubAdapter()->fireSteeringWheelHeatAttributeChanged(heatAttribute);
    }));

    subscriptionHandles_.push_back(logicFactory_.getSeatVentDriverProperty().subscribe([](auto ventAttribute) {
        log_debug() << "fireSeatVentDriverAttributeChanged, " << ventAttribute;
        // getStubAdapter()->fireSeatVentDriverAttributeChanged(ventAttribute);
    }));

    subscriptionHandles_.push_back(logicFactory_.getSeatVentPassengerProperty().subscribe([](auto ventAttribute) {
        log_debug() << "fireSeatVentPassengerAttributeChanged, " << ventAttribute;
        // getStubAdapter()->fireSeatVentPassengerAttributeChanged(ventAttribute);
    }));

    subscriptionHandles_.push_back(logicFactory_.getIonizerProperty().subscribe([](auto ionizerAttribute) {
        log_debug() << "fireIonizerAttributeChanged" << ionizerAttribute;
        // getStubAdapter()->fireIonizerAttributeChanged(ionizerAttribute);
    }));
}

/* Cleanzone */
const FirstRow::CleanzoneState& FirstRowStubImpl::getCleanzoneAttribute() {
    static FirstRow::CleanzoneState val;
    val = logicFactory_.cleanZone_.get();
    return val;
}

/* Air Conditioner */
const FirstRow::AirConditionerState& FirstRowStubImpl::getAirConditionerAttribute() {
    static FirstRow::AirConditionerState val;
    val = logicFactory_.airConditioner_.get();
    return val;
}

void FirstRowStubImpl::requestAirConditioner(FirstRow::AirConditionerRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.airConditionerLogic_.requestAirConditioner(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Air Distribution */

const FirstRow::AirDistributionAngle& FirstRowStubImpl::getAirDistributionAttribute() {
    static FirstRow::AirDistributionAngle val;
    val = logicFactory_.airDistribution_.get();
    return val;
}

void FirstRowStubImpl::requestAirDistribution(FirstRow::AirDistributionAngleRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.airDistributionLogic_.request(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Climate Reset */
void FirstRowStubImpl::requestClimateReset(GenericReply _reply) {
    logicFactory_.climateResetLogic_.request();
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

/* Fan level front */
const FirstRow::FanLevelFrontValue& FirstRowStubImpl::getFanLevelFrontAttribute() {
    static FirstRow::FanLevelFrontValue val;
    val = logicFactory_.fanLevelFront_.get();
    return val;
}

void FirstRowStubImpl::requestFanLevelFront(FirstRow::FanLevelFrontRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.fanLevelFrontLogic_.requestFanLevel(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Max Defroster */
const FirstRow::MaxDefrosterState& FirstRowStubImpl::getMaxDefrosterAttribute() {
    static FirstRow::MaxDefrosterState val;
    val = commonFactory_.getMaxDefrosterProperty().get();
    return val;
}

void FirstRowStubImpl::requestMaxDefroster(FirstRow::MaxDefrosterRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.maxDefrosterLogic_.request(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Auto Climate */
const FirstRow::AutoClimateState& FirstRowStubImpl::getAutoClimateAttribute() {
    static FirstRow::AutoClimateState val;
    val = logicFactory_.autoClimate_.get();
    return val;
}

void FirstRowStubImpl::requestAutoClimate(GenericReply _reply) {
    logicFactory_.autoClimateLogic_.requestAutoClimate();
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Manual Recirc

const FirstRow::ManualRecircState& FirstRowStubImpl::getManualRecircAttribute() {
    static FirstRow::ManualRecircState val;
    val = logicFactory_.manualRecirc_.get();
    return val;
}

void FirstRowStubImpl::requestManualRecirc(FirstRow::ManualRecircRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.manualRecircLogic_.requestManualRecirc(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

/* Electric Defroster Windscreen */

const FirstRow::ElectricDefrosterWindscreenState& FirstRowStubImpl::getElectricDefrosterWindscreenAttribute() {
    static FirstRow::ElectricDefrosterWindscreenState val;
    val = logicFactory_.getElectricDefrosterWindscreenProperty().get();
    return val;
}

void FirstRowStubImpl::requestElectricDefrosterWindscreen(FirstRow::ElectricDefrosterWindscreenRequest _value,
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

const FirstRow::ElectricDefrosterRearState& FirstRowStubImpl::getElectricDefrosterRearAttribute() {
    static FirstRow::ElectricDefrosterRearState val;
    val = logicFactory_.getElectricDefrosterRearProperty().get();
    return val;
}

void FirstRowStubImpl::requestElectricDefrosterRear(FirstRow::ElectricDefrosterRearRequest _value,
                                                    GenericReply _reply) {
    if (_value.validate()) {
        logicFactory_.electricDefrosterRearLogic_.request(_value);
        logicFactory_.electricDefrosterRearPopupLogic_.requestElectricDefrosterRear(_value);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

// Driver temperature

const FirstRow::TemperatureAttribute& FirstRowStubImpl::getDriverTemperatureAttribute() {
    static FirstRow::TemperatureAttribute val;
    val.setValue(logicFactory_.driverConvertedTemp_.get());
    val.setCurrState(logicFactory_.driverTempState_.get());
    val.setRange(logicFactory_.driverTemperatureLogic_.range());
    return val;
}

void FirstRowStubImpl::requestDriverTemperature(double _value, GenericReply _reply) {
    logicFactory_.driverTemperatureLogic_.request(_value);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Passenger temperature

const FirstRow::TemperatureAttribute& FirstRowStubImpl::getPassengerTemperatureAttribute() {
    static FirstRow::TemperatureAttribute val;
    val.setValue(logicFactory_.passengerConvertedTemp_.get());
    val.setCurrState(logicFactory_.passengerTempState_.get());
    val.setRange(logicFactory_.passengerTemperatureLogic_.range());
    return val;
}

void FirstRowStubImpl::requestPassengerTemperature(double _value, GenericReply _reply) {
    logicFactory_.passengerTemperatureLogic_.request(_value);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Temperature sync

const FirstRow::TemperatureSyncState& FirstRowStubImpl::getTemperatureSyncAttribute() {
    static FirstRow::TemperatureSyncState val;
    val = commonFactory_.getTemperatureSyncProperty().get();
    return val;
}

void FirstRowStubImpl::requestTemperatureSync(FirstRow::TemperatureSyncRequest _value, GenericReply _reply) {
    if (_value.validate()) {
        _reply(logicFactory_.temperatureSyncLogic_.request(_value));
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

// Driver Seat Heat

const FirstRow::HeatAttribute& FirstRowStubImpl::getSeatHeatDriverAttribute() {
    static FirstRow::HeatAttribute val;
    val = logicFactory_.getSeatHeatDriverProperty().get();
    return val;
}

void FirstRowStubImpl::requestSeatHeatDriver(FirstRow::HeatLevel _level, GenericReply _reply) {
    log_debug() << "requestSeatHeatDriver";
    logicFactory_.seatHeatDriverLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Passenger Seat Heat

const FirstRow::HeatAttribute& FirstRowStubImpl::getSeatHeatPassengerAttribute() {
    static FirstRow::HeatAttribute val;
    val = logicFactory_.getSeatHeatPassengerProperty().get();
    return val;
}

void FirstRowStubImpl::requestSeatHeatPassenger(FirstRow::HeatLevel _level, GenericReply _reply) {
    log_debug() << "requestSeatHeatPassenger";
    logicFactory_.seatHeatPassengerLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

/* Electric Defroster Windscreen Popup */

const FirstRow::ElectricDefrosterPopupState& FirstRowStubImpl::getElectricDefrosterWindscreenPopupAttribute() {
    static FirstRow::ElectricDefrosterPopupState val;
    val = logicFactory_.getElectricDefrosterWindscreenPopupProperty().get();
    return val;
}

void FirstRowStubImpl::requestElectricDefrosterWindscreenPopup(FirstRow::ElectricDefrosterPopupRequest _state,
                                                               GenericReply _reply) {
    log_debug() << "requestElectricDefrosterWindscreenPopUpStatus";
    if (_state.validate()) {
        logicFactory_.electricDefrosterWindscreenPopupLogic_.requestElectricDefrosterWindscreenPopup(_state);
        _reply(CommonTypes::ReturnCode::SUCCESS);
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}

const FirstRow::ElectricDefrosterPopupState& FirstRowStubImpl::getElectricDefrosterRearPopupAttribute() {
    static FirstRow::ElectricDefrosterPopupState val;
    val = logicFactory_.electricDefrosterRearPopup_.get();
    return val;
}

void FirstRowStubImpl::requestElectricDefrosterRearPopup(FirstRow::ElectricDefrosterPopupRequest _value,
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

const FirstRow::HeatAttribute& FirstRowStubImpl::getSteeringWheelHeatAttribute() {
    static FirstRow::HeatAttribute val;
    val = logicFactory_.getSteeringWheelHeatProperty().get();
    return val;
}

void FirstRowStubImpl::requestSteeringWheelHeat(FirstRow::HeatLevel _level, GenericReply _reply) {
    log_debug() << "Request Steering Wheel Heat level";
    logicFactory_.steeringWheelHeatLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Driver Seat Vent

const FirstRow::VentAttribute& FirstRowStubImpl::getSeatVentDriverAttribute() {
    static FirstRow::VentAttribute val;
    val = logicFactory_.getSeatVentDriverProperty().get();
    return val;
}

void FirstRowStubImpl::requestSeatVentDriver(FirstRow::VentLevel _level, GenericReply _reply) {
    log_debug() << "requestSeatVentDriver";
    logicFactory_.seatVentDriverLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Passenger Seat Vent

const FirstRow::VentAttribute& FirstRowStubImpl::getSeatVentPassengerAttribute() {
    static FirstRow::VentAttribute val;
    val = logicFactory_.getSeatVentPassengerProperty().get();
    return val;
}

void FirstRowStubImpl::requestSeatVentPassenger(FirstRow::VentLevel _level, GenericReply _reply) {
    log_debug() << "requestSeatVentPassenger";
    logicFactory_.seatVentPassengerLogic_.request(_level);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

// Ionizer

const FirstRow::IonizerState& FirstRowStubImpl::getIonizerAttribute() {
    static FirstRow::IonizerState val;
    val = logicFactory_.getIonizerProperty().get();
    return val;
}

void FirstRowStubImpl::requestIonizer(FirstRow::IonizerRequest _value, GenericReply _reply) {
    log_debug() << "requestIonizer";

    if (_value.validate()) {
        _reply(logicFactory_.ionizerLogic_.request(_value));
    } else {
        _reply(CommonTypes::ReturnCode::INVALID_REQUEST);
    }
}
