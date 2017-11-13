/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
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
