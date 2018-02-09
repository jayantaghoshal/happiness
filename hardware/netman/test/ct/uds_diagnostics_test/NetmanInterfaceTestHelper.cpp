/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "NetmanInterfaceTestHelper.h"

#include <cutils/log.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using ::vendor::volvocars::hardware::uds::V1_0::IDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckReport;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus;

NetmanInterfaceTestHelper::NetmanInterfaceTestHelper() {}

void NetmanInterfaceTestHelper::TakeDownNetworkInterface(const std::string& interface_name) {
    // taking downthe interface
    std::string interface_down = "/vendor/bin/ip netns exec vcc /vendor/bin/ifconfig " + interface_name + " down";
    system(interface_down.c_str());
}
void NetmanInterfaceTestHelper::BringUpNetworkInterfaces() {
    // Resetting all the interfaces
    system("/vendor/bin/ip netns exec vcc /vendor/bin/ifconfig apix0 up");
    system("/vendor/bin/ip netns exec vcc /vendor/bin/ifconfig meth0 up");
    system("/vendor/bin/ip netns exec vcc /vendor/bin/ifconfig tcam0 up");
}
NetmanInterfaceTestHelper::~NetmanInterfaceTestHelper() {}
