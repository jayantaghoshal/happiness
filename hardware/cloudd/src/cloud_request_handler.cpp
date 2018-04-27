/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cloud_request_handler.h"

#include <boost/filesystem.hpp>
#include <fstream>

#include <IDispatcher.h>

#define LOG_TAG "CloudD"
#include <cutils/log.h>

using namespace tarmac::eventloop;

namespace Connectivity {

// Helper methods for verifying the success of setting options in curl. One for easy handles and one for multi handles.
template <typename T>
void verified_curl_multi_setopt(CURLM* curl, const CURLMoption opt, T data) {
    const CURLMcode res = curl_multi_setopt(curl, opt, data);
    if (res != CURLMcode::CURLM_OK) {
        throw std::runtime_error("Failed to set curl multi option: " + std::to_string(opt) + ", error: " +
                                 std::to_string(res) + " " + curl_multi_strerror(res));
    }
}

template <typename T>
void verified_curl_easy_setopt(CURL* curl, const CURLoption opt, T data) {
    const CURLcode res = curl_easy_setopt(curl, opt, data);
    if (res != CURLcode::CURLE_OK) {
        throw std::runtime_error("Failed to set curl option: " + std::to_string(opt) + ", error: " +
                                 std::to_string(res) + " " + curl_easy_strerror(res));
    }
}

CloudRequestHandler::CloudRequestHandler() : timer_id_{-1}, multi_{nullptr} {
    curl_global_init(CURL_GLOBAL_ALL);

    curl_version_info_data* data = curl_version_info(CURLVERSION_NOW);

    // TODO: Check version??
    ALOGD("[RequestHandler] Version: %s\n", data->version);

    multi_ = curl_multi_init();

    verified_curl_multi_setopt(multi_, CURLMOPT_SOCKETDATA, this);
    verified_curl_multi_setopt(multi_, CURLMOPT_SOCKETFUNCTION, SocketCallback);
    verified_curl_multi_setopt(multi_, CURLMOPT_TIMERDATA, this);
    verified_curl_multi_setopt(multi_, CURLMOPT_TIMERFUNCTION, TimerCallback);
}

CloudRequestHandler::~CloudRequestHandler() {
    curl_global_cleanup();
    multi_ = nullptr;
}

int CloudRequestHandler::SocketCallback(CURL* easy, curl_socket_t fd, int operation, void* user_data, void* s) {
    ALOGV("[RequestHandler] sock_cb: socket=%d, what=%d, sockp=%p\n", fd, operation, s);

    CloudRequestHandler* request_handler = static_cast<CloudRequestHandler*>(user_data);

    const char* whatstr[] = {"none", "IN", "OUT", "INOUT", "REMOVE"};

    int* actionp = (int*)s;
    if (operation == CURL_POLL_REMOVE) {
        IDispatcher::GetDefaultDispatcher().RemoveFd(fd);
    } else {
        if (!s) {
            ALOGV("[RequestHandler] Adding data: %s\n", whatstr[operation]);
            IDispatcher::GetDefaultDispatcher().AddFd(
                    fd,
                    [request_handler, fd]() { return Perform(request_handler->GetMultiHandle(), fd); },
                    EPOLLIN | EPOLLOUT);
        }
    }
    return CURLcode::CURLE_OK;
}

int CloudRequestHandler::TimerCallback(CURLM* multi, long timeout_ms, void* user_data) {
    ALOGV("[RequestHandler] multi_timer_cb: timeout_ms %ld\n", timeout_ms);

    int code = CURLcode::CURLE_OK;

    CloudRequestHandler* request_handler = static_cast<CloudRequestHandler*>(user_data);
    int* timer_id = request_handler->GetTimerId();

    /* cancel running timer */
    IDispatcher::GetDefaultDispatcher().Cancel(*timer_id);

    if (timeout_ms > 0) {
        /* update timer */
        *timer_id = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay(
                std::chrono::milliseconds(timeout_ms), [multi]() { Perform(multi, CURL_SOCKET_TIMEOUT); });
        ALOGV("[RequestHandler] Timer Set With Timer ID: %d\n", *timer_id);
    } else if (timeout_ms == 0) {
        ALOGV("[RequestHandler] Timer Off immediately\n");
        code = Perform(request_handler->GetMultiHandle(), CURL_SOCKET_TIMEOUT);
    } else {
        ALOGV("[RequestHandler] Timer aborted");
    }

    return code;
}

int CloudRequestHandler::Perform(CURL* multi, curl_socket_t fd) {
    int running_handles;
    curl_multi_socket_action(multi, fd, 0, &running_handles);

    int msgs_queue_out = 0;

    do {
        CURLMsg* curl_message = curl_multi_info_read(multi, &msgs_queue_out);

        if (curl_message && curl_message->msg == CURLMSG_DONE) {
            ALOGV("[RequestHandler] Done %i\n", msgs_queue_out);

            std::int32_t response_code = -1;
            int ret1 = curl_easy_getinfo(curl_message->easy_handle, CURLINFO_RESPONSE_CODE, &response_code);

            CloudRequest* cr;
            int ret2 = curl_easy_getinfo(curl_message->easy_handle, CURLINFO_PRIVATE, &cr);

            const CURLcode resultCode = curl_message->data.result;

            if (ret1 != CURLcode::CURLE_OK) {
                ALOGW("[RequestHandler] Failed to get CURL response1: %d\n", ret1);
            } else if (resultCode == CURLcode::CURLE_OPERATION_TIMEDOUT) {
                ALOGV("[RequestHandler] Request timeout\n");
                cr->GetCallback()(599, "", "");
            } else if (resultCode != CURLcode::CURLE_OK) {
                ALOGW("[RequestHandler] Request failed with error: %d, %s\n",
                      resultCode,
                      curl_easy_strerror(resultCode));
                cr->GetCallback()(-1, "", "");
            } else {
                cr->GetCallback()(response_code, *cr->GetResponseDataBuffer(), *cr->GetResponseHeaderBuffer());
            }
        }

    } while (msgs_queue_out > 0);

    return CURLcode::CURLE_OK;
}

int CloudRequestHandler::WriteCallback(char* ptr, const size_t size, const size_t nmemb, void* userdata) {
    bool success = false;
    const size_t real_size = size * nmemb;
    CloudRequest* cr = static_cast<CloudRequest*>(userdata);

    if (!cr->GetFilePath().empty()) {
        // Create directory if it doesn't exist already
        boost::filesystem::path file_name = cr->GetFilePath().c_str();
        boost::filesystem::path folder_name = file_name.parent_path();
        ALOGD("[RequestHandler] Check if path exists: %s", folder_name.string().c_str());
        if (!boost::filesystem::exists(folder_name)) {
            ALOGD("[RequestHandler] Directory does not exist, create it");
            boost::filesystem::create_directories(folder_name);
        }

        std::ofstream out_file;
        out_file.open(cr->GetFilePath(), std::ios_base::app);
        if (out_file.is_open()) {
            out_file.write(ptr, real_size);
            success = true;
        } else {
            ALOGW("[RequestHandler] Failed to write to file: %s", cr->GetFilePath().c_str());
        }
        out_file.close();
    } else {
        std::string* write_buffer = cr->GetResponseDataBuffer();

        try {
            write_buffer->append(ptr, real_size);
            success = true;
        } catch (...) {
            ALOGW("[RequestHandler] Failed to write to request buffer");
        }
    }

    if (success) {
        return real_size;
    } else {
        return 0;
    }
}

int CloudRequestHandler::WriteHeaderCallback(char* ptr, const size_t size, const size_t nmemb, void* userdata) {
    const size_t real_size = size * nmemb;

    std::string* write_buffer = static_cast<std::string*>(userdata);

    try {
        write_buffer->append(ptr, real_size);
        return real_size;
    } catch (...) {
        ALOGW("[RequestHandler] Failed to write to request buffer");
    }
    return 0;
}

int CloudRequestHandler::DebugCallback(CURL* handle, curl_infotype type, char* data, size_t size, void* userp) {
    const char* text;
    (void)handle;
    (void)userp;

    switch (type) {
        case CURLINFO_TEXT:
            ALOGD("[RequestHandler] == Info: %s", data);
        default: /* in case a new one is introduced to shock us */
            return 0;

        case CURLINFO_HEADER_OUT:
            text = "Send header =>";
            break;
        case CURLINFO_DATA_OUT:
            text = "Send data =>";
            break;
        case CURLINFO_SSL_DATA_OUT:
            text = "Send SSL data =>";
            break;
        case CURLINFO_HEADER_IN:
            text = "Recv header <=";
            break;
        case CURLINFO_DATA_IN:
            text = "Recv data <=";
            break;
        case CURLINFO_SSL_DATA_IN:
            text = "Recv SSL data <=";
            break;
    }

    ALOGD("[RequestHandler] %s %s", text, data);

    return 0;
}

int CloudRequestHandler::OnCreateOpenSslContext(CURL* curl, void* ssl_ctx, void* user_param) {
    CloudRequest* cr = static_cast<CloudRequest*>(user_param);

    CertificateValidationStatus certStatus = cr->GetCertHandler()->OnCreateOpenSslContext(ssl_ctx);

    if (certStatus == CertificateValidationStatus::Validated) {
        return CURLE_OK;
    } else {
        return CURLE_SSL_CERTPROBLEM;
    }
}

int CloudRequestHandler::SendCloudRequest(std::shared_ptr<CloudRequest> request) {
    CURL* easy = curl_easy_init();
    if (!request->SetCurlHandle(easy)) {
        throw std::runtime_error("Failed to initialise an easy handle.");
    }

    // Associate CloudRequest with curl handle
    verified_curl_easy_setopt(easy, CURLOPT_PRIVATE, request.get());
    // Follow redirections
    verified_curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1L);
    // Force HTTP method
    if (request->GetRequestMethod() == CloudRequest::HttpMethod::PUT) {
        verified_curl_easy_setopt(easy, CURLOPT_PUT, 1L);
        // TODO: Missing implementation. Add Data for PUT-request
        throw std::runtime_error("PUT request not implemented");
    } else if (request->GetRequestMethod() == CloudRequest::HttpMethod::POST) {
        ALOGV("[RequestHandler] SendCloudRequest: [Method: POST, body %s]", request->GetRequestBody().c_str());
        verified_curl_easy_setopt(easy, CURLOPT_HTTPPOST, 1L);
        verified_curl_easy_setopt(easy, CURLOPT_COPYPOSTFIELDS, request->GetRequestBody().c_str());
    } else {
        ALOGV("[RequestHandler] SendCloudRequest: [Method: GET]");
        verified_curl_easy_setopt(easy, CURLOPT_HTTPGET, 1L);
    }
    //
    // Set URL
    verified_curl_easy_setopt(easy, CURLOPT_URL, request->GetURL().c_str());
    // Set Timeout
    verified_curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, request->GetTimeout().count());

    if (!request->GetHeaderList().empty()) {
        struct curl_slist* chunk = NULL;
        for (auto p : request->GetHeaderList()) {
            ALOGV("[RequestHandler] SendCloudRequest: [Header: %s]", p.c_str());
            chunk = curl_slist_append(chunk, p.c_str());
        }
        verified_curl_easy_setopt(easy, CURLOPT_HTTPHEADER, chunk);
    }

    if (request->GetUseHttps()) {
        // Force HTTPS as protocol
        verified_curl_easy_setopt(easy, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);
        // SSL Context setup function and data to send to it
        verified_curl_easy_setopt(easy, CURLOPT_SSL_CTX_FUNCTION, OnCreateOpenSslContext);
        verified_curl_easy_setopt(easy, CURLOPT_SSL_CTX_DATA, request.get());
        // SSL version and algortihms
        verified_curl_easy_setopt(easy, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        verified_curl_easy_setopt(easy, CURLOPT_SSL_CIPHER_LIST, "ECDHE-RSA-AES128-SHA");
        verified_curl_easy_setopt(easy, CURLOPT_SSL_VERIFYPEER, 1L);
    }

    // Write callback and write location
    verified_curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, WriteCallback);
    verified_curl_easy_setopt(easy, CURLOPT_WRITEDATA, request.get());
    // Save Header data
    verified_curl_easy_setopt(easy, CURLOPT_HEADERDATA, request->GetResponseHeaderBuffer());
    verified_curl_easy_setopt(easy, CURLOPT_HEADERFUNCTION, WriteHeaderCallback);

    // Debug logging
    char const* const verboseEnv = getenv("NWH_CLOUDREQUEST_VERBOSE");
    const bool verbose = (verboseEnv != nullptr && (strcmp("0", verboseEnv) != 0));
    verified_curl_easy_setopt(easy, CURLOPT_VERBOSE, verbose);
    verified_curl_easy_setopt(easy, CURLOPT_DEBUGFUNCTION, DebugCallback);

    // Add easy handle to multi. This will trigger the call.
    curl_multi_add_handle(multi_, request->GetCurlHandle());

    return CURLcode::CURLE_OK;
}

}  // namespace Connectivity