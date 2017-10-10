#ifndef VENDOR_VOLVOCARS_HARDWARE_GNSSD_SRC_GNSSSERVICE_H
#define VENDOR_VOLVOCARS_HARDWARE_GNSSD_SRC_GNSSSERVICE_H

#include <IDispatcher.h>
#include <cutils/log.h>

extern "C" {
    #include <pl/asn_base/asn_base.h>
}

#include "binderimpl/gnss/Gnss.h"
#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IMessageCallback.h>

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using ::tarmac::eventloop::IDispatcher;

using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;

using namespace android::hardware::gnss::V1_0::implementation;
using namespace android::hardware::gnss::V1_0;
//using namespace InfotainmentIpService::Utils;


class GnssService : public IMessageCallback
{
public:
    GnssService();
    ~GnssService() = default;

    void StartSubscribe();

    // Methods from ::vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback follow.
    Return<void> onMessageRcvd(const Msg &msg) override;

    /**
       * Initialize IpLmService and the internal Link Manager functional block.
       */
    bool Initialize();

    void GNSSPositionDataNotificationHandler(const Msg &msg);
    void GNSSPositionDataAccuracyNotificationHandler(const Msg &msg);

private:
    sp<IVehicleCom> ipcbServer_;

    tarmac::eventloop::IDispatcher &timeProvider_;

    android::hardware::gnss::V1_0::implementation::Gnss gnss_;
    android::hardware::gnss::V1_0::GnssLocation location_;
    bool expect_location_accuracy_;

    ASN_BYTE m_session_buffer_msgd[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_msgd;
    bool connectionError = false;
};

#endif  // VENDOR_VOLVOCARS_HARDWARE_GNSSD_SRC_GNSSSERVICE_H
