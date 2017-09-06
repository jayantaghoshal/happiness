#include <iostream>
#include <thread>
#include <chrono>
#include <cutils/log.h>


#include <cutils/log.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <vsm.h>
#include <chrono>
#include <thread>
#include "desip_handler.h"

// TBD Remove when HIDL impl is done
#include <chrono>

#undef LOG_TAG
#define LOG_TAG "VSD"

// static void printStartupErrorMessage(ECD_Error_Codes error, char* uartPath);

int main(int argc, char* argv[])
{
  bool startOnlyDBUS = false;

  if (3 > argc)
  {
    ALOGE("Serial port and/or UART speed not specified when starting the Vehicle Signals Manager.");

    // log_error() << "";
    return EXIT_FAILURE;
  }
  else if (3 == argc || 4 == argc)
  {
    if (4 == argc && strcmp(argv[3], "onlydbus") == 0)
    {
      startOnlyDBUS = true;
    }
  }
  else
  {
    ALOGE(
        "Wrong number of parameters specified when starting "
        "Flexray/LIN vehicle signals communication service.");
    return EXIT_FAILURE;
  }

  // if (!DBUSSignalServer::start())
  {
    ALOGE("Failed to start DBUS server");
    return EXIT_FAILURE;
  }

  if (!startOnlyDBUS)
  {
    ALOGI("VSM communication service is starting...");

    // Initialize the VSM module. After this function is called the VSM
    // will be able to handle injected messages from lower layers
    vsm_init();

    bool result = initDesip(argv[1], argv[2]);

    if (!result)
    {
      return EXIT_FAILURE;
    }

    // Start the VSM module. After this function is called
    // the VSM module will sink messages to lower layers
    vsm_start();

    ALOGI("Flexray/LIN vehicle signals communication service calls app.exec().");
  }

  while(true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ALOGD("VSD Running");
    std::cout << "vsd running" << std::endl; 
  }    
    
}

