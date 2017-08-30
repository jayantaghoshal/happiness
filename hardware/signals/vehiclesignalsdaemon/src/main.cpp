#include <iostream>
#include <thread>
#include <chrono>
#include <cutils/log.h>

#define  LOG_TAG    "VSD"

int main()
{
    // Placeholder service, to be replaced by vehicle signals daemon
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        ALOGD("VSD Running");
        std::cout << "vsd running" << std::endl; 
    }    
}