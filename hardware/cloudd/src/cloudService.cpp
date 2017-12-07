/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "cloudService.h"
#include <curl/curl.h>
#include "certificate_handler.h"

#include <stdlib.h>
#include <unistd.h>

#define LOG_TAG "CloudD.service"
#include <cutils/log.h>

namespace Connectivity {

CloudService::CloudService()
    : eventDispatcher_{IDispatcher::GetDefaultDispatcher()},
      certHandler_(new CertHandler{CLIENT_CERT_PEM(), CLIENT_KEY_PEM(), CA_CERT_PEM()}),
      entry_point_fetcher_{*certHandler_, eventDispatcher_, cloud_request_} {}

bool CloudService::Initialize() {
    if (!cloud_request_.Init()) {
        ALOGE("Unable to init Reachability Cloud Request.");
        return false;
    }

    std::string lcfg_entrypoint_url = cloudd_local_config_.GetCloudEntryPointAddress();

    entry_point_fetcher_.WhenResultAvailable([&](const VPNEntryPointParser::EntryPoint &entry_point) {
        ALOGD("Cloud client received entry point");

        cep_url_ = entry_point.host;
        cep_port_ = entry_point.port;

    });

    std::string entry_point_url = "https://" + lcfg_entrypoint_url + "/?client_id=11";

    entry_point_fetcher_.Fetch(entry_point_url);

    return true;
}
}