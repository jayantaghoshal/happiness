/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <initializer_list>
#include <vsomeip/vsomeip.hpp>

namespace vcc {
namespace remotectrl {

struct ServiceInfo {
    const char* service_name_;
    vsomeip::service_t service_id_;
    vsomeip::instance_t instance_id_;
    std::initializer_list<vsomeip::method_t> methods_;
    std::initializer_list<vsomeip::event_t> events_;
};

}  // namespace remotectrl
}  // namespace vcc
