/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_SHUTDOWN_CLIENT_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_SHUTDOWN_CLIENT_H_

// TODO !!!!! currently not in the build

#include "cedric/cedric_nodestate.h"
#include "isocket.h"

namespace Connectivity
{
class ShutdownClient : public cedric::core::IShutdownClient
{
public:
    ShutdownClient(ISocket& socket) : socket_(socket){};

    int onLifeCycleRequest(unsigned int request, unsigned int requestId) override;

private:
    ISocket& socket_;
};

}  // namespace Connectivity;

#endif  // _CONNECTIVITY_LIBIPCOMMANDBUS_SHUTDOWN_CLIENT_H_
