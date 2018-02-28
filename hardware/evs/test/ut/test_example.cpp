/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>

#define LOG_TAG "EvsExampleTest"
#include <log/log.h>

namespace {

TEST(EVSExampleTestCase, LogTest) {
    ALOGD("EVS Debug Log Test");
    ALOGI("EVS Info Log Test");
}

TEST(EVSExampleTestCase, NotEqualTest) {
    ASSERT_NE(1, 5);
    ASSERT_STRNE("Test", nullptr);
}

}  // namespace
