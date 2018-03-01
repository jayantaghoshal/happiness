/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <curl/curl.h>

#include <memory>
#include <string>

#include "certificate_handler.h"
#include "cloud_request.h"
#include "cloud_request_handler_interface.h"

namespace Connectivity {

/*
 * Owner of the libcurl implementation and is responsible for configuring curl at start up and then each request that is
 * sent.
 */
class CloudRequestHandler : public CloudRequestHandlerInterface,
                            public std::enable_shared_from_this<CloudRequestHandler> {
  private:
  public:
    CloudRequestHandler();
    virtual ~CloudRequestHandler();

    int SendCloudRequest(std::shared_ptr<CloudRequest> request);

    CURL* GetMultiHandle() { return multi_; }
    int* GetTimerId() { return &timer_id_; }

  private:
    int timer_id_;
    CURL* multi_;

    // Static methods injected into libcurl.
    static int SocketCallback(CURL* easy, curl_socket_t fd, int operation, void* user_data, void* s);
    static int TimerCallback(CURLM* multi, long timeout_ms, void* user_data);
    static int Perform(CURL* multi, curl_socket_t fd);
    static int WriteCallback(char* ptr, const size_t size, const size_t nmemb, void* userdata);
    static int WriteHeaderCallback(char* ptr, const size_t size, const size_t nmemb, void* userdata);

    static int OnCreateOpenSslContext(CURL* curl, void* ssl_ctx, void* user_param);

    static int DebugCallback(CURL* handle, curl_infotype type, char* data, size_t size, void* userp);
};

}  // namespace Connectivity