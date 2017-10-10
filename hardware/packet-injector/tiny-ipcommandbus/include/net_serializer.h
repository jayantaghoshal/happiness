/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _TINY_IPCOMMANDBUS_NET_SERIALIZER_H_
#define _TINY_IPCOMMANDBUS_NET_SERIALIZER_H_

#include <cstdint>
#include <vector>

namespace Connectivity
{
    class NetSerializer
    {
    public:
        NetSerializer(std::vector<std::uint8_t> &vector) : vector(vector)
        {
        }

        void write_uint8(std::uint8_t value);
        void write_uint16(std::uint16_t value);
        void write_uint32(std::uint32_t value);

    private:
        std::vector<std::uint8_t> &vector;
    };
}

#endif // _TINY_IPCOMMANDBUS_NET_SERIALIZER_H_
