/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "ipcommandbus/TransportServices.h"

#include <ivi-logging.h>

namespace Connectivity
{
LOG_DECLARE_DEFAULT_CONTEXT(g_ts_log_context, "TS00", "Transport Services")

const char *TransportServices::ErrorTypeToCString(ErrorType error_type)
{
    switch (error_type)
    {
        case ErrorType::OK:
        {
            return "OK";
        }
        break;
        case ErrorType::REMOTE_ERROR:
        {
            return "REMOTE_ERROR";
        }
        break;
        case ErrorType::LOCAL_TIMEOUT:
        {
            return "LOCAL_TIMEOUT";
        }
        break;
        case ErrorType::PAYLOAD_DECODE_ERROR:
        {
            return "PAYLOAD_DECODE_ERROR";
        }
        break;
    }
    return ("Unknown error type");
}

}  // namespace Connectivity
