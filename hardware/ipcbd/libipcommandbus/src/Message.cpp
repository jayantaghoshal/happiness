/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ipcommandbus/Message.h"

namespace Connectivity {
const char* Message::EcuStr(Ecu ecu) {
    switch (ecu) {
        case UNKNOWN:
            return "UNKNOWN";
        case ALL:
            return "ALL";
        case IHU:
            return "IHU";
        case DIM:
            return "DIM";
        case TCAM:
            return "TCAM";
        case VGM:
            return "VGM";
        case ENUM_NR_OF_ECUs:
            return "UNKNOWN";

            // Intentionally left out "default:" since we will then get compile error on unhandled values.
    }

    return "UNKNOWN";
}

Message::Message(Pdu&& p) { pdu = p; }

}  // Connectivity
