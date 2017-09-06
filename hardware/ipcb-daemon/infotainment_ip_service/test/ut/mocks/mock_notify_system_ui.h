/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_NOTIFY_SYSTEM_UI_H
#define MOCK_NOTIFY_SYSTEM_UI_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "util/notify_system_ui.h"

class MockNotifySystemUI;
extern ::testing::NiceMock<MockNotifySystemUI>* g_mockNotifySystemUI;

class MockNotifySystemUI
{
public:
    MockNotifySystemUI()
    {
    }

    ~MockNotifySystemUI()
    {
        g_mockNotifySystemUI = nullptr;
    }

    MOCK_METHOD2(SendUINotification, bool(const std::string& notification_id, const std::string& json));
    MOCK_METHOD4(NotifySMS,
                 bool(const std::string& notification_id,
                      const std::string& sender,
                      const std::string& number,
                      const std::string& body));
};

#endif  // MOCK_NOTIFY_SYSTEM_UI_H
