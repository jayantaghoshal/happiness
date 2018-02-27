/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "electric_defroster_windscreen_popup_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"
#include "notifiable_property.h"

#include <carconfig.h>
#include <cc_parameterlist.h>

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

ElectricDefrosterWindscreenPopupLogic::ElectricDefrosterWindscreenPopupLogic(
        NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>& electricWindscreenPopupAttribute,
        ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterWindscreenState>& ElectricDefrosterWindscreen)
    : previousWindowscreenState_(FirstRowGen::ElectricDefrosterWindscreenState::OFF),
      HmiDefrosterWindscreenStatus_(autosar::ActrDefrstSts::NotAvailable),
      shareWindscreenPopupAttribute_{electricWindscreenPopupAttribute},
      shareElectricDefrosterWindscreen(ElectricDefrosterWindscreen) {
    log_debug() << "Electric Defroster POP UP constructor intialised and signal subcsription";

    shareWindscreenPopupAttribute_.set(ElectricDefrosterWindscreenPopupState::OFF);

    HmiDefrosterStatus.subscribe([this]() { handleHmiDefrosterStatus(); });

    ElectricDefrosterWindscreenHandle = shareElectricDefrosterWindscreen.subscribe([this](const auto state) {
        (void)state;
        this->handleElectricDefrosterWindscreen();
    });

    handleHmiDefrosterStatus();
    handleElectricDefrosterWindscreen();
}

ElectricDefrosterWindscreenPopupLogic::~ElectricDefrosterWindscreenPopupLogic() = default;

// Request from HMI to turn off the Pop UP
void ElectricDefrosterWindscreenPopupLogic::requestElectricDefrosterWindscreenPopup(
        FirstRowGen::ElectricDefrosterPopupRequest requestedState) {
    if (requestedState == FirstRowGen::ElectricDefrosterPopupRequest::OFF) {
        shareWindscreenPopupAttribute_.set(ElectricDefrosterWindscreenPopupState::OFF);
    }
}

void ElectricDefrosterWindscreenPopupLogic::requestElectricDefrosterWindscreen(
        FirstRowGen::ElectricDefrosterWindscreenRequest requestState) {
    if ((HmiDefrosterWindscreenStatus_ == autosar::ActrDefrstSts::NotAvailable) &&
        (requestState == FirstRowGen::ElectricDefrosterWindscreenState::ON)) {
        shareWindscreenPopupAttribute_.set(ElectricDefrosterWindscreenPopupState::ON);
    }
}

// CCSM request handling
void ElectricDefrosterWindscreenPopupLogic::request(OnOff requestedState) {
    if (HmiDefrosterWindscreenStatus_ == autosar::ActrDefrstSts::NotAvailable && requestedState == OnOff::On) {
        shareWindscreenPopupAttribute_.set(ElectricDefrosterWindscreenPopupState::ON);
    }
}

// Defroster Windscreen signal handling
void ElectricDefrosterWindscreenPopupLogic::handleHmiDefrosterStatus() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    auto status = HmiDefrosterStatus.get().value();

    if (HmiDefrosterStatus.get().isOk()) {
        HmiDefrosterWindscreenStatus_ = status.Frnt;
        log_debug() << "HmiDefrosterWindscreenStatus   : " << static_cast<int>(HmiDefrosterWindscreenStatus_);

        if (HmiDefrosterWindscreenStatus_ != autosar::ActrDefrstSts::NotAvailable) {
            shareWindscreenPopupAttribute_.set(ElectricDefrosterWindscreenPopupState::OFF);
        }
    }
}

// Electric Defrost Windscreen shared object handling
void ElectricDefrosterWindscreenPopupLogic::handleElectricDefrosterWindscreen() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    log_debug() << "handleElectricDefrosterWindscreen() =  " << shareElectricDefrosterWindscreen.get();

    if (HmiDefrosterStatus.get().value().Frnt == autosar::ActrDefrstSts::NotAvailable &&
        (previousWindowscreenState_ == FirstRowGen::ElectricDefrosterWindscreenState::ON &&
         shareElectricDefrosterWindscreen.get() != FirstRowGen::ElectricDefrosterWindscreenState::ON)) {
        shareWindscreenPopupAttribute_.set(ElectricDefrosterWindscreenPopupState::ON);
    }

    previousWindowscreenState_ = shareElectricDefrosterWindscreen.get();
}
