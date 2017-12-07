#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDREQUESTINTERFACE_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDREQUESTINTERFACE_H

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "certificate_handler_interface.h"

namespace Connectivity {
typedef std::function<void(std::int32_t, const std::string&)> CloudResponseCallback;

struct RequestConfig {
    bool use_stunnel_proxy = false;
    std::chrono::milliseconds timeout = std::chrono::milliseconds{36000000};
    std::vector<std::string> header_list = {};
};

class ICloudRequest {
  public:
    class IRequestHandleData {
      public:
        virtual ~IRequestHandleData() = default;
    };

    // RequestHandle must be kept alive throughout the operation, when destroyed the operation is automatically
    // canceled.
    using RequestHandle = std::unique_ptr<IRequestHandleData>;

    virtual ~ICloudRequest() = default;

    virtual bool Init() = 0;
    virtual RequestHandle HttpGet(const std::string& uri, const RequestConfig& config,
                                  CloudResponseCallback&& response_callback) = 0;
    virtual RequestHandle HttpsGet(const std::string& uri, const RequestConfig& config,
                                   CloudResponseCallback&& response_callback, ICertHandler* certHandler) = 0;
};
}
#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDREQUESTINTERFACE_H