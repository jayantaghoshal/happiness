#pragma once

#include <curl/curl.h>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "certificate_handler_interface.h"

namespace Connectivity {
typedef std::function<void(std::int32_t, const std::string&, const std::string&)> ResponseCallback;

/*
 * Container class for a single request. Used for passing around along the life cycle of a request in libcurl
 * implementation
 */
class CloudRequest {
  private:
    std::string url_;
    CURL* curl_handle_;
    std::shared_ptr<CertHandlerInterface> certificate_handler_;
    std::vector<std::string> header_list_;
    bool use_https_;
    std::chrono::milliseconds timeout_;

    ResponseCallback response_callback_;
    std::string response_data_;
    std::string response_header_;

  public:
    CloudRequest(std::shared_ptr<CertHandlerInterface> cert_handler = nullptr)
        : url_(""),
          curl_handle_(nullptr),
          certificate_handler_(cert_handler),
          header_list_({}),
          use_https_(true),
          timeout_(36000000) {}

    ~CloudRequest() = default;

    bool SetCurlHandle(CURL* curl_handle);
    CURL* GetCurlHandle();

    void SetURL(std::string url);
    std::string GetURL();

    CertHandlerInterface* GetCertHandler();

    void SetHeaderList(std::vector<std::string> header_list);
    std::vector<std::string> GetHeaderList();

    void SetUseHttps(bool use_https);
    bool GetUseHttps();

    void SetTimeout(std::chrono::milliseconds timeout);
    std::chrono::milliseconds GetTimeout();

    void SetCallback(ResponseCallback&& callback);
    ResponseCallback GetCallback();

    std::string* GetResponseDataBuffer();
    std::string* GetResponseHeaderBuffer();
};
}  // namespace Connectivity