/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "user_selection_stubimpl.hpp"

#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(UserSelectionContext)

using namespace v0::org::volvocars::climate;

UserSelectionStubImpl::UserSelectionStubImpl(UserSelectionFactory& logicFactory)
    : remoteEventHandler_(this), InterfaceVersion(0, 1), logicFactory_(logicFactory) {}

void UserSelectionStubImpl::initStubAdapter() {
    subscriptionHandles_.push_back(logicFactory_.autoDriverSeatHeat_.subscribe([this](const auto&) {
        log_debug() << "fireAutoDriverSeatHeatAttributeChanged("
                    << logicFactory_.autoDriverSeatHeat_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->fireAutoDriverSeatHeatAttributeChanged(logicFactory_.autoDriverSeatHeat_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.autoDriverSeatHeatLevel_.subscribe([this](const auto&) {
        log_debug() << "fireAutoDriverSeatHeatLevelAttributeChanged("
                    << logicFactory_.autoDriverSeatHeatLevel_.get().getCurrentLevel().value_ << ")";
        // getStubAdapter()->fireAutoDriverSeatHeatLevelAttributeChanged(logicFactory_.autoDriverSeatHeatLevel_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.autoPassengerSeatHeat_.subscribe([this](const auto&) {
        log_debug() << "fireAutoPassengerSeatHeatAttributeChanged("
                    << logicFactory_.autoPassengerSeatHeat_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->fireAutoPassengerSeatHeatAttributeChanged(logicFactory_.autoPassengerSeatHeat_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.autoPassengerSeatHeatLevel_.subscribe([this](const auto&) {
        log_debug() << "fireAutoPassengerSeatHeatLevelAttributeChanged("
                    << logicFactory_.autoPassengerSeatHeatLevel_.get().getCurrentLevel().value_ << ")";
        // getStubAdapter()->fireAutoPassengerSeatHeatLevelAttributeChanged(
            logicFactory_.autoPassengerSeatHeatLevel_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.autoSteeringWheelHeat_.subscribe([this](const auto&) {
        log_debug() << "fireAutoSteeringWheelHeatAttributeChanged("
                    << logicFactory_.autoSteeringWheelHeat_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->fireAutoSteeringWheelHeatAttributeChanged(logicFactory_.autoSteeringWheelHeat_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.autoSteeringWheelHeatLevel_.subscribe([this](const auto&) {
        log_debug() << "fireAutoSteeringWheelHeatLevelAttributeChanged("
                    << logicFactory_.autoSteeringWheelHeatLevel_.get().getCurrentLevel().value_ << ")";
        // getStubAdapter()->fireAutoSteeringWheelHeatLevelAttributeChanged(
            logicFactory_.autoSteeringWheelHeatLevel_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.manualRecircTimer_.subscribe([this](const auto&) {
        log_debug() << "fireManualRecircTimerAttributeChanged("
                    << logicFactory_.manualRecircTimer_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->fireManualRecircTimerAttributeChanged(logicFactory_.manualRecircTimer_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.airQualitySensor_.subscribe([this](const auto&) {
        log_debug() << "fireAirQualitySensorAttributeChanged("
                    << logicFactory_.airQualitySensor_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->fireAirQualitySensorAttributeChanged(logicFactory_.airQualitySensor_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.preconditioningHeatSource_.subscribe([this](const auto&) {
        log_debug() << "firePreconditioningHeatSourceAttributeChanged("
                    << logicFactory_.preconditioningHeatSource_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->firePreconditioningHeatSourceAttributeChanged(
            logicFactory_.preconditioningHeatSource_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.autoFrontDefroster_.subscribe([this](const auto&) {
        log_debug() << "fireAutoFrontDefrosterAttributeChanged("
                    << logicFactory_.autoFrontDefroster_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->fireAutoFrontDefrosterAttributeChanged(logicFactory_.autoFrontDefroster_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.autoRearDefroster_.subscribe([this](const auto&) {
        log_debug() << "fireAutoRearDefrosterAttributeChanged("
                    << logicFactory_.autoRearDefroster_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->fireAutoRearDefrosterAttributeChanged(logicFactory_.autoRearDefroster_.get());
    }));

    subscriptionHandles_.push_back(logicFactory_.additionalHeater_.subscribe([this](const auto&) {
        log_debug() << "fireAdditionalHeaterAttributeChanged("
                    << logicFactory_.additionalHeater_.get().getCurrentSelection().value_ << ")";
        // getStubAdapter()->fireAdditionalHeaterAttributeChanged(logicFactory_.additionalHeater_.get());
    }));

    return &remoteEventHandler_;
}

const UserSelection::OffOnSelection& UserSelectionStubImpl::getAutoDriverSeatHeatAttribute() {
    static UserSelection::OffOnSelection val;
    val = logicFactory_.autoDriverSeatHeat_.get();
    return val;
}

const UserSelection::LevelSelection& UserSelectionStubImpl::getAutoDriverSeatHeatLevelAttribute() {
    static UserSelection::LevelSelection val;
    val = logicFactory_.autoDriverSeatHeatLevel_.get();
    return val;
}

const UserSelection::OffOnSelection& UserSelectionStubImpl::getAutoPassengerSeatHeatAttribute() {
    static UserSelection::OffOnSelection val;
    val = logicFactory_.autoPassengerSeatHeat_.get();
    return val;
}

const UserSelection::LevelSelection& UserSelectionStubImpl::getAutoPassengerSeatHeatLevelAttribute() {
    static UserSelection::LevelSelection val;
    val = logicFactory_.autoPassengerSeatHeatLevel_.get();
    return val;
}

const UserSelection::OffOnSelection& UserSelectionStubImpl::getAutoSteeringWheelHeatAttribute() {
    static UserSelection::OffOnSelection val;
    val = logicFactory_.autoSteeringWheelHeat_.get();
    return val;
}

const UserSelection::LevelSelection& UserSelectionStubImpl::getAutoSteeringWheelHeatLevelAttribute() {
    static UserSelection::LevelSelection val;
    val = logicFactory_.autoSteeringWheelHeatLevel_.get();
    return val;
}

const UserSelection::OffOnSelection& UserSelectionStubImpl::getAirQualitySensorAttribute() {
    static UserSelection::OffOnSelection val;
    val = logicFactory_.airQualitySensor_.get();
    return val;
}

const UserSelection::OffOnSelection& UserSelectionStubImpl::getManualRecircTimerAttribute() {
    static UserSelection::OffOnSelection val;
    val = logicFactory_.manualRecircTimer_.get();
    return val;
}

const UserSelection::OffOnSelection& UserSelectionStubImpl::getAutoFrontDefrosterAttribute() {
    static UserSelection::OffOnSelection val;
    val = logicFactory_.autoFrontDefroster_.get();
    return val;
}

const UserSelection::OffOnSelection& UserSelectionStubImpl::getAutoRearDefrosterAttribute() {
    static UserSelection::OffOnSelection val;
    val = logicFactory_.autoRearDefroster_.get();
    return val;
}

const UserSelection::PreconditioningHeatSourceStruct& UserSelectionStubImpl::getPreconditioningHeatSourceAttribute() {
    static UserSelection::PreconditioningHeatSourceStruct val;
    val = logicFactory_.preconditioningHeatSource_.get();
    return val;
}

const UserSelection::OffOnSelection& UserSelectionStubImpl::getAdditionalHeaterAttribute() {
    static UserSelection::OffOnSelection val;
    val = logicFactory_.additionalHeater_.get();
    return val;
}

void UserSelectionStubImpl::requestAutoDriverSeatHeat(UserSelection::OffOnType _autoHeatSelection,
                                                      GenericReply2 _reply) {
    logicFactory_.autoDriverSeatHeatLogic_.request(_autoHeatSelection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAutoDriverSeatHeatLevel(UserSelection::LevelType _autoHeatLevel,
                                                           GenericReply2 _reply) {
    logicFactory_.autoDriverSeatHeatLevelLogic_.request(_autoHeatLevel);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAutoPassengerSeatHeat(UserSelection::OffOnType _autoHeatSelection,
                                                         GenericReply2 _reply) {
    logicFactory_.autoPassengerSeatHeatLogic_.request(_autoHeatSelection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAutoPassengerSeatHeatLevel(UserSelection::LevelType _autoHeatLevel,
                                                              GenericReply2 _reply) {
    logicFactory_.autoPassengerSeatHeatLevelLogic_.request(_autoHeatLevel);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAutoSteeringWheelHeat(UserSelection::OffOnType _autoHeatSelection,
                                                         GenericReply2 _reply) {
    logicFactory_.autoSteeringWheelHeatLogic_.request(_autoHeatSelection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAutoSteeringWheelHeatLevel(UserSelection::LevelType _autoHeatLevel,
                                                              GenericReply2 _reply) {
    logicFactory_.autoSteeringWheelHeatLevelLogic_.request(_autoHeatLevel);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAirQualitySensor(UserSelection::OffOnType _selection, GenericReply2 _reply) {
    logicFactory_.airQualitySensorLogic_.request(_selection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestManualRecircTimer(UserSelection::OffOnType _selection, GenericReply2 _reply) {
    logicFactory_.manualRecircTimerLogic_.request(_selection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAutoFrontDefroster(UserSelection::OffOnType _selection, GenericReply2 _reply) {
    logicFactory_.autoFrontDefrosterLogic_.request(_selection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestPreconditioningHeatSource(UserSelection::PreconditioningHeatSourceEnum _selection,
                                                             GenericReply2 _reply) {
    logicFactory_.preconditioningHeatSourceLogic_.request(_selection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAutoRearDefroster(UserSelection::OffOnType _selection, GenericReply2 _reply) {
    logicFactory_.autoRearDefrosterLogic_.request(_selection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}

void UserSelectionStubImpl::requestAdditionalHeater(UserSelection::OffOnType _selection, GenericReply2 _reply) {
    logicFactory_.additionalHeaterLogic_.request(_selection);
    _reply(CommonTypes::ReturnCode::SUCCESS);
}
