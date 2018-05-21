/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

// TODO !!!!! currently not in the build

#include "cedric/cedric_nodestate.h"
#include "isocket.h"

namespace Connectivity {
class ShutdownClient : public cedric::core::IShutdownClient {
  public:
    ShutdownClient(ISocket& socket) : socket_(socket){};

    int onLifeCycleRequest(unsigned int request, unsigned int requestId) override;

  private:
    ISocket& socket_;
};

}  // namespace Connectivity
