/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "ipcommandbus/Pdu.h"
#include <cutils/log.h>
#include <iomanip>
#include <sstream>

#define LOG_TAG "lipcb.Pdu"

namespace Connectivity {

// TODO: Why is fromData not const?
bool Pdu::fromData(std::vector<uint8_t> &fromData) {
    header = VCCPDUHeader::from_data(fromData);

    // Sanity check of header length field
    if (header.length < VCCPDUHeader::BASE_LENGTH ||
        header.length > fromData.end() - fromData.begin() - VCCPDUHeader::BASE_LENGTH) {
        ALOGE("Invalid header length parameter, some packets might be lost");
        fromData.erase(fromData.begin(), fromData.end());
        return false;
    }

    // Check header length
    uint32_t pduSize = VCCPDUHeader::DATA_SIZE + header.length - 8;
    if (fromData.size() < pduSize) {
        ALOGE("Not enough data in PDU buffer. Expected %d, but have %zu", pduSize, fromData.size());
        return false;
    }

    auto payloadStart = fromData.begin() + VCCPDUHeader::DATA_SIZE;
    auto payloadEnd = payloadStart + (header.length - 8);

    payload.assign(payloadStart, payloadEnd);
    fromData.erase(fromData.begin(), payloadEnd);

    return true;
}

void Pdu::toData(std::vector<uint8_t> &buffer) const {
    header.to_data(buffer);
    buffer.insert(buffer.end(), payload.begin(), payload.end());
}

void Pdu::createHeader(IpCmdTypes::ServiceId serviceId, IpCmdTypes::OperationId operationId,
                       IpCmdTypes::OperationType operationType, IpCmdTypes::DataType dataType, uint8_t sequenceId) {
    header.service_id = serviceId;
    header.operation_id = operationId;
    header.length = VCCPDUHeader::DATA_SIZE - 8;
    header.sender_handle_id = ((static_cast<uint16_t>(serviceId) & 0xFF) << 24) |
                              ((static_cast<uint16_t>(operationId) & 0xFF) << 16) |
                              ((static_cast<uint8_t>(operationType) & 0xFF) << 8) | sequenceId;
    header.protocol_version = 0;  // Protocol version will be set by TransportServicesupon send
    header.operation_type = operationType;
    header.data_type = dataType;
    header.process_flag_and_reserved = 0;
}

void Pdu::setPayload(std::vector<uint8_t> &&data) {
    payload = std::move(data);
    header.length = VCCPDUHeader::DATA_SIZE - 8 + payload.size();
}

std::string Pdu::toString(const Pdu &pdu) {
    std::stringstream ss;
    ss << "[" << std::setfill('0') << std::setw(8) << std::hex << pdu.header.sender_handle_id << "] ";
    ss << "(";
    ss << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(pdu.header.service_id) << ", ";
    ss << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(pdu.header.operation_id) << ", ";
    ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(pdu.header.operation_type);
    ss << ")";
    return ss.str();
}

}  // Connectivity
