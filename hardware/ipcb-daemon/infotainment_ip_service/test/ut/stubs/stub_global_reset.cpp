/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "global_reset.h"
#include "iplm_service.h"

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_global_reset_log_context, "GRES", "Global Reset")

GlobalReset::GlobalReset()
{
}

bool GlobalReset::Initialize(Connectivity::MessageDispatcher *msgDispatcher, IpLmServiceStubImpl &iplm)
{
    return true;
}

void GlobalReset::Uninitialize()
{
}

void GlobalReset::onSessionStateChanged(const std::string &sessionStateName, int seatId, int sessionState)
{
    (void)sessionStateName;
    (void)seatId;
    (void)sessionState;
}

void GlobalReset::cbFactoryDefaultRestoreResponse(Message &msg,
                                                  std::shared_ptr<MessageDispatcher::CallerData> pCallerData)
{
    (void)msg;
    (void)pCallerData;
}

void GlobalReset::SendFactoryDefaultRestore(Message::Ecu ecu)
{
}

void GlobalReset::OnFactoryDefaultRestoreCompleted(Message::Ecu ecu,
                                                   bool _restorationResult,
                                                   std::shared_ptr<FactoryDefaultRestoreRespCallback> ipcb_callback_obj)
{
    (void)ecu;
    (void)_restorationResult;
    (void)ipcb_callback_obj;
}

}  // Connectivity
