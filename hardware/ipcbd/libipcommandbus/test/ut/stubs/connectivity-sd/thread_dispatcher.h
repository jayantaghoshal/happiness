/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <functional>

namespace Connectivity {
class ThreadDispatcher {
  public:
    virtual void enqueueForDispatch(std::function<void()>&& f) = 0;
    virtual ~ThreadDispatcher() = default;
};
}
