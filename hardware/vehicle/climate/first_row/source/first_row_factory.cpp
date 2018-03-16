/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "first_row_factory.h"

#include "common_factory.h"
#include "kpi_log.h"
#include "logging_context.h"
#include "setting_factory.h"

#include <cc_parameterlist.h>
#include <vcc/localconfig.h>
#include <user_selection_stubimpl.hpp>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

using namespace autosar;

FirstRowFactory::FirstRowFactory(const vcc::LocalConfigReaderInterface* lcfg,
                                 const android::sp<SettingsFramework::SettingsManager>& settingsManager,
                                 tarmac::timeprovider::TimerManagerInterface& timeProvider,
                                 signal_proxy::Proxies& proxies,
                                 UserSelectionFactory& userSelections,
                                 common::daemon::Factory& commonFactory)
    : sf_{settingsManager},
      userSelections_{userSelections},
      timeProvider_{timeProvider},
      airDistribution_{FirstRowGen::AirDistributionAngle::DISABLED},
      autoClimate_{FirstRowGen::AutoClimateState::DISABLED},
      autoClimateEvent_{AutoClimateLogic::AutoClimateEvent::NO_REQ},
      cleanZone_{FirstRowGen::CleanzoneState::OFF},
      climateReset_{},
      fanLevelFront_{FirstRowGen::FanLevelFrontValue::OFF},
      manualRecirc_{FirstRowGen::ManualRecircState::OFF},
      airConditioner_{FirstRowGen::AirConditionerState::DISABLED},
      driverConvertedTemp_{22.0},
      driverStoredTemp_{22.0},
      driverStoredTempHiLoN_{autosar::HmiCmptmtTSpSpcl::Norm},
      driverTempState_{FirstRowGen::StateType::DISABLED},
      passengerConvertedTemp_{22.0},
      passengerStoredTemp_{22.0},
      passengerStoredTempHiLoN_{autosar::HmiCmptmtTSpSpcl::Norm},
      passengerTemp_{22.0},
      passengerTempHiLoN_{autosar::HmiCmptmtTSpSpcl::Norm},
      passengerTempState_{FirstRowGen::StateType::DISABLED},
      electricDefrosterWindscreen_{FirstRowGen::ElectricDefrosterWindscreenState::OFF},
      electricDefrosterRear_{FirstRowGen::ElectricDefrosterRearState::OFF},
      electricDefrosterWindscreenPopup_{FirstRowGen::ElectricDefrosterPopupState::OFF},
      electricDefrosterRearPopup_{FirstRowGen::ElectricDefrosterPopupState::OFF},
      seatHeatDriver_{{FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF}},
      seatHeatPassenger_{{FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF}},
      steeringWheelHeat_{{FirstRowGen::HeatState::DISABLED, FirstRowGen::HeatLevel::OFF}},
      seatVentDriver_{{FirstRowGen::VentState::DISABLED, FirstRowGen::VentLevel::OFF}},
      seatVentPassenger_{{FirstRowGen::VentState::DISABLED, FirstRowGen::VentLevel::OFF}},
      ionizer_{FirstRowGen::IonizerState::NOT_PRESENT},
      fanLevelFrontLogic_{lcfg,
                          sf_.lastAutoFanLevelFrontProxy_,
                          sf_.lastFanLevelFrontProxy_,
                          fanLevelFront_,
                          commonFactory.maxDefroster_,
                          autoClimate_,
                          climateReset_,
                          fanLevelFrontnDispatcher_},
      airDistributionLogic_{airDistribution_,
                            sf_.AirDistributionProxy_,
                            climateReset_,
                            fanLevelFront_,
                            autoClimate_,
                            commonFactory.maxDefroster_},
      autoClimateLogic_{autoClimate_, autoClimateEvent_, airDistribution_, fanLevelFront_, commonFactory.maxDefroster_},
      cleanZoneLogic_{cleanZone_},
      climateResetLogic_{climateReset_},
      manualRecircLogic_{lcfg,
                         userSelections_.manualRecircTimer_,
                         userSelections_.airQualitySensor_,
                         sf_.ManualRecircProxy_,
                         manualRecirc_,
                         climateReset_,
                         manualRecircDispatcher_,
                         fanLevelFront_,
                         autoClimateEvent_,
                         commonFactory.maxDefroster_},
      airConditionerLogic_{sf_.AirConditionerProxy_,
                           airConditioner_,
                           fanLevelFront_,
                           commonFactory.maxDefroster_,
                           autoClimateEvent_,
                           climateReset_},
      electricDefrosterWindscreenLogic_{lcfg,
                                        electricDefrosterWindscreen_,
                                        userSelections_.autoFrontDefroster_,
                                        electricDefrosterWindscreenDispatcher_,
                                        proxies.hmiDefrstElecReq_},
      electricDefrosterRearPopupLogic_{electricDefrosterRear_, electricDefrosterRearPopup_},
      electricDefrosterRearLogic_{lcfg,
                                  electricDefrosterRear_,
                                  userSelections_.autoRearDefroster_,
                                  electricDefrosterRearDispatcher_,
                                  proxies.hmiDefrstElecReq_},
      seatHeatDriverLogic_{lcfg,
                           SeatHeatLogic::UserLocation::DRIVER,
                           proxies.seatClimateStatus_,
                           seatHeatDriver_,
                           userSelections_.autoDriverSeatHeat_,
                           userSelections_.autoDriverSeatHeatLevel_,
                           sf_.seatHeatLevelDriverProxy_,
                           timeProvider_},
      seatHeatPassengerLogic_{lcfg,
                              SeatHeatLogic::UserLocation::PASSENGER,
                              proxies.seatClimateStatus_,
                              seatHeatPassenger_,
                              userSelections_.autoPassengerSeatHeat_,
                              userSelections_.autoPassengerSeatHeatLevel_,
                              sf_.seatHeatLevelPassengerProxy_,
                              timeProvider_},
      electricDefrosterWindscreenPopupLogic_{electricDefrosterWindscreenPopup_, electricDefrosterWindscreen_},
      temperatureProxy_{driverStoredTemp_,
                        driverStoredTempHiLoN_,
                        passengerStoredTemp_,
                        passengerStoredTempHiLoN_,
                        commonFactory.driverTemp_,
                        commonFactory.driverTempHiLoN_,
                        passengerTemp_,
                        passengerTempHiLoN_,
                        commonFactory.maxDefroster_,
                        sf_.TemperatureDriverProxy_,
                        sf_.TemperatureDriverHiLoNProxy_,
                        sf_.TemperaturePassengerProxy_,
                        sf_.TemperaturePassengerHiLoNProxy_,
                        proxies.hmiCmptmtTSp_},
      driverTemperatureLogic_{true,
                              commonFactory.temperatureConverter_,
                              driverConvertedTemp_,
                              driverStoredTemp_,
                              driverStoredTempHiLoN_,
                              commonFactory.driverTemp_,
                              commonFactory.driverTempHiLoN_,
                              driverTempState_,
                              fanLevelFront_,
                              commonFactory.maxDefroster_,
                              climateReset_},
      passengerTemperatureLogic_{TemperatureLogic::isPassengerCarConfigValid(),
                                 commonFactory.temperatureConverter_,
                                 passengerConvertedTemp_,
                                 passengerStoredTemp_,
                                 passengerStoredTempHiLoN_,
                                 passengerTemp_,
                                 passengerTempHiLoN_,
                                 passengerTempState_,
                                 fanLevelFront_,
                                 commonFactory.maxDefroster_,
                                 climateReset_},
      temperatureSyncLogic_{commonFactory.temperatureSync_,
                            commonFactory.driverTemp_,
                            commonFactory.driverTempHiLoN_,
                            passengerTemp_,
                            passengerTempHiLoN_,
                            commonFactory.tempRearLeft_,
                            commonFactory.tempHiLoNRearLeft_,
                            commonFactory.tempRearRight_,
                            commonFactory.tempHiLoNRearRight_,
                            commonFactory.maxDefroster_,
                            sf_.createTemperatureSyncProxy()},
      temperatureSynchronizer_{passengerTemperatureLogic_,
                               commonFactory.temperatureSync_,
                               commonFactory.maxDefroster_,
                               commonFactory.driverTemp_,
                               commonFactory.driverTempHiLoN_},
      maxDefrosterLogic_{commonFactory.maxDefroster_, fanLevelFront_, airDistribution_, autoClimate_, climateReset_},
      steeringWheelHeatLogic_{lcfg,
                              steeringWheelHeat_,
                              userSelections_.autoSteeringWheelHeat_,
                              userSelections_.autoSteeringWheelHeatLevel_,
                              sf_.steeringWheelHeatLevelProxy_,
                              timeProvider_},
      seatVentDriverLogic_{SeatVentLogic::UserLocation::DRIVER,
                           proxies.seatClimateStatus_,
                           seatVentDriver_,
                           sf_.seatVentLevelDriverProxy_},
      seatVentPassengerLogic_{SeatVentLogic::UserLocation::PASSENGER,
                              proxies.seatClimateStatus_,
                              seatVentPassenger_,
                              sf_.seatVentLevelPassengerProxy_},
      ionizerLogic_{ionizer_, sf_.ionizerProxy_},
      defrosterWindscreenCCSM_{lcfg,
                               commonFactory.maxDefroster_,
                               electricDefrosterWindscreen_,
                               defrosterWindscreenCCSMDispatcher_,
                               electricDefrosterWindscreenLogic_,
                               electricDefrosterWindscreenPopupLogic_,
                               maxDefrosterLogic_},
      defrosterRearCCSM_{electricDefrosterRear_, electricDefrosterRearLogic_, electricDefrosterRearPopupLogic_},
      VFCDrivingClimatisationLogic_{VFCDrivingClimatisationDispatcher_},
      VFCVisibilityLogic_{} {
    // TODO(climateport): Common API setup was here
}

