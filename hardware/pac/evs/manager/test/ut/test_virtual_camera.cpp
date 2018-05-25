/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <i_evs_video_provider.h>
#include <virtual_camera.h>
#include "mock_hw_camera.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

using testing::_;  // Matches any type

class MockIEvsVideoProvider : public IEvsVideoProvider {
  public:
    MOCK_METHOD0(MakeVirtualCamera, sp<IVirtualCamera>());
    MOCK_METHOD1(DisownVirtualCamera, void(sp<IVirtualCamera>& virtual_camera));
    MOCK_METHOD0(GetHwCamera, sp<IEvsCamera>());
    MOCK_METHOD0(GetClientCount, std::list<wp<IVirtualCamera>>::size_type());
    MOCK_METHOD0(RequestVideoStream, Return<EvsResult>());
    MOCK_METHOD0(ReleaseVideoStream, void());
    MOCK_METHOD1(DoneWithFrame, void(const BufferDesc& buffer));
    MOCK_METHOD1(ChangeFramesInFlight, bool(uint32_t extra_frames));
    MOCK_METHOD1(deliverFrame, Return<void>(const BufferDesc& buffer));
};

class MockIEvsCameraStream : public IEvsCameraStream {
  public:
    MOCK_METHOD1(deliverFrame, Return<void>(const BufferDesc& buffer));
};

class VirtualCameraTest : public ::testing::Test {
  protected:
    sp<MockHwCamera> mock_hw_camera;
    sp<MockIEvsVideoProvider> mock_input_stream;
    sp<MockIEvsCameraStream> mock_output_stream;
    sp<VirtualCamera> virtual_camera;
    void SetUp() override {
        mock_hw_camera = new MockHwCamera();
        mock_input_stream = new MockIEvsVideoProvider();
        mock_output_stream = new MockIEvsCameraStream();
        virtual_camera = new VirtualCamera(mock_input_stream);
    }

