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
        : url_("www.example.com"),
          curl_handle_(nullptr),
          certificate_handler_(cert_handler),
          header_list_({}),
          use_https_(true),
          timeout_(36000000) {}

    ~CloudRequest() = default;

    bool SetCurlHandle(CURL* curl_handle);
    CURL* GetCurlHandle() { return curl_handle_; }

    void SetURL(std::string url) { url_ = url; }
    std::string GetURL() { return url_; }

    CertHandlerInterface* GetCertHandler() { return certificate_handler_.get(); }

    void SetHeaderList(std::vector<std::string> header_list) { header_list_ = header_list; }
    std::vector<std::string> GetHeaderList() { return header_list_; }

    void SetUseHttps(bool use_https) { use_https_ = use_https; }
    bool GetUseHttps() { return use_https_; }

    void SetTimeout(std::chrono::milliseconds timeout) { timeout_ = timeout; }
    std::chrono::milliseconds GetTimeout() { return timeout_; }

    void SetCallback(ResponseCallback&& callback) { response_callback_ = callback; }
    ResponseCallback GetCallback() { return response_callback_; }

    std::string* GetResponseDataBuffer() { return &response_data_; }
    std::string* GetResponseHeaderBuffer() { return &response_header_; }
};
}  // namespace Connectivity