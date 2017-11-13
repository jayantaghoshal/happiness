#include "restart_client.h"
#include <ivi-logging.h>
#include <cstring>

extern "C" {
#include <NodeStateTypes.h>
}

// Define a context for the IVI-logging library to use.
typedef logging::DefaultLogContext LogContext;

LOG_DECLARE_DEFAULT_CONTEXT(carconfig_updater_restart_log_context, "MAIN", "Main thread");

restartClient::restartClient() {
    error = SD_BUS_ERROR_NULL;
    bus = NULL;
    m = NULL;
}

restartClient::~restartClient() {
    sd_bus_error_free(&error);
    sd_bus_message_unref(m);
    sd_bus_unref(bus);
}

bool restartClient::restart() {
    std::uint32_t errorcode;
    int r;

    r = sd_bus_open_system(&bus);
    if (r < 0) {
        log_error() << "Failed to connect to system bus: " << strerror(-r);
        return false;
    }

    int reason = NsmRestartReason_CarConfig;
    unsigned type = NSM_SHUTDOWNTYPE_NORMAL;

    /* Issue the method call and store the response message in m */
    r = sd_bus_call_method(bus, "org.genivi.NodeStateManager",              /* service to contact */
                           "/org/genivi/NodeStateManager/LifecycleControl", /* object path */
                           "org.genivi.NodeStateManager.LifecycleControl",  /* interface name */
                           "RequestNodeRestart",                            /* method name */
                           &error,                                          /* object to return error in */
                           &m,                                              /* return message on success */
                           "iu",                                            /* input signature */
                           reason,                                          /* 1st arg, restart reason */
                           type);                                           /* 2nd arg, restart type */
    if (r < 0) {
        log_error() << "Failed to issue method call: " << error.message;
        return false;
    }

    /* Parse the response message */
    r = sd_bus_message_read(m, "i", &errorcode);
    if (r < 0) {
        log_error() << "Failed to parse response message: " << strerror(-r);
        return false;
    }

    log_debug() << "Restart error code: " << errorcode;
    return errorcode == NsmErrorStatus_Ok;
}
