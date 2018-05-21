/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <VccIpCmdApi.h>
#include <android/hardware/gnss/1.0/IGnss.h>
#include <android/hardware/gnss/1.0/IGnssCallback.h>
#include <android/hardware/gnss/1.0/types.h>
#include <asn_codec.h>
#include <gtest/gtest.h>
#include <ipcb_simulator.h>
#include <type_conversion_helpers.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <thread>

// Note that the auto-generated ASN1 files are in C, not C++
extern "C" {
#include "infotainmentIpBus.h"
#include "pl/asn_base/asn_base.h"
}

#undef LOG_TAG
#define LOG_TAG "VtsGnssIntegrationTest"
#include <cutils/log.h>

using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;

using ::android::sp;
using ::android::hardware::hidl_string;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::gnss::V1_0::GnssLocation;
using ::android::hardware::gnss::V1_0::IGnss;
using ::android::hardware::gnss::V1_0::IGnssCallback;
using ::android::hidl::base::V1_0::DebugInfo;

namespace {

constexpr auto TCAM_INTERFACE_NAME = "tcam0_ac:0";
constexpr auto IHU_IP = "198.19.101.66";
constexpr auto BROADCAST_IP = "198.19.101.95";
constexpr auto TCAM_IP = "198.19.101.67";
constexpr auto NETMASK = "255.255.255.224";
constexpr uint16_t IHU_PORT = 50000;
constexpr uint16_t IHU_BROADCAST_PORT = 50001;
constexpr uint16_t TCAM_PORT = 50000;
constexpr uint16_t TCAM_BROADCAST_PORT = 50002;

std::vector<std::pair<Connectivity::Message::Ecu, Connectivity::ISocket::EcuAddress>> EcuMap() {
    using namespace Connectivity;
    return std::vector<std::pair<Message::Ecu, ISocket::EcuAddress>>{
            std::make_pair(Message::Ecu::IHU, ISocket::EcuAddress{IHU_IP, IHU_PORT}),
            std::make_pair(Message::Ecu::ALL, ISocket::EcuAddress{BROADCAST_IP, TCAM_BROADCAST_PORT}),
            std::make_pair(Message::Ecu::TCAM, ISocket::EcuAddress{TCAM_IP, TCAM_PORT})};
}

struct GnssCallback : public IGnssCallback {
    // Methods from ::android::hardware::gnss::V1_0::IGnssCallback follow.
    Return<void> gnssLocationCb(const GnssLocation& location) override {
        ALOGD("GnssCallback invoked");
        if (onLocationCallback != nullptr) {
            onLocationCallback(location);
        }
        return Void();
    };
    Return<void> gnssStatusCb(IGnssCallback::GnssStatusValue status) override {
        (void)status;
        return Void();
    };
    Return<void> gnssSvStatusCb(const IGnssCallback::GnssSvStatus& svInfo) override {
        (void)svInfo;
        return Void();
    };
    Return<void> gnssNmeaCb(int64_t timestamp, const hidl_string& nmea) override {
        (void)timestamp;
        (void)nmea;
        return Void();
    };
    Return<void> gnssSetCapabilitesCb(uint32_t capabilities) override {
        (void)capabilities;
        return Void();
    };
    Return<void> gnssAcquireWakelockCb() override { return Void(); };
    Return<void> gnssReleaseWakelockCb() override { return Void(); };
    Return<void> gnssRequestTimeCb() override { return Void(); };
    Return<void> gnssSetSystemInfoCb(const IGnssCallback::GnssSystemInfo& info) override {
        (void)info;
        return Void();
    };

    std::function<void(const GnssLocation& location)> onLocationCallback;
    // Methods from ::android::hidl::base::V1_0::IBase follow.
};
}  // namespace

class VtsGnssIntegrationTest : public ::testing::Test {
  protected:
    static void SetUpTestCase() {
        ALOGV("+ %s", __func__);

        // Move test case into namespace for network mocks to work
        int file_descriptor;
        const char* namespace_path = "/dev/vendor/netns/vcc";

        file_descriptor = open(namespace_path, O_RDONLY | O_CLOEXEC);
        if (file_descriptor > 0) {
            if (setns(file_descriptor, CLONE_NEWNET) != 0) {
                ASSERT_TRUE(false) << "Set NS failed!";
            } else {
                ALOGD("Namespace is: %s", namespace_path);
            }
        } else {
            ASSERT_TRUE(false) << "Open NS filedescriptor failed!";
        }
        close(file_descriptor);

        // Start IpcbD for test with mocked localconfig
        getCmdOut("stop ipcbd-infotainment");
        getCmdOut("start ipcbd-infotainment");

        // Wait for service to start
        uint8_t count = 0;
        while (IVehicleCom::tryGetService("ipcb")) {
            ASSERT_TRUE(20 == ++count) << "Timed out while waiting for service to be registered";
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }

        // loopback interface is needed, bring it up.
        getCmdOut("/vendor/bin/ifconfig lo up");
        // Set up an alias for the TCAM simulation to bind to.
        getCmdOut("/vendor/bin/ifconfig " + std::string(TCAM_INTERFACE_NAME) + " " + TCAM_IP + " netmask " +
                  std::string(NETMASK) + " up");
        // Mangle the packets to spoof TCAM sending broadcast packages.
        getCmdOut("/vendor/bin/iptables -t nat -I POSTROUTING -s " + std::string(IHU_IP) + " -p udp --dport " +
                  std::to_string(IHU_BROADCAST_PORT) + " -j SNAT --to-source " + std::string(TCAM_IP) + " -w");

        /*
         * NATing is only used for the first packet in a connection. For UDP a connection is defined as established as
         * long as the time out after the last received packet does not fire. Therefore, in order to enforce NATing on
         * every packet we set this time out to 0, 30 is the default.
         */
        getCmdOut("sysctl -w net.netfilter.nf_conntrack_udp_timeout=0");

        // Waiting for GNSS service is started and fully connected to ipcb.
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        ALOGV("- %s", __func__);
    }

