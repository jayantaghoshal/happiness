
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "vendor/volvocars/hardware/signals/1.0/ISignals.h"
#include "vendor/volvocars/hardware/signals/1.0/ISignalsChangedCallback.h"
#include "vendor/volvocars/hardware/signals/1.0/types.h"

using namespace vendor::volvocars::hardware::signals::V1_0;



#include <json.hpp>
using json = nlohmann::json;

#include "socketserver.h"
socketserver server;


static std::string tag = "*";
static Dir dir;

class SignalChangedCallback : public ISignalsChangedCallback
{
 public:
  ::android::hardware::Return<void> signalChanged(const ::android::hardware::hidl_string& signalName, 
                                                  Dir dir,
                                                  const ::android::hardware::hidl_string& data)
  {
    json j;
    j["SignalName"] = json(std::string(signalName));
    j["Dir"] = json((int)dir);
    j["Data"] = json(std::string(data));

    std::string jsonData = j.dump();
    printf("X%s\n", jsonData.c_str());

    try
    {
      server.sendMessage(jsonData);
    }
    catch(const std::exception& ex)
    {
      printf("Error: socketserver::sendMessage()\n");
    }

    return ::android::hardware::Void();
  }
};

void printHelp()
{
  printf("\nUsage: [log]\n");
  printf("Example: 'carsim_bridge' normal run, output normal signal data\n");
  printf("Example: 'carsim_bridge log' extended output with application data aswell\n");
}

bool handleArguments(int argc, char* argv[])
{
  for (int i = 1; i < argc; i++)
  {
    std::string arg = argv[i];
    std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);

    if (arg == "--help" || arg == "-help" || arg == "-h")
    {
      printHelp();
      return false;
    }
    else if (arg == "log")
    {
      // TODO-Add some logging for developing purposes
    }  

    tag = arg;
  }

  return true;
}

int main(int argc, char* argv[])
{
  if (!handleArguments(argc, argv)) return 0;

  ::android::sp<ISignals> service = ISignals::getService();
  ::android::sp<ISignalsChangedCallback> signalChanged = new SignalChangedCallback();
  
  printf("<Subscribe to HIDL signals>\n");
  /*
  auto subscribe = service->subscribe(tag, dir, signalChanged);
  if (!subscribe.isOk())
  {
    printf("Failed to subscribe to server. Description: %s", subscribe.description().c_str());
    return 0;
  }
  */

  


  printf("<Start listen to client connection request>\n");
  try
  {
    server.connect();
  }
  catch(const std::exception& ex)
  {
    printf("Error: No connection established, exiting...\n");
    return 0;
  }

  printf("<Endless loo while listening for client messages>\n");  
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    printf("Waiting for message...\n");
    std::string message = server.readMessage();
  
     
    json j = json::parse(message);//Parse the message to a json object

    std::string jsonData = j.dump();
    printf("<json dump>%s\n", jsonData.c_str());//Should be the same as the message


    printf("<A>\n");
    try // Start Remove-Send is done in the signalChanged callback function, remove when that is working
    {
      //j["SignalName"] = json(std::string("Magic"));
      //j["value"] = json(std::uint32_t(1));
      j["value"] = json(1);

      //json jData = j["Data"];
      //std::string jsonDataData = jData.dump();
      //printf("<DATA>%s\n", jsonDataData.c_str());

      jsonData = j.dump();  
      server.sendMessage(jsonData.c_str());
    }
    catch(const std::exception& ex)
    {
      printf("Error: socketserver::sendMessage()\n");
    } // <-- End Remove
    
  }
}
