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

#include "remotectrl_client_mediactrl.h"

using namespace vcc::remotectrlsimclient;
using namespace vcc::remotectrl;

const ClientInfo media_info = {.app_name_ = "MediaCtrlSim",
                               .service_id_ = REMOTECTRL_MEDIACTRL_SERVICE_ID,
                               .instance_id_ = REMOTECTRL_MEDIACTRL_SERVICE_INSTANCE_ID,
                               .eventgroup_id_ = REMOTECTRL_MEDIACTRL_EVENTGROUP_ID,
                               .methods_ = {REMOTECTRL_MEDIACTRL_METHOD_ID_SETMEDIAPLAYERPLAYBACK},
                               .events_ = {REMOTECTRL_MEDIACTRL_EVENT_ID_MEDIAPLAYERPLAYBACKSTATUS}};

MediaClient::MediaClient(const std::string& path) : RemoteCtrlClientBase(path, media_info) {
    for (auto e : actionMap) {
        actions.push_back(e.first);
    }
}

MediaClient::~MediaClient() { StopClient(); }

void MediaClient::Run() {
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

void MediaClient::SetMediaPlayback() {
    std::cout << "Set Playback status" << std::endl
              << "Stop (0), Play (1), Pause (2), Next (5) or Previous (6)" << std::endl;
    int playback;
    std::cin >> playback;
    std::cout << "Setting media control to " << playback << std::endl;

    EnqueueRequest([&]() {
        SendRequest(REMOTECTRL_MEDIACTRL_METHOD_ID_SETMEDIAPLAYERPLAYBACK, {static_cast<uint8_t>(playback)});
    });

    using namespace std::chrono_literals;
    if (WaitForNotification(3s)) {
        if (vsomeip::return_code_e::E_OK == received_notification_->get_return_code()) {
            if (2 == received_notification_->get_payload()->get_length()) {
                std::cout << "Media Playback Set to: "
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