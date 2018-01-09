/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>
#include <sys/wait.h>
#include "apix_test_helpers.h"

#define LOG_TAG "ApixMIITest"
#include <cutils/log.h>

TEST(ApixMIITest, ApixMIIPingTest) {
    ALOGI("ApixMIIPingTest starting...");
    // Arrange
    const std::string address = "198.18.24.1";
    int attempts = 5;
    std::string result;
    // Act & Assert
    EXPECT_EQ(true, Ping(address, attempts, result));
}

TEST(ApixMIITest, ApixMIISetupSocketTest) {
    ALOGI("ApixMIISetupSocketTest starting...");
    // Arrange
    int command_result = system("ip netns exec vcc ./vendor/bin/apix_setup_sock_util");
    // Act & Assert
    ASSERT_TRUE(WIFEXITED(command_result) && WEXITSTATUS(command_result) == 0) << "Setup TCP socket failed";
}
