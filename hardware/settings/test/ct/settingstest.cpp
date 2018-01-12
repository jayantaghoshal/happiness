/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <IDispatcher.h>
#include <cutils/log.h>
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include "libsettings/setting.h"
#include "libsettings/settingsmanagerhidl.h"
#undef LOG_TAG
#define LOG_TAG "SettingsTests"

using namespace SettingsFramework;
using namespace std::chrono_literals;
using json = nlohmann::json;
using persDataType = std::vector<json>;

class SettingTest : public ::testing::Test {
  public:
    SettingTest() {
        system("stop settingstorage-hidl-server");   // NOLINT
        system("start settingstorage-hidl-server");  // NOLINT
        manager = new SettingsManagerHidl{*dispatcher},
        context = std::make_shared<SettingsFramework::SettingsContext>(*dispatcher, manager);
    };

    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher =
            tarmac::eventloop::IDispatcher::CreateDispatcher(false);
    android::sp<SettingsManagerHidl> manager;
    std::shared_ptr<SettingsFramework::SettingsContext> context;

    void RunDispatcherUntil(std::chrono::milliseconds timeout, const std::function<bool()>& stop_condition) {
        bool stop2 = false;
        auto stop_condition_with_timeout = [&stop2, stop_condition] { return stop2 || stop_condition(); };
        tarmac::eventloop::IDispatcher::JobId id = dispatcher->EnqueueWithDelay(timeout, [&stop2]() {
            ALOGD("Condition timeout");
            stop2 = true;
        });
        dispatcher->RunUntil(stop_condition_with_timeout);
        dispatcher->Cancel(id);
    }
};

TEST_F(SettingTest, GetReturnsDefaultVeryFirstTime_int) {
    int value = 0;
    Setting<int, UserScope::NOT_USER_RELATED> s1("testsetting_int", 1234, context);
    s1.setCallback([&]() { value = s1.get(); });
    RunDispatcherUntil(200ms, [&]() { return value == 1234; });
    EXPECT_EQ(1234, value);
}

TEST_F(SettingTest, CallbackNotFiredAfterSettingDestruction) {
    bool called = false;
    {
        Setting<int, UserScope::NOT_USER_RELATED> s1("testsetting_int", 1234, context);
        s1.setCallback([&]() { called = true; });
    }
    RunDispatcherUntil(200ms, [&]() { return called; });
    EXPECT_FALSE(called);
}

TEST_F(SettingTest, NewInstanceOfSettingUsingPreviouslySetValueAndNotDefault) {
    {
        std::chrono::seconds value = 0s;
        Setting<std::chrono::seconds, UserScope::NOT_USER_RELATED> s1("test.durationsetting", 1234s, context);
        s1.setCallback([&]() {
            value = s1.get();
            s1.setCallback(nullptr);  // Don't set any more values into Promise
        });
        RunDispatcherUntil(200ms, [&]() { return value == 1234s; });
        EXPECT_EQ(1234s, value);

        // Set value and destroy object to make sure value is saved across instances
        s1.set(std::chrono::minutes{9876});
    }
    {
        std::chrono::seconds value2 = 0s;
        // New instance of settings should have the same value that we just set(), not default
        Setting<std::chrono::seconds, UserScope::NOT_USER_RELATED> s1("test.durationsetting", 1234s, context);
        s1.setCallback([&]() { value2 = s1.get(); });
        RunDispatcherUntil(200ms, [&]() { return value2 == 9876min; });
        EXPECT_EQ(9876min, value2);
    }
}

/* TODO(ee): Enable this test once gtest is compiled with -fexceptions
TEST_F(SettingTest, EmptyNameShouldNotWork) {
    EXPECT_ANY_THROW(Setting<std::string, UserScope::USER> s1("", "s1default", context));
}
*/
