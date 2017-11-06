#if defined(__COVERITY__) && !defined(__INCLUDE_LEVEL__)
/* Systemd's use of gcc's __INCLUDE_LEVEL__ extension macro appears to confuse
* Coverity. Here's a kludge to unconfuse it.
*/
#define __INCLUDE_LEVEL__ 2
#endif
#include <stdlib.h>
#include <systemd/sd-bus.h>

/**
* This file contains the DBUS Interface for the flexray signals that are
* received and received by the IHU. Also IHU internal signals are defined in
* this file.
*/

/// The number of additional entries that needs to be added besides the properties
/// +5 Start,SignaChanged,VSMStarted, WritePropMethod, End
#define NUMBER_OF_EXTRA_SD_BUS_ENTRIES 5

/**
 * Called when property is read, implemented elsewhere in the C++ world
 */
int vsm_signal_property_read(sd_bus* bus, const char* path, const char* interface, const char* property,
                             sd_bus_message* reply, void* userdata, sd_bus_error* ret_error);
/**
 * Called when property is written, implemented elsewhere in the C++ world
 */
int vsm_signal_property_write(sd_bus* bus, const char* path, const char* interface, const char* property,
                              sd_bus_message* value, void* userdata, sd_bus_error* ret_error);

int vsm_signal_method_set(sd_bus_message* m, void* userdata, sd_bus_error* ret_error);

// ====================================

sd_bus_vtable get_vtable_start(int flags) {
    const sd_bus_vtable r = SD_BUS_VTABLE_START(flags);
    return r;
}

sd_bus_vtable get_vtable_wrproperty(const char* name, sd_bus_property_get_t getter, sd_bus_property_set_t setter) {
    const sd_bus_vtable r =
            SD_BUS_WRITABLE_PROPERTY(name, "s", getter, setter, 0, SD_BUS_VTABLE_PROPERTY_EMITS_INVALIDATION);
    return r;
}

sd_bus_vtable get_vtable_method(const char* name, const char* signature, const char* result,
                                sd_bus_message_handler_t handler, int flags) {
    const sd_bus_vtable r = SD_BUS_METHOD(name, signature, result, handler, flags);
    return r;
}

sd_bus_vtable get_vtable_signal(const char* name, const char* type) {
    const sd_bus_vtable r = SD_BUS_SIGNAL(name, type, 0);
    return r;
}

sd_bus_vtable get_vtable_end() {
    const sd_bus_vtable r = SD_BUS_VTABLE_END;
    return r;
}

const char* SIGNALS_CHANGED_PROP_NAME = "DESignalChanged";
const char* VSM_STARTED_SIGNAL_NAME = "VSMStarted";
const char* VSM_SET_DE_SIGNAL_METHOD_NAME = "SetDESignal";

sd_bus_vtable* vsm_in_signals_vtable = NULL;
sd_bus_vtable* vsm_out_signals_vtable = NULL;
sd_bus_vtable* vsm_internal_signals_vtable = NULL;

// Build the start of table, returns the next index
int startBuildVtables(int numInSignals, int numOutSignals, int numInternalSignals) {
    vsm_in_signals_vtable = calloc(numInSignals + NUMBER_OF_EXTRA_SD_BUS_ENTRIES, sizeof(sd_bus_vtable));
    vsm_out_signals_vtable = calloc(numOutSignals + NUMBER_OF_EXTRA_SD_BUS_ENTRIES, sizeof(sd_bus_vtable));
    vsm_internal_signals_vtable = calloc(numInternalSignals + NUMBER_OF_EXTRA_SD_BUS_ENTRIES, sizeof(sd_bus_vtable));

    vsm_in_signals_vtable[0] = get_vtable_start(0);
    vsm_out_signals_vtable[0] = get_vtable_start(0);
    vsm_internal_signals_vtable[0] = get_vtable_start(0);
    return 1;
}

int addVtablePropertyEntry(sd_bus_vtable* table, const char* name, int index) {
    table[index] = get_vtable_wrproperty(name, vsm_signal_property_read, vsm_signal_property_write);
    index++;
    return index;
}

void endBuildVtable(sd_bus_vtable* table, int index) {
    table[index] = get_vtable_signal(SIGNALS_CHANGED_PROP_NAME, "s");
    table[index + 1] = get_vtable_signal(VSM_STARTED_SIGNAL_NAME, "");
    /*A return value "s" is defined as when the SD_BUS_VTABLE_METHOD_NO_REPLY it was not found when the client was
     * trying to call it  */
    table[index + 2] = get_vtable_method(VSM_SET_DE_SIGNAL_METHOD_NAME, "ss", "s", vsm_signal_method_set,
                                         SD_BUS_VTABLE_UNPRIVILEGED);
    table[index + 3] = get_vtable_end();
}
