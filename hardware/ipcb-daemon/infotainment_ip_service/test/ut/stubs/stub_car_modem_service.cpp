/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "../util/type_conversion_helpers.h"
#include "car_modem_service_factory.h"

namespace Connectivity
{
CarModemServiceFactory::CarModemServiceFactory()
{
}

bool CarModemServiceFactory::Initialize(Connectivity::MessageDispatcher *msgDispatcher)
{
    (void)msgDispatcher;
    return true;
}

void CarModemServiceFactory::Uninitialize()
{
}

std::shared_ptr<gen_conn::CarModemStub> CarModemServiceFactory::GetStub()
{
    return nullptr;
}

void CarModemService::cbCarModemPSIMMessageSetRequest(Message &msg)
{
    (void)msg;
}

}  // Connectivity
