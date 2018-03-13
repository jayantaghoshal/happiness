/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

namespace vcc {
namespace remotectrl {

const uint8_t VCC_SOMEIP_PROTOCOL_VERSION = 0x01U;
const uint8_t VCC_SOMEIP_INTERFACE_VERSION = 0x01U;
const uint8_t VCC_CLIENT_ID = 0x00U;

// Currently using dummy values here
// RemoteCtrl_AudioCtrl Service
const uint16_t REMOTECTRL_AUDIOCTRL_SERVICE_ID = 0x1100U;
const uint16_t REMOTECTRL_AUDIOCTRL_SERVICE_INSTANCE_ID = 0x1101U;
const uint16_t REMOTECTRL_AUDIOCTRL_EVENTGROUP_ID = 0x1150U;
const uint16_t REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME = 0x1133U;
const uint16_t REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME = 0x1144U;
const uint16_t REMOTECTRL_AUDIOCTRL_EVENT_ID_VOLUMESTATUS = 0x1122U;

// RemoteCtrl_ClimateCtrl

}  // namespace remotectrl
}  // namespace vcc
