/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "connectivity_manager.h"

#include "signal_handler_interface.h"

#undef LOG_TAG
#define LOG_TAG "ConMan.ConMan"
#include <cutils/log.h>

namespace vcc {
namespace conman {

ConnectivityManager::ConnectivityManager() : connectivity_manager_service_(nullptr, nullptr) {
    ALOGD("%s", __FUNCTION__);
}

void ConnectivityManager::Initialize(
        std::weak_ptr<ISignalHandler> signal_handler /*, std::weak_ptr<INetmanClient> netman_client*/) {
    ALOGD("%s", __FUNCTION__);

    if (auto spt = signal_handler.lock()) {
        std::swap(signal_handler_, signal_handler);
    } else {
        throw std::runtime_error("Signal Handler was expired");
    }

    connectivity_manager_service_ = UniqueConnectivityManagerService(new ConnectivityManagerService(shared_from_this()),
                                                                     [](ConnectivityManagerService* p) {
                                                                         p->StopService();
                                                                         delete p;
                                                                     });

    connectivity_manager_service_->StartService();
}

bool ConnectivityManager::RequestWifiStationMode() {
    ALOGV("%s", __FUNCTION__);
    if (auto spt = signal_handler_.lock()) {
        return spt->RequestWifiStationMode();
    }

    throw std::runtime_error("Signal Handler has expired");
}

bool ConnectivityManager::RequestWifiStationModeChange(conman_hal_v1_0::WifiStationMode mode) {
    ALOGV("%s", __FUNCTION__);

    (void)mode;

    // Convert Hidl WifiStationMode to SignalHandler WifiStationMode ...
    if (auto spt = signal_handler_.lock()) {
        return spt->RequestSetWifiStationMode(/*signal_handler_mode*/);
    }

    throw std::runtime_error("Signal Handler has expired");
}

}  // namespace conman
}  // namespace vcc