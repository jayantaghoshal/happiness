/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <stdio.h>
#include <vendor/volvocars/hardware/uds/1.0/IDataCollectorTestPoint.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#define LOG_TAG "apix_obd_split_test"
#include <cutils/log.h>

using namespace ::vendor::volvocars::hardware::uds::V1_0;

class ApixObdTrafficSplitFixture : public ::testing::Test {
    void SetUp() override {
        // Fetch DataCollector service's test point
        data_collector_tp = IDataCollectorTestPoint::getService();
        if (data_collector_tp == nullptr) {
            FAIL() << "Cannot continue test since UDS is null";
            exit(EXIT_FAILURE);
        }

        // Remove previously configured spliting rules is existing
        disableApixOnObdIfActive();
    }

    void TearDown() override {
        // Put iptables back to pre-test state
        disableApixOnObdIfActive();
    }

  protected:
    // TODO (Samuel.Idowu):  Replace with DID constants from header file to be provided by UDS
    const uint16_t APIX_DID = 0xEE34;
    const uint8_t APIX_OBD_OFF = 0x0000u;
    const uint8_t APIX_OBD_ON = 0x0001u;

    ::android::sp<IDataCollectorTestPoint> data_collector_tp;

    std::vector<uint8_t> off_data{APIX_OBD_OFF};
    std::vector<uint8_t> on_data{APIX_OBD_ON};

    DidReadResult disabled_did{DidReadStatusCode::SUCCESS, off_data};
    DidReadResult enabled_did{DidReadStatusCode::SUCCESS, on_data};

    const std::string eth0_name = "apix0";           // localconfig's eth0.name
    const std::string eth1_gw_addr = "198.18.34.1";  // localconfig's eth1.ip-address

    // Formed rules in iptables
    const std::string apix_obd_prerouting =
            "-A PREROUTING -i " + eth0_name + " -p tcp -j TEE --gateway " + eth1_gw_addr;
    const std::string apix_obd_postrouting =
            "-A POSTROUTING -o " + eth0_name + " -p tcp -j TEE --gateway " + eth1_gw_addr;

    bool readIptable(const std::string& table, std::string& output) {
        std::array<char, 128> buffer;
        std::string vcc_netns = "/vendor/bin/ip netns exec vcc ";
        // TODO (Samuel.Idowu): Switch to /vendor/bin/iptables
        std::string cmd = vcc_netns + "/system/bin/iptables -w 5 -S -t " + table + " 2>&1";
        std::unique_ptr<FILE, decltype(&pclose)> pFile(popen(cmd.c_str(), "re"), pclose);
        if (!pFile) {
            ALOGE("popen() failed!");
            return false;
        }

        while (!feof(pFile.get())) {
            if (fgets(buffer.data(), 128, pFile.get()) != nullptr) output += buffer.data();
        }
        return true;
    }

    void disableApixOnObdIfActive() {
        auto read_ret = data_collector_tp->readDidValue(APIX_DID, [this](const DidReadResult& didread) {
            if (didread == enabled_did) {
                DidWriteStatusCode ret = data_collector_tp->writeDidValue(APIX_DID, off_data);
                EXPECT_THAT(ret, DidWriteStatusCode::SUCCESS);
            }
        });
        EXPECT_TRUE(read_ret.isOk());
    }

    bool isSplitRuleInIptable() {
        std::string iptable_rules;
        bool res = readIptable("mangle", iptable_rules);
        EXPECT_THAT(res, true);
        if (iptable_rules.find(apix_obd_prerouting) == std::string::npos) {
            return false;
        }
        if (iptable_rules.find(apix_obd_postrouting) == std::string::npos) {
            return false;
        }
        return true;
    }
};

/**
 * TEST: ApixObdTrafficSplit
 * EXPECT:
 *   * Traffic mirroring is enabled
 *   * Iptable entries added to allow traffic split
 *   * Uds reads correct value of 0x0001.
 *
 *   * Traffic mirroring is disabled
 *   * Iptables reconfigured to disable traffic split
 *   * Uds reads correct value of 0x0000.
 */
TEST_F(ApixObdTrafficSplitFixture, ApixObdTrafficSplit) {
    // Enable Apix on OBD.
    DidWriteStatusCode ret = data_collector_tp->writeDidValue(APIX_DID, on_data);
    EXPECT_THAT(ret, DidWriteStatusCode::SUCCESS);

    // Confirm iptable rule
    EXPECT_THAT(isSplitRuleInIptable(), true);

    // Check that uds is updated to apix_on_obd active
    auto read_result1 = data_collector_tp->readDidValue(
            APIX_DID, [this](const DidReadResult& didread) { EXPECT_THAT(enabled_did, didread); });
    EXPECT_TRUE(read_result1.isOk());

    // Disable Apix on OBD.
    DidWriteStatusCode ret2 = data_collector_tp->writeDidValue(APIX_DID, off_data);
    EXPECT_THAT(ret2, DidWriteStatusCode::SUCCESS);

    // Confirm iptable rule is removed
    EXPECT_THAT(isSplitRuleInIptable(), false);

    // Check that uds is updated to apix_on_obd disabled.
    auto read_result2 = data_collector_tp->readDidValue(
            APIX_DID, [this](const DidReadResult& didread) { EXPECT_THAT(disabled_did, didread); });
    EXPECT_TRUE(read_result2.isOk());
}
