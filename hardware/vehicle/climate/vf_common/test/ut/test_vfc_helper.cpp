/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "vfc_helper.h"

#include <ECD_dataelement.h>
#include <gtest/gtest.h>

using namespace testing;
using namespace std::chrono_literals;

class TestVfcHelper : public ::testing::Test
{
public:
    TestVfcHelper()
    {
        DataElementFramework::instance().reset();
    }

protected:
    ECDDataElement::DESink<ActivateVfc_info> activateVfc;
};

TEST_F(TestVfcHelper, RequestVfc_WillUseRequestedVfc)
{
    EXPECT_TRUE(activateVfc.get().isError());

    request_vfc(Vfc::SeatComfortFunctions);

    EXPECT_TRUE(activateVfc.get().isOk());

    EXPECT_EQ(Vfc::SeatComfortFunctions, activateVfc.get().value().vfcToActivate);
}

TEST_F(TestVfcHelper, RequestVfc_WhenNoTimeSpecified_WillUseDefaultTime)
{
    request_vfc(Vfc::SeatComfortFunctions);

    EXPECT_EQ(3, activateVfc.get().value().secondsToKeepActive);
}

TEST_F(TestVfcHelper, RequestVfc_WillUseRequestedTime)
{
    request_vfc(Vfc::SeatComfortFunctions, 8s);

    EXPECT_EQ(8, activateVfc.get().value().secondsToKeepActive);
}

TEST_F(TestVfcHelper, RequestVfc_WillUseMax60s)
{
    request_vfc(Vfc::SeatComfortFunctions, 3min);

    EXPECT_EQ(60, activateVfc.get().value().secondsToKeepActive);
}
