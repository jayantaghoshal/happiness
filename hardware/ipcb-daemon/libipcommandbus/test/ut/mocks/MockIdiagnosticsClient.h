/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_IDIAGNOSTICS_CLIENT_H
#define _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_IDIAGNOSTICS_CLIENT_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "ipcommandbus/idiagnostics_client.h"

using namespace Connectivity;

class MockIDiagnosticsClient : public IDiagnosticsClient
{
public:
    MOCK_METHOD2(SetInvalidData, void(const Message::Ecu ecu, const bool error));
};

#endif  // _CONNECTIVITY_LIBIPCOMMANDBUS_MOCK_IDIAGNOSTICS_CLIENT_H
