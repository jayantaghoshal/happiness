/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <evs_camera_stream.h>
#include "mock_hw_camera.h"

namespace android {
namespace hardware {
namespace automotive {
namespace evs {
namespace V1_0 {
namespace vcc_implementation {

using size_type_clients = std::list<wp<IVirtualCamera>>::size_type;

using android::hardware::automotive::evs::V1_0::vcc_implementation::EvsCameraStream;

class EvsCameraStreamTest : public ::testing::Test {
  protected:
    sp<MockHwCamera> mock_hw_camera;
    sp<EvsCameraStream> evs_camera_stream;
    void SetUp() override {
        mock_hw_camera = new MockHwCamera();
        evs_camera_stream = new EvsCameraStream(mock_hw_camera);
    }

    void TearDown() override {
        mock_hw_camera.clear();
        evs_camera_stream.clear();
    }
};

TEST_F(EvsCameraStreamTest, ConstructObject) { EXPECT_NE(evs_camera_stream, nullptr); }

TEST_F(EvsCameraStreamTest, DeleteObject) {
    evs_camera_stream.clear();
    EXPECT_EQ(evs_camera_stream, (EvsCameraStream*)nullptr);
}

TEST_F(EvsCameraStreamTest, MakeAndDisownClient) {
    size_type_clients client_count_initial = evs_camera_stream->GetClientCount();
    size_type_clients client_count_initial_expected = 0;
    EXPECT_EQ(client_count_initial, client_count_initial_expected);

    sp<IVirtualCamera> evs_camera = evs_camera_stream->MakeVirtualCamera();
    size_type_clients client_count_after_make = evs_camera_stream->GetClientCount();
    EXPECT_EQ(client_count_after_make, client_count_initial + 1);

    evs_camera_stream->DisownVirtualCamera(evs_camera);
    size_type_clients client_count_after_disown = evs_camera_stream->GetClientCount();
    EXPECT_EQ(client_count_after_disown, client_count_initial);
}

TEST_F(EvsCameraStreamTest, MakeAndDisownMultipleClients) {
    sp<IVirtualCamera> client_1 = evs_camera_stream->MakeVirtualCamera();
    sp<IVirtualCamera> client_2 = evs_camera_stream->MakeVirtualCamera();
    size_type_clients client_count_after_make = evs_camera_stream->GetClientCount();
    EXPECT_EQ(client_count_after_make, static_cast<size_type_clients>(2));

    evs_camera_stream->DisownVirtualCamera(client_1);
    evs_camera_stream->DisownVirtualCamera(client_2);
    size_type_clients client_count_after_disown = evs_camera_stream->GetClientCount();
    EXPECT_EQ(client_count_after_disown, static_cast<size_type_clients>(0));
}

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android