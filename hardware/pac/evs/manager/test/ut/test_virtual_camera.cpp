/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

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
    MOCK_METHOD1(DisownVirtualCamera, void(const sp<IVirtualCamera>& virtual_camera));
    MOCK_METHOD0(GetHwCamera, sp<IEvsCamera>());
    MOCK_METHOD0(GetClientCount, std::list<wp<IVirtualCamera>>::size_type());
    MOCK_METHOD0(RequestVideoStream, Return<EvsResult>());
    MOCK_METHOD0(ReleaseVideoStream, void());
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

TEST_F(VirtualCameraTest, ConstructObject) { EXPECT_NE(virtual_camera, nullptr); }

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
    virtual_camera->output_stream_state_ = StreamState::RUNNING;
    virtual_camera->output_stream_ = mock_output_stream;
    EXPECT_CALL(*mock_output_stream, deliverFrame(_)).WillOnce(testing::Return(testing::ByMove(Void())));
    EXPECT_CALL(*mock_input_stream, ReleaseVideoStream());
    // Test and verify
    virtual_camera->stopVideoStream();
    ASSERT_FALSE(virtual_camera->IsStreaming());
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android