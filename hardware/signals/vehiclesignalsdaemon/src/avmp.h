/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <stdint.h>
namespace avmp {
// Version and CRC constants used in version handshaking
const uint8_t avmpVersion = 0;

// Masks and IDs used in header interpretation/construction
const uint16_t signalGroupMask = 0x2000;
const uint16_t errorMask = 0x4000;
const uint16_t controlMsgMask = 0x8000;
const uint16_t signalIdMask = 0x07FF;
const uint16_t controlMsgIdMask = 0x7FFF;
const uint8_t controlMsgByteMask = 0x80;
const uint8_t sendAllMsgId = 0x01;
const uint8_t heartBeatMsgId = 0x02;
const uint8_t versionHandshakeMsgId = 0x03;
const uint8_t activateVfcMsgId = 0x04;

// Size constants used in header interpretation/construction
const uint8_t avmpHeaderSize = 2;
const uint8_t sendAllPayloadSize = 0;
const uint8_t heartBeatPayloadSize = 1;
const uint8_t versHandshakePayloadSize = 9;
const uint8_t activateVfcPayloadSize = 2;
const uint8_t errorPayloadSize = 1;
const uint8_t payloadOffset = 2;

}  // end of namespace
