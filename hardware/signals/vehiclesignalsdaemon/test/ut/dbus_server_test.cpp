

#include "sd_bus_signal_server.h"
#include "systemd/systemd_mock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pthread_mock.h>
#include <vector>

extern "C" {
extern int __real_open(const char* pathname, int flags, mode_t mode);
extern ssize_t __real_read(int fd, void* buf, size_t count);
extern std::vector<autosar::DataElemInfo*> VipFrameworkStub_elements;
extern sd_bus_vtable*                      vsm_in_signals_vtable;
extern sd_bus_vtable*                      vsm_out_signals_vtable;
extern sd_bus_vtable*                      vsm_internal_signals_vtable;
extern "C" int vsm_signal_property_write(sd_bus*         bus,
                                         const char*     path,
                                         const char*     interface,
                                         const char*     property,
                                         sd_bus_message* value,
                                         void*           userdata,
                                         sd_bus_error*   ret_error);
extern "C" int vsm_signal_property_read(sd_bus*         bus,
                                        const char*     path,
                                        const char*     interface,
                                        const char*     property,
                                        sd_bus_message* reply,
                                        void*           userdata,
                                        sd_bus_error*   ret_error);
}

class DEInfoTest : public autosar::DataElemInfo
{
public:
    const char* name() const override
    {
        return _name.c_str();
    }
    const char* typeName() const override
    {
        return _typeName.c_str();
    }
    autosar::Dir direction() const override
    {
        return _dir;
    }

    std::string  _name;
    std::string  _typeName;
    autosar::Dir _dir;

    DEInfoTest(const std::string& name, const std::string& type, autosar::Dir dir)
    {
        _name     = name;
        _typeName = type;
        _dir      = dir;
    }
};

/// This class tests the dbus server part of the VSM, for example that the correct DBUS calls are being done
class DBusServerTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
        VipFrameworkStub_elements.clear();
        if (vsm_in_signals_vtable)
        {
            delete vsm_in_signals_vtable;
            vsm_in_signals_vtable = NULL;
        }
        if (vsm_out_signals_vtable)
        {
            delete vsm_out_signals_vtable;
            vsm_out_signals_vtable = NULL;
        }
        if (vsm_internal_signals_vtable)
        {
            delete vsm_internal_signals_vtable;
            vsm_internal_signals_vtable = NULL;
        }
    }

    void checkVTableEntry(int vtableType, const std::string& name, std::string type, sd_bus_vtable tableEntry)
    {
        EXPECT_EQ(vtableType, tableEntry.type);
        EXPECT_STREQ(name.c_str(), tableEntry.x.property.member);
        EXPECT_STREQ(type.c_str(), tableEntry.x.property.signature);
    }

protected:
    ::testing::NiceMock<SystemDMock> _sdBusMock;
};

extern "C" {
int __wrap_open(const char* pathName, int flags, mode_t mode)
{
    return __real_open(pathName, flags, mode);
}

ssize_t __wrap_read(int fd, void* buf, size_t count)
{
    return __real_read(fd, buf, count);
} /* close off extern "C" specification */
}

using ::testing::_;
using ::testing::StrEq;
using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::SetArgPointee;
using ::testing::SaveArgPointee;

TEST_F(DBusServerTest, WhenStart_CorrectSDBusCallsAreDone)
{
    // Check that the correct SD Bus calls are being done in start method
    EXPECT_CALL(_sdBusMock, sd_bus_open_system(_));
    EXPECT_CALL(_sdBusMock, sd_bus_request_name(_, StrEq("com.ihu.VehicleSignalsManager.In"), _));
    EXPECT_CALL(_sdBusMock, sd_bus_request_name(_, StrEq("com.ihu.VehicleSignalsManager.Out"), _));
    EXPECT_CALL(_sdBusMock, sd_bus_request_name(_, StrEq("com.ihu.VehicleSignalsManager.Internal"), _));
    EXPECT_CALL(_sdBusMock, sd_bus_add_object_vtable(_, _, _, _, _, _)).Times(3);

    EXPECT_TRUE(DBUSSignalServer::start(false));
}

