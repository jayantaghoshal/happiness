/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef IDIAGNOSTICS_CLIENT_H
#define IDIAGNOSTICS_CLIENT_H

#include "ipcommandbus/Message.h"

namespace Connectivity {

/**
 * Interface for reporting to diagnostics service.
 */
class IDiagnosticsClient {
  public:
    virtual ~IDiagnosticsClient() = default;
    /**
     *   Set 'invalid data from <ecu>' diagnostic condition.
     *   Must be be implemented in a stateful manner, only reporting to diagnostics server ONCE
     *   for ok and error status respectively.
     *   Also ok status may only be reported if error has not been reported already!
     */
    virtual void SetInvalidData(const Message::Ecu ecu, const bool error) = 0;
};

}  // namespace

#endif  // IDIAGNOSTICS_CLIENT_H
