/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <cstdint>
#include <functional>
#include <vector>

/*
 * This framework is only to be used by the VIP Signal Handler. It provides functions to encode & decode AVMP frames
 * as well as other information needed for the AVMP handshaking.
 * https://delphisweden.atlassian.net/wiki/display/VI/VIP-MP+signaling+design
 */

namespace autosar {
struct DataElemInfo;
}

namespace vipcomm {
using SignalID = uint16_t;
using OutgoingAvmpHandler = std::function<void(SignalID signalId, void* signalData, size_t length)>;

// Get the crc16 for the input files:
// COM.arxml, SWC.arxml, Rte_Type.h, Com_Cfg.h
void getFileCrc(uint16_t& com_arxml, uint16_t& swc_arxml, uint16_t& rte_type, uint16_t& com_cfg);

// Call this to initalize all inject instances (DEInject). Don't call it until you are ready to receive
// avmp signal messages from the VIP.
// Shall only be called once per process!
bool initializeInject();

// Call this to initialize all sink instances (DESink). Don't call it before you are ready to receive
// outsignals from the applications and before you are able to send signals to the VIP.
// The argument is a function that will be called once for each out signals to send to the VIP.
// Shall only be called once per process!
bool initializeSink(const OutgoingAvmpHandler& outgoingAvmpHandler);

// Pre-req: initializeInject()
// Call this to inject an incoming signal in state OK
void injectIncomingOkSignal(SignalID signalId, void* signalData, size_t length);

// Pre-req: initializeInject()
// Call this to inject an incoming signal in state ERROR
void injectIncomingErrorSignal(SignalID signalId, uint8_t errorCode);

// get a vector that contains all available dataelements (signals)
const std::vector<autosar::DataElemInfo*>& getAllDataelements();
}
