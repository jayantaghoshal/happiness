/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "MockLibSdBus.h"

::testing::NiceMock<MockLibSdBus> *g_mockLibSdBus = nullptr;

/*
 * Mock method implementation
 */
int sd_bus_call_method(sd_bus *bus,
                       const char *destination,
                       const char *path,
                       const char *interface,
                       const char *member,
                       sd_bus_error *ret_error,
                       sd_bus_message **reply,
                       const char *types,
                       ...)
{
    EXPECT_TRUE(nullptr != g_mockLibSdBus);

    return g_mockLibSdBus->sd_bus_call_method(bus, destination, path, interface, member, ret_error, reply, types);
}

int sd_bus_message_read_basic(sd_bus_message *m, char type, void *p)
{
    EXPECT_TRUE(nullptr != g_mockLibSdBus);

    if (type == 'i')
    {
        *static_cast<int *>(p) = 1;
    }

    return g_mockLibSdBus->sd_bus_message_read_basic(m, type, p);
}

int sd_bus_open_system(sd_bus **ret)
{
    EXPECT_TRUE(nullptr != g_mockLibSdBus);

    *ret = reinterpret_cast<sd_bus *>(4);
    return g_mockLibSdBus->sd_bus_open_system(*ret);
}

int sd_bus_open_user(sd_bus **ret)
{
    EXPECT_TRUE(nullptr != g_mockLibSdBus);

    *ret = reinterpret_cast<sd_bus *>(4);
    return g_mockLibSdBus->sd_bus_open_user(*ret);
}

int sd_bus_message_new_method_call(sd_bus *bus,
                                   sd_bus_message **m,
                                   const char *destination,
                                   const char *path,
                                   const char *interface,
                                   const char *member)
{
    EXPECT_TRUE(nullptr != g_mockLibSdBus);

    return g_mockLibSdBus->sd_bus_message_new_method_call(bus, m, destination, path, interface, member);
}

int sd_bus_call(sd_bus *bus, sd_bus_message *m, uint64_t usec, sd_bus_error *ret_error, sd_bus_message **reply)
{
    EXPECT_TRUE(nullptr != g_mockLibSdBus);

    return g_mockLibSdBus->sd_bus_call(bus, m, usec, ret_error, reply);
}

int sd_bus_message_append(sd_bus_message *m, const char *types, ...)
{
    EXPECT_TRUE(nullptr != g_mockLibSdBus);

    va_list l;
    va_start(l, types);

    if (!strcmp(types, "susssas"))
    {
        const char *arg = va_arg(l, const char *);
        int arg2 = va_arg(l, int);
        const char *arg3 = va_arg(l, const char *);
        const char *arg4 = va_arg(l, const char *);
        const char *arg5 = va_arg(l, const char *);
        const char *arg6 = va_arg(l, const char *);

        return g_mockLibSdBus->sd_bus_message_append(m, types, arg, arg2, arg3, arg4, arg5, arg6);
    }
    else if (!strcmp(types, "a{sv}"))
    {
        int arg = va_arg(l, int);
        const char *arg2 = va_arg(l, const char *);
        const char *arg3 = va_arg(l, const char *);
        bool arg4 = va_arg(l, int);

        return g_mockLibSdBus->sd_bus_message_append(m, types, arg, arg2, arg3, arg4);
    }
    else if (!strcmp(types, "i"))
    {
        int arg = va_arg(l, int);

        return g_mockLibSdBus->sd_bus_message_append(m, types, arg);
    }

    va_end(l);

    return -1;
}

/*
 * Stub implementations
 */
int sd_event_set_watchdog(sd_event *e, int b)
{
    (void)(e);
    (void)(b);
    return 0;
}
void sd_bus_close(sd_bus *bus)
{
    (void)(bus);
}
int sd_bus_add_match(
    sd_bus *bus, sd_bus_slot **slot, const char *match, sd_bus_message_handler_t callback, void *userdata)
{
    (void)(bus);
    (void)(slot);
    (void)(match);
    (void)(callback);
    (void)(userdata);
    return 0;
}
sd_bus_message *sd_bus_message_unref(sd_bus_message *m)
{
    (void)(m);
    return nullptr;
}
void sd_bus_error_free(sd_bus_error *e)
{
    (void)(e);
}
sd_event *sd_event_source_get_event(sd_event_source *s)
{
    (void)(s);
    return nullptr;
}
int sd_event_exit(sd_event *e, int code)
{
    (void)(e);
    return 0;
}
sd_event *sd_event_unref(sd_event *e)
{
    (void)(e);
    return nullptr;
}
int sd_event_add_signal(sd_event *e, sd_event_source **s, int sig, sd_event_signal_handler_t callback, void *userdata)
{
    (void)(e);
    (void)(s);
    (void)(sig);
    (void)(userdata);
    return 0;
}
sd_event_source *sd_event_source_unref(sd_event_source *s)
{
    (void)(s);
    return nullptr;
}
int sd_event_loop(sd_event *e)
{
    (void)(e);
    return 0;
}
sd_bus *sd_bus_flush_close_unref(sd_bus *bus)
{
    (void)(bus);
    return nullptr;
}
