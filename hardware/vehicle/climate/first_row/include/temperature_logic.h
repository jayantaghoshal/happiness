/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "itemperature_logic.h"

#include "climate_reset_logic.h"
#include "fan_level_front_logic.h"
#include "v0/org/volvocars/climate/FirstRow.hpp"

#include <Application_dataelement.h>
#include <ECD_dataelement.h>
#include <libsettings/setting.h>
#include <mutex>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

namespace common {
namespace daemon {
class TemperatureConverter;
}
}

class TemperatureLogic : public common::daemon::ITemperatureLogic {
  public:
    TemperatureLogic(bool isValidCarConfig,
                     common::daemon::TemperatureConverter& tempConverter,
                     NotifiableProperty<double>& convertedTemp,
                     NotifiableProperty<double>& storedTemp,
                     NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& storedTempHiLoN,
                     NotifiableProperty<double>& temp,
                     NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& tempHiLoN,
                     NotifiableProperty<FirstRowGen::StateType>& state,
                     ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront,
                     ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
                     ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateResetEvent);

    virtual ~TemperatureLogic();

    static bool isPassengerCarConfigValid();

    void request(double temp);
    void request(autosar::HmiCmptmtTSpSpcl tempHiLoN) override;

    FirstRowGen::Range range();

  private:
    bool signalsOk() const;
    bool activationCheck(const autosar::UsgModSts1 usgMode,
                         const autosar::CarModSts1 carMode,
                         const autosar::OnOff1 parkingClimate) const;

    void update(double temp);
    void update(autosar::HmiCmptmtTSpSpcl tempHiLoN);
    void maxDefrosterChanged(const FirstRowGen::MaxDefrosterState& maxDefroster);
    void fanLevelFrontChanged(FirstRowGen::FanLevelFrontValue fanLevel);
    void resetChanged(ClimateResetLogic::ClimateResetEvent resetEvent);

    common::daemon::TemperatureConverter& tempConverter_;
    NotifiableProperty<double>& convertedTemp_;
    NotifiableProperty<double>& storedTemp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& storedTempHiLoN_;
    NotifiableProperty<double>& temp_;
    NotifiableProperty<autosar::HmiCmptmtTSpSpcl>& tempHiLoN_;
    NotifiableProperty<FirstRowGen::StateType>& state_;
    ReadOnlyNotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront_;
    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster_;
    ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateResetEvent_;

    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehModMngtGlbSafe1_;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climateActive_;
    ECDDataElement::DESink<autosar::IndcnUnit_info> indcnUnit_;

    bool active_;
    autosar::AmbTIndcdUnit tUnit_;
    FirstRowGen::MaxDefrosterState prevMaxDefroster_;
    SubscriptionHandle storedTempId_;
    SubscriptionHandle storedTempHiLoNId_;
    SubscriptionHandle tempId_;
    SubscriptionHandle tempHiLoNId_;
    SubscriptionHandle climateResetEventId_;
    SubscriptionHandle fanLevelFrontId_;
    SubscriptionHandle maxDefrosterId_;
    std::recursive_mutex mutex_;
};
