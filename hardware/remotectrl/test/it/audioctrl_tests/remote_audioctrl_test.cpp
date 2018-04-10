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
        .app_name_ = "AudioCtrlSim",
        .service_id_ = REMOTECTRL_AUDIOCTRL_SERVICE_ID,
        .instance_id_ = REMOTECTRL_AUDIOCTRL_SERVICE_INSTANCE_ID,
        .eventgroup_id_ = REMOTECTRL_AUDIOCTRL_EVENTGROUP_ID,
        .methods_ = {REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME},
        .events_ = {REMOTECTRL_AUDIOCTRL_EVENT_ID_VOLUMESTATUS}};

const std::uint16_t GETVOLUME_RESPONSE_EXEPCTED_PAYLOAD_LEN = 0x02U;
const std::uint16_t SETVOLUME_RESPONSE_EXEPCTED_PAYLOAD_LEN = 0x00U;
const std::uint16_t MEDIA_CONTEXT = 0x01U;

}  // namespace

class RemoteAudioCtrlFixture : public ::testing::Test, public RemoteCtrlClientBase {
  public:
    RemoteAudioCtrlFixture() : RemoteCtrlClientBase("/data/local/tmp/vsomeip_client.json", client_info) {
        // Move test case into namespace for network mocks to work
        const std::string& nameSpace = "/dev/vendor/netns/vcc";

        file_descriptor_ = open(nameSpace.c_str(), O_RDONLY | O_CLOEXEC);
        if (-1 == file_descriptor_ || -1 == setns(file_descriptor_, CLONE_NEWNET)) {
            std::runtime_error("Failed to move Test in VCC namespace");
        }
    }

    ~RemoteAudioCtrlFixture() override {
        if (file_descriptor_ > 0) {
            close(file_descriptor_);
            file_descriptor_ = -1;
        }
    }

    int file_descriptor_ = -1;
};

/* GetVolume_SuccessTest
 * Asserts that on success IHU returns volume within range [0, 100]
 */
TEST_F(RemoteAudioCtrlFixture, GetVolume_SuccessTest) {
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, {MEDIA_CONTEXT}); });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);

    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), GETVOLUME_RESPONSE_EXEPCTED_PAYLOAD_LEN);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[0], MEDIA_CONTEXT);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[1], AllOf(Ge(0), Le(100)));
}

/* GetVolume_FailureTest
 * Asserts that on receiving malformed message IHU returns no response
 */
TEST_F(RemoteAudioCtrlFixture, GetVolume_FailureTest) {
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, {MEDIA_CONTEXT, 0x03}); });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), false);
}

/* SetVolume_GetVolume_SuccessTest
 * Asserts that on success IHU sets volume to requested volume level
 */
TEST_F(RemoteAudioCtrlFixture, SetVolume_GetVolume_SuccessTest) {
    const uint8_t volume_level = 0x40U;
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME, {MEDIA_CONTEXT, volume_level}); });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), true);
    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), SETVOLUME_RESPONSE_EXEPCTED_PAYLOAD_LEN);

    EnqueueRequest([this]() { SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, {MEDIA_CONTEXT}); });

    ASSERT_THAT(WaitForResponse(2s), true);
    ASSERT_THAT(received_reply_->get_return_code(), vsomeip::return_code_e::E_OK);
    ASSERT_THAT(received_reply_->get_payload()->get_length(), GETVOLUME_RESPONSE_EXEPCTED_PAYLOAD_LEN);
    EXPECT_THAT(received_reply_->get_payload()->get_data()[0], MEDIA_CONTEXT);
    // TODO (Abhi) : because of up/down scaling involved in volume levels in between ConvAPI and Android; below
    // expectioation is currently disabled. This should be enabled/removed(?) after discussion is concluded
    // EXPECT_THAT(received_reply_->get_payload()->get_data()[1], volume_level);
}

/* SetVolume_FailureTest
 * Asserts that on receiving malformed message IHU returns no response
 */
TEST_F(RemoteAudioCtrlFixture, SetVolume_FailureTest) {
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME, {MEDIA_CONTEXT}); });
    StartClient();

    using namespace std::chrono_literals;
    ASSERT_THAT(WaitForResponse(2s), false);
}