    void TearDown() override {
        mock_hw_camera.clear();
        mock_input_stream.clear();
        mock_output_stream.clear();
        virtual_camera.clear();
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

TEST_F(VirtualCameraTest, ConstructObject) {
    EXPECT_NE(virtual_camera, nullptr);
}

TEST_F(VirtualCameraTest, DeleteObject) {
    virtual_camera.clear();
    EXPECT_EQ(virtual_camera, (VirtualCamera*)nullptr);
}

TEST_F(VirtualCameraTest, getCameraInfo) {
    // Implement dummy function to use as input
    IEvsCamera::getCameraInfo_cb dummy_get_camera_info_cb = [](CameraDesc info) { info.vendorFlags++; };

    // Verify that the getCameraInfo method is called on the mock of the hw camera
    EXPECT_CALL(*mock_input_stream, GetHwCamera()).WillOnce(testing::Return(testing::ByMove(mock_hw_camera)));
    EXPECT_CALL(*mock_hw_camera, getCameraInfo(_)).WillOnce(testing::Return(testing::ByMove(Void())));
    virtual_camera->getCameraInfo(dummy_get_camera_info_cb);
}

TEST_F(VirtualCameraTest, getExtendedInfo) {
    uint32_t dummy_identifier = 10;
    int32_t dummy_returnvalue = 42;
    EXPECT_CALL(*mock_input_stream, GetHwCamera()).WillOnce(testing::Return(testing::ByMove(mock_hw_camera)));
    EXPECT_CALL(*mock_hw_camera, getExtendedInfo(dummy_identifier))
            .WillRepeatedly(testing::Return(testing::ByMove(dummy_returnvalue)));
    int32_t value_returned = virtual_camera->getExtendedInfo(dummy_identifier);
    EXPECT_EQ(value_returned, dummy_returnvalue);
}

TEST_F(VirtualCameraTest, setExtendedInfo) {
    uint32_t dummy_identifier = 10;
    int32_t dummy_setvalue = 42;
    EXPECT_CALL(*mock_input_stream, GetHwCamera()).WillOnce(testing::Return(testing::ByMove(mock_hw_camera)));
    EXPECT_CALL(*mock_hw_camera, setExtendedInfo(dummy_identifier, dummy_setvalue))
            .WillOnce(testing::Return(testing::ByMove(EvsResult::OK)));
    EvsResult result = virtual_camera->setExtendedInfo(dummy_identifier, dummy_setvalue);
    EXPECT_EQ(result, EvsResult::OK);
}

TEST_F(VirtualCameraTest, startVideoStreamWhenStreamAlreadyRunning) {
    // Test that we return an error if stream is already running.
    // SetUp
    virtual_camera->output_stream_state_ = StreamState::RUNNING;

    // Test and verify
    EXPECT_EQ(virtual_camera->startVideoStream(mock_output_stream), EvsResult::STREAM_ALREADY_RUNNING);
}

TEST_F(VirtualCameraTest, startVideoStreamUnderlyingServiceError) {
    // EvsCameraStream returns error -> expect call to return "UNDERLYING_SERVICE_ERROR"
    // SetUp
    ASSERT_FALSE(virtual_camera->IsStreaming());
    EXPECT_CALL(*mock_input_stream, RequestVideoStream())
            .WillOnce(testing::Return(testing::ByMove(EvsResult::UNDERLYING_SERVICE_ERROR)));
    // Test and verify
    EXPECT_EQ(virtual_camera->startVideoStream(mock_output_stream), EvsResult::UNDERLYING_SERVICE_ERROR);
}

TEST_F(VirtualCameraTest, startVideoStream) {
    // Happy path test, we are not already streaming, and the EvsCameraStream returns OK.
    // SetUp
    ASSERT_FALSE(virtual_camera->IsStreaming());
    EXPECT_CALL(*mock_input_stream, RequestVideoStream())
            .WillRepeatedly(testing::Return(testing::ByMove(EvsResult::OK)));
    // Test and verify
    EXPECT_EQ(virtual_camera->startVideoStream(mock_output_stream), EvsResult::OK);
    EXPECT_TRUE(virtual_camera->IsStreaming());
}

TEST_F(VirtualCameraTest, stopVideoStreamWhenStreamNotRunning) {
    // The intended behaviour here is for the method to do nothing.
    // SetUp
    ASSERT_FALSE(virtual_camera->IsStreaming());
    // Test and verify
    virtual_camera->stopVideoStream();  // Implicitly checks that no "uninteresting" functions calls are made.
    ASSERT_FALSE(virtual_camera->IsStreaming());
}

TEST_F(VirtualCameraTest, stopVideoStream) {
    // The intended behaviour here is for the method to close the output stream and inform the input
    // stream that we no longer need it.
    // SetUp
    virtual_camera->output_stream_ = mock_output_stream;
    virtual_camera->output_stream_state_ = StreamState::RUNNING;
    EXPECT_CALL(*mock_output_stream, deliverFrame(_)).WillOnce(testing::Return(testing::ByMove(Void())));
    EXPECT_CALL(*mock_input_stream, ReleaseVideoStream());
    // Test and verify
    virtual_camera->stopVideoStream();
    ASSERT_FALSE(virtual_camera->IsStreaming());
}

TEST_F(VirtualCameraTest, DeliverFrameOutputStreamStopped) {
    // We should not accept frames if the output stream is stopped, i.e. if we have no use for them.
    // SetUp
    virtual_camera->output_stream_ = mock_output_stream;
    virtual_camera->output_stream_state_ = StreamState::STOPPED;
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(1);
    auto initial_size = virtual_camera->frames_held_.size();

    // Test and verify
    bool result = virtual_camera->DeliverFrame(dummy_buffer);
    EXPECT_FALSE(result);
    auto size_after_call = virtual_camera->frames_held_.size();
    EXPECT_EQ(size_after_call, initial_size);
}

TEST_F(VirtualCameraTest, DeliverFrameEndOfStream) {
    // Test handling of input of an empty buffer.
    // An empty buffer indicates end of stream. Normally the input stream should not end
    // without a call to stopVideoStream(), so if we receive an empty buffer anyway we
    // close the output stream since we have no video to deliver.
    // SetUp
    BufferDesc empty_buffer = {};
    virtual_camera->output_stream_ = mock_output_stream;
    virtual_camera->output_stream_state_ = StreamState::RUNNING;
    EXPECT_CALL(*mock_output_stream, deliverFrame(_));
    EXPECT_CALL(*mock_input_stream, ReleaseVideoStream());

    // Test and verify
    bool result = virtual_camera->DeliverFrame(empty_buffer);
    EXPECT_FALSE(result);
    EXPECT_EQ(virtual_camera->output_stream_state_, StreamState::STOPPED);
}

TEST_F(VirtualCameraTest, DeliverFrameTooManyFramesHeld) {
    // Test handling of too many frames held, should reject frame unless it is end of stream marker.
    // SetUp
    virtual_camera->output_stream_ = mock_output_stream;
    virtual_camera->output_stream_state_ = StreamState::RUNNING;
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(1);
    BufferDesc empty_buffer = {};
    virtual_camera->frames_allowed_ = 0;

    // Test and verify
    // General case
    bool result = virtual_camera->DeliverFrame(dummy_buffer);
    EXPECT_FALSE(result);
    // End of stream case
    EXPECT_CALL(*mock_output_stream, deliverFrame(_));
    EXPECT_CALL(*mock_input_stream, ReleaseVideoStream());  // Implies that stopVideoStream was called.
    bool result_end_of_stream = virtual_camera->DeliverFrame(empty_buffer);
    EXPECT_FALSE(result_end_of_stream);
}

TEST_F(VirtualCameraTest, DeliverFrame) {
    // Test the "Happy Path"
    // SetUp
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(1);
    virtual_camera->output_stream_ = mock_output_stream;
    virtual_camera->output_stream_state_ = StreamState::RUNNING;
    EXPECT_CALL(*mock_output_stream, deliverFrame(_));

    // Test and verify
    bool result = virtual_camera->DeliverFrame(dummy_buffer);
    EXPECT_TRUE(result);
    uint32_t held_frame_id = virtual_camera->frames_held_.begin()->bufferId;
    EXPECT_EQ(held_frame_id, dummy_buffer.bufferId);
}

TEST_F(VirtualCameraTest, doneWithFrameEmptyBuffer) {
    // Test handling of the empty buffer used to indicate end of stream.
    // SetUp
    BufferDesc empty_buffer = {};

    // Test and verify
    auto result = virtual_camera->doneWithFrame(empty_buffer);  // Implicitly verifies that no function calls were made.
    EXPECT_TRUE(result.isOk());
}

TEST_F(VirtualCameraTest, doneWithFrameUnrecognizedBufferId) {
    // Test handling of a buffer with an unknown Id.
    // SetUp
    BufferDesc dummy_buffer = test_utils::CreateDummyBuffer(1);

    // Test and verify
    auto result = virtual_camera->doneWithFrame(dummy_buffer);  // Implicitly verifies that no function calls were made.
    EXPECT_TRUE(result.isOk());
}

TEST_F(VirtualCameraTest, doneWithFrame) {
    // Test handling of a known buffer, the "Happy Path" case
    // SetUp
    BufferDesc dummy_buffer_1 = test_utils::CreateDummyBuffer(1);
    BufferDesc dummy_buffer_2 = test_utils::CreateDummyBuffer(2);
    BufferDesc dummy_buffer_3 = test_utils::CreateDummyBuffer(3);
    virtual_camera->frames_held_.emplace_back(dummy_buffer_1);
    virtual_camera->frames_held_.emplace_back(dummy_buffer_2);
    virtual_camera->frames_held_.emplace_back(dummy_buffer_3);

    EXPECT_CALL(*mock_input_stream, DoneWithFrame(_));
    auto initial_size = virtual_camera->frames_held_.size();

    // Test and verify
    auto result = virtual_camera->doneWithFrame(dummy_buffer_1);
    EXPECT_TRUE(result.isOk());
    // Verify that entry was deleted
    auto size_after_call = virtual_camera->frames_held_.size();
    EXPECT_NE(size_after_call, initial_size);
    virtual_camera->doneWithFrame(
            dummy_buffer_1);  // Implicitly verifies deletion since DoneWithFrame is not called on mock_input_stream
}

TEST_F(VirtualCameraTest, ShutDownWhenStreaming) {
    // If ShutDown is somehow called when the camera is streaming we should return all held frames before destructing.
    // SetUp
    // Ensure we are streaming
    EXPECT_CALL(*mock_input_stream, RequestVideoStream()).WillOnce(testing::Return(testing::ByMove(EvsResult::OK)));
    virtual_camera->startVideoStream(mock_output_stream);
    ASSERT_TRUE(virtual_camera->IsStreaming());
    // Ensure we hold buffers
    BufferDesc dummy_buffer_1 = test_utils::CreateDummyBuffer(1);
    BufferDesc dummy_buffer_2 = test_utils::CreateDummyBuffer(2);
    EXPECT_CALL(*mock_output_stream, deliverFrame(_)).Times(2);
    ASSERT_TRUE(virtual_camera->DeliverFrame(dummy_buffer_1));
    ASSERT_TRUE(virtual_camera->DeliverFrame(dummy_buffer_1));
    // Expected calls from ShutDown
    EXPECT_CALL(*mock_input_stream, DoneWithFrame(_)).Times(2);

    // Test and verify
    virtual_camera->ShutDown();
    EXPECT_FALSE(virtual_camera->IsStreaming());
}

TEST_F(VirtualCameraTest, setMaxFramesInFlightDecreaseError) {
    // SetUp
    uint32_t buffer_count = 4;
    uint32_t allowed_initial = virtual_camera->GetAllowedBuffers();
    ASSERT_TRUE(buffer_count < allowed_initial);
    EXPECT_CALL(*mock_input_stream, ChangeFramesInFlight(0)).WillOnce(testing::Return(false));

    // Test and verify
    EvsResult result = virtual_camera->setMaxFramesInFlight(buffer_count);
    EXPECT_TRUE(result == EvsResult::UNDERLYING_SERVICE_ERROR);           // We got the expected error
    EXPECT_TRUE(virtual_camera->GetAllowedBuffers() == allowed_initial);  // Allowed buffers has not changed
}

TEST_F(VirtualCameraTest, setMaxFramesInFlightDecrease) {
    // SetUp
    uint32_t buffer_count = 4;
    ASSERT_TRUE(buffer_count < virtual_camera->GetAllowedBuffers());
    EXPECT_CALL(*mock_input_stream, ChangeFramesInFlight(0)).WillOnce(testing::Return(true));

    // Test and verify
    EvsResult result = virtual_camera->setMaxFramesInFlight(buffer_count);
    EXPECT_TRUE(result == EvsResult::OK);                              // We got the expected result
    EXPECT_TRUE(virtual_camera->GetAllowedBuffers() == buffer_count);  // Allowed buffers has changed to buffer_count
}

TEST_F(VirtualCameraTest, setMaxFramesInFlightFramesNotAvailable) {
    // SetUp
    uint32_t buffer_count = 12;
    uint32_t allowed_initial = virtual_camera->GetAllowedBuffers();
    ASSERT_TRUE(buffer_count > allowed_initial);
    uint32_t diff = buffer_count - allowed_initial;
    EXPECT_CALL(*mock_input_stream, ChangeFramesInFlight(diff)).WillOnce(testing::Return(false));

    // Test and verify
    EvsResult result = virtual_camera->setMaxFramesInFlight(buffer_count);
    EXPECT_TRUE(result == EvsResult::BUFFER_NOT_AVAILABLE);               // We got the expected error
    EXPECT_TRUE(virtual_camera->GetAllowedBuffers() == allowed_initial);  // Allowed buffers has not changed
}

TEST_F(VirtualCameraTest, setMaxFramesInFlightFramesAvailable) {
    // SetUp
    uint32_t buffer_count = 12;
    uint32_t allowed_initial = virtual_camera->GetAllowedBuffers();
    ASSERT_TRUE(buffer_count > allowed_initial);
    uint32_t diff = buffer_count - allowed_initial;
    EXPECT_CALL(*mock_input_stream, ChangeFramesInFlight(diff)).WillOnce(testing::Return(true));

    // Test and verify
    EvsResult result = virtual_camera->setMaxFramesInFlight(buffer_count);
    EXPECT_TRUE(result == EvsResult::OK);                              // We got the expected result
    EXPECT_TRUE(virtual_camera->GetAllowedBuffers() == buffer_count);  // Allowed buffers has changed to buffer_count
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android