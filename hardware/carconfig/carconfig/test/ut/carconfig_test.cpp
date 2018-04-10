/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/**
 * Unit test user facing carconfig api.
 *
 * Production code under test:
 *   carconfig/src/carconfig_reader.cpp
 *
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include "carconfig_reader.h"

class VtsCarConfigUT : public ::testing::Test {
  public:
    CarConfigReader rd;

  protected:
};

TEST_F(VtsCarConfigUT, getRawValue_GivenParameterOutOfMaxRange_ExceptionShouldBeThown) {
    ASSERT_THROW(rd.getRawValue(0), std::out_of_range);
    ASSERT_THROW(rd.getRawValue(1779), std::out_of_range);
}

TEST_F(VtsCarConfigUT, getValue_GivenParameterOutOfMaxRange_ExceptionShouldBeThown) {
    ASSERT_THROW(rd.getValue(0), std::out_of_range);
    ASSERT_THROW(rd.getValue(1779), std::out_of_range);
}