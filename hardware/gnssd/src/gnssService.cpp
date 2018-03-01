/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "gnssService.h"

#include "VccIpCmdApi.h"
#include "asn_codec.h"
#include "type_conversion_helpers.h"

#define LOG_TAG "GnssD.service"
#include <cutils/log.h>

using ::vendor::volvocars::hardware::vehiclecom::V1_0::OperationType;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::CommandResult;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::SubscribeResult;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;
using ::vendor::volvocars::hardware::common::V1_0::Ecu;

using namespace Connectivity;

GnssService::GnssService() : timeProvider_{IDispatcher::GetDefaultDispatcher()} {
    //// Init for IpService base class.
    //// Fix me: Update IpService constructor with arguments and do work there. Avoid fiddling with details here and in
    /// other services...

    // IpService::service_name_ = "GNSS";
    ALOGI("GnssD started");

    expect_location_accuracy_ = false;  // set this according to which VCM ver you are currently using
    location_.timestamp = 0;
    location_.gnssLocationFlags = 0;

    m_session_msgd = ASN_Session_Create(m_session_buffer_msgd, sizeof(m_session_buffer_msgd));

    StartSubscribe();
}

void GnssService::serviceDied(uint64_t, const wp<IBase>&) {
    ALOGE("ipcbD died, Trying to reconnect");
    StartSubscribe();
}

void GnssService::StartSubscribe() {
    ipcbServer_ = IVehicleCom::getService("ipcb");

    if (ipcbServer_ != NULL) {
        connectionError = false;
        bool subscriptionsfailed = false;
        ALOGV("IpcbD found, subscribing");
        // Install callback
        IMessageCallback* this_as_callback = this;
        SubscribeResult resultPosNotification, resultPosCyclic, resultAccNotification, resultAccCyclic;
        // TODO: Handle subscription ID returned from subscribe in order to unsubscribe?

        auto returnPosNotification =
                ipcbServer_->subscribe((uint16_t)VccIpCmd::ServiceId::Positioning,
                                       (uint16_t)VccIpCmd::OperationId::GNSSPositionData,
                                       OperationType::NOTIFICATION,
                                       this_as_callback,
                                       [&resultPosNotification](SubscribeResult sr) { resultPosNotification = sr; });
        if (returnPosNotification.isOk() && resultPosNotification.commandResult.success) {
            // Save subscription ID
            pos_notification_id = resultPosNotification.subscriberId;
        } else {
            ALOGV("Failed to subscribe to GNSSPositionData with notification");
            subscriptionsfailed = true;
        }

        auto returnPosCyclic = ipcbServer_->subscribe((uint16_t)VccIpCmd::ServiceId::Positioning,
                                                      (uint16_t)VccIpCmd::OperationId::GNSSPositionData,
                                                      OperationType::NOTIFICATION_CYCLIC,
                                                      this_as_callback,
                                                      [&resultPosCyclic](SubscribeResult sr) { resultPosCyclic = sr; });
        if (returnPosCyclic.isOk() && resultPosCyclic.commandResult.success) {
            // Save subscription ID
            pos_cyclic_id = resultPosCyclic.subscriberId;
        } else {
            ALOGV("Failed to subscribe to GNSSPositionData with notification_cyclic");
            subscriptionsfailed = true;
        }

        auto returnAccNotification =
                ipcbServer_->subscribe((uint16_t)VccIpCmd::ServiceId::Positioning,
                                       (uint16_t)VccIpCmd::OperationId::GNSSPositionDataAccuracy,
                                       OperationType::NOTIFICATION,
                                       this_as_callback,
                                       [&resultAccNotification](SubscribeResult sr) { resultAccNotification = sr; });

        if (returnAccNotification.isOk() && resultAccNotification.commandResult.success) {
            // Save subscription ID
            acc_notification_id = resultAccNotification.subscriberId;
        } else {
            ALOGV("Failed to subscribe to GNSSPositionDataAccuracy with notification");
            subscriptionsfailed = true;
        }

        auto returnAccCyclic = ipcbServer_->subscribe((uint16_t)VccIpCmd::ServiceId::Positioning,
                                                      (uint16_t)VccIpCmd::OperationId::GNSSPositionDataAccuracy,
                                                      OperationType::NOTIFICATION_CYCLIC,
                                                      this_as_callback,
                                                      [&resultAccCyclic](SubscribeResult sr) { resultAccCyclic = sr; });
        if (returnAccCyclic.isOk() && resultAccCyclic.commandResult.success) {
            // Save subscription ID
            acc_cyclic_id = resultAccCyclic.subscriberId;
        } else {
            ALOGV("Failed to subscribe to GNSSPositionDataAccuracy with notification_cyclic");
            subscriptionsfailed = true;
        }

        // Check if all subscriptions was successful
        if (subscriptionsfailed || returnPosNotification.isDeadObject() || returnPosCyclic.isDeadObject() ||
            returnAccNotification.isDeadObject() || returnAccCyclic.isDeadObject()) {
            unsubscribeAll();
            connectionError = true;
            ALOGE("ipcbD is started & registered but gnssd failed to subscribe to notifications, retrying every 1 "
                  "second");
            timeProvider_.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { StartSubscribe(); });
        } else {
            // Great success, all subscriptions set
            ALOGI("GnssD all set and waiting for positions");
            hidl_death_recipient* this_as_recipient = this;
            ipcbServer_->linkToDeath(this_as_recipient, 0);
        }
    } else {
        if (!connectionError) {
            ALOGV("IpcbD not found, retrying every 1 second");
        }
        connectionError = true;
        timeProvider_.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { StartSubscribe(); });
    }
}

