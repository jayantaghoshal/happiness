/*===========================================================================*\
 * Copyright 2016 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef CEDRIC_NODESTATE_H
#define CEDRIC_NODESTATE_H

#include <string>

namespace cedric
{
namespace core
{
class INodeStateClient
{
};
/**
 * This is the actual Cedric C++ wrapper class for dealing with the Node State Manager
 *  in a simple way.
 */
class NodeState
{
public:
    /**
     * @brief Constructor to use if no shutdown client is required
     */
    NodeState()
    {
    }  // Constructor to use if you do not want to be a shutdown client

    /**
     * @brief Constructor to use if you want the object to be able to register as a shutfown client
     * @param shutdown_mode NSM_SHUTDOWNTYPE_NORMAL, NSM_SHUTDOWNTYPE_FAST from NSM's NodeStateTypes.h
     */
    ~NodeState()
    {
    }

    // For Cedric clients

    /**
     * @return error code as defined by NsmErrorStatus_e from NSM. Or -1 if error talking to NSM
     */
    int SetSessionState(const std::string& /*sessionName*/,
                        const std::string& /*sessionOwner*/,
                        int /*seatId*/,
                        int /*sessionState*/)
    {
        return 0;
    }

    /**
     * @return SessionState, values defined by NsmSessionState_e. Or -1 if error
     */
    int GetSessionState(const std::string& /*sessionName*/, int /*seatId*/)
    {
        return 0;
    }

    int RegisterSession(const std::string& /*sessionName*/,
                        const std::string& /*sessionOwner*/,
                        int /*seatId*/,
                        int /*sessionState*/)
    {
        return 0;
    }

    void setNodeStateClient(INodeStateClient* /*ns_client*/)
    {
    }
};
}
}
#endif  // CEDRIC_NODESTATE_H
