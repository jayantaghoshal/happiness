#ifndef DIAGNOSTICS_CLIENT_H
#define DIAGNOSTICS_CLIENT_H

#include "ipcommandbus/idiagnostics_client.h"

namespace Connectivity
{

class DiagnosticsClient :
//    public IpService,
    public IDiagnosticsClient
{
public:
    /**
     * Constructor
     * @param timeout
     * @param sender
     */
    DiagnosticsClient()
    {
//        IpService::service_name_ = "DIAG";
    }

    /**
     * Initialize the client
     * @param msg_dispatcher Message dispatcher
     */
//    void Initialize(MessageDispatcher *msg_dispatcher);

    /**
     *   Set 'invalid data from <ecu>' diagnostic condition.
     *   This is a wrapper function.
     *   Actual work performed by SetInvalidDataFromTEM, SetInvalidDataFromVCM.
     */
    void SetInvalidData(const Message::Ecu ecu, const bool error) override;
};

}  // namespace

#endif  // DIAGNOSTICS_CLIENT_H
