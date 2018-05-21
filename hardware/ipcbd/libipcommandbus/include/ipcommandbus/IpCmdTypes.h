/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>

namespace Connectivity {
/**
 * Namespace for IP Command Types
 */
namespace IpCmdTypes {

/**
 * Typedefs for VCC IP Command Bus types for ASN1, used for convenience after breaking out
 * VCC IP Command Bus ASN1 types to it's own module.
 */
typedef std::uint16_t ServiceId;
typedef std::uint16_t OperationId;

/**
 * Possible Operation Types
 * Reference: VDR-NOTE-SWRS-31843799-INS-01-14 VCCIP.pdf [VCC IP Prot: 0020/;-1]
 */
enum class OperationType : std::uint8_t {
    REQUEST = 0x00,
    SETREQUEST_NORETURN = 0x01,
    SETREQUEST = 0x02,
    NOTIFICATION_REQUEST = 0x03,
    RESPONSE = 0x04,
    NOTIFICATION = 0x05,
    NOTIFICATION_CYCLIC = 0x06,
    ACK = 0x70,
    ERROR = 0xE0,
    UNDEFINED = 0xFF,
};

/**
 * Possible Data Types, i.e. type of data payload.
 * Reference: VDR-NOTE-SWRS-31843799-INS-01-14 VCCIP.pdf [VCC IP Prot: 0094/;-0]
 */
enum class DataType : std::uint8_t {
    ENCODED = 0x00,
    NOT_ENCODED = 0x01,
    UNDEFINED = 0xFF,
};

typedef std::uint32_t SenderHandleId;

/**
 * Returns the string literal for the passed enumerations.
 */
std::string toString(ServiceId si);
std::string toString(ServiceId si, OperationId oi, const char* separator = ".");

/**
 * Returns the string literal for the passed enumeration.
 */
const char* toString(OperationType ot);

/**
 * Returns the string literal for the passed enumeration.
 */
const char* toString(DataType dt);

}  // namespace IpCmdTypes
}  // namespace Connectivity
