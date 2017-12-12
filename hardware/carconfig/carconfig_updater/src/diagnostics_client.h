/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef DIAGNOSTICSCLIENT_H_
#define DIAGNOSTICSCLIENT_H_

#include <CommonAPI/CommonAPI.hpp>
#include <cstdint>
#include <v1/com/ihu/Diagnostics/DiagnosticsProxy.hpp>
#include <vector>
#include "diag_message_keys.h"

class diagnosticsClient {
  public:
    diagnosticsClient();
    void connect();
    void sendDiagnosticsMessage(uint16_t id, std::vector<uint8_t> message);
    void registerDID(uint16_t id, std::vector<uint8_t> values);
    void updateDID(uint16_t id, std::vector<uint8_t> values);
    std::map<uint16_t, std::vector<uint8_t>> testGetDIDList();
    void runForever();

  private:
    void capiCallback(uint16_t key, std::vector<uint8_t> payload);
    void printMessage(uint16_t id, std::vector<uint8_t> message);
    std::map<uint16_t, std::vector<uint8_t>> DIDList;
    std::shared_ptr<CommonAPI::Runtime> runtime;
    std::shared_ptr<v1::com::ihu::Diagnostics::DiagnosticsProxy<>> diagnosticsProxy;
};

enum class MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS_BYTE_0 : uint8_t {
    not_configured_no_error = 0b00000000,
    not_configured_error = 0b00000001,
    not_configured_not_tested = 0b00000010,
    invalid_unconfigured_no_error = 0b00000000,
    invalid_unconfigured_error = 0b00000100,
    invalid_unconfigured_not_tested = 0b00001000
};

#endif  // DIAGNOSTICSCLIENT_H_
