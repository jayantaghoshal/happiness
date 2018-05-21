/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "diagnostics_client.h"
#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "IpcbSim"

namespace vcc {
namespace ipcb {
namespace testing {

void DiagnosticsClient::SetInvalidData(const Connectivity::Message::Ecu ecu, const bool error) {
    (void)ecu;
    (void)error;
    ALOGW("%s - Ignoring...", __func__);
}

}  // namespace testing
}  // namespace ipcb
}  // namespace vcc