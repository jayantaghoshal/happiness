/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <ipcommandbus/idiagnostics_client.h>

namespace vcc {
namespace ipcb {
namespace testing {

class DiagnosticsClient : public Connectivity::IDiagnosticsClient {
  public:
    DiagnosticsClient() {}
    void SetInvalidData(const Connectivity::Message::Ecu ecu, const bool error) override;
};

}  // namespace testing
}  // namespace ipcb
}  // namespace vcc
