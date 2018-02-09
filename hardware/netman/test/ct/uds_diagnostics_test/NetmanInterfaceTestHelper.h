/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cutils/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <string>
#include "vendor/volvocars/hardware/uds/1.0/IDataCollector.h"

using ::vendor::volvocars::hardware::uds::V1_0::IDataCollector;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckReport;
using ::vendor::volvocars::hardware::uds::V1_0::DiagnosticCheckStatus;

class NetmanInterfaceTestHelper {
  public:
    NetmanInterfaceTestHelper();
    ~NetmanInterfaceTestHelper();
    void TakeDownNetworkInterface(const std::string& interface_name);
    void BringUpNetworkInterfaces();
};
