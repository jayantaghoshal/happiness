/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "router.h"

namespace {
const char* REMOTECTRL_ROUTER_APP_NAME = "RemoteCtrl_Router";
}  // namespace

namespace vcc {
namespace remotectrl {
namespace router {

Router::Router() : vsomeip_appl_(vsomeip::runtime::get()->create_application(REMOTECTRL_ROUTER_APP_NAME)) {
    // TODO (Abhi): Need to setup multicast route. Should Netman do it? or this router should do it?

    if (nullptr == vsomeip_appl_) {
        throw std::runtime_error(std::string("Failed to create_application ") + REMOTECTRL_ROUTER_APP_NAME);
    }

    // Initialize service application
    if (!vsomeip_appl_->init()) {
        throw std::runtime_error(std::string("Couldn't initialize Convenience ") + REMOTECTRL_ROUTER_APP_NAME);
    }
}

Router::~Router() {
    Stop();
}

void Router::Start() {
    vsomeip_appl_->start();
}
void Router::Stop() {
    vsomeip_appl_->stop();
}

}  // namespace router
}  // namespace remotectrl
}  // namespace vcc
