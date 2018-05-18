/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cloud_service.h"
#include <IDispatcher.h>
#include <curl/curl.h>
#include "certificate_handler.h"

#include <stdlib.h>
#include <unistd.h>
#include <future>
#include <regex>

#define LOG_TAG "CloudD"
#include <cutils/log.h>

namespace Connectivity {

CloudService::CloudService()
    : state_{INIT},
      listeners_{},
      cloud_request_handler_{std::make_shared<CloudRequestHandler>()},
      cert_handler_(std::make_shared<CertHandler>(CLIENT_CERT_PEM(), CLIENT_KEY_PEM(), CA_CERT_PEM())),
      entry_point_fetcher_{cert_handler_, cloud_request_handler_} {}

bool CloudService::Initialize() {
    android::status_t status = ICloudConnection::registerAsService();
    if (status != android::OK) {
        ALOGW("[Service] Failed to register Http binder service: %d", status);
        return false;
    } else {
        ALOGV("[Service] Http binder service register ok");
    }
    return FetchEntryPoint();
}

bool CloudService::FetchEntryPoint() {
    std::string lcfg_entrypoint_url = cloudd_local_config_.GetCloudEntryPointAddress();

    entry_point_fetcher_.WhenResultAvailable([&, lcfg_entrypoint_url](const EntryPointParser::EntryPoint& entry_point) {
        if (entry_point.host.empty()) {
            ALOGW("[Service] Entry point URL is empty, what do?");
        } else {
            ALOGV("[Service] Cloud daemon received entry point");
        }

        size_t pos = lcfg_entrypoint_url.find("://");
        std::string protocol;
        if (pos != std::string::npos) {
            protocol = lcfg_entrypoint_url.substr(0, pos + 3);
        } else {
            ALOGW("[Service] CNEP URL %s doesn't contain a protocol. Using 'https' as default.",
                  lcfg_entrypoint_url.c_str());
            protocol = "https://";
        }

        if ((protocol == "https://") || (protocol == "http://")) {
            cep_url_ = protocol + entry_point.host;
            cep_port_ = entry_point.port;
        }

        cep_mqtt_server_ = entry_point.signal_service_uri;

        ALOGD("[Service] CEP URL: %s:%d", cep_url_.c_str(), cep_port_);
        ALOGD("[Service] CEP MQTT server: %s", cep_mqtt_server_.c_str());

        state_ = ConnectionState::CONNECTED;
        ConnectToMqttServer();

    });

    try {
        entry_point_fetcher_.Fetch(lcfg_entrypoint_url);
    } catch (std::exception& e) {
        ALOGE("[Service] Failed to fetch entry point: %s", e.what());
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
        ALOGW("[Service] TODO: Do we want to include status and code in header? [%s]", header_strings[0].c_str());
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

void CloudService::SetConnectionState(const ConnectionState state) {
    if (state_ != state) {
        state_ = state;

        for (std::vector<android::sp<ICloudConnectionEventListener>>::iterator listener = listeners_.begin();
             listener != listeners_.end();
             /* increased inside loop */) {
            auto result = listener->get()->isConnected(state == ConnectionState::CONNECTED);
            if (!result.isOk()) {
                ALOGE("Listener no longer valid, removing from list");
                listener = listeners_.erase(listener);
            } else {
                ++listener;
            }
        }
    }
}

void CloudService::ConnectToMqttServer() {
    client_ = std::make_shared<mqtt::async_client>(cep_mqtt_server_, CLIENT_ID);
    connopts_.set_keep_alive_interval(KEEP_ALIVE_INTERVAL);
    connopts_.set_clean_session(true);
    connopts_.set_automatic_reconnect(2, 2);

    mqtt_cb_.SetConnectionHandler([&]() { client_->subscribe(TOPIC, QOS, nullptr, mqtt_cb_.subListener_); });

    mqtt_cb_.SetMessageHandler([&](mqtt::const_message_ptr msg) {
        ALOGD("\ttopic: '%s'", msg->get_topic().c_str());
        ALOGD("\tpayload: '%s", msg->to_string().c_str());
    });
    client_->set_callback(mqtt_cb_);

    // Start the connection.
    // When completed, the callback will subscribe to topic.

    try {
        ALOGD("Connect first attempt to the MQTT broker ...");
        client_->connect(connopts_, nullptr, mqtt_cb_);
        ALOGD("the server is: %s", client_->get_server_uri().c_str());
    } catch (const mqtt::exception& exc) {
        ALOGE("ERROR: %s", exc.what());
        ALOGE("ERROR: Unable to connect to MQTT broker: %s''", cep_mqtt_server_.c_str());
        return;
    }
}
// Methods from ICloudConnection follow.
Return<void> CloudService::registerCloudConnectionEventListener(
        const android::sp<ICloudConnectionEventListener>& listener) {
    listeners_.push_back(listener);

    IDispatcher::EnqueueOnDefaultDispatcher(
            [&, listener]() { listener->isConnected(state_ == ConnectionState::CONNECTED); });

    return Void();
}

Return<void> CloudService::doGetRequest(const hidl_string& uri,
                                        const HttpHeaders& headers,
                                        uint32_t timeout,
                                        doGetRequest_cb _hidl_cb) {
    ALOGV("[Service] doGetRequest with uri: %s", uri.c_str());

    if (state_ != ConnectionState::CONNECTED) {
        ALOGW("[Service] Illegal call: CEP URL not fetch yet.");
        ALOGE("[Service] TODO: Fix HIDL interface to manage calls before CEP URL is fetched...");
        Response error;
        error.httpResponse = 600;  // Made up HTTP code. This one stands for "Not Ready Yet". I.E., no CEP fetched.
        _hidl_cb(error);
        return Void();
    }

    std::string url = cep_url_ + ":" + std::to_string(cep_port_);
    std::string path(uri.c_str());

    size_t pos = path.find("/", 0);
    if (0 == pos) {
        url = url + path;
    } else {
        url = url + "/" + path;
    }
    ALOGV("[Service] doGetRequest url: %s", url.c_str());
    std::promise<Response> promise;
    std::future<Response> future_response = promise.get_future();

    std::shared_ptr<CloudRequest> cr;

    try {
        cr = std::make_shared<CloudRequest>(cert_handler_);

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
        ALOGW("[Service] Failed to initiate cloud request: %s", e.what());
        Response error;
        error.httpResponse = 400;  // Bad request.
        _hidl_cb(error);
        return Void();
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

Return<void> CloudService::doPostRequest(const hidl_string& uri,
                                         const HttpHeaders& headers,
                                         const hidl_string& body,
                                         uint32_t timeout,
                                         doPostRequest_cb _hidl_cb) {
    ALOGV("[Service] doPostRequest with uri: %s and body: %s", uri.c_str(), body.c_str());
    if (state_ != ConnectionState::CONNECTED) {
        ALOGW("[Service] Illegal call: CEP URL not fetch yet.");
        ALOGE("[Service] TODO: Fix HIDL interface to manage calls before CEP URL is fetched...");
        Response error;
        error.httpResponse = 600;  // Made up HTTP code. This one stands for "Not Ready Yet". I.E., no CEP fetched.
        _hidl_cb(error);
        return Void();
    }

    std::string url = cep_url_ + ":" + std::to_string(cep_port_);
    std::string path(uri.c_str());

    size_t pos = path.find("/", 0);
    if (0 == pos) {
        url = url + path;
    } else {
        url = url + "/" + path;
    }

    ALOGV("[Service] doPostRequest url: %s", url.c_str());

    std::promise<Response> promise;
    std::future<Response> future_response = promise.get_future();

    std::shared_ptr<CloudRequest> cr;

    try {
        cr = std::make_shared<CloudRequest>(cert_handler_);

        cr->SetTimeout(std::chrono::milliseconds(timeout));
        cr->SetRequestMethod(CloudRequest::HttpMethod::POST);
        cr->SetRequestBody(std::string(body.c_str()));
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
        ALOGW("[Service] Failed to initiate cloud request: %s", e.what());
        Response error;
        error.httpResponse = 400;  // Bad request.
        _hidl_cb(error);
        return Void();
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

Return<void> CloudService::downloadRequest(const hidl_string& uri,
                                           const HttpHeaders& headers,
                                           const hidl_string& file_path,
                                           uint32_t timeout,
                                           const android::sp<ICloudConnectionDownloadResponseCallback>& callback) {
    ALOGV("[Service] downloadRequest with uri: %s and file_path: %s", uri.c_str(), file_path.c_str());

    if (state_ != ConnectionState::CONNECTED) {
        ALOGW("[Service] Illegal call: CEP URL not fetch yet.");
        ALOGE("[Service] TODO: Fix HIDL interface to manage calls before CEP URL is fetched...");
        return Void();
    }

    std::string url = cep_url_ + ":" + std::to_string(cep_port_);
    std::string path(uri.c_str());

    size_t pos = path.find("/", 0);
    if (0 == pos) {
        url = url + path;
    } else {
        url = url + "/" + path;
    }
    ALOGV("[Service] downloadRequest url: %s", url.c_str());

    try {
        download_request_ = std::make_shared<CloudRequest>(cert_handler_);

        download_request_->SetTimeout(std::chrono::milliseconds(timeout));
        download_request_->SetURL(url);
        download_request_->SetFilePath(file_path);
        download_request_->SetCallback(
                [this, callback](std::int32_t code, const std::string& data, const std::string& header) {
                    callback->updateDownloadStatus(BuildResponse(code, data, header));
                });

        std::vector<std::string> header_list;
        for (auto header : headers) {
            header_list.push_back(std::string(header.name.c_str()) + ":" + std::string(header.value.c_str()));
        }

        download_request_->SetHeaderList(header_list);

        cloud_request_handler_->SendCloudRequest(
                download_request_);  // May throw Runtime Exception if curl fails to set options.

    } catch (const std::exception& e) {
        ALOGW("[Service] Failed to initiate cloud request: %s", e.what());
    }

    return Void();
}

}  // namespace Connectivity