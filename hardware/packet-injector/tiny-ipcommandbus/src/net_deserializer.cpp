/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <cassert>
#include <cstdint>
#include <vector>

#include "tiny-ipcommandbus/include/net_deserializer.h"

namespace Connectivity
{
    std::size_t NetDeserializer::bytes_available() const
    {
        return position >= vector.size() ? 0 : vector.size() - position;
    }

    std::uint8_t NetDeserializer::read_uint8()
    {
        // TODO: Document that it is up to caller to check that
        //       "data.size() >= 1" so this does not occur. Or rework to
        //       return value and error flag tuple. For now return 0.
        //       This previously used exceptions thrown by vector::at().
        if (bytes_available() < 1) {
            assert(bytes_available() > 0);
            return 0;
        }
        std::uint8_t value = static_cast<std::uint8_t>(vector.at(position));
        position++;
        return value;
    }

    std::uint16_t NetDeserializer::read_uint16()
    {
        return (read_uint8() << 8) | read_uint8();
    }

    std::uint32_t NetDeserializer::read_uint32()
    {
        return (read_uint8() << 24) | (read_uint8() << 16) | (read_uint8() << 8) | read_uint8();
    }
}
