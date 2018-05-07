/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>

#include <IDispatcher.h>
#include <dispatcher_watchdog.h>
#include <gsl/span>
#include "SettingsStorage.h"

#undef LOG_TAG
#define LOG_TAG "SettingsStorage"

using namespace vendor::volvocars::hardware::settings::V1_0::implementation;

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    const auto dispatcher = tarmac::eventloop::IDispatcher::CreateDispatcher(false);
    const auto watchdog = tarmac::eventloop::WatchDog::Create();
    watchdog->Watch(dispatcher, std::chrono::seconds(30));

    // Setup hidl transport into file descriptor notification mode and connect the
    // file descriptor to dispatcher so that all server calls and client callbacks from hidl
    // are called on the dispatcher thread.
    // This way we can ensure to only have a single thread in the application
    const int hidl_transport_fd = android::hardware::setupTransportPolling();
    assert(hidl_transport_fd >= 0);
    auto handleTransportPoll = [&]() {
        auto status = android::hardware::handleTransportPoll(hidl_transport_fd);
        if (status != android::OK) {
            ALOGE("handleTransportPoll done error status: %d", status);
            assert(status == android::OK);
        }
    };
    dispatcher->AddFd(hidl_transport_fd, handleTransportPoll);

    android::sp<SettingsStorage> service = new SettingsStorage(dispatcher);

    ALOGI("Registering as service...");
    android::status_t status = service->registerAsService();
    if (status != android::OK) {
        ALOGE("Failed to register SettingsStorage binder service: %d", status);
        assert(false);
    }

    handleTransportPoll();
    dispatcher->RunUntil([]() { return false; });
}
