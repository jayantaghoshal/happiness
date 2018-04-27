/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <evs_video_provider.h>
#include "mock_hw_camera.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

using size_type_clients = std::list<wp<IVirtualCamera>>::size_type;

using android::hardware::automotive::evs::V1_0::vcc_implementation::EvsVideoProvider;
using testing::_;  // Matches any type
using testing::StrictMock;

class MockIEvsCameraStream : public IEvsCameraStream {
  public:
    MOCK_METHOD1(deliverFrame, Return<void>(const BufferDesc& buffer));
};

class MockVirtualCamera : public IVirtualCamera {
  public:
    MOCK_METHOD0(Shutdown, void());
    MOCK_METHOD0(GetEvsVideoProvider, sp<IEvsVideoProvider>());
    MOCK_METHOD0(IsStreaming, bool());
    MOCK_METHOD1(DeliverFrame, bool(const BufferDesc& buffer));
    MOCK_METHOD1(getCameraInfo, Return<void>(getCameraInfo_cb hidl_cb));
    MOCK_METHOD1(setMaxFramesInFlight, Return<EvsResult>(uint32_t buffer_count));
    MOCK_METHOD1(startVideoStream, Return<EvsResult>(const sp<IEvsCameraStream>& stream));
    MOCK_METHOD1(doneWithFrame, Return<void>(const BufferDesc& buffer));
    MOCK_METHOD0(stopVideoStream, Return<void>());
    MOCK_METHOD1(getExtendedInfo, Return<int32_t>(uint32_t opaque_identifier));
    MOCK_METHOD2(setExtendedInfo, Return<EvsResult>(uint32_t opaque_identifier, int32_t opaque_value));
};

class EvsVideoProviderTest : public ::testing::Test {
  protected:
    sp<MockHwCamera> mock_hw_camera;
    sp<StrictMock<MockHwCamera>> strict_mock_hw_camera;
    sp<EvsVideoProvider> evs_video_provider;
    void SetUp() override {
        mock_hw_camera = new MockHwCamera();
        strict_mock_hw_camera = new StrictMock<MockHwCamera>();
        evs_video_provider = new EvsVideoProvider(mock_hw_camera);
    }

    void TearDown() override {
        mock_hw_camera.clear();
        strict_mock_hw_camera.clear();
        evs_video_provider.clear();
    }
};

namespace {
namespace test_utils {
using android::hardware::hidl_handle;
using android::hardware::automotive::evs::V1_0::BufferDesc;

// Helper method for creating dummy buffer
BufferDesc CreateDummyBuffer(uint32_t buffer_id) {
    BufferDesc dummy_buffer = {};
    native_handle_t* dummy_native_handle = new native_handle_t();
    hidl_handle dummy_handle = hidl_handle(dummy_native_handle);
    dummy_buffer.memHandle = dummy_handle;
    dummy_buffer.bufferId = buffer_id;
    return dummy_buffer;
}

}  // namespace test_utils
}  // namespace

TEST_F(EvsVideoProviderTest, ConstructObject) {
    EXPECT_NE(evs_video_provider, nullptr);
}

TEST_F(EvsVideoProviderTest, DeleteObject) {
    evs_video_provider.clear();
    EXPECT_EQ(evs_video_provider, (EvsVideoProvider*)nullptr);
}

TEST_F(EvsVideoProviderTest, MakeAndDisownClient) {
    size_type_clients client_count_initial = evs_video_provider->GetClientCount();
    size_type_clients client_count_initial_expected = 0;
    EXPECT_EQ(client_count_initial, client_count_initial_expected);

    sp<IVirtualCamera> evs_camera = evs_video_provider->MakeVirtualCamera();
    size_type_clients client_count_after_make = evs_video_provider->GetClientCount();
    EXPECT_EQ(client_count_after_make, client_count_initial + 1);

    evs_video_provider->DisownVirtualCamera(evs_camera);
    size_type_clients client_count_after_disown = evs_video_provider->GetClientCount();
    EXPECT_EQ(client_count_after_disown, client_count_initial);
}

TEST_F(EvsVideoProviderTest, MakeAndDisownMultipleClients) {
    sp<IVirtualCamera> client_1 = evs_video_provider->MakeVirtualCamera();
    sp<IVirtualCamera> client_2 = evs_video_provider->MakeVirtualCamera();
    size_type_clients client_count_after_make = evs_video_provider->GetClientCount();
    EXPECT_EQ(client_count_after_make, static_cast<size_type_clients>(2));

    evs_video_provider->DisownVirtualCamera(client_1);
    evs_video_provider->DisownVirtualCamera(client_2);
    size_type_clients client_count_after_disown = evs_video_provider->GetClientCount();
    EXPECT_EQ(client_count_after_disown, static_cast<size_type_clients>(0));
}

