/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <json_v211.hpp>
#include <sstream>
#include <thread>

#include "vendor/volvocars/hardware/signals/1.0/ISignals.h"
#include "vendor/volvocars/hardware/signals/1.0/ISignalsChangedCallback.h"
#include "vendor/volvocars/hardware/signals/1.0/types.h"
using namespace vendor::volvocars::hardware::signals::V1_0;

#include "hidl_handler.h"
#include "socket_connection.h"
#include "socket_server.h"
#define LOG_TAG "CarSim_Bridge"

const int PORT{8080};

class DeathRecipientToFunction : public android::hardware::hidl_death_recipient {
  public:
    DeathRecipientToFunction(const std::function<void()> onDeath) : onDeath{onDeath} {}
    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override {
        (void)who;
        onDeath();
    }

  private:
    const std::function<void()> onDeath;
};

int main(int argc, char* argv[]) {
    static std::string tag = "*";
    static Dir dir;
    CarSim::SocketServer server{PORT};

    const ::android::sp<DeathRecipientToFunction> deathSubscriber = new DeathRecipientToFunction([&] {
        ALOGE("Lost connection to HIDL server");
        std::exit(-2);
    });

    try {
        while (true) {
            // Connect to client
            ALOGD("Listen to client connection request...");
            auto connection = server.Connect();
            ALOGD("Connected, Subscribe to HIDL signals.");
            ::android::sp<ISignals> service = ISignals::getService();
            ::android::sp<CarSim::HidlHandler> myCallBackObj(new CarSim::HidlHandler());
            myCallBackObj->SetSocketConnection(connection);

            auto result = service->linkToDeath(deathSubscriber, 1234);
            if (!result.isOk()) {
                ALOGE("Failed to linkToDeath");
                return -1;
            }

            auto subscribe = service->subscribe(tag, dir, myCallBackObj);
            if (!subscribe.isOk()) {
                ALOGE("Error: Failed to subscribe to HIDL service. Description: %s.\nExiting...",
                      subscribe.description().c_str());
                return 0;
            }
            ALOGD("Subscribe to HIDL signals done");

            try {
                while (true) {
                    std::string message = connection->Read();

                    if (message.empty()) {
                        ALOGV("Connection closed by client.");
                        break;  // break out to reconnect while-loop
                    }

                    nlohmann::json j = nlohmann::json::parse(message);  // Parse the message to a json object
                    std::string signal_name = j["SignalName"];
                    Dir signal_dir = static_cast<Dir>(std::uint16_t(j["Dir"]));
                    nlohmann::json jdata = j["Data"];
                    std::string signal_data = jdata.dump();

                    ALOGV("signal_name:%s\nsignal_dir:%zd\nsignal_data:%s", signal_name.c_str(), signal_dir,
                          signal_data.c_str());

                    // Send message to HIDL
                    ::android::hardware::Return<void> retcode = service->send(signal_name, signal_dir, signal_data);
                    (void)retcode.isOk();

                    // Give other threads a chance
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            } catch (const std::exception& ex) {
                printf("Error reading (inner ex: %s), trying to reconnect...\n", ex.what());
                ALOGD("Error reading (inner ex: %s), trying to reconnect...", ex.what());

                // Make it clear that no connection exists.
                myCallBackObj->SetSocketConnection(nullptr);

                // TODO: When the HIDL service supports unsubscribe, put it here
                // (and move the subscribe into the first while-loop)
            }
        }
    } catch (const std::exception& ex) {
        printf("Error: No connection established (internal ex: %s), exiting...\n", ex.what());
        ALOGD("Error: No connection established (internal ex: %s), exiting...", ex.what());
        return 0;
    }
}