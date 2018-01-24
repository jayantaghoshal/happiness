/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cloud_service.h"
#include <curl/curl.h>
#include "certificate_handler.h"

#include <stdlib.h>
#include <unistd.h>
#include <future>
#include <regex>

#define LOG_TAG "CloudD.service"
#include <cutils/log.h>

namespace Connectivity {

CloudService::CloudService()
    : cloud_request_handler_{std::make_shared<CloudRequestHandler>()},
      cert_handler_(std::make_shared<CertHandler>(CLIENT_CERT_PEM(), CLIENT_KEY_PEM(), CA_CERT_PEM())),
      entry_point_fetcher_{cert_handler_, cloud_request_handler_} {}

bool CloudService::Initialize() {
    // Subscribe to ipcbd
    android::status_t status = ICloudConnection::registerAsService();
    if (status != android::OK) {
        ALOGW("Failed to register Http binder service: %d", status);
        return false;
    } else {
        ALOGV("Http binder service register ok");
    }

    return FetchEntryPoint();
}

bool CloudService::FetchEntryPoint() {
    std::string lcfg_entrypoint_url = cloudd_local_config_.GetCloudEntryPointAddress();

    entry_point_fetcher_.WhenResultAvailable([&](const EntryPointParser::EntryPoint& entry_point) {
        if (entry_point.host.empty()) {
            ALOGW("Entry point URL is empty, what do?");
        } else {
            ALOGV("Cloud daemon received entry point");
        }

        cep_url_ = entry_point.host;
        cep_port_ = entry_point.port;
    });

    try {
        entry_point_fetcher_.Fetch(lcfg_entrypoint_url);
    } catch (std::exception& e) {
        ALOGE("Failed to fetch entry point: %s", e.what());
        return false;
    }
    return true;
}

Response CloudService::BuildResponse(std::int32_t code, const std::string& data, const std::string& header) {
    Response rsp;
    rsp.httpResponse = code;
    std::vector<uint8_t> d(data.begin(), data.end());
    rsp.responseData = d;

    // split header string into multiple header strings
    std::regex linebreak("\n");
    std::vector<std::string> header_strings{std::sregex_token_iterator(header.begin(), header.end(), linebreak, -1),
                                            {}};

    if (!header_strings.empty()) {
        ALOGW("TODO: Do we want to include status and code in header? [%s]", header_strings[0].c_str());
    }

    std::vector<HttpHeaderField> http_headers;
    for (auto s : header_strings) {
        HttpHeaderField http_header;
        auto index = s.find_first_of(":");
        if (index != std::string::npos) {
            http_header.name = s.substr(0, index);
            http_header.value = s.substr(index + 1, std::string::npos);
            http_headers.push_back(http_header);
        } else {
            // What should we do if the header doesn't have a ':' delimmiter (Like HTTP/1.1 200 OK)
            // Code is stored in httpResponse
        }
    }

    rsp.responseHeaders = http_headers;

    return rsp;
}

// Methods from IHttpRequest follow.
Return<void> CloudService::doGetRequest(const hidl_string& uri, const HttpHeaders& headers, bool use_https,
                                        uint32_t timeout, doGetRequest_cb _hidl_cb) {
    if (cep_url_.empty()) {
        ALOGW("Illegal call: CEP URL not fetch yet.");
        ALOGE("TODO: Fix HIDL interface to manage calls before CEP URL is fetched...");
        Response error;
        error.httpResponse = 600;  // Made up HTTP code. This one stands for "Not Ready Yet". I.E., no CEP fetched.
        _hidl_cb(error);
    }

    std::string url = (use_https ? "https://" : "http://") + cep_url_ + ":" + std::to_string(cep_port_);
    std::string path(uri.c_str());

    size_t pos = path.find("/", 0);
    if (0 == pos) {
        url = url + path;
    } else {
        url = url + "/" + path;
    }

    std::promise<Response> promise;
    std::future<Response> future_response = promise.get_future();

    std::shared_ptr<CloudRequest> cr;

    try {
        cr = std::make_shared<CloudRequest>(cert_handler_);

        cr->SetUseHttps(use_https);
        cr->SetTimeout(std::chrono::milliseconds(timeout));
        cr->SetURL(url);
        cr->SetCallback([&](std::int32_t code, const std::string& data, const std::string& header) {
            promise.set_value(BuildResponse(code, data, header));
        });

        std::vector<std::string> header_list;
        for (auto header : headers) {
            header_list.push_back(std::string(header.name.c_str()) + ":" + std::string(header.value.c_str()));
        }

        cr->SetHeaderList(header_list);

        cloud_request_handler_->SendCloudRequest(cr);  // May throw Runtime Exception if curl fails to set options.

    } catch (const std::exception& e) {
        ALOGW("Failed to initiate cloud request: %s", e.what());
        Response error;
        error.httpResponse = 400;  // Bad request.
        _hidl_cb(error);
    }

    Response response;
    std::chrono::milliseconds span(timeout);
    if (future_response.wait_for(span) != std::future_status::timeout) {
        response = future_response.get();
    } else {
        response.httpResponse = 408;  // HTTP code for time out
    }

    _hidl_cb(response);

    return Void();
}
}  // namespace Connectivity