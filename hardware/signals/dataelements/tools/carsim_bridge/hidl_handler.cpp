/*
 * Copyright 2017-2018 Volvo Car Corporation
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
#define LOG_TAG "CarSim_Bridge"

namespace CarSim {

::android::hardware::Return<void> HidlHandler::signalChanged(const ::android::hardware::hidl_string& signalName,
                                                             Dir dir,
                                                             const ::android::hardware::hidl_string& data) {
    // Avoid spamming the tender client by keeping shut for half a second
    // TODO REMOVE when client can handle it.
    ALOGD("Signal changed %s", signalName.c_str());

    // Send the data if connection.
    try {
        std::shared_ptr<CarSim::SocketConnection> keepConnAlive = connection_;
        if (keepConnAlive != nullptr) {
            nlohmann::json j;
            j["SignalName"] = nlohmann::json(std::string(signalName));
            j["Dir"] = nlohmann::json((int)dir);
            // NOTE: Data is an opaque string (that happens to contain json) from carsims perspective, not a json
            // object!!!
            j["Data"] = std::string(data);

            std::string jsonData = j.dump();
            ALOGV("<server to client> %s", jsonData.c_str());

            keepConnAlive->Send(jsonData);
        } else {
            ALOGV("Not connected, signal not forwarded to client");
        }
    } catch (const std::exception& ex) {
        ALOGD("Error: HidlHandler::signalChanged (internal ex: %s), exiting...", ex.what());
    }

    return ::android::hardware::Void();
}

void HidlHandler::SetSocketConnection(std::shared_ptr<CarSim::SocketConnection> connection) {
    connection_ = connection;
}

}  // CarSim