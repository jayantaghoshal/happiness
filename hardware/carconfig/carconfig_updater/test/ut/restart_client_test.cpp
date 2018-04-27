/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/**
 * Unit test of the restart_client file.
 *
 * Production code under test:
 *   ihu_cedric/carconfig_updater/src/restart_client.cpp
 *
 */

#include "restart_client.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class RestartClientTestFixture : public ::testing::Test {
  public:
    restartClient r;

    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(RestartClientTestFixture, restart) {
    EXPECT_TRUE(r.restart());
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
