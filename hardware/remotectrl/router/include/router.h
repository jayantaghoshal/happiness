/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>
#include <vsomeip/vsomeip.hpp>

namespace vcc {
namespace remotectrl {
namespace router {

class Router final {
  public:
    Router();
    ~Router();

    void Start();
    void Stop();

  private:
    const std::shared_ptr<vsomeip::application> vsomeip_appl_;
};

}  // namespace router
}  // namespace remotectrl
}  // namespace vcc