TEST_F(DBusServerTest, WhenStart_VtablesAreOk)
{
    // Add a number of AutoSAR messages in the stubbed vector

    std::vector<autosar::DataElemInfo*> deInfos
        = { new DEInfoTest("SignalName1", "SignalType1", autosar::Dir::IN),
            new DEInfoTest("SignalName2", "SignalType2", autosar::Dir::IN),
            new DEInfoTest("SignalName3", "SignalType3", autosar::Dir::OUT),
            new DEInfoTest("SignalName4", "SignalType4", autosar::Dir::OUT),
            new DEInfoTest("SignalName5", "SignalType5", autosar::Dir::OUT),
            new DEInfoTest("SignalName6", "SignalType6", autosar::Dir::INTERNAL) };

    for (auto deInfoPtr : deInfos)
        VipFrameworkStub_elements.push_back(deInfoPtr);

    // This method will fetch the DE elements from the vector and build the sd dbus vtable
    EXPECT_TRUE(DBUSSignalServer::start(false));

    // Check that the vtables are correct
    // BEGIN,Property 1,Property 2,....,DESignalChanged, VSMStarted, End

    // Check in signals
    sd_bus_vtable tableEntry = vsm_in_signals_vtable[0];
    EXPECT_EQ(_SD_BUS_VTABLE_START, tableEntry.type);

    tableEntry = vsm_in_signals_vtable[1];
    checkVTableEntry(_SD_BUS_VTABLE_WRITABLE_PROPERTY, "SignalName1", "s", tableEntry);

    tableEntry = vsm_in_signals_vtable[2];
    checkVTableEntry(_SD_BUS_VTABLE_WRITABLE_PROPERTY, "SignalName2", "s", tableEntry);

    tableEntry = vsm_in_signals_vtable[3];
    checkVTableEntry(_SD_BUS_VTABLE_SIGNAL, "DESignalChanged", "s", tableEntry);

    tableEntry = vsm_in_signals_vtable[4];
    checkVTableEntry(_SD_BUS_VTABLE_SIGNAL, "VSMStarted", "", tableEntry);

    tableEntry = vsm_in_signals_vtable[5];
    EXPECT_EQ(_SD_BUS_VTABLE_METHOD, tableEntry.type);

    tableEntry = vsm_in_signals_vtable[6];
    EXPECT_EQ(_SD_BUS_VTABLE_END, tableEntry.type);

    // Check out table
    tableEntry = vsm_out_signals_vtable[0];
    EXPECT_EQ(_SD_BUS_VTABLE_START, tableEntry.type);
    tableEntry = vsm_out_signals_vtable[1];
    checkVTableEntry(_SD_BUS_VTABLE_WRITABLE_PROPERTY, "SignalName3", "s", tableEntry);
    tableEntry = vsm_out_signals_vtable[4];
    checkVTableEntry(_SD_BUS_VTABLE_WRITABLE_PROPERTY, "ActivateVfc", "s", tableEntry); // Added internally
    tableEntry = vsm_out_signals_vtable[8];
    EXPECT_EQ(_SD_BUS_VTABLE_END, tableEntry.type);

    // Check internal table
    tableEntry = vsm_internal_signals_vtable[0];
    EXPECT_EQ(_SD_BUS_VTABLE_START, tableEntry.type);
    tableEntry = vsm_internal_signals_vtable[1];
    checkVTableEntry(_SD_BUS_VTABLE_WRITABLE_PROPERTY, "SignalName6", "s", tableEntry);
    tableEntry = vsm_internal_signals_vtable[5];
    EXPECT_EQ(_SD_BUS_VTABLE_END, tableEntry.type);
}

TEST_F(DBusServerTest, WhenStart_VSMStartedSignalEmitted)
{
    EXPECT_CALL(_sdBusMock,
                sd_bus_emit_signal_s(_,
                                     StrEq("/com/ihu/VehicleSignalsManager/In"),
                                     StrEq("com.ihu.VehicleSignalsManager.In"),
                                     StrEq("VSMStarted"),
                                     NULL,
                                     StrEq("")));
    EXPECT_CALL(_sdBusMock,
                sd_bus_emit_signal_s(_,
                                     StrEq("/com/ihu/VehicleSignalsManager/Out"),
                                     StrEq("com.ihu.VehicleSignalsManager.Out"),
                                     StrEq("VSMStarted"),
                                     NULL,
                                     StrEq("")));
    EXPECT_CALL(_sdBusMock,
                sd_bus_emit_signal_s(_,
                                     StrEq("/com/ihu/VehicleSignalsManager/Internal"),
                                     StrEq("com.ihu.VehicleSignalsManager.Internal"),
                                     StrEq("VSMStarted"),
                                     NULL,
                                     StrEq("")));

    DBUSSignalServer::eventLoopStartedHandler(NULL, NULL);
}

TEST_F(DBusServerTest, WhenPropertyAreWritten_SignalChangedSignalEmitted)
{
    // Setup so that message data is returned when sd_bus_msg_read are called
    EXPECT_CALL(_sdBusMock, message_read_s(_)).WillOnce(DoAll(SetArgPointee<0>("PropertyValue"), Return(0)));

    EXPECT_CALL(_sdBusMock,
                sd_bus_emit_signal_s(_,
                                     StrEq("/com/ihu/VehicleSignalsManager/In"),
                                     StrEq("com.ihu.VehicleSignalsManager.In"),
                                     StrEq("DESignalChanged"),
                                     StrEq("s"),
                                     StrEq("Property")));

    vsm_signal_property_write(NULL,
                              "/com/ihu/VehicleSignalsManager/In",
                              "com.ihu.VehicleSignalsManager.In",
                              "Property",
                              NULL,
                              &DBUSSignalServer::_dbusServerInfo,
                              NULL);
}

TEST_F(DBusServerTest, WhenPropertyAreWritten_PropertyValueCanBeRead)
{
    // Setup so that message data is returned when sd_bus_msg_read are called
    EXPECT_CALL(_sdBusMock, message_read_s(_)).WillOnce(DoAll(SetArgPointee<0>("PropertyValue"), Return(0)));

    vsm_signal_property_write(NULL,
                              "/com/ihu/VehicleSignalsManager.In",
                              "com.ihu.VehicleSignalsManager.In",
                              "Property",
                              NULL,
                              &DBUSSignalServer::_dbusServerInfo,
                              NULL);

    // Check that the property value is appended to the reply in the vsm_signal_property_read function
    EXPECT_CALL(_sdBusMock, sd_bus_message_append_s(_, StrEq("s"), StrEq("PropertyValue")));

    vsm_signal_property_read(NULL,
                             "/com/ihu/VehicleSignalsManager.In",
                             "com.ihu.VehicleSignalsManager.In",
                             "Property",
                             NULL,
                             &DBUSSignalServer::_dbusServerInfo,
                             NULL);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
