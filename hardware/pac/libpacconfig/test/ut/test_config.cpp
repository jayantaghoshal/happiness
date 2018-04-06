/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>
#include <pacconfig/config.h>
#include "stub_properties.h"

namespace pac {
namespace config {

class PacConfigTest : public ::testing::Test {
  protected:
    static void SetUpTestCase() { property_reset_stub_value(); }
};

TEST_F(PacConfigTest, EvsServicesEnabled) {
    property_set_stub_value(1);
    ASSERT_TRUE(PacConfig::Evs::IsEvsServicesEnabled());
}

TEST_F(PacConfigTest, EvsServicesDisabled) {
    property_set_stub_value(0);
    ASSERT_FALSE(PacConfig::Evs::IsEvsServicesEnabled());
}

}  // namespace config
}  // namespace pac
