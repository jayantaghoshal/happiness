
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <json.hpp>
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

int main(int argc, char* argv[]) {
  static std::string tag = "*";
  static Dir dir;
  CarSim::SocketServer server{PORT};

  try {
    while (true) {
      // Connect to client
      printf("Listen to client connection request...\n");
      auto connection = server.Connect();
      printf("Connected.\n");

      // Subscribe HIDL
      printf("Subscribe to HIDL signals.\n");
      ::android::sp<ISignals> service = ISignals::getService();
      ::android::sp<CarSim::HidlHandler> myCallBackObj(new CarSim::HidlHandler());
      myCallBackObj->SetSocketConnection(connection);

      auto subscribe = service->subscribe(tag, dir, myCallBackObj);
      if (!subscribe.isOk()) {
        printf("Error: Failed to subscribe to HIDL service. Description: %s.\nExiting...\n",
               subscribe.description().c_str());
        ALOGD("Error: Failed to subscribe to HIDL service. Description: %s.\nExiting...",
              subscribe.description().c_str());
        return 0;
      }

      try {
        while (true) {
          std::string message = connection->Read();

          if (message.empty()) {
            printf("Connection closed by client.\n");
            ALOGV("Connection closed by client.");
            break;  // break out to reconnect while-loop
          }

          nlohmann::json j = nlohmann::json::parse(message);  // Parse the message to a json object
          std::string signal_name = j["SignalName"];
          Dir signal_dir = static_cast<Dir>(std::uint16_t(j["Dir"]));
          nlohmann::json jdata = j["Data"];
          std::string signal_data = jdata.dump();

          ALOGV("signal_name:%s\nsignal_dir:%zd\nsignal_data:%s", signal_name.c_str(), signal_dir, signal_data.c_str());

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