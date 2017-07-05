/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once

#if defined(__COVERITY__) && !defined(__INCLUDE_LEVEL__)
/* Systemd's use of gcc's __INCLUDE_LEVEL__ extension macro appears to confuse
 * Coverity. Here's a kludge to unconfuse it.
 */
#define __INCLUDE_LEVEL__ 2
#endif  // __COVERITY__

#include <memory>
#include <string>

#include <IDispatcher.h>

namespace Connectivity
{
/**
 * @brief Main Loop implementation
 */
class MainLoop
{
public:
    MainLoop();

    /**
     * @brief Enter main loop
     * @return false is failed to enter main loop
     */
    bool Run();
    tarmac::eventloop::IDispatcher& GetDispatcher();

private:
    tarmac::eventloop::IDispatcher& event_;
    bool InitSignals();
    void SigTermHandler(int fd);
    void SigHupHandler(int fd);
    void SigIntHandler(int fd);
};

}  // namespace Connectivity
