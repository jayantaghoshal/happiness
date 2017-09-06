/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "mock_notify_system_ui.h"
#include <assert.h>

::testing::NiceMock<MockNotifySystemUI>* g_mockNotifySystemUI = nullptr;

bool InfotainmentIpService::Utils::SendUINotification(const std::string& notification_id, const std::string& json)
{
    EXPECT_TRUE(nullptr != g_mockNotifySystemUI);
    return g_mockNotifySystemUI->SendUINotification(notification_id, json);
}

bool InfotainmentIpService::Utils::NotifySMS(const std::string& notification_id,
                                             const std::string& sender,
                                             const std::string& number,
                                             const std::string& body)
{
    EXPECT_TRUE(nullptr != g_mockNotifySystemUI);
    return g_mockNotifySystemUI->NotifySMS(notification_id, sender, number, body);
}