void GnssService::unsubscribeAll() {
    if (ipcbServer_ != NULL) {
        CommandResult cresult;
        if (pos_notification_id != 0) {
            ipcbServer_->unsubscribe(pos_notification_id, [&cresult](CommandResult cr) { cresult = cr; });
            pos_notification_id = 0;
            if (!cresult.success) {
                ALOGV("Failed to unsubscribe position notification");
            }
        }

        if (pos_cyclic_id != 0) {
            ipcbServer_->unsubscribe(pos_cyclic_id, [&cresult](CommandResult cr) { cresult = cr; });
            pos_cyclic_id = 0;
            if (!cresult.success) {
                ALOGV("Failed to unsubscribe position cyclic");
            }
        }

        if (acc_notification_id != 0) {
            ipcbServer_->unsubscribe(acc_notification_id, [&cresult](CommandResult cr) { cresult = cr; });
            acc_notification_id = 0;
            if (!cresult.success) {
                ALOGV("Failed to unsubscribe accuracy notification");
            }
        }

        if (acc_cyclic_id != 0) {
            ipcbServer_->unsubscribe(acc_cyclic_id, [&cresult](CommandResult cr) { cresult = cr; });
            acc_cyclic_id = 0;
            if (!cresult.success) {
                ALOGV("Failed to unsubscribe accuracy cyclic");
            }
        }
    }
}

// Methods from vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback follow.
Return<void> GnssService::onMessageRcvd(const Msg& msg) {
    ALOGV("onMessageRcvd %04X.%04X.%02d 0x%08X(size: %d)",
          msg.pdu.header.serviceID,
          (int)msg.pdu.header.operationID,
          (int)msg.pdu.header.operationType,
          msg.pdu.header.seqNbr,
          (int)msg.pdu.payload.size());

    // Handle error ? Should not be possible to get here i think....
    if (OperationType::ERROR == msg.pdu.header.operationType) {
        ALOGE("Received Error! (%04X.%04X) This is not handled!",
              msg.pdu.header.serviceID,
              (int)msg.pdu.header.operationID);

        return Void();
    }

    // Do we need to deep copy msg here? msg is allocated in lambda function in IpcbD and may be deallocated
    // if we dispatch it to a new thread?!

    if ((int)msg.pdu.header.operationID == (int)VccIpCmd::OperationId::GNSSPositionData) {
        IDispatcher::EnqueueTask([msg, this]() { GNSSPositionDataNotificationHandler(msg); });
    } else if ((int)msg.pdu.header.operationID == (int)VccIpCmd::OperationId::GNSSPositionDataAccuracy) {
        IDispatcher::EnqueueTask([msg, this]() { GNSSPositionDataAccuracyNotificationHandler(msg); });
    } else {
        ALOGV("Unhandled signal received! (%04X.%04X)", msg.pdu.header.serviceID, (int)msg.pdu.header.operationID);
    }

    return Void();
}

