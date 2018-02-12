/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

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
  public:
    enum HttpMethod { GET, POST, PUT };

  private:
    std::string url_;
    CURL* curl_handle_;
    std::shared_ptr<CertHandlerInterface> certificate_handler_;
    std::vector<std::string> header_list_;
    bool use_https_;
    std::chrono::milliseconds timeout_;
    std::string body_;
    HttpMethod method_;

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
          timeout_(36000000),
          body_(""),
          method_{GET} {}

    ~CloudRequest() { curl_easy_cleanup(curl_handle_); };

    bool SetCurlHandle(CURL* curl_handle);
    CURL* GetCurlHandle();

    void SetURL(std::string url);
    std::string GetURL();

    CertHandlerInterface* GetCertHandler();

    void SetHeaderList(std::vector<std::string> header_list);
    std::vector<std::string> GetHeaderList();

    bool GetUseHttps();

    void SetTimeout(std::chrono::milliseconds timeout);
    std::chrono::milliseconds GetTimeout();

    void SetRequestBody(std::string body);
    std::string GetRequestBody();

    void SetRequestMethod(HttpMethod method);
    HttpMethod GetRequestMethod();

    void SetCallback(ResponseCallback&& callback);
    ResponseCallback GetCallback();

    std::string* GetResponseDataBuffer();
    std::string* GetResponseHeaderBuffer();
};
}  // namespace Connectivity