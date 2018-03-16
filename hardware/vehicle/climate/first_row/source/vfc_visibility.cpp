/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vfc_visibility.h"

#include "logging_context.h"
#include "vfc_helper.h"

LOG_SET_DEFAULT_CONTEXT(FirstRowContext)

VFCVisibilityLogic::VFCVisibilityLogic() {
    bt4PanFrontReq_.subscribe([this]() {

        if (VehModMngtGlbSafe1_.get().isOk() &&
            (VehModMngtGlbSafe1_.get().value().UsgModSts == autosar::UsgModSts1::UsgModDrvg)) {
            if (bt4PanFrontReq_.get().isOk() && (bt4PanFrontReq_.get().value() == autosar::PsdNotPsd::Psd)) {
                log_debug() << "request_vfc(Vfc::Visibility)";
                ApplicationDataElement::Helper::request_vfc(Vfc::Visibility);
            }
        }
    });

    bt5PanFrontReq_.subscribe([this]() {
        if (VehModMngtGlbSafe1_.get().isOk() &&
            (VehModMngtGlbSafe1_.get().value().UsgModSts == autosar::UsgModSts1::UsgModDrvg)) {
            if (bt5PanFrontReq_.get().isOk() && (bt5PanFrontReq_.get().value() == autosar::PsdNotPsd::Psd)) {
                log_debug() << "request_vfc(Vfc::Visibility)";
                ApplicationDataElement::Helper::request_vfc(Vfc::Visibility);
            }
        }
    });
}
