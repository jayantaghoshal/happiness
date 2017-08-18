#include <gtest/gtest.h>

#include <cutils/log.h>

#include <vcc/localconfig.h>

#define  LOG_TAG    "netman_ct"

#include "netman_test_helpers.h"


TEST(NetmanTest, NetmanStartedAfterBoot)
{
  EXPECT_NE( -1, FindProcessId("netman") );
}

TEST(NetmanTest, IpAddressAfterBoot)
{
    // poll eth1 each 200ms during 5 secs
    const int total_duration = 5000; // 5 secs
    const int poll_intervall = 200;
    int loop_cnt = total_duration / poll_intervall;

    ALOGI("Waiting for ip address ...");

    while (loop_cnt--) {
        const std::string ipaddress = GetIpAddressForInterface("eth1");
        if (ipaddress=="")
        { // nothing set yet, just wait
        }
        else if (ipaddress=="198.18.34.1")
        { // yes we found it
            ALOGI("Found ip address %s",ipaddress.c_str());
            EXPECT_TRUE(true);
            return; // test is over
        }
        else
        {
            // We found something else. Print out and wait
            ALOGW("Found ip address %s, waiting further ...",ipaddress.c_str());
        }
        usleep(poll_intervall*1000);
    }

    EXPECT_TRUE(false); // test failed
}

TEST(NetmanTest, NetmaskAfterBoot)
{
    // poll eth1 each 200ms during 5 secs
    const int total_duration = 5000; // 5 secs
    const int poll_intervall = 200;
    int loop_cnt = total_duration / poll_intervall;

    ALOGI("Waiting for netmask ...");

    while (loop_cnt--) {
        const std::string netmask = GetNetmaskForInterface("eth1");
        if (netmask=="")
        { // nothing set yet, just wait
        }
        else if (netmask == "255.255.240.0")
        { // yes we found it
            ALOGI("Found netmask %s", netmask.c_str());
            EXPECT_TRUE(true);
            return; // test is over
        }
        else
        {
            // We found something else. Print out and wait
            ALOGW("Found netmask %s, waiting further ...", netmask.c_str());
        }
        usleep(poll_intervall*1000);
    }

    EXPECT_TRUE(false); // test failed
}

TEST(NetmanTest, BroadcastAfterBoot)
{
    // poll eth1 each 200ms during 5 secs
    const int total_duration = 5000; // 5 secs
    const int poll_intervall = 200;
    int loop_cnt = total_duration / poll_intervall;

    ALOGI("Waiting for broadcast address...");

    while (loop_cnt--) {
        const std::string broadcast_address = GetBroadcastAddressForInterface("eth1");
        if (broadcast_address =="" )
        { // nothing set yet, just wait
        }
        else if (broadcast_address == "198.18.255.255")
        { // yes we found it
            ALOGI("Found broadcast address %s", broadcast_address.c_str());
            EXPECT_TRUE(true);
            return; // test is over
        }
        else
        {
            // We found something else. Print out and wait
            ALOGW("Found broadcast address %s, waiting further ...", broadcast_address.c_str());
        }
        usleep(poll_intervall*1000);
    }

    EXPECT_TRUE(false); // test failed
}

TEST(NetmanTest, MacAddressAfterBoot)
{
    // poll eth1 each 200ms during 5 secs
    const int total_duration = 5000; // 5 secs
    const int poll_intervall = 200;
    int loop_cnt = total_duration / poll_intervall;

    ALOGI("Waiting for mac address ...");

    while (loop_cnt--) {
        const std::string mac_address = GetMacAddressForInterface("eth1");
        if (mac_address=="")
        {// nothing set yet, just wait
        }
        else if (mac_address=="02:00:00:02:12:01")
        {// yes we found it
            ALOGI("Found mac address %s", mac_address.c_str());
            EXPECT_TRUE(true);
            return; // test is over
        }
        else
        {
            // We found something else. Print out and wait
            ALOGW("Found mac address %s, waiting further ...", mac_address.c_str());
        }
        usleep(poll_intervall*1000);
    }

    EXPECT_TRUE(false); // test failed
}

TEST(NetmanTest, MtuAddressAfterBoot)
{
    // poll eth1 each 200ms during 5 secs
    const int total_duration = 5000; // 5 secs
    const int poll_intervall = 200;
    int loop_cnt = total_duration / poll_intervall;

    ALOGI("Waiting for mtu ...");

    while (loop_cnt--) {
        const int mtu = GetMtuForInterface("eth1");
        if (mtu==-1)
        { // nothing set yet, just wait
        }
        else if (mtu==1500)
        { // yes we found it
            ALOGI("Found mtu %i", mtu);
            EXPECT_TRUE(true);
            return; // test is over
        }
        else
        {
            // We found something else. Print out and wait
            ALOGW("Found mtu %i, waiting further ...", mtu);
        }
        usleep(poll_intervall*1000);
    }

    EXPECT_TRUE(false); // test failed
}
