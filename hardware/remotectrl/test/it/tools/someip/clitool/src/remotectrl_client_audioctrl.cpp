/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "remotectrl_client_base.h"
#include "signals_def.h"

#include "remotectrl_client_audioctrl.h"

using namespace vcc::remotectrlsimclient;
using namespace vcc::remotectrl;

const ClientInfo audio_info = {
        .app_name_ = "AudioCtrlSim",
        .service_id_ = REMOTECTRL_AUDIOCTRL_SERVICE_ID,
        .instance_id_ = REMOTECTRL_AUDIOCTRL_SERVICE_INSTANCE_ID,
        .eventgroup_id_ = REMOTECTRL_AUDIOCTRL_EVENTGROUP_ID,
        .methods_ = {REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME},
        .events_ = {REMOTECTRL_AUDIOCTRL_EVENT_ID_VOLUMESTATUS}};

AudioClient::AudioClient(std::string path) : RemoteCtrlClientBase(path, audio_info) {
    for (auto e : actionMap) {
        actions.push_back(e.first);
    }
}
AudioClient::~AudioClient() {
    StopClient();
}

void AudioClient::Run() {
    StartClient();

    while (1) {
        std::cout << std::endl << "What do you want to do?" << std::endl;

        std::cout << "0. Return" << std::endl;

        int i = 1;
        for (auto e : actionMap) {
            std::cout << i << ". " << e.first << std::endl;
            i++;
        }

        int val = -1;
        std::cin >> val;

        if (0 == val) {
            StopClient();
            break;
        }

        try {
            Func p = actionMap.at(actions.at(val - 1));
            (this->*p)();
        } catch (std::out_of_range e) {
            std::cout << "No." << std::endl;
        }
    }
}

void AudioClient::GetAudioVolume() {
    EnqueueRequest([this]() { SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, {1}); });

    using namespace std::chrono_literals;
    if (WaitForResponse(10s)) {
        if (vsomeip::return_code_e::E_OK == received_reply_->get_return_code()) {
            if (2 == received_reply_->get_payload()->get_length()) {
                std::cout << "Audio Volume Level: " << static_cast<int>(received_reply_->get_payload()->get_data()[1])
                          << std::endl;
            } else {
                std::cout << "Data Length NOK: " << received_reply_->get_payload()->get_length() << std::endl;
            }
        } else {
            std::cout << "Status NOK" << std::endl;
        }

    } else {
        std::cout << "Failed" << std::endl;
    }
}

void AudioClient::SetAudioVolume() {
    std::cout << "Set Audio Volume Level" << std::endl;
    int value;
    std::cout << "Enter temperature to set, 0-35" << std::endl;
    std::cin >> value;
    std::cout << "Setting Audio Volume Level to " << value << std::endl;

    EnqueueRequest([&]() { SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME, {1, static_cast<uint8_t>(value)}); });

    using namespace std::chrono_literals;
    if (WaitForNotification(3s)) {
        if (vsomeip::return_code_e::E_OK == received_notification_->get_return_code()) {
            if (2 == received_notification_->get_payload()->get_length()) {
                std::cout << "Audio Volume Level Set to: "
                          << static_cast<int>(received_notification_->get_payload()->get_data()[1]) << std::endl;
            } else {
                std::cout << "Data Length NOK: " << received_notification_->get_payload()->get_length() << std::endl;
            }
        } else {
            std::cout << "Status NOK" << std::endl;
        }

    } else {
        std::cout << "Failed" << std::endl;
    }
}
