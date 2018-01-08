/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <hidl/HidlTransportSupport.h>
#include "DataCollector.h"

#undef LOG_TAG
#define LOG_TAG "uds-collector"
#include <cutils/log.h>

using android::sp;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// TODO(krzysztof.wesolowski@volvocars.com) subscribe as DiagKeys handler,
// translate them back to DIDs and ask registered providers.
int main() {
    configureRpcThreadpool(1, true /*caller thread would join the threadpool */);

    sp<DataCollector> data_collector = new DataCollector();

    auto* collector = static_cast<uds_v1_0::IDataCollector*>(data_collector.get());
    auto* collector_test_point = static_cast<uds_v1_0::IDataCollectorTestPoint*>(data_collector.get());

    auto collector_status = collector->registerAsService();
    if (collector_status != android::NO_ERROR) {
        ALOGE("Registration as a collector service failed, error = %i", collector_status);
        return collector_status;
    }

#if ENABLE_DATA_COLLECTOR_TESTPOINT
    auto test_point_status = collector_test_point->registerAsService();

    if (test_point_status != android::NO_ERROR) {
        ALOGE("Registration as a provider service (test-point) failed, error = %i", test_point_status);
        return test_point_status;
    }
#endif

    joinRpcThreadpool();
}
