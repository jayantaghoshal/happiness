#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_VPNENTRYPOINTFETCHER_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_VPNENTRYPOINTFETCHER_H

#include <curl/curl.h>

#include <functional>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include "IDispatcher.h"
#include "certificate_handler_interface.h"
#include "cloud_request_interface.h"
#include "vpn_entry_point_parser.h"

namespace Connectivity {

using ::tarmac::eventloop::IDispatcher;

class VPNEntryPointFetcher final {
  public:
    VPNEntryPointFetcher(ICertHandler& certHandler, IDispatcher& dispatcher, ICloudRequest& cloudRequest,
                         const bool allowRetry = true);

    void Fetch(const std::string& entry_point_url);

    void Stop();

    void WhenResultAvailable(std::function<void(const VPNEntryPointParser::EntryPoint&)>&& callback);

    static VPNEntryPointParser::EntryPoint parse(const char* const data);

  private:
    void Restart();
    void worker();

    IDispatcher& dispatcher_;
    ICertHandler& certHandler;
    ICloudRequest& cloudRequest;
    std::string entryPointUrl;
    const bool allowRetry;
    size_t retryIndex = 0;
    IDispatcher::JobId retryTimerHandle;
    ICloudRequest::RequestHandle currentlyPendingRequest;

    std::function<void(const VPNEntryPointParser::EntryPoint&)> whenResultAvailableCallback;
};

}  // namespace Connectivity
#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_VPNENTRYPOINTFETCHER_H