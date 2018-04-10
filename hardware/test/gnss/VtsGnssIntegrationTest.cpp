/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

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
#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>

// Note that the auto-generated ASN1 files are in C, not C++
extern "C" {
#include "infotainmentIpBus.h"
}
extern "C" {
#include "pl/asn_base/asn_base.h"
}

using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;

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

static int new_ipcb_pid = -1;

static bool setup_test_case_successful = false;

class VtsGnssIntegrationTest : public ::testing::Test {
  protected:
    static void SetUpTestCase() {
        ALOGD("+ SetUpTestCase ");

        // Expected that all tests would be aborted here, but that is not the case.
        // For now, abort all tests manually
        ASSERT_TRUE(fileExists("/data/local/tmp/localconfig.json"));

        // Kill conflicting IpcbD
        int ipcb_pid = getProcIdByName("/vendor/bin/ipcbd ipcb UDP");
        if (-1 != ipcb_pid) {
            kill(ipcb_pid, SIGTERM);
        }

        // Start IpcbD for test with mocked localconfig
        std::string new_ipcb_pid_str = getCmdOut(
                "VCC_LOCALCONFIG_PATH=/data/local/tmp/localconfig.json "
                "/vendor/bin/ipcbd ipcb UDP "
                "& echo $!");

        std::string::size_type sz;  // alias of size_t
        new_ipcb_pid = std::stoi(new_ipcb_pid_str, &sz);

        // Wait for service to start
        uint8_t count = 0;
        while (NULL == IVehicleCom::getService("ipcb").get()) {
            usleep(100000);

            if (!processExists(new_ipcb_pid)) {
                ASSERT_TRUE(false) << "PID lost while waiting for service to be registered";
            }

            if (20 == ++count) {
                ASSERT_TRUE(false) << "Timed out while waiting for service to be registered";
            }
        }

        // Wait for GnssD to reconnect to new IpcbD
        sleep(2);

        setup_test_case_successful = true;

        ALOGD("- SetUpTestCase ");
    }

    static bool processExists(int pid) {
        // Calling kill with signal 0 will just return 0 if prcess is running
        return (0 == kill(pid, 0));
    }

    static bool fileExists(const std::string& name) {
        if (FILE* file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        } else {
            return false;
        }
    }

    static int getProcIdByName(std::string procName) {
        int pid = -1;

        // Open the /proc directory
        DIR* dp = opendir("/proc");
        if (dp != NULL) {
            // Enumerate all entries in directory until process found
            struct dirent* dirp;
            while (pid < 0 && (dirp = readdir(dp))) {
                // Skip non-numeric entries
                int id = atoi(dirp->d_name);
                if (id > 0) {
                    // Read contents of virtual /proc/{pid}/cmdline file
                    std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                    std::ifstream cmdFile(cmdPath.c_str());
                    std::string cmdLine;
                    getline(cmdFile, cmdLine);
                    replace(cmdLine.begin(), cmdLine.end(), '\0', ' ');
                    if (!cmdLine.empty()) {
                        cmdLine.erase(cmdLine.end() - 1);  // Remove the last character
                        if (procName == cmdLine) pid = id;
                    }
                }
            }
        }

        closedir(dp);

        return pid;
    }

    static std::string getCmdOut(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            ALOGE("popen() FAILED");
        }
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != NULL) {
                result += buffer.data();
                break;  // break it since we just interested to get process id
            }
        }
        return result;
    }

    static void TearDownTestCase() {
        ALOGD("+ TearDownTestCase");

        // Clean up, kill started processes
        if (-1 != new_ipcb_pid) {
            kill(new_ipcb_pid, SIGTERM);
        }

        // Start original IpcbD service to restore state
        getCmdOut("start ipcbd-infotainment");

        ALOGD("- TearDownTestCase");
    }

    void SetUp() {
        ALOGD("+ SetUp ");

        // Make sure Test Case Setup was executed correctly
        ASSERT_TRUE(setup_test_case_successful) << "Setup Test Case failed, failing test";

        // Make sure IpcbD is still running
        ASSERT_TRUE(processExists(new_ipcb_pid)) << "IpcbD is not running anymore, did it crash?";
    }

    void TearDown() {
        // code here will be called just after the test completes
        // ok to through exceptions from here if need be
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

    ALOGD("Encode message!");
    InfotainmentIpBus::Utils::encodeMessage(
            msg, Icb_OpGNSSPositionData_Response_Encode, Icb_OpGNSSPositionData_Response_EncodedSize, &payload);
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
