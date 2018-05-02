/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <stdio.h>
#include <array>
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#define LOG_TAG "netns_bridge_test"
#include <cutils/log.h>

class RtpAudioNamespaceBridge : public ::testing::Test {
    void SetUp() override {}

    void TearDown() override {}

  protected:
    const std::string rtp_audio_vlan_ipaddr = "198.19.102.161";

    bool validateCommandStatus(int command_status) {
        if ((command_status < 0) || !WIFEXITED(command_status) || WEXITSTATUS(command_status) != EXIT_SUCCESS) {
            return false;
        }
        return true;
    }

    bool checkAutonomousDrive() {
        const std::string cmd = "getprop | grep -i '\\[vendor.carconfig.CC100_AutonomousDriveType*\\]: \\[3*\\]'";
        return validateCommandStatus(system(cmd.c_str()));
    }
};

/**
 * TEST: IptablesNatTableIsConfigured
 * EXPECT:
 *   * tcam0_cd masquarading rule is added to nat iptable
 */
TEST_F(RtpAudioNamespaceBridge, IptablesNatTableIsConfigured) {
    if (checkAutonomousDrive()) {
        const std::string vcc_netns = "/vendor/bin/ip netns exec vcc ";
        const std::string cmd = vcc_netns + "/vendor/bin/iptables -t mangle -C POSTROUTING -o tcam0_cd -j MASQUERADE";

        int command_status = std::system(cmd.c_str());

        EXPECT_TRUE(validateCommandStatus(command_status));
    } else {
        ALOGD("Nothing to test. Test not supported.");
    }
}

/**
 * TEST: IptablesMangleTableIsConfigured
 * EXPECT:
 *   * checksum-fill for rtp_aud0 is added to mangle iptable
 */
TEST_F(RtpAudioNamespaceBridge, IptablesMangleTableIsConfigured) {
    if (checkAutonomousDrive()) {
        const std::string vcc_netns = "/vendor/bin/ip netns exec vcc ";
        const std::string cmd = vcc_netns +
                                "/vendor/bin/iptables -t mangle -C POSTROUTING -d 10.0.0.0/24 -o rtp_aud0 -p udp -m "
                                "udp --dport 68 -j CHECKSUM --checksum-fill";

        int command_status = std::system(cmd.c_str());
        EXPECT_TRUE(validateCommandStatus(command_status));
    } else {
        ALOGD("Nothing to test. Test not supported.");
    }
}

/**
 * TEST: PingBetweenNamespaces
 * EXPECT:
 *   * successful pings between default and VCC network namespaces
 */
TEST_F(RtpAudioNamespaceBridge, PingBetweenNamespaces) {
    if (checkAutonomousDrive()) {
        const std::string cmd = "ping -c 1 -w 3 " + rtp_audio_vlan_ipaddr;
        EXPECT_TRUE(validateCommandStatus(system(cmd.c_str())));
    } else {
        ALOGD("Nothing to test. Test not supported.");
    }
}