/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "signal_handler.h"

#include "asn_codec.h"
#include "infotainmentIpBus.h"
#include "type_conversion_helpers.h"

#include "conversion_utils.h"

#undef LOG_TAG
#define LOG_TAG "ConMan.SignalHandler"
#include <cutils/log.h>

namespace {
constexpr int COOKIE = 1001;
constexpr Connectivity::VccIpCmd::OperationId NOTIFICATIONS_TO_SUBSCRIBE[] = {
        Connectivity::VccIpCmd::OperationId::WLANMode};
}  // namespace

namespace vcc {
namespace conman {

SignalHandler::SignalHandler(const std::weak_ptr<IConnectivityManager>& connectivity_manager)
    : dispatcher_(tarmac::eventloop::IDispatcher::GetDefaultDispatcher()) {
    if (auto spt = connectivity_manager.lock()) {
        connectivity_manager_ = connectivity_manager;
        // std::swap(connectivity_manager_, connectivity_manager);
    } else {
        throw std::runtime_error("Connectivity Manager has expired");
    }

    StartSubscribe();
}

void SignalHandler::StartSubscribe() {
    ALOGD("%s", __func__);
    {
        std::lock_guard<std::mutex> lock(guard_);
        ipcb_server_ = vehicle_com::IVehicleCom::tryGetService("ipcb");
        if (ipcb_server_) {
            ALOGD("IPCB Server found. Subscribing...");

            bool subscription_success = false;

            for (auto operation_id : NOTIFICATIONS_TO_SUBSCRIBE) {
                subscription_success = Subscribe(Connectivity::VccIpCmd::ServiceId::WLAN,
                                                 operation_id,
                                                 vehicle_com::OperationType::NOTIFICATION,
                                                 this);
                if (!subscription_success) {
                    break;
                }
            }

            if (subscription_success) {
                ALOGI("Subscription Successful! ");
                hidl_death_recipient* this_as_recipient = this;
                ipcb_server_->linkToDeath(this_as_recipient, COOKIE);
                return;
            }
        } else {
            ALOGW("No IPCB Server found.");
        }
    }

    ALOGE("Subscription Failed. Scheduling retry...");
    UnsubscribeAll();
    using namespace std::chrono_literals;
    dispatcher_.EnqueueWithDelay(1s, [this]() { StartSubscribe(); });
}

bool SignalHandler::Subscribe(const Connectivity::VccIpCmd::ServiceId& service_id,
                              const Connectivity::VccIpCmd::OperationId& operation_id,
                              const vehicle_com::OperationType& operation_type,
                              vehicle_com::IMessageCallback* message_callback) {
    vehicle_com::SubscribeResult subscribe_result;
    ::android::hardware::Return<void> result;

    result = ipcb_server_->subscribe(static_cast<uint16_t>(service_id),
                                     static_cast<uint16_t>(operation_id),
                                     operation_type,
                                     message_callback,
                                     [&subscribe_result](vehicle_com::SubscribeResult sr) { subscribe_result = sr; });

    if (result.isOk() && subscribe_result.commandResult.success) {
        subscription_ids_.push_back(subscribe_result.subscriberId);
        return true;
    }
    return false;
}

void SignalHandler::UnsubscribeAll() {
    std::lock_guard<std::mutex> lock(guard_);
    if (ipcb_server_) {
        vehicle_com::CommandResult cresult;
        for (auto subscription_id : subscription_ids_) {
            ipcb_server_->unsubscribe(subscription_id, [&cresult](vehicle_com::CommandResult cr) { cresult = cr; });
            if (!cresult.success) {
                ALOGD("Failed to unsubscribe notification with id %lu", subscription_id);
            }
        }
    }
    subscription_ids_.clear();
}

::android::hardware::Return<void> SignalHandler::onMessageRcvd(const vehicle_com::Msg& msg) {
    ALOGD("Message received: %04X.%04X.%02hhX 0x%08X(size: %lu)",
          msg.pdu.header.serviceID,
          msg.pdu.header.operationID,
          msg.pdu.header.operationType,
          msg.pdu.header.seqNbr,
          msg.pdu.payload.size());

    if (static_cast<uint32_t>(Connectivity::VccIpCmd::OperationType::ERROR) ==
        static_cast<uint32_t>(msg.pdu.header.operationType)) {
        ALOGE("Received Error! (%04X.%04X) This is not handled!", msg.pdu.header.serviceID, msg.pdu.header.operationID);
        return ::android::hardware::Return<void>();
    }

    if (msg.pdu.header.operationID == static_cast<int>(Connectivity::VccIpCmd::OperationId::WLANMode)) {
        dispatcher_.Enqueue([msg, this]() {
            Icb_OpWLANMode_Notification p = nullptr;
            if (InfotainmentIpBus::Utils::DecodeMessage(msg.pdu.payload,
                                                        session_msgd_,
                                                        p,
                                                        Icb_OpWLANMode_Notification_Create,
                                                        Icb_OpWLANMode_Notification_Decode)) {
                // Decode ok.
                if (auto spt = connectivity_manager_.lock()) {
                    spt->NotifyWifiStationMode(utils::toHidl(p->wLANMode));
                }
            }
        });
    } else {
        ALOGV("Unhandled signal received! (%04X.%04X)", msg.pdu.header.serviceID, msg.pdu.header.operationID);
    }

    return ::android::hardware::Return<void>();
}

::android::hardware::Return<void> SignalHandler::onResponseRcvd(const vehicle_com::Msg& resp) {
    ALOGD("Message received: %04X.%04X.%02hhX 0x%08X(size: %lu)",
          resp.pdu.header.serviceID,
          resp.pdu.header.operationID,
          resp.pdu.header.operationType,
          resp.pdu.header.seqNbr,
          resp.pdu.payload.size());

    if (static_cast<uint8_t>(Connectivity::VccIpCmd::OperationType::ERROR) ==
        static_cast<uint8_t>(resp.pdu.header.operationType)) {
        ALOGE("Received Error! (%04X.%04X) This is not handled!",
              resp.pdu.header.serviceID,
              resp.pdu.header.operationID);
        return ::android::hardware::Return<void>();
    }

    if (resp.pdu.header.operationID == static_cast<uint16_t>(Connectivity::VccIpCmd::OperationId::WLANMode)) {
        dispatcher_.Enqueue([resp, this]() {
            Icb_OpWLANMode_Response decoded_message = nullptr;
            if (InfotainmentIpBus::Utils::DecodeMessage(resp.pdu.payload,
                                                        session_msgd_,
                                                        decoded_message,
                                                        Icb_OpWLANMode_Response_Create,
                                                        Icb_OpWLANMode_Response_Decode)) {
                // Decode ok.
                if (auto spt = connectivity_manager_.lock()) {
                    spt->NotifyWifiStationMode(utils::toHidl(decoded_message->wLANMode));
                }
            }
        });
    } else {
        ALOGV("Unhandled signal received! (%04X.%04X)", resp.pdu.header.serviceID, resp.pdu.header.operationID);
    }

    return ::android::hardware::Return<void>();
}

::android::hardware::Return<void> SignalHandler::onErrorRcvd(const vehicle_com::Error& error) {
    ALOGW("Error received: %04X.%04X", error.errorCode, error.errorInfo);

    return ::android::hardware::Return<void>();
}

void SignalHandler::serviceDied(uint64_t cookie, const ::android::wp<android_hidl::IBase>& who) {
    (void)who;
    if (COOKIE == cookie) {
        ALOGW("Service Died. Retry subscribing...");
        StartSubscribe();
    }
}

bool SignalHandler::RequestWifiStationMode() {
    ALOGD("%s", __FUNCTION__);

    vehicle_com::Pdu pdu;
    pdu.header.serviceID = static_cast<uint16_t>(Connectivity::VccIpCmd::ServiceId::WLAN);
    pdu.header.operationID = static_cast<uint16_t>(Connectivity::VccIpCmd::OperationId::WLANMode);
    pdu.header.operationType = vehicle_com::OperationType::REQUEST;
    pdu.header.encoded = true;
    pdu.header.seqNbr = sequence_nr_++;

    vehicle_com::Msg msg = {.ecu = vehicle::Ecu::TCAM, .pdu = pdu};
    vehicle_com::RetryInfo info = {.overrideDefault = false};
    vehicle_com::IResponseCallback* this_as_callback = this;

    vehicle_com::CommandResult command_result;
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (ipcb_server_) {
            ipcb_server_->sendRequest(msg, info, this_as_callback, [&command_result](vehicle_com::CommandResult cr) {
                command_result = cr;
            });
        }
    }

