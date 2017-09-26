#include "gnssService.h"

extern "C" {
  #include "infotainmentIpBus.h"
}

#include "asn_decoder.h"
#include "type_conversion_helpers.h"
#include "VccIpCmdApi.h"

#define LOG_TAG "GnssD.service"

using ::vendor::volvocars::hardware::ipcb::V1_0::OperationType;
using ::vendor::volvocars::hardware::ipcb::V1_0::Msg;
using ::vendor::volvocars::hardware::common::V1_0::Ecu;

using namespace Connectivity;

GnssService::GnssService() : timeProvider_{IDispatcher::GetDefaultDispatcher()}
{
    //// Init for IpService base class.
    //// Fix me: Update IpService constructor with arguments and do work there. Avoid fiddling with details here and in
    /// other services...

    // IpService::service_name_ = "GNSS";

    expect_location_accuracy_ = false;  // set this according to which VCM ver you are currently using
    location_.timestamp = 0;
    location_.gnssLocationFlags = 0;

    ipcbServer_ = IIpcb::getService("ipcb");

    m_session_msgd = ASN_Session_Create(m_session_buffer_msgd, sizeof(m_session_buffer_msgd));

    // Install callback
    ipcbServer_.get()->subscribeMessage((uint16_t) VccIpCmd::ServiceId::Positioning,
                                        (uint16_t) VccIpCmd::OperationId::GNSSPositionData,
                                        {OperationType::NOTIFICATION, OperationType::NOTIFICATION_CYCLIC},
                                        this);

    ipcbServer_.get()->subscribeMessage((uint16_t)VccIpCmd::ServiceId::Positioning,
                                        (uint16_t)VccIpCmd::OperationId::GNSSPositionDataAccuracy,
                                        {OperationType::NOTIFICATION, OperationType::NOTIFICATION_CYCLIC},
                                        this);
}

// Methods from vendor::volvocars::hardware::ipcb::V1_0::IMessageCallback follow.
Return<void> GnssService::onMessageRcvd(const Msg &msg)
{
    ALOGV("onMessageRcvd %04X.%04X.%02d 0x%08X(size: %d)",
          msg.pdu.header.serviceID,
          (int)msg.pdu.header.operationID,
          (int)msg.pdu.header.operationType,
          msg.pdu.header.seqNbr,
          (int)msg.pdu.payload.size());

    // Handle error ? Should not be possible to get here i think....
    if (OperationType::ERROR == msg.pdu.header.operationType)
    {
        ALOGE("Received Error! (%04X.%04X) This is not handled!",
              msg.pdu.header.serviceID,
              (int)msg.pdu.header.operationID);

        return Void();
    }

    // Do we need to deep copy msg here? msg is allocated in lambda function in IpcbD and may be deallocated
    // if we dispatch it to a new thread?!

    if ((int)msg.pdu.header.operationID == (int)VccIpCmd::OperationId::GNSSPositionData)
    {
        IDispatcher::EnqueueTask([msg, this](){
            GNSSPositionDataNotificationHandler(msg);
        });
    }
    else if ((int)msg.pdu.header.operationID == (int)VccIpCmd::OperationId::GNSSPositionDataAccuracy)
    {
        IDispatcher::EnqueueTask([msg, this](){
            GNSSPositionDataAccuracyNotificationHandler(msg);
        });
    }
    else
    {
        ALOGV("Unhandled signal received! (%04X.%04X)",
              msg.pdu.header.serviceID,
              (int)msg.pdu.header.operationID);
    }

    return Void();
}

bool GnssService::Initialize()
{
    //// Init for IpService base class.
    //// Fix me: Update IpService::Init() with arguments and do work there. Avoid fiddling with details here and in
    /// other services...
    // IpService::setDispatcher(IDispatcher::GetDefaultDispatcher());

    // Subscribe to ipcbd

    android::status_t status = gnss_.registerAsService();

    if (status != android::OK)
    {
        ALOGE("Failed to register Gnss binder service: %d", status);
    }
    else
    {
        ALOGI("Gnss binder service register ok");
    }

    return true;
}

