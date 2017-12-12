/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H

#include <IDispatcher.h>
#include <cutils/log.h>

using ::tarmac::eventloop::IDispatcher;

namespace Connectivity {

class CloudService {
  public:
    CloudService();
    ~CloudService() = default;

    bool Initialize();

  private:
    void ConnectionTest();  // Simple test to verify that we have internet. TODO:Remove next interation
    IDispatcher &eventDispatcher_;
};

}  // Namespace
#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H