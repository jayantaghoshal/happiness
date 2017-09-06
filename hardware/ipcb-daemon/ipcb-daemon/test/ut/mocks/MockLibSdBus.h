/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef _MOCK_LIBSDBUS_H_
#define _MOCK_LIBSDBUS_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <systemd/sd-bus.h>

class MockLibSdBus;
extern ::testing::NiceMock<MockLibSdBus> *g_mockLibSdBus;

class MockLibSdBus
{
public:
    MOCK_METHOD8(sd_bus_call_method,
                 int(sd_bus *bus,
                     const char *destination,
                     const char *path,
                     const char *interface,
                     const char *member,
                     sd_bus_error *ret_error,
                     sd_bus_message **reply,
                     const char *types));
    MOCK_METHOD1(sd_bus_open_system, int(sd_bus *bus));
    MOCK_METHOD3(sd_bus_message_read_basic, int(sd_bus_message *m, char type, void *p));
    MOCK_METHOD1(sd_bus_open_user, int(sd_bus *bus));
    MOCK_METHOD6(sd_bus_message_new_method_call,
                 int(sd_bus *bus,
                     sd_bus_message **m,
                     const char *destination,
                     const char *path,
                     const char *interface,
                     const char *member));

    MOCK_METHOD5(sd_bus_call,
                 int(sd_bus *bus, sd_bus_message *m, uint64_t usec, sd_bus_error *ret_error, sd_bus_message **reply));

    MOCK_METHOD8(sd_bus_message_append,
                 int(sd_bus_message *m,
                     const char *types,
                     const char *arg,
                     int arg2,
                     const char *arg3,
                     const char *arg4,
                     const char *arg5,
                     const char *arg6));

    MOCK_METHOD6(sd_bus_message_append,
                 int(sd_bus_message *m, const char *types, int arg, const char *arg2, const char *arg3, bool arg4));

    MOCK_METHOD3(sd_bus_message_append, int(sd_bus_message *m, const char *types, int arg));

    ~MockLibSdBus()
    {
        g_mockLibSdBus = nullptr;
    }
};

#endif  // _MOCK_LIBSDBUS_H_
