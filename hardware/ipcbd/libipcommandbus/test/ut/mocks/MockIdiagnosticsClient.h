/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "ipcommandbus/idiagnostics_client.h"

using namespace Connectivity;

class MockIDiagnosticsClient : public IDiagnosticsClient {
  public:
    MOCK_METHOD2(SetInvalidData, void(const Message::Ecu ecu, const bool error));
};
