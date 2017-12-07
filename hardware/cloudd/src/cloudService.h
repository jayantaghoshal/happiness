/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H
#define VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H

#include <IDispatcher.h>
#include <list>
#include "certificate_handler_interface.h"
#include "cloud_request.h"
#include "cloudd_local_config.h"
#include "vpn_entry_point_fetcher.h"

using ::tarmac::eventloop::IDispatcher;

namespace Connectivity {

class CloudService {
  public:
    CloudService();
    ~CloudService() = default;

    bool Initialize();

  private:
    IDispatcher& eventDispatcher_;

    ClouddLocalConfig cloudd_local_config_;

    CloudRequest cloud_request_;

    ICertHandler* certHandler_;

    VPNEntryPointFetcher entry_point_fetcher_;

    std::string cep_url_;
    int cep_port_;
};

}  // Namespace
#endif  // VENDOR_VOLVOCARS_HARDWARE_CLOUDD_SRC_CLOUDSERVICE_H