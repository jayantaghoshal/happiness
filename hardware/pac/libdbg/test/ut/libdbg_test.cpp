/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>
#include <libdbg.h>

#include "stub_property_set.h"

namespace {

class libdbgTest : public ::testing::Test {
  protected:
    void TearDown() override { stub_property_reset(); }
};

class klogTest : public libdbgTest {
  protected:
    void SetUp() override {
        const char* kPropertyValueSet = "not running";
        stub_property_set(PROPERTY_LOGCAT_STATUS, kPropertyValueSet);

        // Setting the log level below warn, error and fatal since klog is the main focus of the tests from here on out
        kPropertyValueSet = "verbose";
        stub_property_set(PROPERTY_LOG_LEVEL, kPropertyValueSet);
    }
};

TEST_F(libdbgTest, PropertyEqualTest) {
    const char* kPropertyValueSet = "error";

    stub_property_set(PROPERTY_LOG_LEVEL, kPropertyValueSet);

    char property_value_get[strlen(kPropertyValueSet)];

    property_get(PROPERTY_LOG_LEVEL, property_value_get, nullptr);

    EXPECT_TRUE(strcmp(kPropertyValueSet, property_value_get) == 0);
}

TEST_F(libdbgTest, dbgFATALTest) {
    const char* kPropertyValueSet = "fatal";

    stub_property_set(PROPERTY_LOG_LEVEL, kPropertyValueSet);

    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_FATAL, __FILENAME__, __FUNCTION__, __LINE__, "fatal message"));
    EXPECT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_ERROR, __FILENAME__, __FUNCTION__, __LINE__, "fatal message"));
}

TEST_F(libdbgTest, dbgERRORTest) {
    const char* kPropertyValueSet = "error";
    stub_property_set(PROPERTY_LOG_LEVEL, kPropertyValueSet);

    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_ERROR, __FILENAME__, __FUNCTION__, __LINE__, "error message"));
    EXPECT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_WARN, __FILENAME__, __FUNCTION__, __LINE__, "error message"));
}

TEST_F(libdbgTest, dbgWARNTest) {
    const char* kPropertyValueSet = "warn";
    stub_property_set(PROPERTY_LOG_LEVEL, kPropertyValueSet);

    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_WARN, __FILENAME__, __FUNCTION__, __LINE__, "warn message"));
    EXPECT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_INFO, __FILENAME__, __FUNCTION__, __LINE__, "warn message"));
}

TEST_F(libdbgTest, dbgDEBUGTest) {
    const char* kPropertyValueSet = "debug";
    stub_property_set(PROPERTY_LOG_LEVEL, kPropertyValueSet);

    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_DEBUG, __FILENAME__, __FUNCTION__, __LINE__, "debug message"));
    EXPECT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_VERBOSE, __FILENAME__, __FUNCTION__, __LINE__, "debug message"));
}

TEST_F(libdbgTest, dbgINFOTest) {
    const char* kPropertyValueSet = "info";
    stub_property_set(PROPERTY_LOG_LEVEL, kPropertyValueSet);

    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_INFO, __FILENAME__, __FUNCTION__, __LINE__, "info message"));
    EXPECT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_DEBUG, __FILENAME__, __FUNCTION__, __LINE__, "info message"));
}

TEST_F(libdbgTest, dbgVERBOSETest) {
    const char* kPropertyValueSet = "verbose";
    stub_property_set(PROPERTY_LOG_LEVEL, kPropertyValueSet);

    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_VERBOSE, __FILENAME__, __FUNCTION__, __LINE__, "verbose message"));
    EXPECT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_DEFAULT, __FILENAME__, __FUNCTION__, __LINE__, "verbose message"));
}

TEST_F(klogTest, klogLogLevelBoundsTest) {
    EXPECT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_INFO, __FILENAME__, __FUNCTION__, __LINE__, "klog info message"));
    EXPECT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_SILENT, __FILENAME__, __FUNCTION__, __LINE__, "klog silent message"));
}

TEST_F(klogTest, klogFATALTest) {
    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_FATAL, __FILENAME__, __FUNCTION__, __LINE__, "klog fatal message"));
}

TEST_F(klogTest, klogERRORTest) {
    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_ERROR, __FILENAME__, __FUNCTION__, __LINE__, "klog error message"));
}

TEST_F(klogTest, klogWARNTest) {
    EXPECT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_WARN, __FILENAME__, __FUNCTION__, __LINE__, "klog warn message"));
}
}  // namespace