    if (!command_result.success) {
        ALOGW("Failed to send WLAN Mode Get-Request.");
        return false;
    }

    return true;
}

bool SignalHandler::RequestSetWifiStationMode(const conman_hal_v1_0::WifiStationMode& mode) {
    std::vector<uint8_t> payload;
    Icb_OpWLANMode_SetRequest msg_to_encode = Icb_OpWLANMode_SetRequest_Create(session_msgd_);
    msg_to_encode->wLANMode = utils::toIpcb(mode);

    InfotainmentIpBus::Utils::encodeMessage(
            msg_to_encode, Icb_OpWLANMode_SetRequest_Encode, Icb_OpWLANMode_SetRequest_EncodedSize, &payload);

    vehicle_com::Pdu pdu;
    pdu.header.serviceID = static_cast<uint16_t>(Connectivity::VccIpCmd::ServiceId::WLAN);
    pdu.header.operationID = static_cast<uint16_t>(Connectivity::VccIpCmd::OperationId::WLANMode);
    pdu.header.operationType = vehicle_com::OperationType::SETREQUEST;
    pdu.header.encoded = true;
    pdu.header.seqNbr = sequence_nr_++;
    pdu.payload = std::move(payload);

    vehicle_com::Msg msg = {.ecu = vehicle::Ecu::TCAM, .pdu = pdu};
    vehicle_com::RetryInfo info = {.overrideDefault = false};
    vehicle_com::IResponseCallback* this_as_callback = this;

    vehicle_com::CommandResult command_result;
    {
        std::lock_guard<std::mutex> lock(guard_);
        if (ipcb_server_) {
            ipcb_server_->sendRequest(msg, info, this_as_callback, [&command_result](vehicle_com::CommandResult cr) {
                command_result = cr;
            });
        }
    }

    if (!command_result.success) {
        ALOGW("Failed to send WLAN Mode Set-Request.");
        return false;
    }

    return true;
}

}  // namespace conman
}  // namespace vcc