/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "vipcomm/VipFramework.h"
#include "vipcomm/VipFrameworkPrivate.h"

#include <type_traits>

#include "logging.h"
LOG_SET_DEFAULT_CONTEXT(deleContext)


// Types needed by MpRouter_Signals.h below
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using boolean = bool;
// Include files generated by the Vector tool-chain
#include "gen/MpRouter_Signals.h"


// Create all DESink and DEInject variables
#include "ECD_dataelement.h"
using namespace ECDDataElement;
// These two files are generated by AutosarCodeGen/Autosar_Python_src/rungenerate.sh
#include "vsm/vsm_inject_variables_cpp.h"
#include "vsm/vsm_sink_variables_cpp.h"

namespace {
const uint16_t SignalGroup = 0x2000; // Signal group bit according to avmp
const uint16_t SignalIdMask = 0x3FFF; // Mask for the whole signalid including the signal group bit

bool initializeSinkInternal(const vipcomm::OutgoingAvmpHandler& outgoingAvmpHandler)
{
    static const vipcomm::OutgoingAvmpHandler sendAvmpMessageToVip(outgoingAvmpHandler);
    // Generated by AutosarCodeGen/Autosar_Python_src/rungenerate.sh
    #include "vsm/vsm_sink_subscribe_cpp.h"
    return true;
}

bool initializeInjectInternal()
{
    // Generated by AutosarCodeGen/Autosar_Python_src/rungenerate.sh
    #include "vsm/vsm_inject_instances_cpp.h"
    return true;
}
}

namespace vipcomm {

    bool initializeInject()
    {
        // Using an internal initialize function here together with a static variable is to
        // guarantee that the "real" initialization is only called once (indep on how many times
        // the user calls initializeInject()
        static const bool initialized( initializeInjectInternal() );
        return initialized;
    }

    bool initializeSink( const OutgoingAvmpHandler& outgoingAvmpHandler )
    {
        // Using an internal initialize function here together with a static variable is to
        // guarantee that the "real" initialization is only called once (indep on how many times
        // the user calls initializeSink(...)
        static const bool initialized( initializeSinkInternal(outgoingAvmpHandler) );
        return initialized;
    }

    void injectIncomingOkSignal(SignalID signalId, void* buffer, size_t length)
    {
        switch (signalId & SignalIdMask)
        {
            // Generated by AutosarCodeGen/Autosar_Python_src/rungenerate.sh
            #include "vsm/vsm_inject_switch_ok_cpp.h"

            default:
                log_error() << "handleIncomingOkSignal unknown signalId: " << signalId;
            break;
        }

    }

    void injectIncomingErrorSignal(SignalID signalId, uint8_t errorCode)
    {
        switch (signalId & SignalIdMask)
        {
            // Generated by AutosarCodeGen/Autosar_Python_src/rungenerate.sh
            #include "vsm/vsm_inject_switch_error_cpp.h"

            default:
                log_error() << "handleIncomingErrorSignal unknown signalId: " << signalId;
            break;
        }
    }

    const std::vector<autosar::DataElemInfo*>& getAllDataelements()
    {
        static const std::vector<autosar::DataElemInfo*> dataelements {
            #include "vsm/vsm_all_dataelements_cpp.h"
        };

        return dataelements;
    }
}
