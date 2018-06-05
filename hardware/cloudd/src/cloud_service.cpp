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

void CloudService::Initialize() {
    android::status_t status = ICloudConnection::registerAsService();
    if (status != android::OK) {
        ALOGW("[Service] Failed to register Http binder service: %d", status);
    } else {
        ALOGV("[Service] Http binder service register ok");
    }
    status = ICloudNotifications::registerAsService();
    if (status != android::OK) {
        ALOGW("[Service] Failed to register MqttClient binder service: %d", status);
        return;
    } else {
        ALOGV("[Service] MqttClient binder service register ok");
    }
    FetchEntryPoint();
    return;
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

        // Remove trailing "/" if there is one and copy everything from the previous "/" :
        // "http://cep.eu.vcctest.ericssoncvc.com/features-1/" -> "features-1"
        client_uri_ = entry_point.client_uri;
        if (client_uri_.at(client_uri_.length() - 1) == '/') {
            client_uri_.erase(client_uri_.end() - 1);
        }
        client_uri_ = client_uri_.substr(client_uri_.rfind("/") + 1);

        cep_mqtt_server_ = entry_point.signal_service_uri;

        ALOGD("[Service] CEP URL: %s:%d", cep_url_.c_str(), cep_port_);
        ALOGD("[Service] CEP MQTT server: %s", cep_mqtt_server_.c_str());

        SetConnectionState(ConnectionState::CONNECTED);

        // Connect to MQTT if we have cep to MQTT in entry point data

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
            auto result = listener->get()->isConnected(state == ConnectionState::CONNECTED, client_uri_);
            if (!result.isOk()) {
                ALOGE("Listener no longer valid, removing from list");
                listener = listeners_.erase(listener);
            } else {
                ++listener;
            }
        }
    }
}

void CloudService::split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> CloudService::split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

Return<bool> CloudService::registerTopicCallback(const hidl_string& topic,
                                                 const android::sp<ICloudMessageArrivedCallback>& callback) {
    std::string topic_ = topic;
    // split the topic string into individual levels string elements for better searching later
    std::vector<std::string> topic_hierarchy_vector_ = split(topic_, '/');
    if (callback && !topic_.empty()) {
        topic_callback_map_.emplace_back(topic_hierarchy_vector_, callback);
        ALOGD("Callback successfully registered for topic: %s", topic_.c_str());
        ConnectToMqttServer();
        return true;
    } else {
        ALOGD("Callback failed to register for topic: %s", topic_.c_str());
        return false;
    }
}

void CloudService::ConnectToMqttServer() {
    topic_list_ = std::make_shared<mqtt::string_collection>();
    client_ = std::make_shared<mqtt::async_client>(cep_mqtt_server_, CLIENT_ID);
    connopts_.set_keep_alive_interval(KEEP_ALIVE_INTERVAL);
    connopts_.set_clean_session(true);
    connopts_.set_automatic_reconnect(minRetryInterval, maxRetryInterval);

    mqtt_cb_.SetConnectionHandler([&]() {
        std::vector<std::pair<std::vector<std::string>, android::sp<ICloudMessageArrivedCallback>>>::iterator
                topic_iterator;
        ALOGD("Subscribing to the following topics: ");
        for (topic_iterator = topic_callback_map_.begin(); topic_iterator != topic_callback_map_.end();
             topic_iterator++) {
            // build back the topic string before subscribe to add it to the topic_list_
            std::string tp;
            for (std::vector<std::string>::iterator it = topic_iterator->first.begin();
                 it != topic_iterator->first.end();
                 it++) {
                tp = tp + *it + "/";
            }
            tp.back() = '\0';
            ALOGD("topic : %s", tp.c_str());
            QOS.push_back(2);  // TODO: Change QOS to zero after testing is done
            topic_list_->push_back(tp);
        }
        try {
            client_->subscribe(topic_list_, QOS, nullptr, mqtt_cb_.subListener_);
        } catch (mqtt::exception e) {
            ALOGE("Subscribe Exception %s", e.to_string().c_str());
        }
    });

    mqtt_cb_.SetMessageHandler([&](mqtt::const_message_ptr msg) {
        ALOGD("\ttopic: '%s'", msg->get_topic().c_str());
        ALOGD("\tpayload: '%s", msg->to_string().c_str());

        std::vector<std::string> received_topic_str_ = split(msg->get_topic(), '/');

        for (uint64_t i = 0; i < topic_callback_map_.size(); i++) {
            bool matched = false;
            for (std::vector<std::string>::iterator recv_it = received_topic_str_.begin(),
                                                    stored_it = topic_callback_map_[i].first.begin();
                 (recv_it != received_topic_str_.end()) && (stored_it != topic_callback_map_[i].first.end());
                 recv_it++, stored_it++) {
                if (*recv_it != *stored_it) {
                    matched = false;
                    break;
                } else {
                    matched = true;
                }
            }
            if (matched == true) topic_callback_map_[i].second->messageArrived(msg->to_string());
        }

    });
    client_->set_callback(mqtt_cb_);

    // Start the connection.
    // When completed, the callback will subscribe to topic.

    try {
        ALOGD("Connect first attempt to the MQTT broker ...");
        client_->connect(connopts_, nullptr, mqtt_cb_);
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
            [&, listener]() { listener->isConnected(state_ == ConnectionState::CONNECTED, client_uri_); });

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