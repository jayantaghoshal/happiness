/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/**
 * @file
 *
 * @brief Pdu class
 *
 */
#pragma once

#include "ipcommandbus/IpCmdTypes.h"
#include "ipcommandbus/vcc_pdu_header.h"

#include <vector>

namespace Connectivity {
/**
 * Container of the IP command bus PDU.
 * The data in this container is data actually sent on the bus.
 */
class Pdu {
  public:
    /**
     * Initializes this Pdu from data in the provided vector. Used data will be removed from the vector.
     *
     * @param[inout] fromData                       Vector with serialized Pdu data.
     * @return                                      True if header size and extracted bytes match.
     *                                              False if header contain incorrect length.
     */
    bool fromData(std::vector<uint8_t>& fromData);

    /**
     * Serialize this Pdu into a provided vector. Data will be appended to the vector.
     *
     * @param[inout] buffer                         Serialized Pdu data will be placed in this vector.
     */
    void toData(std::vector<uint8_t>& buffer) const;

    /**
     * Convenience function to set up the header fields. E.g. sets the length, sender handle id, reserved bits, etc.
     *
     * @param[in] serviceId                     The service id to set
     * @param[in] operationId                   The operation id to set
     * @param[in] operationType                 The operation to use.
     * @param[in] dataType                      Defines if the payload is ASN.1/PER encoded or not.
     * @param[in] sequenceId                    Unique number for this specific triplet(srvId, opId, opType). Typically
     * this is just a number that is increased by one each time the triplet is sent.
     */
    void createHeader(IpCmdTypes::ServiceId serviceId, IpCmdTypes::OperationId operationId,
                      IpCmdTypes::OperationType operationType, IpCmdTypes::DataType dataType, uint8_t sequenceId);

    /**
     * Assign Pdu payload.
     * @param[in] data                              The data to set.
     */
    void setPayload(std::vector<uint8_t>&& data);

    VCCPDUHeader header;           ///< Pdu header fields
    std::vector<uint8_t> payload;  ///< Pdu payload

    static std::string toString(const Pdu& pdu);
};

}  // Connectivity
