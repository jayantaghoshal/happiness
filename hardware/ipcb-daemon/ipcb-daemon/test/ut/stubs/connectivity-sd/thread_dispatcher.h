/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once
#include <functional>

namespace Connectivity
{
class ThreadDispatcher
{
public:
    virtual void enqueueForDispatch(std::function<void()>&& f) = 0;
    virtual ~ThreadDispatcher() = default;
};
}