bool GnssService::Initialize() {
    //// Init for IpService base class.
    //// Fix me: Update IpService::Init() with arguments and do work there. Avoid fiddling with details here and in
    /// other services...
    // IpService::setDispatcher(IDispatcher::GetDefaultDispatcher());

    // Subscribe to ipcbd
    android::status_t status = gnss_.registerAsService();
    if (status != android::OK) {
        ALOGE("Failed to register Gnss binder service: %d", status);
        return false;
    } else {
        ALOGV("Gnss binder service register ok");
    }

    return true;
}

void GnssService::GNSSPositionDataNotificationHandler(const Msg& msg) {
    Icb_OpGNSSPositionData_Response p = nullptr;

    ALOGV("cbGNSSPositionDataNotification %04X.%04X.%02d 0x%08X. size: %d",
          (int)msg.pdu.header.serviceID,
          (int)msg.pdu.header.operationID,
          (int)msg.pdu.header.operationType,
          msg.pdu.header.seqNbr,
          (int)msg.pdu.payload.size());

    if (InfotainmentIpBus::Utils::DecodeMessage(msg.pdu.payload,
                                                m_session_msgd,
                                                p,
                                                Icb_OpGNSSPositionData_Response_Create,
                                                Icb_OpGNSSPositionData_Response_Decode)) {
        // All ok.
        const int64_t mssince1970 = InfotainmentIpBus::Utils::ToMsSince1970(p->gnssPositionData->utcTime);
        ALOGV("   UTC: %d-%d-%d %d:%d:%d:%ld",
              p->gnssPositionData->utcTime->year,
              p->gnssPositionData->utcTime->month,
              p->gnssPositionData->utcTime->day,
              p->gnssPositionData->utcTime->hour,
              p->gnssPositionData->utcTime->minute,
              p->gnssPositionData->utcTime->second,
              mssince1970);

#ifdef OpGNSSPositionData_PRINT_ENABLED
        Icb_OpGNSSPositionData_Response_Print(p);
#endif

        bool sendnow = false;
        if (!expect_location_accuracy_) {
            // we don't expect any accuracy signal to arrive so lets send this right now
            // We set some dummy values  for horizontal accuracy
            // otherwise Android completely filters out the location samples.
            location_.gnssLocationFlags |= (uint16_t)GnssLocationFlags::HAS_HORIZONTAL_ACCURACY;
            location_.horizontalAccuracyMeters = 10.0;
            sendnow = true;
        } else if (mssince1970 == location_.timestamp) {
            // We seem to have already received the accuracy signal so lets send now
            sendnow = true;
        } else if (expect_location_accuracy_ && location_.timestamp != 0) {
            // This is just to detect misconfigurations; that we expect accuracy but we dont seem to get them
            ALOGV("Received cbGNSSPositionDataNotification but no accuracy in sight");
        }
        location_.timestamp = mssince1970;

        location_.gnssLocationFlags |=
                (uint16_t)GnssLocationFlags::HAS_LAT_LONG | (uint16_t)GnssLocationFlags::HAS_ALTITUDE |
                (uint16_t)GnssLocationFlags::HAS_SPEED | (uint16_t)GnssLocationFlags::HAS_BEARING;

        location_.latitudeDegrees =
                InfotainmentIpBus::Utils::FixedPoint32ToDegreesD(p->gnssPositionData->position->longLat->latitude);
        location_.longitudeDegrees =
                InfotainmentIpBus::Utils::FixedPoint32ToDegreesD(p->gnssPositionData->position->longLat->longitude);
        location_.altitudeMeters = p->gnssPositionData->position->altitude / 10.0;    // dm -> meters
        location_.speedMetersPerSec = p->gnssPositionData->movement->speed / 1000.0;  // mm/s -> m/s
        location_.bearingDegrees = p->gnssPositionData->heading / 100.0;              // 1/100 degrees -> degrees
        ALOGV("lat=%.4lf , long=%.4lf", location_.latitudeDegrees, location_.longitudeDegrees);

        if (sendnow) {
            ALOGV("location send to gnss, triggered by location");
            gnss_.updateLocation(location_);
            location_.timestamp = 0;
            location_.gnssLocationFlags = 0;
        }
    }
}

