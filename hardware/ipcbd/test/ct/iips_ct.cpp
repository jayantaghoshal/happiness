#include <gtest/gtest.h>

#include <cutils/log.h>
#include <sys/types.h>

#define  LOG_TAG    "iips_ct"

#include "iips_test_helpers.h"

#define IIPS_BINDER_INTERFACE   "android.hardware.gnss@1.0::IGnss/default"


TEST(IipsTest, IipsUpAndRunningWithBinderInterface)
{
    ALOGI("IipsUpAndRunning starting");
    // !!!! the process name is actually "infotainment-ip-service" but the /proc file system only keeps the first
    // 15 characters (!!! very strange). Since we will anyway change name to something like "tcamd"
    // or similar soon we do it like this for the moment.
    pid_t pid = FindProcessId("infotainment-ip");
    ALOGI("pid=%d",pid);
    ASSERT_NE( -1, pid ); // expect a valid pid

    // Look for a registered binder interface by infotainment-ip-service
    std::vector<std::string> lshal_output = ExecuteCommand("/system/bin/lshal");

    ASSERT_FALSE(lshal_output.empty());

    const std::string pidstr = std::to_string(pid);

    for(std::string oneline : lshal_output)
    {
        if (oneline.find(IIPS_BINDER_INTERFACE) != std::string::npos) {
            // we found the interface
            // now also verify that the pid is correct
            EXPECT_TRUE( oneline.find(pidstr) != std::string::npos );
            ALOGI("IipsUpAndRunning found binder if %s", oneline.c_str());
            return;
        }
    }

    // we should never come here
    ALOGI("IipsUpAndRunning failed, no binder if found");
    EXPECT_TRUE(false);
}