TEST_F(EvsVideoProviderTest, RequestVideoStreamWhenOutputStreamStopped) {
    // Test that hw camera is called on to start streaming if initial stream state is STOPPED.
    // SetUp
    ASSERT_EQ(evs_video_provider->output_stream_state_, StreamState::STOPPED);
    EXPECT_CALL(*mock_hw_camera, startVideoStream(_)).WillOnce(testing::Return(testing::ByMove(EvsResult::OK)));

    // Test
    Return<EvsResult> result = evs_video_provider->RequestVideoStream();

    // Verify
    EXPECT_EQ(result, EvsResult::OK);
    EXPECT_EQ(evs_video_provider->output_stream_state_, StreamState::RUNNING);
}

TEST_F(EvsVideoProviderTest, RequestVideoStreamWhenOutputStreamRunning) {
    // Test that method returns ok if output stream state is not STOPPED.
    // SetUp
    evs_video_provider->output_stream_state_ = StreamState::RUNNING;

    // Test
    Return<EvsResult> result = evs_video_provider->RequestVideoStream();

    // Verify
    EXPECT_EQ(result, EvsResult::OK);
}

TEST_F(EvsVideoProviderTest, ReleaseVideoStreamLastClient) {
    // Test that hw camera is called on to stop video stream if no clients are still running.
    // SetUp
    sp<IVirtualCamera> client_1 = evs_video_provider->MakeVirtualCamera();
    client_1->stopVideoStream();
    ASSERT_FALSE(client_1->IsStreaming());
    EXPECT_CALL(*mock_hw_camera, stopVideoStream());

    // Test
    evs_video_provider->ReleaseVideoStream();

    // Verify
    // Implictly done by strict_mock_hw_camera, it would fail on "uninteresting call" if called upon.
}

TEST_F(EvsVideoProviderTest, ReleaseVideoStreamNotLastClient) {
    // Test that nothing happens if there are still clients alive when method is called
    // SetUp
    evs_video_provider = new EvsVideoProvider(strict_mock_hw_camera);
    evs_video_provider->output_stream_state_ = StreamState::RUNNING;
    sp<IVirtualCamera> client_1 = evs_video_provider->MakeVirtualCamera();
    client_1->startVideoStream(new MockIEvsCameraStream());
    // We must have at least one running client for the test call to ReleaseVideoStream
    ASSERT_TRUE(client_1->IsStreaming());

    // Test
    evs_video_provider->ReleaseVideoStream();

    // Verify
    // Implictly done by strict_mock_hw_camera, it would fail on "uninteresting call" if called upon.
}

TEST_F(EvsVideoProviderTest, DoneWithFrameUnrecognizedBufferId) {
    // Test handling of an empty buffer input.
    // SetUp
    evs_video_provider = new EvsVideoProvider(strict_mock_hw_camera);
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(1);

    // Test
    evs_video_provider->DoneWithFrame(dummy_buffer);

    // Verify
    // Implictly done by strict_mock_hw_camera, it would fail on "uninteresting call" if called upon.
}

TEST_F(EvsVideoProviderTest, DoneWithFrameEmptyBuffer) {
    // Test handling of an empty buffer input.
    // SetUp
    evs_video_provider = new EvsVideoProvider(strict_mock_hw_camera);
    BufferDesc empty_buffer = {};

    // Test
    evs_video_provider->DoneWithFrame(empty_buffer);

    // Verify
    // Implictly done by strict_mock_hw_camera, it would fail on "uninteresting call" if called upon.
}

TEST_F(EvsVideoProviderTest, DoneWithFrameNotLastUser) {
    // Test handling of an empty buffer input.
    // SetUp
    evs_video_provider = new EvsVideoProvider(strict_mock_hw_camera);
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(1);
    evs_video_provider->frames_.emplace_back(dummy_buffer.bufferId);
    evs_video_provider->frames_[0].ref_count = 2;

    // Test
    evs_video_provider->DoneWithFrame(dummy_buffer);

    // Verify
    uint32_t expected_ref_count = 1;
    EXPECT_EQ(evs_video_provider->frames_[0].ref_count, expected_ref_count);
    // Also implicitly checks that no call to hardware camera was made.
}

