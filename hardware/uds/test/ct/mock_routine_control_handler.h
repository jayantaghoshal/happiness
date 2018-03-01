/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gmock/gmock.h>
#include <vendor/volvocars/hardware/uds/1.0/IRoutineControlHandler.h>

namespace vendor {
namespace volvocars {
namespace hardware {
namespace uds {
namespace V1_0 {
namespace mocks {

struct MockRoutineControlHandler : public IRoutineControlHandler {
    MOCK_CONST_METHOD1(StartAsynchronousRoutine,
                       RoutineExecutionStartResponse(const RoutineExecutionStartRequest& request));
    MOCK_CONST_METHOD1(StopAsynchronousRoutine, RoutineExecutionStopResponse(const RoutineExecutionStopRequest&));
    MOCK_CONST_METHOD1(GetAsynchronousRoutineResults,
                       RoutineExecutionResultsResponse(const RoutineExecutionResultsRequest&));

    ::android::hardware::Return<void> startAsynchronousRoutine(const RoutineExecutionStartRequest& request,
                                                               startAsynchronousRoutine_cb _hidl_cb) override {
        _hidl_cb(StartAsynchronousRoutine(request));
        return ::android::hardware::Void();
    }

    ::android::hardware::Return<void> stopAsynchronousRoutine(const RoutineExecutionStopRequest& request,
                                                              stopAsynchronousRoutine_cb _hidl_cb) override {
        _hidl_cb(StopAsynchronousRoutine(request));
        return ::android::hardware::Void();
    }

    ::android::hardware::Return<void> getAsynchronousRoutineResults(
            const RoutineExecutionResultsRequest& request,
            getAsynchronousRoutineResults_cb _hidl_cb) override {
        _hidl_cb(GetAsynchronousRoutineResults(request));
        return ::android::hardware::Void();
    }
};

}  // namespace mocks
}  // namespace V1_0
}  // namespace uds
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
