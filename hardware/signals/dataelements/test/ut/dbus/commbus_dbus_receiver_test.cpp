/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelementcommbus_dbus_receiver.h"
#include "pipe_mock.h"
#include "systemd/systemd_mock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vector>
#include "logging.h"
LOG_DECLARE_DEFAULT_CONTEXT(deleContext, "DELE", "DataElements main logging context")

/// TBD
class ComBusDBUSReceiverTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        PipeMock::setObj(&_pipeMock);
    }

    virtual void TearDown()
    {
    }

protected:
    ::testing::NiceMock<SystemDMock> _sdBusMock;
    ::testing::NiceMock<PipeMock>    _pipeMock;
};

using ::testing::_;
using ::testing::StrEq;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::SetArgPointee;
using ::testing::SaveArgPointee;

TEST_F(ComBusDBUSReceiverTest, CreateCommbusDBUSReceiver_OpensPipeAndDBUS)
{
    EXPECT_CALL(_sdBusMock, sd_bus_open_system(_));
    EXPECT_CALL(_pipeMock, Open());
    DataElementCommBusDBusReceiver sut(_pipeMock);
}

TEST_F(ComBusDBUSReceiverTest, StartDBUSEventLoop_CorrectDBUSCalls)
{

    DataElementCommBusDBusReceiver sut(_pipeMock);

    EXPECT_CALL(_sdBusMock, sd_event_default(_));
    EXPECT_CALL(_sdBusMock, sd_event_loop(_));
    EXPECT_CALL(_sdBusMock, sd_event_add_defer(_, _, _, _));
    EXPECT_CALL(_sdBusMock, sd_event_source_set_enabled(_, _));
    sut.startDBUSEventLoop();
}

TEST_F(ComBusDBUSReceiverTest, SubscribeToSignal_AddsToPendSubscriptionsAndWakeupPipeAreWritten)
{

    DataElementCommBusDBusReceiver sut(_pipeMock);

    EXPECT_CALL(_pipeMock, Write(_, _));

    sut.subscribeToSignal(autosar::Dir::IN, "kalle");
    EXPECT_TRUE(sut._pendingSubscriptions.size() > 0);
}

TEST_F(ComBusDBUSReceiverTest, WakeupCallbackCalled_PipeEmptiedAndSignalSubscribedAndRead)
{
    DataElementCommBusDBusReceiver sut(_pipeMock);

    DataElementCommBusDBusReceiver::_theInstance = &sut;

    // Suscribe to some signals
    sut.subscribeToSignal(autosar::Dir::IN, "kalle");
    sut.subscribeToSignal(autosar::Dir::OUT, "pelle");

    EXPECT_CALL(_pipeMock, Read(_, _));
    EXPECT_CALL(_sdBusMock, sd_bus_add_match(_, _, _, _, _)).Times(2);
    EXPECT_CALL(_sdBusMock, sd_bus_get_property_string(_, _, _, _, StrEq("kalle"), _, _)).Times(1);
    EXPECT_CALL(_sdBusMock, sd_bus_get_property_string(_, _, _, _, StrEq("pelle"), _, _)).Times(1);

    sut.wakeupCallback(nullptr, 0, EPOLLIN, nullptr);

    EXPECT_TRUE(sut._pendingSubscriptions.empty());
}

TEST_F(ComBusDBUSReceiverTest, AddMatchCalled_CorrectMatchString)
{
    DataElementCommBusDBusReceiver sut(_pipeMock);
    DataElementCommBusDBusReceiver::_theInstance = &sut;

    std::string dbusMatchString = "sender='com.ihu.VehicleSignalsManager.Out',";
    dbusMatchString += "interface='com.ihu.VehicleSignalsManager.Out',";
    dbusMatchString += "member='" + DECommonDBUS::DBUS_SIGNALS_CHANGED_NAME + "',";
    dbusMatchString += "arg0='SignalName'";

    EXPECT_CALL(_sdBusMock, sd_bus_add_match(_, _, StrEq(dbusMatchString.c_str()), _, _));

    sut.addMatch(autosar::Dir::OUT, "SignalName");

    EXPECT_TRUE(sut._pendingSubscriptions.empty());
}
