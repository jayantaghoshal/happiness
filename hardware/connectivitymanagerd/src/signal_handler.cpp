/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "signal_handler.h"

#undef LOG_TAG
#define LOG_TAG "ConMan.SignalHandler"
#include <cutils/log.h>

namespace vcc {
namespace conman {

SignalHandler::SignalHandler(std::weak_ptr<IConnectivityManager> connectivity_manager) {
    if (auto spt = connectivity_manager.lock()) {
        std::swap(connectivity_manager_, connectivity_manager);
    } else {
        throw std::runtime_error("Connectivity Manager has expired");
    }
}

bool SignalHandler::RequestWifiStationMode() {
    ALOGV("%s", __FUNCTION__);
    ALOGW("Not Implemented");
    return false;
}

bool SignalHandler::RequestSetWifiStationMode(/* WifiStationMode mode */) {
    ALOGV("%s", __FUNCTION__);
    ALOGW("Not Implemented");
    return false;
}

}  // namespace conman
}  // namespace vcc