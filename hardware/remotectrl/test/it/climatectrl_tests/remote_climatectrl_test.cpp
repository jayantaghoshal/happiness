/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "convapi_signals_def.h"
#include "remotectrl_client_base.h"

#include <vsomeip/vsomeip.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fcntl.h>
#include <sched.h>
#include <sys/stat.h>

using namespace vcc::remotectrlsim;
using namespace vcc::remotectrl;

using ::testing::AllOf;
using ::testing::Ge;
using ::testing::Le;

namespace {

const ClientInfo client_info = {
        .app_name_ = "ClimateCtrlSim",
        .service_id_ = REMOTECTRL_CLIMATECTRL_SERVICE_ID,
        .instance_id_ = REMOTECTRL_CLIMATECTRL_SERVICE_INSTANCE_ID,
        .eventgroup_id_ = REMOTECTRL_CLIMATECTRL_EVENTGROUP_ID,
        .methods_ = {REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL, REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL},
        .events_ = {REMOTECTRL_CLIMATECTRL_EVENT_ID_FANLEVELCHANGED}};

constexpr std::uint16_t GETFANLEVEL_RESPONSE_EXEPCTED_PAYLOAD_LEN = 0x02U;
constexpr std::uint16_t SETFANLEVEL_RESPONSE_EXEPCTED_PAYLOAD_LEN = 0x01U;

}  // namespace

class RemoteClimateCtrlFixture : public ::testing::Test, public RemoteCtrlClientBase {
  public:
    RemoteClimateCtrlFixture() : RemoteCtrlClientBase("/data/local/tmp/vsomeip_client.json", client_info) {
        // Move test case into namespace for network mocks to work
        const std::string& nameSpace = "/dev/vendor/netns/vcc";

        file_descriptor_ = open(nameSpace.c_str(), O_RDONLY | O_CLOEXEC);
        if (-1 == file_descriptor_ || -1 == setns(file_descriptor_, CLONE_NEWNET)) {
            std::runtime_error("Failed to move Test in VCC namespace");
        }
    }

    ~RemoteClimateCtrlFixture() override {
        if (file_descriptor_ > 0) {
            close(file_descriptor_);
            file_descriptor_ = -1;
        }
    }

    int file_descriptor_ = -1;
};

/* GetFanLevel_SuccessTest
 * Asserts that on success IHU returns fan_level within range [1, 5]
 */
TEST_F(RemoteClimateCtrlFixture, GetFanLevel_SuccessTest) {
    EnqueueRequest([this]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL, {0x00 /*row*/, 0x01 /*pos*/});
    });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);

    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), GETFANLEVEL_RESPONSE_EXEPCTED_PAYLOAD_LEN);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[0], 0x03);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[1], AllOf(Ge(1), Le(5)));
}

/* GetFanLevel_FailureTest
 * Asserts that on receiving malformed message IHU returns no response
 */
TEST_F(RemoteClimateCtrlFixture, GetFanLevel_FailureTest) {
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL, {0x00 /*row*/}); });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), false);
}

/* GetFanLevel_SetFanLevel_SuccessTest
 * Asserts that on success IHU sets fan_level to requested fan_level level
 */
TEST_F(RemoteClimateCtrlFixture, GetFanLevel_SetFanLevel_SuccessTest) {
    EnqueueRequest([this]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL, {0x00 /*row*/, 0x01 /*pos*/});
    });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);

    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), GETFANLEVEL_RESPONSE_EXEPCTED_PAYLOAD_LEN);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[0], 0x03);
    uint8_t fan_speed = received_reply_->get_payload()->get_data()[1];
    fan_speed = fan_speed == 5 ? fan_speed - 1 : fan_speed + 1;
    EnqueueRequest([this, fan_speed]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL, {0x00 /*row*/, 0x01 /*pos*/, fan_speed});
    });

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);
    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), SETFANLEVEL_RESPONSE_EXEPCTED_PAYLOAD_LEN);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[0], 0x03);
}

/* SetFanLevel_FailureTest
 * Asserts that on receiving malformed message IHU returns no response
 */
TEST_F(RemoteClimateCtrlFixture, SetFanLevel_FailureTest) {
    EnqueueRequest([this]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL, {0x00 /*row*/, 0x00 /*pos*/});
    });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), false);
}
