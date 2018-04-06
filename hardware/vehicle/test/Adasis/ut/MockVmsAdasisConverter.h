/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <log/log.h>
#include "VmsAdasisConverter.h"

namespace geo {
namespace vms {
namespace vms_adasis_converter {

class MockVmsAdasisConverter : public VmsAdasisConverter {
  public:
    MOCK_METHOD0(StartVmsAdasisConverter, bool());
    MOCK_METHOD0(StopVmsAdasisConverter, void());
    MOCK_METHOD1(HandleVmsPacket, void(const ::geo::vms::v1::VmsPacket&));
    MOCK_CONST_METHOD0(GetLayers, std::vector<::geo::vms::v1::VmsLayerTypeAndVersion::VmsLayerType>());
    MOCK_METHOD2(GetNextMessageForType, StatusCode(AdasisMessageType message_type, AdasisMessage* message));
};

}  // namespace vms_adasis_converter
}  // namespace vms
}