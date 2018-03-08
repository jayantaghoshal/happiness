/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include "climate_reset_logic.h"
#include "dfsm.h"
#include "ilegacydispatcher.h"
#include "notifiable_property.h"
#include "settings_proxy.h"
#include "v0/org/volvocars/climate/FirstRow.hpp"

#include <Application_dataelement.h>
#include <libsettings/setting.h>
#include <vcc/localconfig.h>
#include <chrono>

using FirstRowGen = v0::org::volvocars::climate::FirstRow;

class FanLevelFrontLogic : private DFsm_Main {
  public:
    enum FanLevelFrontState { INIT = 0, CARCONFIG_INVALID, SYSTEM_OK, SYSTEM_ERROR, NOT_ACTIVE, ACTIVE, ON, OFF };

    FanLevelFrontLogic(const vcc::LocalConfigReaderInterface* lcfg,
                       SettingsProxyInterface<FirstRowGen::FanLevelFrontValue::Literal>& autoFanLevelSetting,
                       SettingsProxy<FirstRowGen::FanLevelFrontValue::Literal, SettingsFramework::UserScope::USER,
                                     SettingsFramework::UserScope::NOT_USER_RELATED>& fanLevelFrontSetting,
                       NotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront,
                       ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster,
                       ReadOnlyNotifiableProperty<FirstRowGen::AutoClimateState>& autoClimate,
                       ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset,
                       ILegacyDispatcher& timerDispatcher);

    virtual ~FanLevelFrontLogic() = default;

    void requestFanLevel(FirstRowGen::FanLevelFrontRequest fanLevel);
    bool isInState(int state);

  private:
    enum ClimateControl { NONE = 0, ECC, MCC };

    bool carConfigState(ClimateControl& climateControl);

    bool registerFsm();

    bool activationCheck() const;
    FanLevelFrontState activeEntry() const;
    bool changeFanLevelCheck();

    void handleMaxDefroster();
    void handleAutoClimate();
    void handleUsageAndCarMode();
    void handleClimateActive();

    void sendFanLevel(FirstRowGen::FanLevelFrontValue level);
    void setFanLevel(FirstRowGen::FanLevelFrontValue value);
    void setAutoFanLevel(FirstRowGen::FanLevelFrontValue value);
    void resetFanLevel(FirstRowGen::FanLevelFrontValue level);

    SettingsProxyInterface<FirstRowGen::FanLevelFrontValue::Literal>& autoFanLevelSetting_;
    SettingsProxy<FirstRowGen::FanLevelFrontValue::Literal, SettingsFramework::UserScope::USER,
                  SettingsFramework::UserScope::NOT_USER_RELATED>& fanLevelFrontSetting_;
    FirstRowGen::FanLevelFrontValue::Literal autoFanLevelSettingGETPORT_;
    FirstRowGen::FanLevelFrontValue::Literal fanLevelFrontSettingGETPORT_;

    NotifiableProperty<FirstRowGen::FanLevelFrontValue>& fanLevelFront_;
    ReadOnlyNotifiableProperty<FirstRowGen::MaxDefrosterState>& maxDefroster_;
    ReadOnlyNotifiableProperty<FirstRowGen::AutoClimateState>& autoClimate_;
    ReadOnlyNotifiableProperty<ClimateResetLogic::ClimateResetEvent>& climateReset_;

    ILegacyDispatcher& timerDispatcher_;

    // FlexRay signal
    ApplicationDataElement::DESender<autosar::HmiHvacFanLvlFrnt_info> fanLevelFrontSignal_;
    ApplicationDataElement::DEReceiver<autosar::ClimaActv_info> climateActiveSignal_;
    ApplicationDataElement::DEReceiver<autosar::VehModMngtGlbSafe1_info> vehicleModesSignal_;

    std::recursive_mutex mutex_;
    std::vector<SubscriptionHandle> subscriptions_;
    std::chrono::seconds timeout_;

    ClimateControl climateControl_;

    FirstRowGen::FanLevelFrontValue reqFanLevel_;
    FirstRowGen::FanLevelFrontValue currentFanLevel_;
    FirstRowGen::AutoClimateState reqAutoClimate_;
    FirstRowGen::AutoClimateState currentAutoClimate_;
    FirstRowGen::MaxDefrosterState reqMaxDefrost_;
    FirstRowGen::MaxDefrosterState currentMaxDefrost_;

    autosar::UsgModSts1 usageMode_;
    autosar::CarModSts1 carMode_;
    autosar::OnOff1 climateActive_;
};
