/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include "libsettings/setting.h"
#include "libsettings/settingidentifiers.h"
#include "libsettings/settingsmanagerhidl.h"
#undef LOG_TAG
#define LOG_TAG "Settings"

using namespace SettingsFramework;

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    tarmac::eventloop::IDispatcher& dispatcher = tarmac::eventloop::IDispatcher::GetDefaultDispatcher();
    android::sp<SettingsManagerHidl> manager = new SettingsManagerHidl(dispatcher);

    std::unique_ptr<Setting<int, UserScope::USER>> s1;
    dispatcher.Enqueue([&]() {
        s1 = std::make_unique<Setting<int, UserScope::USER>>(SettingId::TestSetting1, 1234, manager);
        s1->setCallback([&](const auto& value) {
            std::cout << "Read setting TestSetting1=" << value.value
                      << " profileId=" << static_cast<uint32_t>(value.profileId) << std::endl;
        });
    });
    std::unique_ptr<Setting<int, UserScope::NOT_USER_RELATED>> s2;
    dispatcher.Enqueue([&]() {
        s2 = std::make_unique<Setting<int, UserScope::NOT_USER_RELATED>>(SettingId::TestSetting2, 1234, manager);
        s2->setCallback([&](const auto& value) {
            std::cout << "Read setting TestSetting2=" << value.value << " global " << std::endl;
        });
    });

    while (true) {
        int setToValue;
        std::cout << "Enter a number to set the settings: ";
        std::string userInput;
        std::getline(std::cin, userInput);
        std::cout << std::endl;
        try {
            setToValue = std::stoi(userInput);
        } catch (const std::exception& e) {
            std::cout << "Invalid number: " << e.what() << std::endl;
            continue;
        }

        std::cout << std::endl;
        dispatcher.Enqueue([&]() { s1->set(setToValue); });
        // Sleep to avoid printing "Enter number" before settingcallback fired
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}