void GnssService::GNSSPositionDataNotificationHandler(const Msg &msg)
{
    Icb_OpGNSSPositionData_Response p = nullptr;

    ALOGD("cbGNSSPositionDataNotification %04X.%04X.%02d 0x%08X. size: %d",
          (int)msg.pdu.header.serviceID,
          (int)msg.pdu.header.operationID,
          (int)msg.pdu.header.operationType,
          msg.pdu.header.seqNbr,
          (int)msg.pdu.payload.size());

    if (InfotainmentIpBus::Utils::DecodeMessage(msg.pdu.payload,
                                                m_session_msgd,
                                                p,
                                                Icb_OpGNSSPositionData_Response_Create,
                                                Icb_OpGNSSPositionData_Response_Decode))
    {
        // All ok.
        const int64_t mssince1970 = InfotainmentIpBus::Utils::ToMsSince1970(p->gnssPositionData->utcTime);
        ALOGD("   UTC: %d-%d-%d %d:%d:%d:%ld",
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
        if (!expect_location_accuracy_)
        {
            // we don't expect any accuracy signal to arrive so lets send this right now
            // We set some dummy values  for horizontal accuracy
            // otherwise Android completely filters out the location samples.
            location_.gnssLocationFlags |= (uint16_t)GnssLocationFlags::HAS_HORIZONTAL_ACCURACY;
            location_.horizontalAccuracyMeters = 10.0;
            sendnow = true;
        }
        else if (mssince1970 == location_.timestamp)
        {
            // We seem to have already received the accuracy signal so lets send now
            sendnow = true;
        }
        else if (expect_location_accuracy_ && location_.timestamp != 0)
        {
            // This is just to detect misconfigurations; that we expect accuracy but we dont seem to get them
            ALOGW("Received cbGNSSPositionDataNotification but no accuracy in sight");
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
        ALOGD("lat=%.4lf , long=%.4lf", location_.latitudeDegrees, location_.longitudeDegrees);

        if (sendnow)
        {
            ALOGD("location send to gnss, triggered by location");
            gnss_.updateLocation(location_);
            location_.timestamp = 0;
            location_.gnssLocationFlags = 0;
        }
    }
}

void GnssService::GNSSPositionDataAccuracyNotificationHandler(const Msg &msg)
{
    Icb_OpGNSSPositionDataAccuracy_Response p = nullptr;

    ALOGD("cbGNSSPositionDataAccuracyNotification %04X.%04X.%02d 0x%08X. size: %d",
          (int)msg.pdu.header.serviceID,
          (int)msg.pdu.header.operationID,
          (int)msg.pdu.header.operationType,
          msg.pdu.header.seqNbr,
          (int)msg.pdu.payload.size());

    if (InfotainmentIpBus::Utils::DecodeMessage(msg.pdu.payload,
                      m_session_msgd,
                      p,
                      Icb_OpGNSSPositionDataAccuracy_Response_Create,
                      Icb_OpGNSSPositionDataAccuracy_Response_Decode))
    {
        // All ok.
        const int64_t mssince1970 = InfotainmentIpBus::Utils::ToMsSince1970(p->gnssPositionDataAccuracy->utcTime);
        ALOGD("   UTC: %d-%d-%d %d:%d:%d:%ld",
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
        if (mssince1970 == location_.timestamp)
        {
            // We seem to have already received the location signal so lets send now
            sendnow = true;
        }
        location_.timestamp = mssince1970;

        if (!expect_location_accuracy_)
        {
            ALOGW("Strange, we dont expect accuracy but still receives it!!");
        }

        location_.gnssLocationFlags |=
            (uint16_t)GnssLocationFlags::HAS_HORIZONTAL_ACCURACY | (uint16_t)GnssLocationFlags::HAS_VERTICAL_ACCURACY;
        // we can currently not provide speed and bearing accuracy since it is not available from the VCM

        // In Android we can only report horizontal accuracy but what we receive are values for
        // the lat and long axis. Now we take "max" of these but maybe "min" or "mean" could be used!?
        double lat_accuracy = p->gnssPositionDataAccuracy->accuracy->sdLatitude / 100.0;   // cm -> m
        double long_accuracy = p->gnssPositionDataAccuracy->accuracy->sdLongitud / 100.0;  // cm -> m
        location_.horizontalAccuracyMeters = std::max(lat_accuracy, long_accuracy);

        location_.verticalAccuracyMeters = p->gnssPositionDataAccuracy->accuracy->sdAltitude / 100.0;  // cm -> m

        if (sendnow)
        {
            ALOGD("location send to gnss, triggered by accuracy");
            gnss_.updateLocation(location_);
            location_.timestamp = 0;
            location_.gnssLocationFlags = 0;
        }
    }
}