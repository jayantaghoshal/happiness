/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "convapi_signals_def.h"
#include "remotectrl_client_base.h"

#include <vsomeip/vsomeip.hpp>

#include <vendor/volvocars/hardware/remotectrl/1.0/types.h>

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

namespace hidl_remotectrl = ::vendor::volvocars::hardware::remotectrl::V1_0;

namespace {

const ClientInfo client_info = {.app_name_ = "GeneralSettingsSim",
                                .service_id_ = REMOTECTRL_GENERALSETTINGS_SERVICE_ID,
                                .instance_id_ = REMOTECTRL_GENERALSETTINGS_SERVICE_INSTANCE_ID,
                                .eventgroup_id_ = REMOTECTRL_GENERALSETTINGS_EVENTGROUP_ID,
                                .methods_ = {REMOTECTRL_GENERALSETTINGS_METHOD_ID_SET_CSD_STATE,
                                             REMOTECTRL_GENERALSETTINGS_METHOD_ID_GET_CSD_STATE},
                                .events_ = {REMOTECTRL_GENERALSETTINGS_EVENT_ID_CSD_STATECHANGED}};
const std::uint16_t SETCSDSTATE_RESPONSE_EXPECTED_PAYLOAD_LEN = 0x01U;
const std::uint16_t GETCSDSTATE_RESPONSE_EXPECTED_PAYLOAD_LEN = 0x02U;
const std::uint16_t CSDSTATE_NOTIFICATION_EXPECTED_PAYLOAD_LEN = 0x02U;

}  // namespace

class RemoteGeneralSettingsFixture : public ::testing::Test, public RemoteCtrlClientBase {
  public:
    RemoteGeneralSettingsFixture() : RemoteCtrlClientBase("/data/local/tmp/vsomeip_client.json", client_info) {
        // Move test case into namespace for network mocks to work
        const std::string& nameSpace = "/dev/vendor/netns/vcc";

        file_descriptor_ = open(nameSpace.c_str(), O_RDONLY | O_CLOEXEC);
        if (-1 == file_descriptor_ || -1 == setns(file_descriptor_, CLONE_NEWNET)) {
            std::runtime_error("Failed to move Test in VCC namespace");
        }
    }

    ~RemoteGeneralSettingsFixture() override {
        if (file_descriptor_ > 0) {
            close(file_descriptor_);
            file_descriptor_ = -1;
        }
    }

    int file_descriptor_ = -1;
};

/* SetCsdStateOn_SuccessTest
 * Asserts that on success IHU notifies changed csd state to requested csd state
 */
TEST_F(RemoteGeneralSettingsFixture, SetCsdStateOn_SuccessTest) {
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_GENERALSETTINGS_METHOD_ID_GET_CSD_STATE, {}); });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);

    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), GETCSDSTATE_RESPONSE_EXPECTED_PAYLOAD_LEN);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[0],
                static_cast<uint8_t>(hidl_remotectrl::RemoteCtrlHalPropertyStatus::AVAILABLE));
    EXPECT_THAT(received_reply_->get_payload()->get_data()[1],
                AllOf(Ge(static_cast<uint8_t>(hidl_remotectrl::CSDState::OFF)),
                      Le(static_cast<uint8_t>(hidl_remotectrl::CSDState::ON))));

    hidl_remotectrl::CSDState initialCsdState =
            static_cast<hidl_remotectrl::CSDState>(received_reply_->get_payload()->get_data()[1]);
    hidl_remotectrl::CSDState setState = initialCsdState == hidl_remotectrl::CSDState::OFF
                                                 ? hidl_remotectrl::CSDState::ON
                                                 : hidl_remotectrl::CSDState::OFF;

    EnqueueRequest([this, setState]() {
        SendRequest(REMOTECTRL_GENERALSETTINGS_METHOD_ID_SET_CSD_STATE, {static_cast<uint8_t>(setState)});
    });

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);

    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), SETCSDSTATE_RESPONSE_EXPECTED_PAYLOAD_LEN);

    ASSERT_THAT(WaitForNotification(2s), true);
    ASSERT_THAT(received_notification_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_notification_->get_payload()->get_length(), CSDSTATE_NOTIFICATION_EXPECTED_PAYLOAD_LEN);
    EXPECT_THAT(received_notification_->get_payload()->get_data()[0],
                static_cast<uint8_t>(hidl_remotectrl::RemoteCtrlHalPropertyStatus::AVAILABLE));
    EXPECT_THAT(received_notification_->get_payload()->get_data()[1], static_cast<uint8_t>(setState));
}

/* SetCsdStateOff_SuccessTest
 * Asserts that on success IHU notifies changed csd state to requested csd state
 */
TEST_F(RemoteGeneralSettingsFixture, SetCsdStateOff_SuccessTest) {
    EnqueueRequest([this]() {
        SendRequest(REMOTECTRL_GENERALSETTINGS_METHOD_ID_SET_CSD_STATE,
                    {static_cast<uint8_t>(hidl_remotectrl::CSDState::OFF)});
    });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);

    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), SETCSDSTATE_RESPONSE_EXPECTED_PAYLOAD_LEN);
    ASSERT_THAT(WaitForNotification(2s), true);
    ASSERT_THAT(received_notification_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_notification_->get_payload()->get_length(), CSDSTATE_NOTIFICATION_EXPECTED_PAYLOAD_LEN);
    EXPECT_THAT(received_notification_->get_payload()->get_data()[0],
                static_cast<uint8_t>(hidl_remotectrl::RemoteCtrlHalPropertyStatus::AVAILABLE));
    EXPECT_THAT(received_notification_->get_payload()->get_data()[1],
                static_cast<uint8_t>(hidl_remotectrl::CSDState::OFF));
}

/* GetCsdState_SuccessTest
 * Asserts that on success IHU returns csd state
 */
TEST_F(RemoteGeneralSettingsFixture, GetCsdState_SuccessTest) {
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_GENERALSETTINGS_METHOD_ID_GET_CSD_STATE, {}); });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);

    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), GETCSDSTATE_RESPONSE_EXPECTED_PAYLOAD_LEN);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[0],
                static_cast<uint8_t>(hidl_remotectrl::RemoteCtrlHalPropertyStatus::AVAILABLE));
    EXPECT_THAT(received_reply_->get_payload()->get_data()[1],
                AllOf(Ge(static_cast<uint8_t>(hidl_remotectrl::CSDState::OFF)),
                      Le(static_cast<uint8_t>(hidl_remotectrl::CSDState::ON))));
}

/* SetCsdState_FailureTest
 * Asserts that on receiving malformed message IHU returns no response
 */
TEST_F(RemoteGeneralSettingsFixture, SetCsdState_FailureTest) {
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_GENERALSETTINGS_METHOD_ID_SET_CSD_STATE, {}); });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), false);

    ASSERT_THAT(WaitForNotification(2s), false);
}
