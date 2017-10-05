
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
  // Connect to HIDL and point it to a function for callback
  ::android::sp<ISignals> service = ISignals::getService();
  ::android::sp<CarSim::HidlHandler> myCallBackObj(new CarSim::HidlHandler());

  printf("Subscribe to HIDL signals.\n");
  static std::string tag = "*";
  static Dir dir;
  auto subscribe = service->subscribe(tag, dir, myCallBackObj);
  if (!subscribe.isOk()) {
    printf("Error: Failed to subscribe to HIDL service. Description: %s.\nExiting...", subscribe.description().c_str());
    return 0;
  }

  // Wait for client to connect and forward anty messages to HIDL.
  // Also any messages from HIDL in callback will be sent to client.
  CarSim::SocketServer server;
  server.Init(PORT);

  try {
    while (true) {
      printf("Listen to client connection request...\n");
      auto connection = server.Connect();
      // When connection is established, give a handle to the HIDL callback function
      myCallBackObj->SetSocketConnection(connection);

      printf("Connected.\n");
      try {
        while (true) {
          std::string message = connection->Read();

          if (message.empty()) {
            printf("Connection closed by client.\n");
            break;  // break out to reconnect while-loop
          }

          nlohmann::json j = nlohmann::json::parse(message);  // Parse the message to a json object
          std::string jsonData = j.dump();
          printf("<client to server> %s\n", jsonData.c_str());  // Should be the same as the message

          std::string signal_name = j["SignalName"];
          Dir signal_dir = static_cast<Dir>(std::uint16_t(j["Dir"]));
          nlohmann::json jdata = j["Data"];
          std::string signal_data = jdata.dump();

          ALOGV("signal_name:%s\nsignal_dir:%zd\nsignal_data:%s\n", signal_name.c_str(), signal_dir,
                signal_data.c_str());

          // Send message to HIDL
          ::android::hardware::Return<void> retcode = service->send(signal_name, signal_dir, signal_data);
          (void)retcode.isOk();

          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
      } catch (const std::exception& ex) {
        printf("Error reading (inner ex: %s), trying to reconnect...\n", ex.what());
        myCallBackObj->SetSocketConnection(nullptr);
      }
    }
  } catch (const std::exception& ex) {
    printf("Error: No connection established, exiting...\n");
    return 0;
  }
}
