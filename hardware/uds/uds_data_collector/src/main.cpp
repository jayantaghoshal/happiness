/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <hidl/HidlTransportSupport.h>

#include "UdsDataCollector.h"

#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "uds-collector"

using android::sp;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// TODO(krzysztof.wesolowski@volvocars.com) subscribe as DiagKeys handler,
// translate them back to DIDs and ask registered providers.
int main() {
    configureRpcThreadpool(1, true /*caller thread would join the threadpool */);

    sp<UdsDataCollector> data_collector = new UdsDataCollector();

    auto* collector = static_cast<IUdsDataCollector*>(data_collector.get());
    auto* aggregated_provider = static_cast<IUdsDataProvider*>(data_collector.get());

    auto collector_status = collector->registerAsService();
    if (collector_status != android::NO_ERROR) {
        ALOGE("Registration as a collector service failed, error = %i", collector_status);
        return collector_status;
    }

    auto test_point_status = aggregated_provider->registerAsService("test-point");

    if (test_point_status != android::NO_ERROR) {
        ALOGE("Registration as a provider service (test-point) failed, error = %i", test_point_status);
        return test_point_status;
    }

    joinRpcThreadpool();
}
