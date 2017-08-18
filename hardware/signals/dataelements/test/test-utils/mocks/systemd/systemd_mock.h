/*===========================================================================*\
 * Copyright 2016 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef SYSTEMD_MOCK_H
#define SYSTEMD_MOCK_H

#include <gmock/gmock.h>
#include <string>
#include <systemd/sd-bus.h>
#include <systemd/sd-event.h>

class SystemDMock
{
public:
    SystemDMock();
    ~SystemDMock();

    //
    // sd-bus.h
    //
    // sd_bus_message_read(sd_bus_message *m, const char *types, ...) needs to be mocked by three helper mock methods
    // Set an EXPECT_CALL for each ... parameter
    MOCK_METHOD1(message_read_i, int(int*));
    MOCK_METHOD1(message_read_s, int(const char**));
    MOCK_METHOD1(message_read_u, int(unsigned int*));

    MOCK_METHOD1(sd_bus_message_get_bus, sd_bus*(sd_bus_message*));
    MOCK_METHOD1(sd_bus_message_ref, sd_bus_message*(sd_bus_message* m));
    MOCK_METHOD1(sd_bus_message_unref, sd_bus_message*(sd_bus_message*));
    MOCK_METHOD3(sd_bus_request_name, int(sd_bus* bus, const char* name, uint64_t flags));
    MOCK_METHOD4(sd_event_add_defer,
                 int(sd_event* e, sd_event_source** s, sd_event_handler_t callback, void* userdata));
    MOCK_METHOD2(sd_event_source_set_enabled, int(sd_event_source* s, int enabled));
    MOCK_METHOD6(sd_bus_emit_signal_s,
                 int(sd_bus*     bus,
                     const char* path,
                     const char* interface,
                     const char* member,
                     const char* types,
                     const char* data));

    MOCK_METHOD8(sd_bus_set_property_s,
                 int(sd_bus*       bus,
                     const char*   destination,
                     const char*   path,
                     const char*   interface,
                     const char*   member,
                     sd_bus_error* ret_error,
                     const char*   type,
                     const char*   value));

    MOCK_METHOD3(sd_bus_message_append_s, int(sd_bus_message* m, const char* types, const char* data));

    MOCK_METHOD1(sd_bus_open_system, int(sd_bus** ret));

    MOCK_METHOD2(sd_bus_get_unique_name, int(sd_bus*, const char**));
    MOCK_METHOD6(sd_bus_add_object_vtable,
                 int(sd_bus*, sd_bus_slot**, const char*, const char*, const sd_bus_vtable*, void*));

    // Note: The variadic ... parameters in sd_bus_call_method are put into the single std::string at the end.
    MOCK_METHOD9(sd_bus_call_method,
                 int(sd_bus*,
                     const char*,
                     const char*,
                     const char*,
                     const char*,
                     sd_bus_error*,
                     sd_bus_message**,
                     const char*,
                     const std::string));

    // Note: The variadic ... parameters in sd_bus_call_method are put into the single std::string at the end.
    MOCK_METHOD2(sd_bus_reply_method_return, int(sd_bus_message*, const std::string));

    MOCK_METHOD5(sd_bus_add_match, int(sd_bus*, sd_bus_slot**, const char*, sd_bus_message_handler_t, void*));
    MOCK_METHOD3(sd_bus_attach_event, int(sd_bus*, sd_event*, int));
    MOCK_METHOD1(sd_bus_error_free, void(sd_bus_error*));
    MOCK_METHOD1(sd_bus_slot_unref, sd_bus_slot*(sd_bus_slot*));
    MOCK_METHOD1(sd_bus_flush_close_unref, sd_bus*(sd_bus*));
    MOCK_METHOD1(sd_bus_unref, sd_bus*(sd_bus*));

    MOCK_METHOD1(sd_bus_close, void(sd_bus*));
    MOCK_METHOD7(sd_bus_get_property_string,
                 int(sd_bus*       bus,
                     const char*   destination,
                     const char*   path,
                     const char*   interface,
                     const char*   member,
                     sd_bus_error* ret_error,
                     char**        ret));

    MOCK_METHOD1(sd_bus_default_system, int(sd_bus** ret));
    //
    // sd-event.h
    //
    MOCK_METHOD2(sd_event_exit, int(sd_event*, int));
    MOCK_METHOD1(sd_event_source_get_event, sd_event*(sd_event_source*));
    MOCK_METHOD1(sd_event_default, int(sd_event**));
    MOCK_METHOD6(sd_event_add_io, int(sd_event*, sd_event_source**, int, uint32_t, sd_event_io_handler_t, void*));
    MOCK_METHOD1(sd_event_loop, int(sd_event*));
    MOCK_METHOD1(sd_event_unref, sd_event*(sd_event*));
};

#endif // SYSTEMD_MOCK_H
