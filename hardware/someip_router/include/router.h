/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>
#include <vsomeip/vsomeip.hpp>

namespace vcc {
namespace someip {
namespace router {

class Router final {
  public:
    Router(const std::string& vsomeip_app_name, const char* environment_value);
    ~Router();

    void Start();
    void Stop();

  private:
    const std::shared_ptr<vsomeip::application> vsomeip_appl_;
};

}  // namespace router
}  // namespace someip
}  // namespace vcc
