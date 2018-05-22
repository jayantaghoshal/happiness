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

#include "remotectrl_client_climatectrl.h"

using namespace vcc::remotectrlsimclient;
using namespace vcc::remotectrl;

const ClientInfo climate_info = {.app_name_ = "ClimateCtrlSim",
                                 .service_id_ = REMOTECTRL_CLIMATECTRL_SERVICE_ID,
                                 .instance_id_ = REMOTECTRL_CLIMATECTRL_SERVICE_INSTANCE_ID,
                                 .eventgroup_id_ = REMOTECTRL_CLIMATECTRL_EVENTGROUP_ID,
                                 .methods_ = {REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE,
                                              REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE},
                                 .events_ = {REMOTECTRL_CLIMATECTRL_EVENT_ID_FANLEVELCHANGED,
                                             REMOTECTRL_CLIMATECTRL_EVENT_ID_MAX_DEFROSTER_STATECHANGED,
                                             REMOTECTRL_CLIMATECTRL_EVENT_ID_AC_STATECHANGED,
                                             REMOTECTRL_CLIMATECTRL_EVENT_ID_TEMPERATURECHANGED}};

ClimateClient::ClimateClient(std::string path) : RemoteCtrlClientBase(path, climate_info) {
    for (auto e : actionMap) {
        actions.push_back(e.first);
    }
}
ClimateClient::~ClimateClient() { StopClient(); }

