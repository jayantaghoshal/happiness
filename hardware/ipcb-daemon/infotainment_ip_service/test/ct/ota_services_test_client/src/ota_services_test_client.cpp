/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <ivi-logging.h>
#include <unistd.h>
#include <CommonAPI/CommonAPI.hpp>
#include <iostream>
#include <string>
#include <typeinfo>
#include <v0/org/volvo/connectivity/OTAProxy.hpp>
#include <vector>
#include "log_context.hpp"

namespace gen_otac = v0::org::volvo::connectivity;

LOG_DECLARE_CONTEXT(mainContext, "MAIN", "OTA Services")
std::shared_ptr<gen_otac::OTAProxy<>> ota_client_proxy = nullptr;

// can be used according to the need in near future
void DownloadStatusRequestAsyncCallback()
{
    log_info() << __FUNCTION__;
}

static void AvailabilityCallBack(const CommonAPI::AvailabilityStatus &status)
{
    log_info().writeFormatted("Availibility status changed for connectivity service %u", status);

    if (CommonAPI::AvailabilityStatus::AVAILABLE == status)
    {
        if (!ota_client_proxy->isAvailable())
        {
            log_info().writeFormatted("Callback status is inconsistent: %u", status);
            return;
        }
        log_info().writeFormatted("Service is available : %u", status);
    }
    else
        log_info() << "Connectivity service is not (no longer) available.";
}

const char *GetCallStatusStr(CommonAPI::CallStatus cs)
{
    switch (cs)
    {
        case CommonAPI::CallStatus::SUCCESS:
            return "SUCCESS";
        case CommonAPI::CallStatus::OUT_OF_MEMORY:
            return "OUT_OF_MEMORY";
        case CommonAPI::CallStatus::NOT_AVAILABLE:
            return "NOT_AVAILABLE";
        case CommonAPI::CallStatus::CONNECTION_FAILED:
            return "CONNECTION_FAILED";
        case CommonAPI::CallStatus::REMOTE_ERROR:
            return "REMOTE_ERROR";
        case CommonAPI::CallStatus::UNKNOWN:
            return "UNKNOWN";
        default:
            return "?";
    }
}

void OnDownloadStatusNotification(const gen_otac::OTATypes::DownloadStatus &dls,
                                  const std::vector<gen_otac::OTATypes::SoftwareProduct> &_downloadList)
{
    log_info() << __FUNCTION__;
    std::printf("%s - Dowload Status: %s \n", __FUNCTION__, dls);
    for (const gen_otac::OTATypes::SoftwareProduct &SW : _downloadList)
    {
        printf("SW: %s \n", SW);
    }
}

void DownloadStatusRequest(std::shared_ptr<gen_otac::OTAProxy<>> ota_client_proxy)
{
    log_info() << __FUNCTION__;
    CommonAPI::CallStatus call_status;
    gen_otac::IpCommandBusTypes::Infotainment_IP_bus_Errors error;
    gen_otac::OTATypes::DownloadStatus downloadStatus;
    std::vector<gen_otac::OTATypes::SoftwareProduct> list;
    ota_client_proxy->DownloadStatusRequest(call_status, error, downloadStatus, list);
    std::printf("call status %s, %s \n", __FUNCTION__, GetCallStatusStr(call_status));
    if (call_status == CommonAPI::CallStatus::SUCCESS)
    {
        for (const gen_otac::OTATypes::SoftwareProduct &SW : list)
        {
            std::printf("SW : %s \n", SW);
        }
    }
    else
        std::cout << "No SW to show.." << std::endl;
}

void DownloadStatusRequestAsync(std::shared_ptr<gen_otac::OTAProxy<>> ota_client_proxy)
{
    log_info() << __FUNCTION__;
    CommonAPI::CallInfo call_info(5000);  // wait for 5 seconds
    ota_client_proxy->DownloadStatusRequestAsync(std::bind(&DownloadStatusRequestAsyncCallback), &call_info);
}

void DownloadSoftwareSetRequest(std::shared_ptr<gen_otac::OTAProxy<>> ota_client_proxy)
{
    log_info() << __FUNCTION__;
    CommonAPI::CallStatus call_status;
    std::vector<gen_otac::OTATypes::UUID> uuid;
    ota_client_proxy->DownloadSoftwareSetRequest(call_status, uuid);
    std::printf("call status %s, %s \n", __FUNCTION__, GetCallStatusStr(call_status));
}

void DeleteStatusSetRequest(std::shared_ptr<gen_otac::OTAProxy<>> ota_client_proxy)
{
    log_info() << __FUNCTION__;
    CommonAPI::CallStatus call_status;
    gen_otac::IpCommandBusTypes::Infotainment_IP_bus_Errors error;
    std::vector<gen_otac::OTATypes::PartIdentifier> pid;
    ota_client_proxy->DeleteStatusSetRequest(call_status, error, pid);
    std::printf("call status %s, %s \n", __FUNCTION__, GetCallStatusStr(call_status));
}

void CancelDownloadSetRequest(std::shared_ptr<gen_otac::OTAProxy<>> ota_client_proxy)
{
    log_info() << __FUNCTION__;
    // uuid needs to be set
    gen_otac::OTATypes::UUID uuid;
    CommonAPI::CallStatus call_status;
    gen_otac::IpCommandBusTypes::Infotainment_IP_bus_Errors error;
    gen_otac::OTATypes::GenericOkStatus ok_status;
    ota_client_proxy->CancelDownloadSetRequest(uuid, call_status, error, ok_status);
    std::printf("call status %s, %s \n", __FUNCTION__, GetCallStatusStr(call_status));
}

int main()
{
    log_info() << "OTA Services test client started!";

    if (!ota_client_proxy)
    {
        // loading runtime
        auto runtime = CommonAPI::Runtime::get();
        assert(runtime.get());

        auto ota_client_proxy = runtime->buildProxy<gen_otac::OTAProxy>(
            "local", "org.volvocars.InfotainmentIpService", "InfotainmentIpServiceDbusConnection");

        std::cout << "Checking service availibilty.." << std::endl;
        log_info() << "Checking service availibilty..";

        while (!ota_client_proxy->isAvailable())
        {
            usleep(1000);
        }

        log_info() << "OTA Service is available!";
        std::cout << "OTA Service is available!" << std::endl;

        DownloadStatusRequest(ota_client_proxy);

        DownloadSoftwareSetRequest(ota_client_proxy);

        DeleteStatusSetRequest(ota_client_proxy);

        CancelDownloadSetRequest(ota_client_proxy);

        gen_otac::OTAProxyBase::DownloadStatusNotificationEvent::Listener dlsnListner(OnDownloadStatusNotification);
        gen_otac::OTAProxyBase::DownloadStatusNotificationEvent::Subscription dlsnSubscription =
            ota_client_proxy->getDownloadStatusNotificationEvent().subscribe(dlsnListner);
        (void)dlsnSubscription;
    }
    /*
     //It will be used in future, have to keep it commented
     if(ota_client_proxy != nullptr) {
     ota_client_proxy->getProxyStatusEvent().subscribe (std::bind(&AvailabilityCallBack, std::placeholders::_1));
     }
     */
}
