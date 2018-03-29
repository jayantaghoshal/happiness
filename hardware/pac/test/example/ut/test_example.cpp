/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>

#define LOG_TAG "PacExampleTest"
#include <log/log.h>

namespace {

TEST(PacExampleTestCase, LogTest) {
    ALOGD("PAC Debug Log Test");
    ALOGI("PAC Info Log Test");
}

TEST(PacExampleTestCase, NotEqualTest) {
    ASSERT_NE(1, 5);
    ASSERT_STRNE("Test", nullptr);
}

}  // namespace
