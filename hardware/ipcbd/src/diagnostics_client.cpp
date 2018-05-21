/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "diagnostics_client.h"
#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "IpcbD"

namespace Connectivity {

void DiagnosticsClient::SetInvalidData(const Message::Ecu ecu, const bool error) {
    switch (ecu) {
        case Message::Ecu::TCAM:
            ALOGI("[DiagnosticsClient] TCAM diag %d", error);
            break;

        case Message::Ecu::VGM:
            ALOGI("[DiagnosticsClient] VGM diag %d", error);
            break;

        default:
            ALOGW("[DiagnosticsClient] Diagnostics interface currently not supporting invalid data from '%s'",
                  Message::EcuStr(ecu));
            break;
    }
}

}  // namespace
