/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "libsettings/setting.h"
#include "libsettings/settingsmanagerhidl.h"
#undef LOG_TAG
#define LOG_TAG "Settings"

using namespace SettingsFramework;

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    tarmac::eventloop::IDispatcher& dispatcher = tarmac::eventloop::IDispatcher::GetDefaultDispatcher();
    android::sp<SettingsManagerHidl> manager = new SettingsManagerHidl(dispatcher);
    std::shared_ptr<SettingsFramework::SettingsContext> context =
            std::make_shared<SettingsFramework::SettingsContext>(dispatcher, manager);

    ALOGI("Init setting");
    std::unique_ptr<Setting<int>> s1;
    dispatcher.Enqueue([&]() {
        s1 = std::make_unique<Setting<int, UserScope::NOT_USER_RELATED>>("s1", 1234, context);
        s1->setCallback([&]() {
            ALOGI("Read setting s1=%d",
                  s1->get());  // NOTE: you are not allowed to call get() before this function has been called
        });
    });
    std::this_thread::sleep_for(std::chrono::seconds(2));
    dispatcher.Enqueue([&]() {
        s1->set(5);
        ALOGI("Write setting s1=5 done");
    });
    std::this_thread::sleep_for(std::chrono::seconds(2));
}