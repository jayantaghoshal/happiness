/*===========================================================================*\
 * Copyright 2016 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/

#include <assert.h>
#include <systemd/systemd_mock.h>

#include <iostream>

static SystemDMock* systemDMock;

SystemDMock::SystemDMock()
{
    assert(systemDMock == nullptr);
    systemDMock = this;
}

SystemDMock::~SystemDMock()
{
    systemDMock = nullptr;
}
extern "C" {
//
// sd-bus.h
//

int sd_bus_message_read(sd_bus_message* m, const char* types, ...)
{
    const char* p;
    int         result = 0;
    va_list     args;
    va_start(args, types);

    for (p = types; *p != '\0'; p++)
    {
        switch (*p)
        {
        case 'i':
        {
            int* i = va_arg(args, int*);
            result = systemDMock->message_read_i(i);
            break;
        }
        case 'u':
        {
            unsigned int* u = va_arg(args, unsigned int*);
            result          = systemDMock->message_read_u(u);
            break;
        }
        case 's':
        {
            const char** s = va_arg(args, const char**);
            result         = systemDMock->message_read_s(s);
            break;
        }
        }
    }
    va_end(args);

    return result;
}

sd_bus* sd_bus_message_get_bus(sd_bus_message* m)
{
    return systemDMock->sd_bus_message_get_bus(m);
}
sd_bus_message* sd_bus_message_ref(sd_bus_message* m)
{
    return systemDMock->sd_bus_message_ref(m);
}
sd_bus_message* sd_bus_message_unref(sd_bus_message* m)
{
    return systemDMock->sd_bus_message_unref(m);
}
int sd_bus_open_system(sd_bus** ret)
{
    return systemDMock->sd_bus_open_system(ret);
}
int sd_bus_get_unique_name(sd_bus* bus, const char** unique)
{
    return systemDMock->sd_bus_get_unique_name(bus, unique);
}

int sd_bus_add_object_vtable(sd_bus*              bus,
                             sd_bus_slot**        slot,
                             const char*          path,
                             const char*          interface,
                             const sd_bus_vtable* vtable,
                             void*                userdata)
{
    return systemDMock->sd_bus_add_object_vtable(bus, slot, path, interface, vtable, userdata);
}

int sd_bus_call_method(sd_bus*          bus,
                       const char*      destination,
                       const char*      path,
                       const char*      interface,
                       const char*      member,
                       sd_bus_error*    ret_error,
                       sd_bus_message** reply,
                       const char*      types,
                       ...)
{
    const char* p;
    va_list     args;
    va_start(args, types);

    std::string non_variadic("");
    if (nullptr != types)
    {
        for (p = types; *p != '\0'; p++)
        {
            switch (*p)
            {
            case 'i':
            {
                int i = va_arg(args, int);
                non_variadic += std::to_string(i);
                break;
            }
            case 'u':
            {
                unsigned int u = va_arg(args, unsigned int);
                non_variadic += std::to_string(u);
                break;
            }
            case 's':
            {
                const char* s = va_arg(args, const char*);
                non_variadic += std::string(s);
                break;
            }
            }
        }
    }
    va_end(args);

    return systemDMock->sd_bus_call_method(
        bus, destination, path, interface, member, ret_error, reply, types, non_variadic);
}

int sd_bus_reply_method_return(sd_bus_message* call, const char* types, ...)
{
    const char* p;
    va_list     args;
    va_start(args, types);

    std::string non_variadic("");
    if (nullptr != types)
    {
        for (p = types; *p != '\0'; p++)
        {
            switch (*p)
            {
            case 'i':
            {
                int i = va_arg(args, int);
                non_variadic += std::to_string(i);
                break;
            }
            case 'u':
            {
                unsigned int u = va_arg(args, unsigned int);
                non_variadic += std::to_string(u);
                break;
            }
            case 's':
            {
                const char* s = va_arg(args, const char*);
                non_variadic += std::string(s);
                break;
            }
            }
        }
    }
    va_end(args);

    return systemDMock->sd_bus_reply_method_return(call, non_variadic);
}

int sd_bus_add_match(
    sd_bus* bus, sd_bus_slot** slot, const char* match, sd_bus_message_handler_t callback, void* userdata)
{
    return systemDMock->sd_bus_add_match(bus, slot, match, callback, userdata);
}

int sd_bus_attach_event(sd_bus* bus, sd_event* e, int priority)
{
    return systemDMock->sd_bus_attach_event(bus, e, priority);
}

sd_bus* sd_bus_unref(sd_bus* bus)
{
    if (systemDMock)
        return systemDMock->sd_bus_unref(bus);
}

int sd_bus_request_name(sd_bus* bus, const char* name, uint64_t flags)
{
    return systemDMock->sd_bus_request_name(bus, name, flags);
}

int sd_event_add_defer(sd_event* e, sd_event_source** s, sd_event_handler_t callback, void* userdata)
{
    return systemDMock->sd_event_add_defer(e, s, callback, userdata);
}

int sd_event_source_set_enabled(sd_event_source* s, int enabled)
{
    return systemDMock->sd_event_source_set_enabled(s, enabled);
}

int sd_bus_emit_signal(sd_bus* bus, const char* path, const char* interface, const char* member, const char* types, ...)
{
    const char* p;
    va_list     args;
    va_start(args, types);

    std::string non_variadic("");
    if (nullptr != types)
    {
        for (p = types; *p != '\0'; p++)
        {
            switch (*p)
            {

            case 's':
            {
                const char* s = va_arg(args, const char*);
                non_variadic += std::string(s);
                break;
            }
            }
        }
    }
    va_end(args);

    return systemDMock->sd_bus_emit_signal_s(bus, path, interface, member, types, non_variadic.c_str());
}

int sd_bus_message_append(sd_bus_message* m, const char* types, ...)
{
    const char* p;
    va_list     args;
    va_start(args, types);

    std::string non_variadic("");
    if (nullptr != types)
    {
        for (p = types; *p != '\0'; p++)
        {
            switch (*p)
            {

            case 's':
            {
                const char* s = va_arg(args, const char*);
                non_variadic += std::string(s);
                break;
            }
            }
        }
    }
    va_end(args);

    return systemDMock->sd_bus_message_append_s(m, types, non_variadic.c_str());
}

int sd_bus_set_property(sd_bus*       bus,
                        const char*   destination,
                        const char*   path,
                        const char*   interface,
                        const char*   member,
                        sd_bus_error* ret_error,
                        const char*   type,
                        ...)
{
    const char* p;
    va_list     args;
    va_start(args, type);

    std::string non_variadic("");
    if (nullptr != type)
    {
        for (p = type; *p != '\0'; p++)
        {
            switch (*p)
            {

            case 's':
            {
                const char* s = va_arg(args, const char*);
                non_variadic += std::string(s);
                break;
            }
            }
        }
    }
    va_end(args);

    return systemDMock->sd_bus_set_property_s(
        bus, destination, path, interface, member, ret_error, type, non_variadic.c_str());
}

void sd_bus_error_free(sd_bus_error* e)
{
    return systemDMock->sd_bus_error_free(e);
}
sd_bus_slot* sd_bus_slot_unref(sd_bus_slot* slot)
{
    return systemDMock->sd_bus_slot_unref(slot);
}
sd_bus* sd_bus_flush_close_unref(sd_bus* bus)
{
    return systemDMock->sd_bus_flush_close_unref(bus);
}
//
// sd-event.h
//
int sd_event_exit(sd_event* e, int code)
{
    return systemDMock->sd_event_exit(e, code);
}
sd_event* sd_event_source_get_event(sd_event_source* s)
{
    return systemDMock->sd_event_source_get_event(s);
}
int sd_event_default(sd_event** e)
{
    return systemDMock->sd_event_default(e);
}
int sd_event_add_io(
    sd_event* e, sd_event_source** s, int fd, uint32_t events, sd_event_io_handler_t callback, void* userdata)
{
    return systemDMock->sd_event_add_io(e, s, fd, events, callback, userdata);
}

int sd_event_loop(sd_event* e)
{
    return systemDMock->sd_event_loop(e);
}
sd_event* sd_event_unref(sd_event* e)
{
    return systemDMock->sd_event_unref(e);
}

void sd_bus_close(sd_bus* bus)
{
    systemDMock->sd_bus_close(bus);
}

int sd_bus_get_property_string(sd_bus*       bus,
                               const char*   destination,
                               const char*   path,
                               const char*   interface,
                               const char*   member,
                               sd_bus_error* ret_error,
                               char**        ret)
{
    return systemDMock->sd_bus_get_property_string(bus, destination, path, interface, member, ret_error, ret);
}

int sd_bus_default_system(sd_bus** ret)
{
    return systemDMock->sd_bus_default_system(ret);
}
}
