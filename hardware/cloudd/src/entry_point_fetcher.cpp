/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "entry_point_fetcher.h"
#include <assert.h>
#include <array>
#include <cassert>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#define LOG_TAG "CloudD"
#include <cutils/log.h>

using ::tarmac::eventloop::IDispatcher;

namespace Connectivity {

EntryPointFetcher::EntryPointFetcher(std::shared_ptr<CertHandlerInterface> cert_handler,
                                     std::shared_ptr<CloudRequestHandlerInterface> cloud_request_handler,
                                     const bool allow_retry)
    : cert_handler_{cert_handler}, cloud_request_handler_{cloud_request_handler}, allow_retry_{allow_retry} {}

void EntryPointFetcher::RequestCallbackHandler(std::int32_t http_response_code,
                                               const std::string& data,
                                               const std::string& header) throw(std::runtime_error) {
    // TODO: Handle retry times in a nicer way? (abengt34)
    using namespace std::chrono_literals;
    const std::array<std::chrono::milliseconds, 1> retry_times = {{1000ms}};
    retry_index_ = std::min(retry_index_ + 1, retry_times.size() - 1);
    auto rt = retry_times[retry_index_];
    std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(rt);

    if (http_response_code < 200 || http_response_code > 299) {
        ALOGD("[EpFetcher] Failed to get entry point (%s), response code %d",
              entry_point_url_.c_str(),
              http_response_code);
        retry_timer_handle_ = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay((std::chrono::microseconds)t,
                                                                                   [this]() { GetEntryPoint(); });
    } else {
        ALOGD("[EpFetcher] Successfully received entry point");
        EntryPointParser::EntryPoint ep = EntryPointParser::parse(data.c_str());

        // If any of the "mandatory" fields are missing, issue a new request
        if (ep.host == "" || ep.client_uri == "" || ep.port == -1) {
            ALOGD("[EpFetcher] Entry point data is incomplete, retry fetch entry point");
            retry_timer_handle_ = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay((std::chrono::microseconds)t,
                                                                                       [this]() { GetEntryPoint(); });
        } else {  // All "mandatory" fields exist, we are done!
            ALOGD("[EpFetcher] Entry point data is complete, done fetching entry point");
            if (when_result_available_callback_) {
                when_result_available_callback_(ep);
            }

            cloud_request_ = nullptr;
        }
    }
}

void EntryPointFetcher::GetEntryPoint() {
    ALOGV("[EpFetcher] + GetEntryPointURL()");

    ALOGV("[EpFetcher] entry point url: %s", entry_point_url_.c_str());

    if (entry_point_url_.find("https") == std::string::npos) {
        cloud_request_ = std::make_shared<CloudRequest>();
    } else {
        cloud_request_ = std::make_shared<CloudRequest>(cert_handler_);
    }
    cloud_request_->SetURL(entry_point_url_);
    cloud_request_->SetCallback(
            [this](std::int32_t http_response_code, const std::string& data, const std::string& header) {
                RequestCallbackHandler(http_response_code, data, header);
            });

    cloud_request_handler_->SendCloudRequest(cloud_request_);

    ALOGV("[EpFetcher] - GetEntryPointURL()");
}

void EntryPointFetcher::Fetch(const std::string& entry_point_url) throw(std::runtime_error) {
    ALOGV("[EpFetcher] + Fetch()");
    Stop();
    entry_point_url_ = entry_point_url;
    GetEntryPoint();
    ALOGV("[EpFetcher] - Fetch()");
}

void EntryPointFetcher::Stop() {
    IDispatcher::GetDefaultDispatcher().Cancel(retry_timer_handle_);
}

void EntryPointFetcher::WhenResultAvailable(std::function<void(const EntryPointParser::EntryPoint&)>&& callback) {
    when_result_available_callback_ = std::move(callback);
}
}
