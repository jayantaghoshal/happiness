#include <VccIpCmdApi.h>
#include <android/hardware/gnss/1.0/IGnss.h>
#include <android/hardware/gnss/1.0/IGnssCallback.h>
#include <android/hardware/gnss/1.0/types.h>
#include <asn_codec.h>
#include <cutils/log.h>
#include <gtest/gtest.h>
#include <ipcb_simulator.h>
#include <stdio.h>
#include <stdlib.h>
#include <type_conversion_helpers.h>
#include <chrono>
#include <future>

// Note that the auto-generated ASN1 files are in C, not C++
extern "C" {
#include "infotainmentIpBus.h"
}
extern "C" {
#include "pl/asn_base/asn_base.h"
}

using ::android::hardware::gnss::V1_0::IGnss;
using ::android::hardware::gnss::V1_0::IGnssCallback;
using ::android::hardware::gnss::V1_0::GnssLocation;
using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_string;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

#define LOG_TAG "VtsGnssIntegrationTest"
// Script to setup gnssd & ipcbd to use mocked ports

#define SHELLSCRIPT \
    "\
  killall -9 ipcbd \n\
  killall -9 gnssd \n\
  sleep 2 \n\
  export VCC_LOCALCONFIG_PATH=/data/local/tmp/localconfig.json \n\
  /vendor/bin/hw/ipcbd ipcb UDP & \n\
  /vendor/bin/hw/gnssd & \n\
  sleep 2 \n"

struct GnssCallback : public IGnssCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssCallback follow.
    Return<void> gnssLocationCb(const GnssLocation& location) override {
        ALOGD("GnssCallback invoked");
        if (onLocationCallback != NULL) {
            onLocationCallback(location);
        }
        return Void();
    };
    Return<void> gnssStatusCb(IGnssCallback::GnssStatusValue status) override { return Void(); };
    Return<void> gnssSvStatusCb(const IGnssCallback::GnssSvStatus& svInfo) override { return Void(); };
    Return<void> gnssNmeaCb(int64_t timestamp, const hidl_string& nmea) override { return Void(); };
    Return<void> gnssSetCapabilitesCb(uint32_t capabilities) override { return Void(); };
    Return<void> gnssAcquireWakelockCb() override { return Void(); };
    Return<void> gnssReleaseWakelockCb() override { return Void(); };
    Return<void> gnssRequestTimeCb() override { return Void(); };
    Return<void> gnssSetSystemInfoCb(const IGnssCallback::GnssSystemInfo& info) override { return Void(); };

    std::function<void(const GnssLocation& location)> onLocationCallback;
    // Methods from ::android::hidl::base::V1_0::IBase follow.
};

class VtsGnssIntegrationTest : public ::testing::Test {
  protected:
    static void SetUpTestCase() {
        ALOGD("Setup services!");
        system(SHELLSCRIPT);
    }
    sp<IGnss> gnssServer_;
};

TEST_F(VtsGnssIntegrationTest, recievedOk) {
    ALOGI("Starting test recievedOk!");
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    std::vector<uint8_t> payload;

    sp<GnssCallback> callback = new GnssCallback();
    ALOGD("Connect to service!");
    gnssServer_ = IGnss::getService();
    ASSERT_TRUE(gnssServer_ != NULL);
    gnssServer_->setCallback(callback);
    callback->onLocationCallback = [&promise](const GnssLocation& location) {
        ALOGD("Lat: %f Long: %f, time %lu, altitudeMeters %f, speed %f, heading %f", location.latitudeDegrees,
              location.longitudeDegrees, location.timestamp, location.altitudeMeters, location.speedMetersPerSec,
              location.bearingDegrees);

        EXPECT_FLOAT_EQ(location.latitudeDegrees, -20.025057);
        EXPECT_FLOAT_EQ(location.longitudeDegrees, -57.550241);
        EXPECT_EQ(location.timestamp, 1507190462000);
        EXPECT_FLOAT_EQ(location.altitudeMeters, 2.1);
        EXPECT_FLOAT_EQ(location.speedMetersPerSec, 9);
        EXPECT_FLOAT_EQ(location.bearingDegrees, 2.34);
        promise.set_value(1);
    };
    gnssServer_->start();

    ALOGD("Creating position message!");
    ASN_BYTE m_session_buffer_send[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session = ASN_Session_Create(m_session_buffer_send, sizeof(m_session_buffer_send));
    Icb_OpGNSSPositionData_Response msg = Icb_OpGNSSPositionData_Response_Create(m_session);

    Pdu temp_pdu;
    temp_pdu.createHeader((IpCmdTypes::ServiceId)Connectivity::VccIpCmd::ServiceId::Positioning,
                          (IpCmdTypes::OperationId)Connectivity::VccIpCmd::OperationId::GNSSPositionData,
                          Connectivity::IpCmdTypes::OperationType::NOTIFICATION,
                          Connectivity::IpCmdTypes::DataType::ENCODED, 1);
    temp_pdu.header.protocol_version = 2;
    msg->gnssPositionData->utcTime->year = 2017;
    msg->gnssPositionData->utcTime->month = 10;
    msg->gnssPositionData->utcTime->day = 5;
    msg->gnssPositionData->utcTime->hour = 8;
    msg->gnssPositionData->utcTime->minute = 1;
    msg->gnssPositionData->utcTime->second = 2;
    msg->gnssPositionData->position->longLat->latitude = S32(-20.025057 * ((double(1 << 30)) / 90.0));
    msg->gnssPositionData->position->longLat->longitude = S32(-57.550241 * ((double(1 << 30)) / 90.0));
    msg->gnssPositionData->position->altitude = 21;
    msg->gnssPositionData->movement->speed = 9000;
    msg->gnssPositionData->heading = 234;

    ALOGD("Encode message!");
    InfotainmentIpBus::Utils::encodeMessage(msg, Icb_OpGNSSPositionData_Response_Encode,
                                            Icb_OpGNSSPositionData_Response_EncodedSize, &payload);
    temp_pdu.setPayload(std::move(payload));
    IpcbSimulator CyclicInjector("127.0.0.1", 60012, 60001, 0);
    CyclicInjector.SendPdu(temp_pdu);
    ALOGD("Message sent!");

    // promise to keep thread alive
    std::future_status status = future.wait_for(std::chrono::milliseconds(2000));
    if (status == std::future_status::deferred) {
        ALOGD("Promise deferred exiting");
        FAIL();
    } else if (status == std::future_status::timeout) {
        ALOGD("Timeout:Didn't recive any gnss message within timelimit. Exiting");
        FAIL();
    } else if (status == std::future_status::ready) {
        ALOGD("Promise completed");
    }
    gnssServer_->stop();
    ALOGI("Exiting test done!");
}
