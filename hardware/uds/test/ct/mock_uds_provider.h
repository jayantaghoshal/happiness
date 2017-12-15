/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef SOURCE_HARDWARE_UDS_TEST_CT_MOCK_UDS_PROVIDER_H_
#define SOURCE_HARDWARE_UDS_TEST_CT_MOCK_UDS_PROVIDER_H_

#include <gmock/gmock.h>
#include <vendor/volvocars/hardware/uds/1.0/IUdsDataProvider.h>

struct MockUdsProvider : public ::vendor::volvocars::hardware::uds::V1_0::IUdsDataProvider {
    MOCK_CONST_METHOD1(ReadDidValue, ::vendor::volvocars::hardware::uds::V1_0::DidReadResult(uint16_t did));

    ::android::hardware::Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override {
        _hidl_cb(this->ReadDidValue(did));
        return ::android::hardware::Void();
    }
};

#endif /* SOURCE_HARDWARE_UDS_TEST_CT_MOCK_UDS_PROVIDER_H_ */
