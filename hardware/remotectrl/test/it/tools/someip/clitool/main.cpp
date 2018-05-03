/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "remotectrl_client_base.h"

#include "convapi_signals_def.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <functional>

using namespace vcc::remotectrlsim;
using namespace vcc::remotectrl;

namespace {

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

const ClientInfo audio_info = {
        .app_name_ = "AudioCtrlSim",
        .service_id_ = REMOTECTRL_AUDIOCTRL_SERVICE_ID,
        .instance_id_ = REMOTECTRL_AUDIOCTRL_SERVICE_INSTANCE_ID,
        .eventgroup_id_ = REMOTECTRL_AUDIOCTRL_EVENTGROUP_ID,
        .methods_ = {REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME},
        .events_ = {REMOTECTRL_AUDIOCTRL_EVENT_ID_VOLUMESTATUS}};
}

class ClimateClient : public RemoteCtrlClientBase {
  public:
    typedef void (ClimateClient::*Func)();

    ClimateClient(std::string path) : RemoteCtrlClientBase(path, climate_info) {
        for (auto e : actionMap) {
            actions.push_back(e.first);
        }
    }
    ~ClimateClient() { StopClient(); }

    void Run() {
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

    void GetFanLevel() {
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

    void SetFanLevel() {
        std::cout << "Set Fan Level" << std::endl
                  << "Enter row(0-2) and pos(0-2) for which to set fan level:" << std::endl;
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
                    std::cout << "Fan Level for ("
                              << static_cast<int>(received_notification_->get_payload()->get_data()[1]) << ":"
                              << static_cast<int>(received_notification_->get_payload()->get_data()[2]) << ") set to "
                              << static_cast<int>(received_notification_->get_payload()->get_data()[3]) << std::endl;
                } else {
                    std::cout << "Data Length NOK: " << received_notification_->get_payload()->get_length()
                              << std::endl;
                }
            } else {
                std::cout << "Status NOK" << std::endl;
            }

        } else {
            std::cout << "Failed" << std::endl;
        }
    }

    void GetMaxDefrost() {
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

    void SetMaxDefrost() {
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
                    std::cout << "Data Length NOK: " << received_notification_->get_payload()->get_length()
                              << std::endl;
                }
            } else {
                std::cout << "Status NOK" << std::endl;
            }

        } else {
            std::cout << "Failed" << std::endl;
        }
    }

    void GetAcState() {
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

    void SetAcState() {
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
                    std::cout << "Data Length NOK: " << received_notification_->get_payload()->get_length()
                              << std::endl;
                }
            } else {
                std::cout << "Status NOK" << std::endl;
            }

        } else {
            std::cout << "Failed" << std::endl;
        }
    }

    void GetTemperature() {
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

    void SetTemperature() {
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
                    std::cout << "Data Length NOK: " << received_notification_->get_payload()->get_length()
                              << std::endl;
                }
            } else {
                std::cout << "Status NOK" << std::endl;
            }

        } else {
            std::cout << "Failed" << std::endl;
        }
    }

  private:
    std::map<std::string, Func> actionMap = {{"Get Fan Level", &ClimateClient::GetFanLevel},
                                             {"Set Fan Level", &ClimateClient::SetFanLevel},
                                             {"Get Max Defrost", &ClimateClient::GetMaxDefrost},
                                             {"Set Max Defrost", &ClimateClient::SetMaxDefrost},
                                             {"Get Ac State", &ClimateClient::GetAcState},
                                             {"Set Ac State", &ClimateClient::SetAcState},
                                             {"Get Temperature", &ClimateClient::GetTemperature},
                                             {"Set Temperature", &ClimateClient::SetTemperature}};

    std::vector<std::string> actions;
};

class AudioClient : public RemoteCtrlClientBase {
  public:
    typedef void (AudioClient::*Func)();

    AudioClient(std::string path) : RemoteCtrlClientBase(path, audio_info) {
        for (auto e : actionMap) {
            actions.push_back(e.first);
        }
    }
    ~AudioClient() { StopClient(); }

    void Run() {
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

    void GetAudioVolume() {
        EnqueueRequest([this]() { SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, {1}); });

        using namespace std::chrono_literals;
        if (WaitForResponse(10s)) {
            if (vsomeip::return_code_e::E_OK == received_reply_->get_return_code()) {
                if (2 == received_reply_->get_payload()->get_length()) {
                    std::cout << "Audio Volume Level: "
                              << static_cast<int>(received_reply_->get_payload()->get_data()[1]) << std::endl;
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

    void SetAudioVolume() {
        std::cout << "Set Audio Volume Level" << std::endl;
        int value;
        std::cout << "Enter temperature to set, 0-35" << std::endl;
        std::cin >> value;
        std::cout << "Setting Audio Volume Level to " << value << std::endl;

        EnqueueRequest([&]() {
            SendRequest(REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME, {1, static_cast<uint8_t>(value)});
        });

        using namespace std::chrono_literals;
        if (WaitForNotification(3s)) {
            if (vsomeip::return_code_e::E_OK == received_notification_->get_return_code()) {
                if (2 == received_notification_->get_payload()->get_length()) {
                    std::cout << "Audio Volume Level Set to: "
                              << static_cast<int>(received_notification_->get_payload()->get_data()[1]) << std::endl;
                } else {
                    std::cout << "Data Length NOK: " << received_notification_->get_payload()->get_length()
                              << std::endl;
                }
            } else {
                std::cout << "Status NOK" << std::endl;
            }

        } else {
            std::cout << "Failed" << std::endl;
        }
    }

  private:
    std::map<std::string, Func> actionMap = {{"Get Audio Volume", &AudioClient::GetAudioVolume},
                                             {"Set Audio Volume", &AudioClient::SetAudioVolume}};

    std::vector<std::string> actions;
};

int main(int argc, char* argv[]) {
    using namespace std::chrono_literals;

    if (argc < 2) {
        // Only expected to happen during development and not in production image
        std::cout << "Expects vsomeip configuration file location as argument" << std::endl;
        std::this_thread::sleep_for(1s);
        return EXIT_FAILURE;
    }

    const char* environment_variable = "VSOMEIP_CONFIGURATION";
    const char* environment_value = argv[1];
    std::cout << environment_variable << ":" << environment_value << std::endl;

    setenv(environment_variable, environment_value, 1 /*override*/);

    std::string path(argv[1]);

    int val = -1;

    while (1) {
        std::cout << "Load Module:" << std::endl
                  << "0. Exit" << std::endl
                  << "1. Climate" << std::endl
                  << "2. Audio" << std::endl;
        std::cin >> val;

        std::cout << "" << val << std::endl;

        if (0 == val) {
            std::cout << "Exiting..." << std::endl;
            break;
        } else if (1 == val) {
            std::cout << "Climate" << std::endl;
            ClimateClient climate(path);
            climate.Run();
        } else if (2 == val) {
            std::cout << "Audio" << std::endl;
            AudioClient audio(path);
            audio.Run();
        } else {
            std::cout << "No." << std::endl;
        }
    }

    return EXIT_SUCCESS;
}