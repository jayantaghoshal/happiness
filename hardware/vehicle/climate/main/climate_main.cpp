/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "climate_main.h"

#include "kpi_log.h"
#include "libsettings/settingsmanagerhidl.h"
#include "logging_context.h"
#include "vfc_helper.h"

#include <chrono>

LOG_SET_DEFAULT_CONTEXT(mainContext)

using namespace std::chrono_literals;

namespace {
bool initGlobalLegacyDispatcher(tarmac::timeprovider::TimeProvider* time_provider) {
    LegacyDispatcher::setGlobalInstanceHackTimeProvider(time_provider);
    return true;
}
}  // namespace

ClimateMain::ClimateMain(const std::shared_ptr<tarmac::eventloop::IDispatcher>& tarmacDispatcher)
    : tarmacDispatcher_{tarmacDispatcher},
      settingsManager_{new SettingsFramework::SettingsManagerHidl(*tarmacDispatcher_)},
      timeProvider_{tarmacDispatcher_},
      hackToInitGlobalLegacyDispatcher{initGlobalLegacyDispatcher(&timeProvider_)},
      signalProxies_{}  //, commonFactory_{}
      ,
      user_selection{settingsManager_},
      first_row{settingsManager_, timeProvider_, signalProxies_, user_selection, commonFactory_}
//, parking_climate{commonFactory_}
//, second_and_third_row{ signalProxies_, commonFactory_ }
{
    // =======================================================================================
    // Each time a User Settings Signal is sent out the VFC UserInputSettings shall be activated. At startup several
    // User
    // Settings Signals
    // are sent but only ONE vfc activation request needs to be sent and that is done here for ALL User Settings Signal.
    // See REQPROD: 329446 + 335396
    // 335396 says that we shall activate the DriverInformationCenterStackDisplayPoll when UsgMod goes Abandoned ->
    // InActive
    // Since we cannot directly detect that transition we do this at start-up instead.
    // The requirement says 3 secs for a "normal" user settings signal but we give it some more time at startup
    ApplicationDataElement::Helper::request_vfc(Vfc::UserInputSettings, 10s);
    ApplicationDataElement::Helper::request_vfc(Vfc::DriverInformationCenterStackDisplayPoll, 3s);
    // =======================================================================================

    log_info() << KPI_MARKER << "Climate service ready" << hackToInitGlobalLegacyDispatcher;
}