ReadOnlyNotifiableProperty<FirstRowGen::HeatAttribute>& FirstRowFactory::getSeatHeatDriverProperty() {
    return seatHeatDriver_;
}

ReadOnlyNotifiableProperty<FirstRowGen::HeatAttribute>& FirstRowFactory::getSeatHeatPassengerProperty() {
    return seatHeatPassenger_;
}

ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>&
FirstRowFactory::getElectricDefrosterWindscreenProperty() {
    return electricDefrosterWindscreen_;
}

ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterRearState>&
FirstRowFactory::getElectricDefrosterRearProperty() {
    return electricDefrosterRear_;
}

ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>&
FirstRowFactory::getElectricDefrosterWindscreenPopupProperty() {
    return electricDefrosterWindscreenPopup_;
}

ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>&
FirstRowFactory::getElectricDefrosterRearPopupProperty() {
    return electricDefrosterRearPopup_;
}

ReadOnlyNotifiableProperty<FirstRowGen::HeatAttribute>& FirstRowFactory::getSteeringWheelHeatProperty() {
    return steeringWheelHeat_;
}

ReadOnlyNotifiableProperty<FirstRowGen::VentAttribute>& FirstRowFactory::getSeatVentDriverProperty() {
    return seatVentDriver_;
}

ReadOnlyNotifiableProperty<FirstRowGen::VentAttribute>& FirstRowFactory::getSeatVentPassengerProperty() {
    return seatVentPassenger_;
}

ReadOnlyNotifiableProperty<FirstRowGen::IonizerState>& FirstRowFactory::getIonizerProperty() { return ionizer_; }
