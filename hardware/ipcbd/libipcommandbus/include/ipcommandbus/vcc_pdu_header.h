/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_VCC_PDU_HEADER_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_VCC_PDU_HEADER_H_

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

#include "ipcommandbus/IpCmdTypes.h"

namespace Connectivity
{
    struct VCCPDUHeader
    {
        static const std::size_t DATA_SIZE = 16;
        static const std::size_t BASE_LENGTH = DATA_SIZE - 8; ///< Length field contain totsl PDU length except first 8 bytes

        static VCCPDUHeader from_data(const std::vector<std::uint8_t> &data);
        void to_data(std::vector<std::uint8_t> &data) const;

        IpCmdTypes::ServiceId service_id = 0x0000; //Undefined
        IpCmdTypes::OperationId operation_id = 0x0000; //Undefined
        std::uint32_t length = 0;
        IpCmdTypes::SenderHandleId sender_handle_id = 0;
        std::uint8_t protocol_version = 0;
        IpCmdTypes::OperationType operation_type = IpCmdTypes::OperationType::UNDEFINED;
        IpCmdTypes::DataType  data_type = IpCmdTypes::DataType::UNDEFINED;
        std::uint8_t process_flag_and_reserved = 0;
    };
}

#endif // _CONNECTIVITY_LIBIPCOMMANDBUS_VCC_PDU_HEADER_H_
