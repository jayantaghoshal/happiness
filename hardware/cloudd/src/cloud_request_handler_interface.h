/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "cloud_request.h"

namespace Connectivity {

class CloudRequestHandlerInterface {
  public:
    /*
     * Creates, configures and starts a curl handle with the parameters given in CloudRequest and other settings
     * needed for the cloud connection.
     */
    virtual int SendCloudRequest(std::shared_ptr<CloudRequest> request) = 0;
};

}  // namespace Connectivity