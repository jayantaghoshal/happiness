/*
 * Copyright 2017 Volvo Car Corporation
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

#define LOG_TAG "CloudD.EpFetcher"
#include <cutils/log.h>

using ::tarmac::eventloop::IDispatcher;

namespace Connectivity {

EntryPointFetcher::EntryPointFetcher(std::shared_ptr<CertHandlerInterface> cert_handler,
                                     std::shared_ptr<CloudRequestHandlerInterface> cloud_request_handler,
                                     const bool allow_retry)
    : cert_handler_{cert_handler}, cloud_request_handler_{cloud_request_handler}, allow_retry_{allow_retry} {}

void EntryPointFetcher::RequestCallbackHandler(std::int32_t http_response_code, const std::string& data,
                                               const std::string& header) throw(std::runtime_error) {
    using namespace std::chrono_literals;
    const std::array<std::chrono::milliseconds, 1> retry_times = {{1000ms}};

    if (http_response_code < 200 || http_response_code > 299) {
        ALOGD("Failed to get entry point, response code %d", http_response_code);
        retry_index_ = std::min(retry_index_ + 1, retry_times.size() - 1);
        auto rt = retry_times[retry_index_];
        std::chrono::milliseconds t = std::chrono::duration_cast<std::chrono::milliseconds>(rt);
        retry_timer_handle_ = IDispatcher::GetDefaultDispatcher().EnqueueWithDelay((std::chrono::microseconds)t,
                                                                                   [this]() { GetEntryPoint(); });
    } else {
        ALOGD("Successfully received entry point");
        EntryPointParser::EntryPoint ep;
        try {
            ep = EntryPointParser::parse(data.c_str());
        } catch (const std::exception& e) {
            ALOGW("Failed while parsing Entry Point Request: %s", e.what());
        }
        if (when_result_available_callback_) when_result_available_callback_(ep);

        cloud_request_ = nullptr;
    }
}

void EntryPointFetcher::GetEntryPoint() {
    ALOGV(" + GetEntryPointURL()");

    ALOGD("entry point url: %s", entry_point_url_.c_str());

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

    ALOGV("- GetEntryPointURL()");
}

void EntryPointFetcher::Fetch(const std::string& entry_point_url) throw(std::runtime_error) {
    ALOGV("+ Fetch()");
    Stop();
    entry_point_url_ = entry_point_url;
    GetEntryPoint();
    ALOGV("- Fetch()");
}

void EntryPointFetcher::Stop() { IDispatcher::GetDefaultDispatcher().Cancel(retry_timer_handle_); }

void EntryPointFetcher::WhenResultAvailable(std::function<void(const EntryPointParser::EntryPoint&)>&& callback) {
    when_result_available_callback_ = std::move(callback);
}
}
