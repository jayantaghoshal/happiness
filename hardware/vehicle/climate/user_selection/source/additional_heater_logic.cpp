/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "additional_heater_logic.h"

#include "local_config_helper.h"
#include "logging_context.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(UserSelectionContext)

using namespace SettingsFramework;

namespace {
bool carConfigOk() {
    try {
        auto const additionalHeaterType = carconfig::getValue<CarConfigParams::CC171_AdditionalHeaterType>();

        return additionalHeaterType == CarConfigParams::CC171_AdditionalHeaterType::With_Additional_Heater;
    } catch (const std::out_of_range& e) {
        log_error() << "Car config threw: " << e.what();
        throw;
    } catch (const std::runtime_error& e) {
        log_error() << "Car config threw: " << e.what();
        throw;
    }
}

auto LOG_PREFIX = "AdditionalHeater: ";
}  // namespace

AdditionalHeaterLogic::AdditionalHeaterLogic(
        const vcc::LocalConfigReaderInterface* lcfg,
        NotifiableProperty<UserSelectionGen::OffOnSelection>& additionalHeater,
        std::unique_ptr<
                SettingsProxy<int, SettingsFramework::UserScope::USER, SettingsFramework::UserScope::NOT_USER_RELATED>>
                additionalHeaterSetting)
    : additionalHeater_{additionalHeater},
      additionalHeaterSetting_{std::move(additionalHeaterSetting)},
      additionalHeaterAvaiable_{util::readLocalConfig<bool>("Climate_AdditionalHeaterAvailability", lcfg)} {
    if (carConfigOk() && additionalHeaterAvaiable_) {
        auto updateSharedObject = [this](auto newSetting) {
            additionalHeater_.set(
                    UserSelectionGen::OffOnSelection{UserSelectionGen::StateType::AVAILABLE,
                                                     static_cast<UserSelectionGen::OffOnType::Literal>(newSetting)});
            sendSignal();
        };

        updateSharedObject(additionalHeaterSetting_->defaultValuePORTHELPER());

        additionalHeaterSetting_->subscribe(updateSharedObject);
    } else {
        additionalHeater_.set(UserSelectionGen::OffOnSelection{UserSelectionGen::StateType::NOT_PRESENT,
                                                               UserSelectionGen::OffOnType::OFF});
    }

    log_debug() << LOG_PREFIX << "state: " << additionalHeater_.get().getCurrentState()
                << ", selection: " << additionalHeater_.get().getCurrentSelection();

    sendSignal();
}

void AdditionalHeaterLogic::sendSignal() {
    if ((additionalHeater_.get().getCurrentState() == UserSelectionGen::StateType::AVAILABLE) &&
        additionalHeater_.get().getCurrentSelection() == UserSelectionGen::OffOnType::ON) {
        HeatrDurgDrvgReqd_.send(autosar::OffOnAut1::Aut);
    } else {
        HeatrDurgDrvgReqd_.send(autosar::OffOnAut1::Off);
    }
}

void AdditionalHeaterLogic::request(UserSelectionGen::OffOnType value) {
    log_debug() << LOG_PREFIX << "request AdditionalHeater value: " << value;
    additionalHeaterSetting_->set(static_cast<int>(value));
}
