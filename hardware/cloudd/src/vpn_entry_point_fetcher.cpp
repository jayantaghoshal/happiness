#include "vpn_entry_point_fetcher.h"
#include <assert.h>
#include <array>
#include <cassert>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#define LOG_TAG "CloudD.VpnEpFetcher"
#include <cutils/log.h>

namespace Connectivity {

VPNEntryPointFetcher::VPNEntryPointFetcher(ICertHandler& certHandler, IDispatcher& dispatcher,
                                           ICloudRequest& cloudRequest, const bool allowRetry)
    : dispatcher_{dispatcher}, certHandler{certHandler}, cloudRequest{cloudRequest}, allowRetry{allowRetry} {}

void VPNEntryPointFetcher::Restart() {
    ALOGI(" + Restart()");
    using namespace std::chrono_literals;
    CloudResponseCallback doneCallback = [&](std::int32_t http_response_code, const std::string& data) {
        ALOGW("DONE CALLBACK");
        const std::array<std::chrono::milliseconds, 1> retryTimes = {{1000ms}};
        if (http_response_code < 200 || http_response_code > 299) {
            ALOGD("Failed to get VPN entry point, response code %d", http_response_code);
            retryIndex = std::min(retryIndex + 1, retryTimes.size() - 1);
            auto rt = retryTimes[retryIndex];
            std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(rt);
            retryTimerHandle = dispatcher_.EnqueueWithDelay((std::chrono::microseconds)t, [&]() { Restart(); });
        } else {
            ALOGD("Successfully received VPN entry point");
            VPNEntryPointParser::EntryPoint ep = VPNEntryPointParser::parse(data.c_str());
            if (whenResultAvailableCallback) whenResultAvailableCallback(ep);
        }
    };

    try {
        RequestConfig config;
        config.timeout = std::chrono::seconds{20};
        ALOGI("entry point url: %s", entryPointUrl.c_str());
        // std::string gpse = "http://www.gp.se";
        // currentlyPendingRequest = cloudRequest.HttpGet(gpse, config, std::move(doneCallback));
        currentlyPendingRequest = cloudRequest.HttpsGet(entryPointUrl, config, std::move(doneCallback), &certHandler);
    } catch (const std::exception& e) {
        ALOGE("Failed to start cloudrequest for vpn entry point: %s", e.what());
    }
    ALOGI("- Restart()");
}

void VPNEntryPointFetcher::Fetch(const std::string& entry_point_url) {
    ALOGI("+ Fetch()");
    Stop();
    entryPointUrl = entry_point_url;
    Restart();
    ALOGI("- Fetch()");
}

void VPNEntryPointFetcher::Stop() {
    currentlyPendingRequest = nullptr;
    dispatcher_.Cancel(retryTimerHandle);
}

void VPNEntryPointFetcher::WhenResultAvailable(std::function<void(const VPNEntryPointParser::EntryPoint&)>&& callback) {
    whenResultAvailableCallback = std::move(callback);
}
}
