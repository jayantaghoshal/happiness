/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/**
 * @file
 *
 * @brief Message class
 *
 */
#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_MESSAGE_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_MESSAGE_H_

#include <cstdint>
#include <memory>

#include "ipcommandbus/Pdu.h"

namespace Connectivity {
/**
 * Container for a Pdu and some extra meta data for the Pdu.
 * Intention is to use this to transfer PDUs between transport services and higher layers.
 * Also stores timeout data for acks/responses.
 */
struct Message {
    enum Ecu {
        UNKNOWN = 0x00,
        ALL = 0x01,
        IHU = 0x02,
        VCM = 0x03,
        TEM = 0x04,
        DIM = 0x05,
        TCAM = 0x06,
        VGM = 0x07,
        ENUM_NR_OF_ECUs
    };

    struct RetryInfo {
        bool override_default;
        uint8_t max_retries;
        uint16_t retry_timeout_ms;
    };

    Message() = default;

    Message(const Message &) = default;
    Message &operator=(const Message &) = default;

    Message(Message &&) = default;
    Message &operator=(Message &&) = default;

    Message(Pdu &&);

    static const char *EcuStr(Ecu ecu);

    /**
     * Creates a message given some header parameters.
     * Note that this will also create a new Pdu structure.
     *
     * @param[in] srvId                         The service id to set
     * @param[in] opId                          The operation id to set
     * @param[in] opType                        The operation to use.
     * @param[in] seqNr                         Unique number for this specific triplet(srvId, opId, opType). Typically
     * this is just a number that is increased by one each time the triplet is sent.
     * @param[in] data                          Payload to ship with the message
     * @param[in] dataType                      Defines if the payload is ASN.1/PER encoded or not.
     */
    Pdu pdu;

    Ecu ecu = UNKNOWN;

    RetryInfo retry_info = {false, 0, 0};
};
}

#endif  // _CONNECTIVITY_LIBIPCOMMANDBUS_MESSAGE_H_
