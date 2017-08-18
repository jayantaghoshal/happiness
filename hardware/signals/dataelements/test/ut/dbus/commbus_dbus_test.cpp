/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelementcommbus.h"
#include "dataelementcommbus_dbus.h"
#include "pipe_mock.h"
#include "systemd/systemd_mock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "logging.h"
LOG_DECLARE_DEFAULT_CONTEXT(deleContext, "DELE", "DataElements main logging context")

extern bool DBUS_Rec_Mock_startDBUSEventLoop_Called;
extern bool DBUS_Rec_Mock_setNewDataElementHandler_Called;
extern bool DBUS_Rec_Mock_subscribeToSignal_Called;
extern bool DBUS_Rec_Mock_setVSMStartedCB_Called;

/// TBD
class ComBusDBUSTest : public ::testing::Test
{
public:
    ComBusDBUSTest()
        : _dbusReceiverMock(_pipeMock)
    {
    }
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
    DataElementCommBusDBusReceiver   _dbusReceiverMock;
};

using ::testing::_;
using ::testing::StrEq;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::SetArgPointee;
using ::testing::SaveArgPointee;

TEST_F(ComBusDBUSTest, CreateCommbusDBUS_CheckDbusRecStartDBUSCalled)
{

    DataElementCommBusDBus sut(_dbusReceiverMock, false);

    EXPECT_TRUE(DBUS_Rec_Mock_startDBUSEventLoop_Called);
}

TEST_F(ComBusDBUSTest, SendNewSignal_DBUSSetPropertyCalled)
{

    DataElementCommBusDBus sut(_dbusReceiverMock, false);
    EXPECT_CALL(_sdBusMock, sd_bus_set_property_s(_, _, _, _, StrEq("SignalName"), _, StrEq("s"), StrEq("SignalData")));

    sut.send("SignalName", "SignalData", autosar::Dir::INTERNAL);
}

TEST_F(ComBusDBUSTest, SendNewSignal_DBUSSetPropertyFailed_AddToPendingMap)
{

    DataElementCommBusDBus sut(_dbusReceiverMock, false);
    EXPECT_CALL(_sdBusMock, sd_bus_set_property_s(_, _, _, _, StrEq("SignalName"), _, StrEq("s"), StrEq("SignalData")))
        .WillOnce(Return(-1));

    sut.send("SignalName", "SignalData", autosar::Dir::INTERNAL);

    EXPECT_TRUE(sut.pendingSendMessages_.size() > 0);
}

TEST_F(ComBusDBUSTest, SetNewDataElementHandler_DBUSReceiverCalled)
{

    DataElementCommBusDBus sut(_dbusReceiverMock, false);
    sut.setNewDataElementHandler([](const std::string& name, const std::string& payload) {});

    EXPECT_TRUE(DBUS_Rec_Mock_setNewDataElementHandler_Called);
}

TEST_F(ComBusDBUSTest, AddName_DBUSReceiverSubscribeCalled)
{

    DataElementCommBusDBus sut(_dbusReceiverMock, false);
    sut.addName(autosar::Dir::INTERNAL, "Signal");

    EXPECT_TRUE(DBUS_Rec_Mock_subscribeToSignal_Called);
}
