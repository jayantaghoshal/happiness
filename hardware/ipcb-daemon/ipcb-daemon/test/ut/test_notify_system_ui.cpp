/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mocks/MockLibSdBus.h"
#include "util/notify_system_ui.h"

namespace iis_utils = InfotainmentIpService::Utils;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AllOf;
using ::testing::SaveArg;
using ::testing::Sequence;
using ::testing::InSequence;
using ::testing::SetArgPointee;
using ::testing::Invoke;
using ::testing::AtLeast;

class NotifySystemUIFixture : public ::testing::Test
{
public:
    NotifySystemUIFixture()
    {
    }

    virtual void SetUp()
    {
        g_mockLibSdBus = &mockLibSdBus_;
    }

    virtual void TearDown()
    {
    }
    NiceMock<MockLibSdBus> mockLibSdBus_;
};

TEST_F(NotifySystemUIFixture, sd_bus_open_system)
{
    EXPECT_CALL(mockLibSdBus_, sd_bus_open_system(_)).Times(1).WillOnce(Return(-1));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), false);

    EXPECT_CALL(mockLibSdBus_, sd_bus_open_system(_)).Times(1).WillOnce(Return(0));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), true);
}

TEST_F(NotifySystemUIFixture, sd_bus_message_new_method_call)
{
    ON_CALL(mockLibSdBus_, sd_bus_open_system(_)).WillByDefault(Return(0));

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_new_method_call(_, _, _, _, _, _)).Times(1).WillOnce(Return(-1));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), false);

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_new_method_call(_, _, _, _, _, _)).Times(1).WillOnce(Return(0));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), true);
}

TEST_F(NotifySystemUIFixture, sd_bus_message_append_6)
{
    ON_CALL(mockLibSdBus_, sd_bus_open_system(_)).WillByDefault(Return(0));
    ON_CALL(mockLibSdBus_, sd_bus_message_new_method_call(_, _, _, _, _, _)).WillByDefault(Return(0));

    std::string title = "test";
    std::string json = "json";
    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, _, 0, _, _, _, _)).Times(1).WillOnce(Return(-1));
    EXPECT_EQ(iis_utils::SendUINotification(title, json), false);

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, _, 0, _, title.c_str(), json.c_str(), _))
        .Times(1)
        .WillOnce(Return(0));
    EXPECT_EQ(iis_utils::SendUINotification(title, json), true);
}

TEST_F(NotifySystemUIFixture, sd_bus_message_append_4)
{
    ON_CALL(mockLibSdBus_, sd_bus_open_system(_)).WillByDefault(Return(0));
    ON_CALL(mockLibSdBus_, sd_bus_message_new_method_call(_, _, _, _, _, _)).WillByDefault(Return(0));

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, 1, _, _, true)).Times(1).WillOnce(Return(-1));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), false);

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, 1, _, _, true)).Times(1).WillOnce(Return(0));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), true);
}

TEST_F(NotifySystemUIFixture, sd_bus_message_append_1)
{
    ON_CALL(mockLibSdBus_, sd_bus_open_system(_)).WillByDefault(Return(0));
    ON_CALL(mockLibSdBus_, sd_bus_message_new_method_call(_, _, _, _, _, _)).WillByDefault(Return(0));

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, 0)).Times(1).WillOnce(Return(-1));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), false);

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, 0)).Times(1).WillOnce(Return(0));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), true);
}

TEST_F(NotifySystemUIFixture, sd_bus_call)
{
    ON_CALL(mockLibSdBus_, sd_bus_open_system(_)).WillByDefault(Return(0));
    ON_CALL(mockLibSdBus_, sd_bus_message_new_method_call(_, _, _, _, _, _)).WillByDefault(Return(0));

    EXPECT_CALL(mockLibSdBus_, sd_bus_call(_, _, 0, _, _)).Times(1).WillOnce(Return(-1));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), false);

    EXPECT_CALL(mockLibSdBus_, sd_bus_call(_, _, 0, _, _)).Times(1).WillOnce(Return(0));
    EXPECT_EQ(iis_utils::SendUINotification("", ""), true);
}

struct MsgArg
{
    int stub_message_append(sd_bus_message *m,
                            const char *types,
                            const char *arg,
                            int arg2,
                            const char *arg3,
                            const char *arg4,
                            const char *arg5,
                            const char *arg6)
    {
        msg = arg5;
        return 0;
    }
    std::string msg;
};

TEST_F(NotifySystemUIFixture, NotifySMS)
{
    EXPECT_CALL(mockLibSdBus_, sd_bus_call(_, _, 0, _, _))
        .Times(AtLeast(1))
        .WillOnce(Return(-1))
        .WillRepeatedly(Return(0));
    EXPECT_EQ(iis_utils::NotifySMS("", "", "", ""), false);

    MsgArg ma;

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, _, 0, _, _, _, _))
        .Times(1)
        .WillOnce(Invoke(&ma, &MsgArg::stub_message_append));

    std::string json = "{\"sender\":\"Kalle\",\"message\":\"Hello!\"}";
    EXPECT_EQ(iis_utils::NotifySMS("0001", "Kalle", "012345678", "Hello!"), true);

    EXPECT_EQ(json, ma.msg);

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, _, 0, _, _, _, _))
        .Times(1)
        .WillOnce(Invoke(&ma, &MsgArg::stub_message_append));

    json = "{\"sender\":\"012345678\",\"message\":\"Hello!\"}";
    EXPECT_EQ(iis_utils::NotifySMS("0001", "", "012345678", "Hello!"), true);

    EXPECT_EQ(json, ma.msg);

    EXPECT_CALL(mockLibSdBus_, sd_bus_message_append(_, _, _, 0, _, _, _, _))
        .Times(1)
        .WillOnce(Invoke(&ma, &MsgArg::stub_message_append));

    json = "{\"sender\":\"012345678\",\"message\":\" \\/ \\\" \\\\ \\n n \\f '! \\r \\b \\t \\u0001 \\u001f\"}";
    EXPECT_EQ(iis_utils::NotifySMS("0001", "", "012345678", " / \" \\ \n n \f '! \r \b \t \x01 \x1f"), true);

    EXPECT_EQ(json, ma.msg);
}
