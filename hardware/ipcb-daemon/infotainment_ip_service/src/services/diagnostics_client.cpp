#define LOG_TAG "diagnostics_client"

#include <cutils/log.h>

#include "diagnostics_client.h"

namespace Connectivity
{

void DiagnosticsClient::SetInvalidData(const Message::Ecu ecu, const bool error)
{
    switch (ecu)
    {
        case Message::Ecu::TEM:
            ALOGI("TEM diag %d",error);
            break;

        case Message::Ecu::VCM:
            ALOGI("VCM diag %d",error);
            break;

        default:
            ALOGW("Diagnostics interface currently not supporting invalid data from '%s'", Message::EcuStr(ecu));
            break;
    }
}

}  // namespace
