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
    std::unique_ptr<Setting<int, UserScope::USER>> s1;

    bool first = true;
    std::promise<void> settingInitializedPromise;
    std::future<void> settingInitialized = settingInitializedPromise.get_future();

    dispatcher.Enqueue([&]() {
        s1 = std::make_unique<Setting<int, UserScope::USER>>("s1", 1234, context);
        s1->setCallback([&]() {
            if (first) {
                first = false;
                settingInitializedPromise.set_value();
            }

            // NOTE: you are not allowed to call get() before this callback has been called
            auto value = s1->get();
            std::cout << std::endl << "Settings change notification, s1=" << value << std::endl;
        });
    });

    std::cout << "Waiting for settings" << std::endl;
    settingInitialized.wait();

    while (true) {
        int setToValue;
        std::cout << "Enter a number to set the settings: ";
        std::string userInput;
        std::getline(std::cin, userInput);
        try {
            setToValue = std::stoi(userInput);
        } catch (const std::exception& e) {
            std::cout << "Invalid number: " << e.what() << std::endl;
            continue;
        }

        std::cout << std::endl;
        dispatcher.Enqueue([&]() { s1->set(setToValue); });
    }
}