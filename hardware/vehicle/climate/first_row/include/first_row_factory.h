/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "air_conditioner_logic.h"
#include "air_distribution_logic.h"
#include "auto_climate_logic.h"
#include "cleanzone_logic.h"
#include "climate_reset_logic.h"
#include "defroster_rear_ccsm.h"
#include "defroster_windscreen_ccsm.h"
#include "electric_defroster_rear_logic.h"
#include "electric_defroster_rear_popup_logic.h"
#include "electric_defroster_windscreen_logic.h"
#include "electric_defroster_windscreen_popup_logic.h"
#include "fan_level_front_logic.h"
#include "first_row_stubimpl.hpp"
#include "ionizer_logic.h"
#include "legacydispatcher.h"
#include "manual_recirc_logic.h"
#include "max_defroster_logic.h"
#include "notifiable_property.h"
#include "seat_heat_logic.h"
#include "seat_vent_logic.h"
#include "setting_factory.h"
#include "signal_proxy.h"
#include "steering_wheel_heat_logic.h"
#include "temperature_converter.h"
#include "temperature_logic.h"
#include "temperature_proxy.h"
#include "temperature_sync.h"
#include "temperature_synchronizer.h"
#include "timer_manager_interface.h"
#include "user_selection_factory.h"
#include "vfc_driving_climatisation.h"
#include "vfc_visibility.h"

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

namespace common {
namespace daemon {
class Factory;
}
}

class SettingFactory;

class FirstRowFactory final {
  public:
    FirstRowFactory(const android::sp<SettingsFramework::SettingsManager>& settingsManager,
                    tarmac::timeprovider::TimerManagerInterface& timeProvider, signal_proxy::Proxies& proxies,
                    UserSelectionFactory& userSelections, common::daemon::Factory& commonFactory);

  private:
    SettingFactory sf_;
    UserSelectionFactory& userSelections_;
    tarmac::timeprovider::TimerManagerInterface& timeProvider_;

  public:
    // First row notifiable properties
    NotifiableProperty<FirstRowGen::AirDistributionAngle> airDistribution_;
    NotifiableProperty<FirstRowGen::AutoClimateState> autoClimate_;
    NotifiableProperty<AutoClimateLogic::AutoClimateEvent> autoClimateEvent_;
    NotifiableProperty<FirstRowGen::CleanzoneState> cleanZone_;
    NotifiableProperty<ClimateResetLogic::ClimateResetEvent> climateReset_;
    NotifiableProperty<FirstRowGen::FanLevelFrontValue> fanLevelFront_;
    NotifiableProperty<FirstRowGen::ManualRecircState> manualRecirc_;
    NotifiableProperty<FirstRowGen::AirConditionerState> airConditioner_;
    NotifiableProperty<double> driverConvertedTemp_;
    NotifiableProperty<double> driverStoredTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> driverStoredTempHiLoN_;
    NotifiableProperty<FirstRowGen::StateType> driverTempState_;
    NotifiableProperty<double> passengerConvertedTemp_;
    NotifiableProperty<double> passengerStoredTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> passengerStoredTempHiLoN_;
    NotifiableProperty<double> passengerTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl> passengerTempHiLoN_;
    NotifiableProperty<FirstRowGen::StateType> passengerTempState_;

    NotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState> electricDefrosterWindscreen_;
    NotifiableProperty<FirstRowGen::ElectricDefrosterRearState> electricDefrosterRear_;
    NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState> electricDefrosterWindscreenPopup_;
    NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState> electricDefrosterRearPopup_;

  private:
    NotifiableProperty<FirstRowGen::HeatAttribute> seatHeatDriver_;
    NotifiableProperty<FirstRowGen::HeatAttribute> seatHeatPassenger_;
    NotifiableProperty<FirstRowGen::HeatAttribute> steeringWheelHeat_;
    NotifiableProperty<FirstRowGen::VentAttribute> seatVentDriver_;
    NotifiableProperty<FirstRowGen::VentAttribute> seatVentPassenger_;

    NotifiableProperty<FirstRowGen::IonizerState> ionizer_;

  public:
    ReadOnlyNotifiableProperty<FirstRowGen::HeatAttribute>& getSeatHeatDriverProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::HeatAttribute>& getSeatHeatPassengerProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& getElectricDefrosterWindscreenProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterRearState>& getElectricDefrosterRearProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>& getElectricDefrosterWindscreenPopupProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>& getElectricDefrosterRearPopupProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::HeatAttribute>& getSteeringWheelHeatProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::VentAttribute>& getSeatVentDriverProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::VentAttribute>& getSeatVentPassengerProperty();
    ReadOnlyNotifiableProperty<FirstRowGen::IonizerState>& getIonizerProperty();

    // First row logic
    LegacyDispatcher fanLevelFrontnDispatcher_;
    FanLevelFrontLogic fanLevelFrontLogic_;
    AirDistributionLogic airDistributionLogic_;
    AutoClimateLogic autoClimateLogic_;
    CleanZoneLogic cleanZoneLogic_;
    ClimateResetLogic climateResetLogic_;
    LegacyDispatcher manualRecircDispatcher_;
    ManualRecircLogic manualRecircLogic_;
    AirConditionerLogic airConditionerLogic_;
    LegacyDispatcher electricDefrosterWindscreenDispatcher_;
    ElectricDefrosterWindscreenLogic electricDefrosterWindscreenLogic_;
    ElectricDefrosterRearPopupLogic electricDefrosterRearPopupLogic_;
    LegacyDispatcher electricDefrosterRearDispatcher_;
    ElectricDefrosterRearLogic electricDefrosterRearLogic_;
    SeatHeatLogic seatHeatDriverLogic_;
    SeatHeatLogic seatHeatPassengerLogic_;
    ElectricDefrosterWindscreenPopupLogic electricDefrosterWindscreenPopupLogic_;

    // Do not change the order of the temperature proxy and logic
    TemperatureProxy temperatureProxy_;
    TemperatureLogic driverTemperatureLogic_;
    TemperatureLogic passengerTemperatureLogic_;
    TemperatureSyncLogic temperatureSyncLogic_;
    common::daemon::TemperatureSynchronizer temperatureSynchronizer_;
    MaxDefrosterLogic maxDefrosterLogic_;
    SteeringWheelHeatLogic steeringWheelHeatLogic_;
    SeatVentLogic seatVentDriverLogic_;
    SeatVentLogic seatVentPassengerLogic_;
    first_row::IonizerLogic ionizerLogic_;
    LegacyDispatcher defrosterWindscreenCCSMDispatcher_;
    DefrosterWindscreenCCSM defrosterWindscreenCCSM_;
    DefrosterRearCCSM defrosterRearCCSM_;

    // VFC
    LegacyDispatcher VFCDrivingClimatisationDispatcher_;
    VFCDrivingClimatisationLogic VFCDrivingClimatisationLogic_;
    VFCVisibilityLogic VFCVisibilityLogic_;
};
