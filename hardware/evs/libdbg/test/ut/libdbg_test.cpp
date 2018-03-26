/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/properties.h>
#include <gtest/gtest.h>
#include <cstdio>

#include <libdbg.h>

#define PROPERTY_LOGLEVEL "debug.evs.loglevel"

namespace {

TEST(libdbgTest, PropertyEqualTest) {
    const char* kPropertyValueSet = "error";

    property_set(PROPERTY_LOGLEVEL, kPropertyValueSet);

    char property_value_get[strlen(kPropertyValueSet)];

    property_get(PROPERTY_LOGLEVEL, property_value_get, nullptr);

    ASSERT_TRUE(strcmp(kPropertyValueSet, property_value_get) == 0);
}

TEST(libdbgTest, dbgFATALTest) {
    const char* kPropertyValueSet = "fatal";

    property_set(PROPERTY_LOGLEVEL, kPropertyValueSet);

    ASSERT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_FATAL, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
    ASSERT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_ERROR, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
}

TEST(libdbgTest, dbgERRORTest) {
    const char* kPropertyValueSet = "error";
    property_set(PROPERTY_LOGLEVEL, kPropertyValueSet);

    ASSERT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_ERROR, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
    ASSERT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_WARN, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
}

TEST(libdbgTest, dbgWARNTest) {
    const char* kPropertyValueSet = "warn";
    property_set(PROPERTY_LOGLEVEL, kPropertyValueSet);

    ASSERT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_WARN, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
    ASSERT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_INFO, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
}

TEST(libdbgTest, dbgDEBUGTest) {
    const char* kPropertyValueSet = "debug";
    property_set(PROPERTY_LOGLEVEL, kPropertyValueSet);

    ASSERT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_DEBUG, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
    ASSERT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_VERBOSE, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
}

TEST(libdbgTest, dbgINFOTest) {
    const char* kPropertyValueSet = "info";
    property_set(PROPERTY_LOGLEVEL, kPropertyValueSet);

    ASSERT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_INFO, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
    ASSERT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_DEBUG, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
}

TEST(libdbgTest, dbgVERBOSETest) {
    const char* kPropertyValueSet = "verbose";
    property_set(PROPERTY_LOGLEVEL, kPropertyValueSet);

    ASSERT_TRUE(pdgb(LOG_TAG, ANDROID_LOG_VERBOSE, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
    ASSERT_FALSE(pdgb(LOG_TAG, ANDROID_LOG_DEFAULT, __FILENAME__, __FUNCTION__, __LINE__, "the message"));
}

}  // namespace