#include <gtest/gtest.h>

#include <cutils/log.h>
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
