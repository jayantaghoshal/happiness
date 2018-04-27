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

class EvsVideoProviderTest : public ::testing::Test {
  protected:
    sp<MockHwCamera> mock_hw_camera;
    sp<EvsVideoProvider> evs_video_provider;
    void SetUp() override {
        mock_hw_camera = new MockHwCamera();
        evs_video_provider = new EvsVideoProvider(mock_hw_camera);
    }

    void TearDown() override {
        mock_hw_camera.clear();
        evs_video_provider.clear();
    }
};

TEST_F(EvsVideoProviderTest, ConstructObject) { EXPECT_NE(evs_video_provider, nullptr); }

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

}  // namespace vcc_implementation
}  // namespace V1_0
}  // namespace evs
}  // namespace automotive
}  // namespace hardware
}  // namespace android