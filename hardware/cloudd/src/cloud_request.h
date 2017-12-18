#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUD_REQUEST_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUD_REQUEST_H

#include <curl/curl.h>

#include <cutils/log.h>
#include <functional>
#include <map>
#include <tuple>
#include "IDispatcher.h"
#include "certificate_handler_interface.h"
#include "cloud_request_interface.h"

namespace Connectivity {

using ::tarmac::eventloop::IDispatcher;

typedef curl_socket_t FdActionKey;

struct CurlEasyHandleCleaner {
    void operator()(CURL* easy) { curl_easy_cleanup(easy); }
};
struct CurlMultiHandleCleaner {
    void operator()(CURLM* multi) { (void)curl_multi_cleanup(multi); }
};

class CurlMultiEasyHandle final {
  public:
    CurlMultiEasyHandle(CURLM* multi, CURL* easy)
        : multi{multi}, easy{easy}, retCode{curl_multi_add_handle(multi, easy)} {
        if (retCode != CURLMcode::CURLM_OK) {
            throw std::runtime_error("Failed to add easy handle to multi handle");
        }
    }
    ~CurlMultiEasyHandle() {
        if (retCode == CURLMcode::CURLM_OK) {
            const auto removeStatus = curl_multi_remove_handle(multi, easy);
            if (removeStatus != CURLMcode::CURLM_OK) {
                ALOG(LOG_ERROR, "CloudD.CloudRequest", "curl_multi_remove_handle unexpected error: %d", removeStatus);
            }
        }
    }
    CurlMultiEasyHandle(const CurlMultiEasyHandle&) = delete;
    CurlMultiEasyHandle(CurlMultiEasyHandle&&) = delete;
    CurlMultiEasyHandle& operator=(const CurlMultiEasyHandle&) = delete;
    CurlMultiEasyHandle& operator=(CurlMultiEasyHandle&&) = delete;

  private:
    CURLM* const multi;
    CURL* const easy;
    const CURLMcode retCode;
};

using CurlEasyUnique = std::unique_ptr<CURL, CurlEasyHandleCleaner>;

class CloudRequest final : public virtual ICloudRequest {
  public:
    CloudRequest();

    bool Init() override;

    RequestHandle HttpGet(const std::string& uri, const RequestConfig& config,
                          CloudResponseCallback&& response_callback) override;
    RequestHandle HttpsGet(const std::string& uri, const RequestConfig& config,
                           CloudResponseCallback&& response_callback, ICertHandler* certHandler) override;

    struct MultiState {
        std::unique_ptr<CURLM, CurlMultiHandleCleaner> curlm_ = nullptr;
        IDispatcher::JobId timer_job_id;
    };

    struct EasyState {
        CurlEasyUnique curle_;
        std::unique_ptr<CurlMultiEasyHandle> multi_easy_connection;
        bool finishedPrematurely = false;
        std::string data_;
        std::string header_;
        CloudResponseCallback response_callback_;
        // TODO: Figure out a better way to define ownership of certHandler, should NOT be raw pointer
        ICertHandler* certHandler = nullptr;
    };

  private:
    MultiState multi_state_;
    RequestHandle GenericGet(const std::string& uri, CloudResponseCallback&& response_callback,
                             std::function<void(CURL* curl, EasyState& easyState)> setOptFunc,
                             ICertHandler* certHandler, const RequestConfig& config);
};
}

#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUD_REQUEST_H
