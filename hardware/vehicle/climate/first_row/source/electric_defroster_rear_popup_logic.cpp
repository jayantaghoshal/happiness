/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "electric_defroster_rear_popup_logic.h"

#include "first_row_printers.h"
#include "logging_context.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

ElectricDefrosterRearPopupLogic::ElectricDefrosterRearPopupLogic(
        ReadOnlyNotifiableProperty<FirstRowGen::ElectricDefrosterRearState>& sElectricDefrosterRear,
        NotifiableProperty<FirstRowGen::ElectricDefrosterPopupState>& sElectricDefrosterPopup)
    : shareElectricDefrosterRear(sElectricDefrosterRear),
      shareElectricDefrosterRearPopup(sElectricDefrosterPopup),
      CurrentState_(InternalPopupState::OFF),
      previousRearState_(FirstRowGen::ElectricDefrosterRearState::DISABLED),
      requestedState_(FirstRowGen::ElectricDefrosterPopupState::OFF),
      HmiDefrosterRearStatus_(ActrDefrstSts::NotAvailable),
      HmiDefrosterMirrorStatus_(ActrDefrstSts::NotAvailable) {
    HmiDefrosterStatus.subscribe([this]() { handleHmiDefrosterStatus(); });
    ElectricDefrosterRearHandle =
            shareElectricDefrosterRear.subscribe([this](const auto) { this->handleElectricDefrosterRear(); });

    handleHmiDefrosterStatus();
    handleElectricDefrosterRear();
}

ElectricDefrosterRearPopupLogic::~ElectricDefrosterRearPopupLogic() = default;

void ElectricDefrosterRearPopupLogic::request(FirstRowGen::ElectricDefrosterPopupRequest requestState) {
    log_debug() << "Requested Value = " << requestState;
    if (requestState == FirstRowGen::ElectricDefrosterPopupRequest::OFF) {
        setState(FirstRowGen::ElectricDefrosterPopupState::OFF);
    }
}

void ElectricDefrosterRearPopupLogic::requestElectricDefrosterRear(
        FirstRowGen::ElectricDefrosterRearRequest requestState) {
    if ((requestState == FirstRowGen::ElectricDefrosterRearState::ON) &&
        (HmiDefrosterRearStatus_ == ActrDefrstSts::NotAvailable &&
         HmiDefrosterMirrorStatus_ == ActrDefrstSts::NotAvailable)) {
        setState(FirstRowGen::ElectricDefrosterPopupState::ON);
    }
}

void ElectricDefrosterRearPopupLogic::request(OnOff requestedState) {
    if (requestedState == OnOff::On && (HmiDefrosterRearStatus_ == ActrDefrstSts::NotAvailable &&
                                        HmiDefrosterMirrorStatus_ == ActrDefrstSts::NotAvailable)) {
        setState(FirstRowGen::ElectricDefrosterPopupState::ON);
    }
}

void ElectricDefrosterRearPopupLogic::setState(FirstRowGen::ElectricDefrosterPopupState state) {
    if (FirstRowGen::ElectricDefrosterPopupState::OFF == state) {
        shareElectricDefrosterRearPopup.set(FirstRowGen::ElectricDefrosterPopupState::OFF);
        CurrentState_ = InternalPopupState::OFF;
    } else if (FirstRowGen::ElectricDefrosterPopupState::ON == state) {
        shareElectricDefrosterRearPopup.set(FirstRowGen::ElectricDefrosterPopupState::ON);
        CurrentState_ = InternalPopupState::ON;
    }
    log_debug() << "shareElectricDefrosterRearPopup = " << shareElectricDefrosterRearPopup.get();
}

/*=====================================================*/
/*                  Subscribe Functions                */
/*=====================================================*/
void ElectricDefrosterRearPopupLogic::handleHmiDefrosterStatus() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    auto status = HmiDefrosterStatus.get().value();

    if (HmiDefrosterStatus.get().isOk()) {
        if (HmiDefrosterRearStatus_ != status.Re || HmiDefrosterMirrorStatus_ != status.Mirrr) {
            HmiDefrosterRearStatus_ = status.Re;
            HmiDefrosterMirrorStatus_ = status.Mirrr;
            log_debug() << "HmiDefrosterRearStatus   : " << static_cast<int>(HmiDefrosterRearStatus_);
            log_debug() << "HmiDefrosterMirrorStatus : " << static_cast<int>(HmiDefrosterMirrorStatus_);
        }
    }
    showPopup();
}

void ElectricDefrosterRearPopupLogic::handleElectricDefrosterRear() {
    std::lock_guard<std::recursive_mutex> locker(mutex_);

    log_debug() << "handleElectricDefrosterRear() =  " << shareElectricDefrosterRear.get();

    showPopup();
    previousRearState_ = shareElectricDefrosterRear.get();
}

void ElectricDefrosterRearPopupLogic::showPopup() {
    if (CurrentState_ == InternalPopupState::OFF) {
        if ((HmiDefrosterRearStatus_ == ActrDefrstSts::NotAvailable &&
             HmiDefrosterMirrorStatus_ == ActrDefrstSts::NotAvailable) &&
            ((previousRearState_ == FirstRowGen::ElectricDefrosterRearState::ON &&
              shareElectricDefrosterRear.get() != FirstRowGen::ElectricDefrosterRearState::ON) ||
             (requestedState_ == FirstRowGen::ElectricDefrosterPopupState::ON))) {
            setState(FirstRowGen::ElectricDefrosterPopupState::ON);
        }
    } else if (CurrentState_ == InternalPopupState::ON) {
        if ((HmiDefrosterRearStatus_ != ActrDefrstSts::NotAvailable &&
             HmiDefrosterMirrorStatus_ != ActrDefrstSts::NotAvailable) ||
            (previousRearState_ != FirstRowGen::ElectricDefrosterRearState::OFF &&
             shareElectricDefrosterRear.get() == FirstRowGen::ElectricDefrosterRearState::OFF)) {
            setState(FirstRowGen::ElectricDefrosterPopupState::OFF);
        }
    }
}