    static void TearDownTestCase() {
        ALOGV("+ %s", __func__);

        // Clean up network configurations.
        // Delete tcam alias
        getCmdOut("/vendor/bin/ifconfig " + std::string(TCAM_INTERFACE_NAME) + " down");
        // Delete mangle rules for spoofing TCAM sending broadcast packages.
        getCmdOut("/vendor/bin/iptables -t nat -D POSTROUTING -s " + std::string(IHU_IP) + " -p udp --dport " +
                  std::to_string(IHU_BROADCAST_PORT) + " -j SNAT --to-source " + std::string(TCAM_IP) + " -w");

        // Restore UDP connection timeout
        getCmdOut("sysctl -w net.netfilter.nf_conntrack_udp_timeout=30");

        // Restart IpcbD service to restore state
        getCmdOut("stop ipcbd-infotainment");
        getCmdOut("start ipcbd-infotainment");

        ALOGV("- %s", __func__);
    }

    void SetUp() override {
        ALOGV("+ %s", __func__);
        ALOGV("- %s", __func__);
    }

    void TearDown() override {
        ALOGV("+ %s", __func__);
        ALOGV("- %s", __func__);
    }

    static std::string getCmdOut(const std::string& cmd) {
        ALOGD("Running command: %s", cmd.c_str());

        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            ALOGE("popen() FAILED");
        }
        while (0 == feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
                result += buffer.data();
                break;  // break it since we just interested to get process id
            }
        }
        return result;
    }

    sp<IGnss> gnssServer_;
};

TEST_F(VtsGnssIntegrationTest, recievedOk) {
    ALOGI("Starting test recievedOk!");
    std::promise<void> promise;
    std::future<void> future = promise.get_future();
    std::vector<uint8_t> payload;

    sp<GnssCallback> callback = new GnssCallback();
    ALOGD("Connect to service!");
    gnssServer_ = IGnss::getService();
    ASSERT_TRUE(gnssServer_ != nullptr);
    gnssServer_->setCallback(callback);
    callback->onLocationCallback = [&promise](const GnssLocation& location) {
        ALOGD("Lat: %f Long: %f, time %lu, altitudeMeters %f, speed %f, heading %f",
              location.latitudeDegrees,
              location.longitudeDegrees,
              location.timestamp,
              location.altitudeMeters,
              location.speedMetersPerSec,
              location.bearingDegrees);

        EXPECT_FLOAT_EQ(location.latitudeDegrees, -20.025057);
        EXPECT_FLOAT_EQ(location.longitudeDegrees, -57.550241);
        EXPECT_EQ(location.timestamp, 1507190462000);
        EXPECT_FLOAT_EQ(location.altitudeMeters, 2.1);
        EXPECT_FLOAT_EQ(location.speedMetersPerSec, 9);
        EXPECT_FLOAT_EQ(location.bearingDegrees, 2.34);
        promise.set_value();
    };
    gnssServer_->start();

    ALOGD("Creating position message!");
    ASN_BYTE m_session_buffer_send[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session = ASN_Session_Create(m_session_buffer_send, sizeof(m_session_buffer_send));
    Icb_OpGNSSPositionData_Response msg = Icb_OpGNSSPositionData_Response_Create(m_session);

    Connectivity::Pdu temp_pdu;
    temp_pdu.createHeader(
            static_cast<Connectivity::IpCmdTypes::ServiceId>(Connectivity::VccIpCmd::ServiceId::Positioning),
            static_cast<Connectivity::IpCmdTypes::OperationId>(Connectivity::VccIpCmd::OperationId::GNSSPositionData),
            Connectivity::IpCmdTypes::OperationType::NOTIFICATION,
            Connectivity::IpCmdTypes::DataType::ENCODED,
            1);
    temp_pdu.header.protocol_version = 3;
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
    msg->gnssPositionData->positioningStatus->fixType = e_Icb_GnssFixType_fix3D;

    std::vector<uint8_t> buffer;
    temp_pdu.toData(buffer);

    ALOGD("Encode message!");
    InfotainmentIpBus::Utils::encodeMessage(
            msg, Icb_OpGNSSPositionData_Response_Encode, Icb_OpGNSSPositionData_Response_EncodedSize, &payload);
    temp_pdu.setPayload(std::move(payload));

    Connectivity::Message message;
    message.pdu = temp_pdu;
    message.ecu = Connectivity::Message::Ecu::IHU;

    vcc::ipcb::testing::IpcbSimulator simulator(EcuMap());
    simulator.Initialize(Connectivity::Message::Ecu::TCAM, "UDP");

    simulator.SendMessage(std::move(message));
    ALOGD("Message sent!");

    using namespace std::chrono_literals;
    std::future_status status = future.wait_for(3s);

    ASSERT_NE(std::future_status::deferred, status) << "Promise deferred exiting";
    ASSERT_NE(std::future_status::timeout, status)
            << "Timeout: Didn't recive any gnss message within timelimit. Exiting";
    ASSERT_EQ(std::future_status::ready, status) << "Unsuccessful";

    gnssServer_->stop();
    ALOGI("Exiting test done!");
}
