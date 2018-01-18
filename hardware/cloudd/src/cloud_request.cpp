/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cloud_request.h"

#define LOG_TAG "CloudD.request"
#include <cutils/log.h>

namespace Connectivity {

bool CloudRequest::SetCurlHandle(CURL* curl_handle) {
    if (curl_handle_) {
        ALOGW("Trying to set a null pointer as curl handle");
        return false;
    }
    curl_handle_ = curl_handle;
    return true;
}
CURL* CloudRequest::GetCurlHandle() { return curl_handle_; }

void CloudRequest::SetURL(std::string url) {
    if (!url.empty()) {
        std::string http = "http://";
        std::string https = "https://";
        std::vector<size_t> positions;

        // Maybe we can use this check to set "use_https" instead of having to specify it? Might enable easier testing..

        size_t pos = url.find(http, 0);
        while (pos != std::string::npos) {
            positions.push_back(pos);
            pos = url.find(http, pos + 1);
        }

        pos = url.find(https, 0);
        while (pos != std::string::npos) {
            positions.push_back(pos);
            pos = url.find(https, pos + 1);
        }

        if (positions.empty() || (positions.size() > 1) ||
            (positions[0] > 0)) {  // If there are no substring with "http://" or "https://" the URL is invalid for IHU
            throw std::runtime_error("Trying to set an invalid URL" + url);
        } else {
            // To validate an URL is extremely complex and consumes enormous resources, so lets not?
            url_ = url;
        }

    } else {
        throw std::runtime_error("Trying to set an empty url");
    }
}
std::string CloudRequest::GetURL() { return url_; }

CertHandlerInterface* CloudRequest::GetCertHandler() { return certificate_handler_.get(); }

void CloudRequest::SetHeaderList(std::vector<std::string> header_list) {
    // Assumes that every entry is correct.
    header_list_ = header_list;
}

std::vector<std::string> CloudRequest::GetHeaderList() { return header_list_; }

void CloudRequest::SetUseHttps(bool use_https) { use_https_ = use_https; }

bool CloudRequest::GetUseHttps() { return use_https_; }

void CloudRequest::SetTimeout(std::chrono::milliseconds timeout) { timeout_ = timeout; }

std::chrono::milliseconds CloudRequest::GetTimeout() { return timeout_; }

void CloudRequest::SetCallback(ResponseCallback&& callback) {
    if (!callback) {
        throw std::runtime_error("Trying to set a null pointer as callback");
    } else {
        response_callback_ = callback;
    }
}

ResponseCallback CloudRequest::GetCallback() { return response_callback_; }

std::string* CloudRequest::GetResponseDataBuffer() { return &response_data_; }
std::string* CloudRequest::GetResponseHeaderBuffer() { return &response_header_; }

}  // namespace Connectivity