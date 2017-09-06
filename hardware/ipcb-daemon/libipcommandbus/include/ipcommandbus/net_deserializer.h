/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _CONNECTIVITY_LIBIPCOMMANDBUS_NET_DESERIALIZER_H_
#define _CONNECTIVITY_LIBIPCOMMANDBUS_NET_DESERIALIZER_H_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Connectivity
{
    class NetDeserializer
    {
    public:
        NetDeserializer(const std::vector<std::uint8_t> &vector) : NetDeserializer(vector, 0)
        {
        }

        NetDeserializer(const std::vector<std::uint8_t> &vector,
                        std::vector<std::uint8_t>::size_type position)
            : vector(vector), position(position)
        {
        }

        std::size_t bytes_available() const;

        std::uint8_t read_uint8();
        std::uint16_t read_uint16();
        std::uint32_t read_uint32();

    private:
        const std::vector<std::uint8_t> &vector;
        std::vector<std::uint8_t>::size_type position;
    };
}

#endif // _CONNECTIVITY_LIBIPCOMMANDBUS_NET_DESERIALIZER_H_
