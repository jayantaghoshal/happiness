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
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"
#undef LOG_TAG
#define LOG_TAG "SettingsTests"

using namespace SettingsFramework;
using namespace std::chrono_literals;
using json = nlohmann::json;
using persDataType = std::vector<json>;

enum class Timeout { Timeout, NoTimeout };

class SettingTest : public ::testing::Test {
  public:
    SettingTest() {
        // NOTE: Starting server without any existing DB usually takes 150ms, need to take this into account in tests
        system("stop settingstorage-hidl-server");      // NOLINT
        system("rm /data/vendor/vehiclesettings.db*");  // NOLINT
        system("start settingstorage-hidl-server");     // NOLINT
        manager_ = new SettingsManagerHidl{*dispatcher};
    };

    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher =
            tarmac::eventloop::IDispatcher::CreateDispatcher(false);
    android::sp<SettingsManagerHidl> manager_;

    Timeout RunDispatcherUntil(std::chrono::milliseconds timeout, const std::function<bool()>& stop_condition) {
        Timeout timeoutStatus = Timeout::NoTimeout;
        auto stop_condition_with_timeout = [&timeoutStatus, stop_condition] {
            return (timeoutStatus == Timeout::Timeout) || stop_condition();
        };
        tarmac::eventloop::IDispatcher::JobId id = dispatcher->EnqueueWithDelay(timeout, [&timeoutStatus]() {
            ALOGW("Condition timeout");
            timeoutStatus = Timeout::Timeout;
        });
        dispatcher->RunUntil(stop_condition_with_timeout);
        dispatcher->Cancel(id);
        return timeoutStatus;
    }
};

TEST_F(SettingTest, GetReturnsDefaultVeryFirstTime_int) {
    int value = 0;
    Setting<int, UserScope::NOT_USER_RELATED> s1(SettingId::TestSetting1, 1234, manager_);
    s1.setCallback([&](const auto& new_value) { value = new_value.value; });
    EXPECT_EQ(Timeout::NoTimeout, RunDispatcherUntil(500ms, [&]() { return value == 1234; }));
    EXPECT_EQ(1234, value);
}

TEST_F(SettingTest, CallbackNotFiredAfterSettingDestruction) {
    bool called = false;
    {
        Setting<int, UserScope::NOT_USER_RELATED> s1(SettingId::TestSetting1, 1234, manager_);
        s1.setCallback([&](const auto& new_value) { called = true; });
    }
    EXPECT_EQ(Timeout::Timeout, RunDispatcherUntil(500ms, [&]() { return called; }));
    EXPECT_FALSE(called);
}

TEST_F(SettingTest, NewInstanceOfSettingUsingPreviouslySetValueAndNotDefault) {
    {
        std::chrono::seconds value = 0s;
        Setting<std::chrono::seconds, UserScope::NOT_USER_RELATED> s1(SettingId::TestSetting2, 1234s, manager_);
        s1.setCallback([&](const auto& new_value) {
            value = new_value.value;
            s1.setCallback(nullptr);  // Don't set any more values into Promise
        });
        EXPECT_EQ(Timeout::NoTimeout, RunDispatcherUntil(500ms, [&]() { return value == 1234s; }));
        EXPECT_EQ(1234s, value);

        // Set value and destroy object to make sure value is saved across instances
        s1.set(std::chrono::minutes{9876});
    }
    {
        std::chrono::seconds value2 = 0s;
        // New instance of settings should have the same value that we just set(), not default
        Setting<std::chrono::seconds, UserScope::NOT_USER_RELATED> s1(SettingId::TestSetting2, 1234s, manager_);
        s1.setCallback([&](const auto& new_value) { value2 = new_value.value; });
        EXPECT_EQ(Timeout::NoTimeout, RunDispatcherUntil(500ms, [&]() { return value2 == 9876min; }));
        EXPECT_EQ(9876min, value2);
    }
}

/* TODO(ee): Enable this test once gtest is compiled with -fexceptions
TEST_F(SettingTest, EmptyNameShouldNotWork) {
    EXPECT_ANY_THROW(Setting<std::string, UserScope::USER> s1("", "s1default", manager_));
}
*/
