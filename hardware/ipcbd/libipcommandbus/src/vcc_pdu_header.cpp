/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "ipcommandbus/net_deserializer.h"
#include "ipcommandbus/net_serializer.h"
#include "ipcommandbus/vcc_pdu_header.h"

#include <cassert>
#include <cstdio>

namespace Connectivity {

VCCPDUHeader VCCPDUHeader::from_data(const std::vector<std::uint8_t> &data) {
    NetDeserializer deserializer(data);

    // TODO: Document that it is up to caller to check that
    //       "data.size() >= VCCPDUHeader::DATA_SIZE" so this does not occur. Or rework to
    //       return value and error flag tuple. Tuple probably makes more sense if more
    //       validation is to be done in this method (see TODO below).
    assert(deserializer.bytes_available() >= DATA_SIZE);
    if (deserializer.bytes_available() < DATA_SIZE) {
        return VCCPDUHeader();
    }

    VCCPDUHeader header;

    header.service_id = static_cast<IpCmdTypes::ServiceId>(deserializer.read_uint16());
    header.operation_id = static_cast<IpCmdTypes::OperationId>(deserializer.read_uint16());
    header.length = deserializer.read_uint32();
    header.sender_handle_id = deserializer.read_uint32();
    header.protocol_version = deserializer.read_uint8();
    header.operation_type = static_cast<IpCmdTypes::OperationType>(deserializer.read_uint8());
    header.data_type = static_cast<IpCmdTypes::DataType>(deserializer.read_uint8());
    header.process_flag_and_reserved = deserializer.read_uint8();

    // TODO: More validation here or somewhere else? Where should e.g. header.length >= 8 be
    // checked ()? Log if sender_handle_id is not according to spec (see 4.3.2.5)?

    return header;
}

void VCCPDUHeader::to_data(std::vector<std::uint8_t> &data) const {
    NetSerializer serializer(data);

    serializer.write_uint16(static_cast<uint16_t>(service_id));
    serializer.write_uint16(static_cast<uint16_t>(operation_id));
    serializer.write_uint32(length);
    serializer.write_uint32(sender_handle_id);
    serializer.write_uint8(protocol_version);
    serializer.write_uint8(static_cast<uint8_t>(operation_type));
    serializer.write_uint8(static_cast<uint8_t>(data_type));
    serializer.write_uint8(process_flag_and_reserved);
}
}
