/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef SOURCE_HARDWARE_UDS_TEST_CT_MOCK_DATA_HANDLER_H_
#define SOURCE_HARDWARE_UDS_TEST_CT_MOCK_DATA_HANDLER_H_

#include <gmock/gmock.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataHandler.h>

namespace vendor {
namespace volvocars {
namespace hardware {
namespace uds {
namespace V1_0 {
namespace mocks {

struct MockDataHandler : public IDataHandler {
    MOCK_CONST_METHOD1(ReadDidValue, DidReadResult(uint16_t did));
    MOCK_CONST_METHOD2(WriteDidValue, DidWriteStatusCode(uint16_t did, const std::vector<uint8_t>&));

    ::android::hardware::Return<void> readDidValue(uint16_t did, readDidValue_cb _hidl_cb) override {
        _hidl_cb(this->ReadDidValue(did));
        return ::android::hardware::Void();
    }

    ::android::hardware::Return<DidWriteStatusCode> writeDidValue(
            uint16_t did, const ::android::hardware::hidl_vec<uint8_t>& data) override {
        return this->WriteDidValue(did, data);
    }
};

}  // namespace mocks
}  // namespace V1_0
}  // namespace uds
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor

#endif /* SOURCE_HARDWARE_UDS_TEST_CT_MOCK_DATA_HANDLER_H_ */
