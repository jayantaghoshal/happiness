/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "ipcommandbus/Pdu.h"
#include "ipcommandbus/log_context.h"

namespace Connectivity
{
bool Pdu::fromData(std::vector<uint8_t> &fromData)
{
    (void)fromData;
    return true;
}

void Pdu::toData(std::vector<uint8_t> &buffer) const
{
    (void)buffer;
}

void Pdu::createHeader(VccIpCmd::ServiceId serviceId,
                       VccIpCmd::OperationId operationId,
                       VccIpCmd::OperationType operationType,
                       VccIpCmd::DataType dataType,
                       uint8_t sequenceId)
{
    header.service_id = serviceId;
    header.operation_id = operationId;
    header.operation_type = operationType;
    header.data_type = dataType;
    header.sender_handle_id = sequenceId;
}

void Pdu::setPayload(std::vector<uint8_t> &&data)
{
    payload = std::move(data);
}

std::string Pdu::toString(const Pdu &pdu)
{
    (void)pdu;
    return "";
}

}  // Connectivity
