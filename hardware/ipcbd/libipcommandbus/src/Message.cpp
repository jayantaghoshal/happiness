
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
        case VCM:
            return "VCM";
        case TEM:
            return "TEM";
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
