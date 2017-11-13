#include "ipcommandbus/IpCmdTypes.h"

#include <cstdio>
#include <vector>

namespace Connectivity {
namespace IpCmdTypes {

const char *toString(OperationType ot) {
    switch (ot) {
        case OperationType::REQUEST:
            return "REQUEST";
        case OperationType::SETREQUEST_NORETURN:
            return "SETREQUEST_NORETURN";
        case OperationType::SETREQUEST:
            return "SETREQUEST";
        case OperationType::NOTIFICATION_REQUEST:
            return "NOTIFICATION_REQUEST";
        case OperationType::RESPONSE:
            return "RESPONSE";
        case OperationType::NOTIFICATION:
            return "NOTIFICATION";
        case OperationType::NOTIFICATION_CYCLIC:
            return "NOTIFICATION_CYCLIC";
        case OperationType::ACK:
            return "ACK";
        case OperationType::ERROR:
            return "ERROR";
        case OperationType::UNDEFINED:
            return "UNDEFINED";
        default:
            return "<Unknown>";
    }
}

const char *toString(DataType dt) {
    switch (dt) {
        case DataType::ENCODED:
            return "ENCODED";
        case DataType::NOT_ENCODED:
            return "NOT_ENCODED";
        case DataType::UNDEFINED:
            return "UNDEFINED";
        default:
            return "<Unknown>";
    }
}

}  // IpCmdTypes
}  // Connectivity
