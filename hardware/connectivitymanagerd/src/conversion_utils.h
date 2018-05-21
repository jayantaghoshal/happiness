/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <vendor/volvocars/hardware/connectivitymanager/1.0/types.h>
extern "C" {
#include "infotainmentIpBus.h"
}

namespace vcc {
namespace conman {
namespace utils {

namespace conman_hal_v1_0 = ::vendor::volvocars::hardware::connectivitymanager::V1_0;

conman_hal_v1_0::WifiStationMode toHidl(const Icb_WLANMode& ipcb_mode) {
    switch (ipcb_mode) {
        case e_Icb_WLANMode_off: {
            return conman_hal_v1_0::WifiStationMode::OFF;
        }
        case e_Icb_WLANMode_sta: {
            return conman_hal_v1_0::WifiStationMode::STATION;
        }
        case e_Icb_WLANMode_ap: {
            return conman_hal_v1_0::WifiStationMode::AP;
        }
        case e_Icb_WLANMode_staWS: {
            return conman_hal_v1_0::WifiStationMode::WORKSHOP;
        }
        default: { throw std::runtime_error("Invalid IPCB value for WLAN Mode"); }
    }
}

Icb_WLANMode toIpcb(const conman_hal_v1_0::WifiStationMode& hidl_mode) {
    switch (hidl_mode) {
        case conman_hal_v1_0::WifiStationMode::OFF: {
            return e_Icb_WLANMode_off;
        }
        case conman_hal_v1_0::WifiStationMode::STATION: {
            return e_Icb_WLANMode_sta;
        }
        case conman_hal_v1_0::WifiStationMode::AP: {
            return e_Icb_WLANMode_ap;
        }
        case conman_hal_v1_0::WifiStationMode::WORKSHOP: {
            return e_Icb_WLANMode_staWS;
        }
        default: { throw std::runtime_error("Invalid IPCB value for WLAN Mode"); }
    }
}

}  // namespace utils
}  // namespace conman
}  // namespace vcc