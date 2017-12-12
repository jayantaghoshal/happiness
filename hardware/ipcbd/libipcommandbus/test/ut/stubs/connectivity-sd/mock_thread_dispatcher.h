/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <connectivity-sd/thread_dispatcher.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <functional>

namespace Connectivity {
class MockThreadDispatcher final : public ThreadDispatcher {
  public:
    void enqueueForDispatch(std::function<void()>&& f) { f(); }
};
}
