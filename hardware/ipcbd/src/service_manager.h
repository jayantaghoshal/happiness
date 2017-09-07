/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H

#include <IDispatcher.h>
#include <ipcommandbus/MessageDispatcher.h>
#include <cutils/log.h>
#include <string>
#include "services/diagnostics_client.h"

// #include <services/common_phone_tele_service.h>
// #include "services/car_modem_service_factory.h"
// #include "services/climate_timer_proxy.h"
// #include "services/common_phone_tele_proxy.h"
// #include "services/connectivity_service.h"
// #include "services/diagnostic_management_client.h"
// #include "services/diagnostic_management_service.h"
// #include "services/diagnostics_client.h"
// #include "services/enhanced_pos_client.h"
// #include "services/global_reset.h"
// #include "services/ota_service.h"
// #include "services/ota_swlm_service.h"
// #include "services/rvdc_client.h"
// #include "services/telematics_service.h"
// #include "services/voc_backend_proxy.h"
// #include "services/wifi_accesspoint.h"
// #include "services/wlan_service.h"
// #include "services/workshop_wlan_service.hpp"

namespace Connectivity
{
/**
 * Service manager for managing Common API service instances.
 *
 */
class ServiceManager
{
public:
    ServiceManager(const ServiceManager&) = delete;

    ServiceManager(tarmac::eventloop::IDispatcher& timeProvider/*, std::shared_ptr<CommonAPI::MainLoopContext> commonApiMainLoopContext*/);

public:
    /**
     * Initialize and register all available Common API services.
     * For more information about what register actually means see
     * RegisterCommonApiService documentation.
     *
     * @param[in] msg_dispatcher      Message dispatcher to use with services.
     */
    /**
     * Register all available Common API services.
     */
    void RegisterAllBinderServices(::Connectivity::MessageDispatcher* msg_dispatcher);

    /**
     * Unregister and uninitialize all registered Common API services.
     * For more information about what unregister actually means see
     * UnregisterCommonApiService documentation.
     */
    /**
     * Unregister all available Common API services.
     */
    void UnregisterAllBinderServices();

private:
    tarmac::eventloop::IDispatcher& timeProvider_;
    DiagnosticsClient diagnostics_client_;

    // EnhancedPosClient enhanced_pos_client_;
    // DiagnosticManagementService diagnostic_management_service;
    // DiagnosticManagementClient diagnostic_management_client;
    // ConnectivityService connectivity_service_;
    // RvdcClient rvdc_client_;
    // WlanService wlan_service_;
    // OTAService ota_service_;
    // OTASwlmService ota_swlm_service_;
    // TelematicsService telematics_service_;
    // CommonPhoneTelematicsService common_phone_telematics_service_;
    // CarModemServiceFactory car_modem_service_;
    // VOCBackendProxy voc_backend_proxy_;
    // ClimateTimerProxy climate_timer_proxy_;
    // GlobalReset global_reset_;
    // WifiAccessPointService wifiAccessPoint;
    // CommonPhoneTeleProxy common_phone_tele_proxy_;
    // WorkshopWlanService workshop_wlan_service_;

    static const std::string s_instance;  // default instance
    static const std::string s_domain;    // default domain
};

}  // namespace Connectivity

#endif  // SERVICE_MANAGER_H