TEST_F(EvsVideoProviderTest, DoneWithFrameLastUser) {
    // Test handling of an empty buffer input.
    // SetUp
    evs_video_provider = new EvsVideoProvider(strict_mock_hw_camera);
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(1);
    evs_video_provider->frames_.emplace_back(dummy_buffer.bufferId);
    evs_video_provider->frames_[0].ref_count = 1;
    EXPECT_CALL(*strict_mock_hw_camera, doneWithFrame(_));

    // Test
    evs_video_provider->DoneWithFrame(dummy_buffer);

    // Verify
    uint32_t expected_ref_count = 0;
    EXPECT_EQ(evs_video_provider->frames_[0].ref_count, expected_ref_count);
    // Implictly done by EXPECT_CALL
}

TEST_F(EvsVideoProviderTest, deliverFrame) {
    using size_type_frames = std::vector<EvsVideoProvider::FrameRecord>::size_type;
    using size_type_clients = std::list<wp<IVirtualCamera>>::size_type;
    // Create mocked virtual cameras
    sp<MockVirtualCamera> client_1 = new MockVirtualCamera();
    // Add mocks to list of clients
    evs_video_provider->clients_.emplace_back(std::move(client_1));
    // Verify that setup worked.
    ASSERT_EQ(evs_video_provider->GetClientCount(), static_cast<size_type_clients>(1));

    // Create dummy buffer
    uint32_t dummy_id = 10;
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(dummy_id);

    // Verify that the frames list is updated correctly when delivering a frame.
    ASSERT_EQ(evs_video_provider->frames_.size(),
              static_cast<size_type_frames>(0));  // Verify that initial number of frames is zero.
    EXPECT_CALL(*client_1, DeliverFrame(_)).WillOnce(testing::Return(true));
    evs_video_provider->deliverFrame(dummy_buffer);
    ASSERT_EQ(evs_video_provider->frames_.size(),
              static_cast<size_type_frames>(1));  // Verify that frame was added to reference list.
    EXPECT_EQ(evs_video_provider->frames_[0].frame_id, dummy_id);
    EXPECT_EQ(evs_video_provider->frames_[0].ref_count,
              static_cast<uint32_t>(1));  // We delivered one frame, so ref_count should be 1;

    // Verify that frame list re-uses entries with ref_count of zero.
    // SetUp
    evs_video_provider->frames_[0].ref_count = 0;
    uint32_t dummy_id_2 = 11;
    BufferDesc dummy_buffer_2 = test_utils::CreateDummyBuffer(dummy_id_2);
    EXPECT_CALL(*client_1, DeliverFrame(_)).WillOnce(testing::Return(true));

    // Test
    evs_video_provider->deliverFrame(dummy_buffer_2);

    // Verify
    ASSERT_EQ(evs_video_provider->frames_.size(),
              static_cast<size_type_frames>(1));  // Verify that reference list did not grow.
    EXPECT_EQ(evs_video_provider->frames_[0].frame_id, dummy_id_2);
    EXPECT_EQ(evs_video_provider->frames_[0].ref_count,
              static_cast<uint32_t>(1));  // We delivered one frame, so ref_count should be 1;
}

TEST_F(EvsVideoProviderTest, deliverFrameNoTakers) {
    // SetUp
    using size_type_frames = std::vector<EvsVideoProvider::FrameRecord>::size_type;
    using size_type_clients = std::list<wp<IVirtualCamera>>::size_type;
    // Create mocked virtual cameras
    sp<MockVirtualCamera> client_1 = new MockVirtualCamera();
    sp<MockVirtualCamera> client_2 = new MockVirtualCamera();
    // Add mocks to list of clients
    evs_video_provider->clients_.emplace_back(client_1);
    evs_video_provider->clients_.emplace_back(client_2);
    // Verify that setup worked.
    ASSERT_EQ(evs_video_provider->GetClientCount(), static_cast<size_type_clients>(2));

    // Create dummy buffer
    uint32_t dummy_id = 10;
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(dummy_id);

    // Verify that we make a "doneWithFrame" call to hw_camera if no client accepts frame.
    size_type_frames expected_size = 0;
    ASSERT_EQ(evs_video_provider->frames_.size(), expected_size);  // Verify that initial number of frames is zero.
    EXPECT_CALL(*client_1, DeliverFrame(_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*client_2, DeliverFrame(_)).WillOnce(testing::Return(false));
    EXPECT_CALL(*mock_hw_camera, doneWithFrame(_));
    evs_video_provider->deliverFrame(dummy_buffer);
    EXPECT_EQ(evs_video_provider->frames_.size(), expected_size);  // Verify that frame was not added to reference list.
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android