void GnssService::GNSSPositionDataAccuracyNotificationHandler(const Msg& msg) {
    Icb_OpGNSSPositionDataAccuracy_Response p = nullptr;

    ALOGV("cbGNSSPositionDataAccuracyNotification %04X.%04X.%02d 0x%08X. size: %d",
          (int)msg.pdu.header.serviceID,
          (int)msg.pdu.header.operationID,
          (int)msg.pdu.header.operationType,
          msg.pdu.header.seqNbr,
          (int)msg.pdu.payload.size());

    if (InfotainmentIpBus::Utils::DecodeMessage(msg.pdu.payload,
                                                m_session_msgd,
                                                p,
                                                Icb_OpGNSSPositionDataAccuracy_Response_Create,
                                                Icb_OpGNSSPositionDataAccuracy_Response_Decode)) {
        // All ok.
        const int64_t mssince1970 = InfotainmentIpBus::Utils::ToMsSince1970(p->gnssPositionDataAccuracy->utcTime);
        ALOGV("   UTC: %d-%d-%d %d:%d:%d:%ld",
              p->gnssPositionDataAccuracy->utcTime->year,
              p->gnssPositionDataAccuracy->utcTime->month,
              p->gnssPositionDataAccuracy->utcTime->day,
              p->gnssPositionDataAccuracy->utcTime->hour,
              p->gnssPositionDataAccuracy->utcTime->minute,
              p->gnssPositionDataAccuracy->utcTime->second,
              mssince1970);

#ifdef OpGNSSPositionDataAccuracy_PRINT_ENABLED
        Icb_OpGNSSPositionDataAccuracy_Response_Print(p);
#endif
        bool sendnow = false;
        if (mssince1970 == location_.timestamp) {
            // We seem to have already received the location signal so lets send now
            sendnow = true;
        }
        location_.timestamp = mssince1970;

        if (!expect_location_accuracy_) {
            ALOGW("Strange, we dont expect accuracy but still receives it!!");
        }

        location_.gnssLocationFlags |= (uint16_t)GnssLocationFlags::HAS_HORIZONTAL_ACCURACY |
                                       (uint16_t)GnssLocationFlags::HAS_VERTICAL_ACCURACY;
        // we can currently not provide speed and bearing accuracy since it is not available from the VCM

        // In Android we can only report horizontal accuracy but what we receive are values for
        // the lat and long axis. Now we take "max" of these but maybe "min" or "mean" could be used!?
        double lat_accuracy = p->gnssPositionDataAccuracy->accuracy->sdLatitude / 100.0;   // cm -> m
        double long_accuracy = p->gnssPositionDataAccuracy->accuracy->sdLongitud / 100.0;  // cm -> m
        location_.horizontalAccuracyMeters = std::max(lat_accuracy, long_accuracy);

        location_.verticalAccuracyMeters = p->gnssPositionDataAccuracy->accuracy->sdAltitude / 100.0;  // cm -> m

        if (sendnow) {
            ALOGV("location send to gnss, triggered by accuracy");
            gnss_.updateLocation(location_);
            location_.timestamp = 0;
            location_.gnssLocationFlags = 0;
        }
    }
}
