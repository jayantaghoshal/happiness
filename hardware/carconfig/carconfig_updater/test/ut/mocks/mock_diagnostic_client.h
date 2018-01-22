/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*
 * mock_diagnostic_client.h
 *
 *  Created on: Aug 26, 2016
 *      Author: bastian
 */

#pragma once

#include <diagnostics_client.h>
#include <gmock/gmock.h>
#include <map>
#include <vector>

typedef std::vector<uint8_t> valueVec_t;
typedef std::map<uint16_t, valueVec_t> valueMap_t;

//
// overwrite the functions provided by diagnosticsClient
//
class MockDiagnosticClient : public diagnosticsClient {
  public:
    MOCK_METHOD0(connect, void());
    MOCK_METHOD2(sendDiagnosticsMessage, void(uint16_t id, valueVec_t message));
    MOCK_METHOD2(registerDID, void(uint16_t id, valueVec_t values));
    MOCK_METHOD2(updateDID, void(uint16_t id, valueVec_t values));
    MOCK_METHOD2(printMessage, void(uint16_t id, valueVec_t values));
    MOCK_METHOD0(runForever, void());
};

void diag_mock_init(MockDiagnosticClient* mockInstance);
