/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

// TODO !!!!! currently not in the build

#include "ipcommandbus/shutdown_client.h"

extern "C" {
#include <NodeStateTypes.h>
}

namespace Connectivity {

int ShutdownClient::onLifeCycleRequest(unsigned int request, unsigned int requestId) {
    (void)requestId;

    if (request == NSM_SHUTDOWNTYPE_NORMAL || request == NSM_SHUTDOWNTYPE_FAST) {
        socket_.endConnection();
    }

    return NsmErrorStatus_Ok;
}

}  // namespace Connectivity;
