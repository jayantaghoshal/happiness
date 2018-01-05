#pragma once

#include <curl/curl.h>

#include <string>

#include "certificate_handler.h"
#include "cloud_request.h"
#include "cloud_request_handler_interface.h"

namespace Connectivity {

class CloudRequestHandler : public CloudRequestHandlerInterface {
  private:
  public:
    CloudRequestHandler();
    virtual ~CloudRequestHandler();

    int SendCloudRequest(std::shared_ptr<CloudRequest> request);

  private:
    static int SocketCallback(CURL *easy, curl_socket_t fd, int operation, void *user_data, void *s);
    static int TimerCallback(CURLM *multi, long timeout_ms, void *user_data);
    static int Perform(curl_socket_t fd);
    static int WriteCallback(char *ptr, const size_t size, const size_t nmemb, void *userdata);

    static int OnCreateOpenSslContext(CURL *curl, void *ssl_ctx, void *user_param);

    static int DebugCallback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp);
};

}  // namespace Connectivity