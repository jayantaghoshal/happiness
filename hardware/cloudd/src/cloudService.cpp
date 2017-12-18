/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cloudService.h"
#include <curl/curl.h>
#include "certificate_handler.h"

#include <stdlib.h>
#include <unistd.h>
#include <regex>
#define LOG_TAG "CloudD.service"
#include <cutils/log.h>

namespace Connectivity {

CloudService::CloudService()
    : eventDispatcher_{IDispatcher::GetDefaultDispatcher()},
      certHandler_(new CertHandler{CLIENT_CERT_PEM(), CLIENT_KEY_PEM(), CA_CERT_PEM()}),
      entry_point_fetcher_{*certHandler_, eventDispatcher_, cloud_request_} {}

bool CloudService::Initialize() {
    if (!cloud_request_.Init()) {
        ALOGD("Unable to init Reachability Cloud Request.");
        return false;
    }

    // Subscribe to ipcbd
    android::status_t status = IHttpRequest::registerAsService();
    if (status != android::OK) {
        ALOGW("Failed to register Http binder service: %d", status);
        return false;
    } else {
        ALOGV("Http binder service register ok");
    }

    std::string lcfg_entrypoint_url = cloudd_local_config_.GetCloudEntryPointAddress();

    entry_point_fetcher_.WhenResultAvailable([&](const VPNEntryPointParser::EntryPoint& entry_point) {
        ALOGD("Cloud client received entry point");

        cep_url_ = entry_point.host;
        cep_port_ = entry_point.port;

    });

    std::string entry_point_url = "https://" + lcfg_entrypoint_url + "/?client_id=11";

    entry_point_fetcher_.Fetch(entry_point_url);

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

    if (!header_strings.empty())
        ALOGW("TODO: Do we want to include status and code in header? [%s]", header_strings[0].c_str());

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
        }
    }

    rsp.responseHeaders = http_headers;

    return rsp;
}
// Methods from IHttpRequest follow.
Return<void> CloudService::doGetRequest(const hidl_string& uri, const HttpHeaders& headers, bool use_https,
                                        uint32_t timeout, doGetRequest_cb _hidl_cb) {
    std::string url = (use_https ? "https://" : "http://") + cep_url_ + "/" + std::string(uri.c_str());

    try {
        RequestConfig rc;
        rc.use_stunnel_proxy = false;
        rc.timeout = std::chrono::seconds{timeout};
        for (auto header : headers) {
            rc.header_list.push_back(std::string(header.name.c_str()) + ":" + std::string(header.value.c_str()));
        }

        if (use_https) {
            cloud_request_.HttpsGet(url, rc,
                                    [&](std::int32_t code, const std::string& data, const std::string& header) {
                                        Response rsp = BuildResponse(code, data, header);
                                        _hidl_cb(rsp);
                                    },
                                    certHandler_);
        } else {
            cloud_request_.HttpGet(url, rc, [&](std::int32_t code, const std::string& data, const std::string& header) {
                Response rsp = BuildResponse(code, data, header);
                _hidl_cb(rsp);
            });
        }

    } catch (const std::exception& e) {
        ALOGW("Failed to initiate cloud request: %s", e.what());
    }

    return Void();
}
}  // namespace Connectivity