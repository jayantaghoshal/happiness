/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "diagnostics_client.h"
#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "IpcbD.diagnostics"

namespace Connectivity {

void DiagnosticsClient::SetInvalidData(const Message::Ecu ecu, const bool error) {
    switch (ecu) {
        case Message::Ecu::TEM:
            ALOGI("TEM diag %d", error);
            break;

        case Message::Ecu::VCM:
            ALOGI("VCM diag %d", error);
            break;

        default:
            ALOGW("Diagnostics interface currently not supporting invalid data from '%s'", Message::EcuStr(ecu));
            break;
    }
}

}  // namespace
