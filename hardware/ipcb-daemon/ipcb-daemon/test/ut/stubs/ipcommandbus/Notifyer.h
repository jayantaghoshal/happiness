/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef NOTIFYER_H
#define NOTIFYER_H

#include <assert.h>
#include <functional>

namespace Connectivity
{
/**
 * Stub notifyer class just to get the MessageDispatcher class to compile.
 * The intended use case is unit testing of service classes,
 * and we catch the send calls already at (mock) IpService level,
 * so this is never supposed to get called...
 *
 * (Q: Why not stub MessageDispatcher instead?
 *  A: Much easier to stub two 10 liner classes that will nearly never change than one of our biggest classes
 *     that will undoubtably be updated some times...
 * )
 */

class Notifyer
{
public:
    Notifyer(std::function<void(void)> cb)
    {
    }

    void trigger()
    {
        fprintf(stderr, "Not supposed to reach this stage in service layer tests...");
        assert(0);
    }

protected:
    std::function<void(void)> cbFunc;
};

}  // Connectivity

#endif  // NOTIFYER_H