void ClimateClient::Run() {
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

void ClimateClient::GetFanLevel() {
    std::cout << "Get Fan Level" << std::endl
              << "Enter row(0-2) and pos(0-2) for which to retrieve fan level:" << std::endl;

    int row, pos;

    std::cin >> row >> pos;

    std::cout << "Fetching Fan Level for " << row << ":" << pos << std::endl;

    EnqueueRequest([&]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_GETFANLEVEL,
                    {static_cast<uint8_t>(row), static_cast<uint8_t>(pos)});
    });

    using namespace std::chrono_literals;
    if (WaitForResponse(3s)) {
        if (vsomeip::return_code_e::E_OK == received_reply_->get_return_code()) {
            if (2 == received_reply_->get_payload()->get_length()) {
                std::cout << "Fan Level: " << static_cast<int>(received_reply_->get_payload()->get_data()[1])
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

void ClimateClient::SetFanLevel() {
    std::cout << "Set Fan Level" << std::endl << "Enter row(0-2) and pos(0-2) for which to set fan level:" << std::endl;
    int row, pos;
    std::cin >> row >> pos;
    int value;
    std::cout << "Enter level to set, 1-5" << std::endl;
    std::cin >> value;
    std::cout << "Setting fan level " << value << " to (" << row << ":" << pos << ")" << std::endl;

    EnqueueRequest([&]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_SETFANLEVEL,
                    {static_cast<uint8_t>(row), static_cast<uint8_t>(pos), static_cast<uint8_t>(value)});
    });

    using namespace std::chrono_literals;
    if (WaitForNotification(3s)) {
        if (vsomeip::return_code_e::E_OK == received_notification_->get_return_code()) {
            if (4 == received_notification_->get_payload()->get_length()) {
                std::cout << "Fan Level for (" << static_cast<int>(received_notification_->get_payload()->get_data()[1])
                          << ":" << static_cast<int>(received_notification_->get_payload()->get_data()[2])
                          << ") set to " << static_cast<int>(received_notification_->get_payload()->get_data()[3])
                          << std::endl;
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

void ClimateClient::GetMaxDefrost() {
    std::cout << "Fetching Max Defrost" << std::endl;

    EnqueueRequest([&]() { SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_MAX_DEFROSTER_STATE, {}); });

    using namespace std::chrono_literals;
    if (WaitForResponse(3s)) {
        if (vsomeip::return_code_e::E_OK == received_reply_->get_return_code()) {
            if (2 == received_reply_->get_payload()->get_length()) {
                std::cout << "Max Defrost Status: "
                          << (0 == static_cast<int>(received_reply_->get_payload()->get_data()[1]) ? "off" : "on")
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

void ClimateClient::SetMaxDefrost() {
    int value;
    std::cout << "Set Max Defrost on(1) or off(0)" << std::endl;
    std::cin >> value;
    std::cout << "Setting Max Defrost " << (0 == value ? "off" : "on") << std::endl;

    EnqueueRequest([&]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_MAX_DEFROSTER_STATE, {static_cast<uint8_t>(value)});
    });

    using namespace std::chrono_literals;
    if (WaitForNotification(3s)) {
        if (vsomeip::return_code_e::E_OK == received_notification_->get_return_code()) {
            if (4 == received_notification_->get_payload()->get_length()) {
                std::cout << "Max Defrost is "
                          << (0 == static_cast<int>(received_notification_->get_payload()->get_data()[1]) ? "off"
                                                                                                          : "on")
                          << std::endl;
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

void ClimateClient::GetAcState() {
    std::cout << "Fetching Ac State" << std::endl;

    EnqueueRequest([&]() { SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_GET_AC_STATE, {}); });

    using namespace std::chrono_literals;
    if (WaitForResponse(3s)) {
        if (vsomeip::return_code_e::E_OK == received_reply_->get_return_code()) {
            if (2 == received_reply_->get_payload()->get_length()) {
                std::cout << "Ac State: "
                          << (0 == static_cast<int>(received_reply_->get_payload()->get_data()[1]) ? "off" : "on")
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

void ClimateClient::SetAcState() {
    int value;
    std::cout << "Set Ac State on(1) or off(0)" << std::endl;
    std::cin >> value;
    std::cout << "Setting Ac State " << (0 == value ? "off" : "on") << std::endl;

    EnqueueRequest(
            [&]() { SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_SET_AC_STATE, {static_cast<uint8_t>(value)}); });

    using namespace std::chrono_literals;
    if (WaitForNotification(3s)) {
        if (vsomeip::return_code_e::E_OK == received_notification_->get_return_code()) {
            if (4 == received_notification_->get_payload()->get_length()) {
                std::cout << "Ac State is "
                          << (0 == static_cast<int>(received_notification_->get_payload()->get_data()[1]) ? "off"
                                                                                                          : "on")
                          << std::endl;
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

void ClimateClient::GetTemperature() {
    std::cout << "Get Temperature" << std::endl
              << "Enter row(0-2) and pos(0-2) for which to get temperature:" << std::endl;
    int row, pos;
    std::cin >> row >> pos;

    EnqueueRequest([&]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_GETTEMPERATURE,
                    {static_cast<uint8_t>(row), static_cast<uint8_t>(pos)});
    });

    using namespace std::chrono_literals;
    if (WaitForResponse(10s)) {
        if (vsomeip::return_code_e::E_OK == received_reply_->get_return_code()) {
            if (2 == received_reply_->get_payload()->get_length()) {
                std::cout << "Temperature: "
                          << (static_cast<int>(received_reply_->get_payload()->get_data()[1]) / 2.0f) + 16.5f
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

void ClimateClient::SetTemperature() {
    std::cout << "Set Temperature" << std::endl
              << "Enter row (0-2) and pos(0-2) for which to set temperature:" << std::endl;
    int row, pos;
    std::cin >> row >> pos;
    int value;
    std::cout << "Enter temperature to set, 0-22" << std::endl;
    std::cin >> value;
    std::cout << "Setting temperature " << value << " to (" << row << ":" << pos << ")" << std::endl;

    EnqueueRequest([&]() {
        SendRequest(REMOTECTRL_CLIMATECTRL_METHOD_ID_SETTEMPERATURE,
                    {static_cast<uint8_t>(row), static_cast<uint8_t>(pos), static_cast<uint8_t>(value)});
    });

    using namespace std::chrono_literals;
    if (WaitForNotification(3s)) {
        if (vsomeip::return_code_e::E_OK == received_notification_->get_return_code()) {
            if (4 == received_notification_->get_payload()->get_length()) {
                std::cout << "Temperature for ("
                          << static_cast<int>(received_notification_->get_payload()->get_data()[1]) << ":"
                          << static_cast<int>(received_notification_->get_payload()->get_data()[2]) << ") set to "
                          << (static_cast<int>(received_notification_->get_payload()->get_data()[3]) / 2.0f) + 16.5f
                          << std::endl;
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
