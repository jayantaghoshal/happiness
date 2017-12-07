#pragma once

#include "cloud_request.h"

namespace Connectivity {

class CloudRequestHandlerInterface {
  public:
    virtual int SendCloudRequest(std::shared_ptr<CloudRequest> request) = 0;
};

}  // namespace Connectivity