/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/


#pragma once
#include "dataelementcommbus_dbus_common.h"
#include "pipe.h"
#include <memory>
#include <mutex>
#include <set>
#include <systemd/sd-bus.h>

// Is responsible for receiving updates of signals and setting up subscriptions
// on signals to receive
class DataElementCommBusDBusReceiver
{
public:
    /// Starts the DBUS event loop, will block forever
    bool startDBUSEventLoop() noexcept;
    /// Subscribes to the signal with the given name
    void subscribeToSignal(autosar::Dir dir, const std::string& name) noexcept;
    /// Sets the callback for receiving signals
    void setNewDataElementHandler(
        std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) noexcept;

    /// VSM started callback
    void setVSMStartedCB(std::function<void(void)>&& vsmStartedCallback) noexcept;

    void exit(); // stop event loop

    // Signgelton
    static DataElementCommBusDBusReceiver& instance() noexcept;

    // delete copy and move constructors and assign operators
    DataElementCommBusDBusReceiver(DataElementCommBusDBusReceiver const&) = delete;            // Copy construct
    DataElementCommBusDBusReceiver(DataElementCommBusDBusReceiver&&)      = delete;            // Move construct
    DataElementCommBusDBusReceiver& operator=(DataElementCommBusDBusReceiver const&) = delete; // Copy assign
    DataElementCommBusDBusReceiver& operator=(DataElementCommBusDBusReceiver&&) = delete;      // Move assign

    ~DataElementCommBusDBusReceiver() noexcept;
#ifndef UNIT_TEST
private:
#endif
    // The DBUS connection object
    sd_bus* _bus;
    sd_event*        _sdevent;

    // The pipe to read to be able to wake up the event loop and add watches to
    // new signals
    Pipe&      _wakeUpPipe;
    std::mutex _pendingSubscriptionMutex;

    std::set<std::tuple<std::string, autosar::Dir>> _pendingSubscriptions;

    /// The callback function which is called when signal is changed.
    std::function<void(const std::string& name, const std::string& payload)> _callback;

    /// The callback function which is called when Vehicle SIgnals Manager is atarted
    std::function<void(void)> _vsmStartedCallback;

    static std::atomic<DataElementCommBusDBusReceiver*> _theInstance;
    static std::mutex                                   _singeltonLock;

    /// DBUS Signal name to detect if VSM is (re)started
    static const std::string VSM_STARTED_SIGNAL_NAME;

    /// Wake up event loop
    bool wakeUpEventLoop();

    void emptyWakeupPipe(std::uint32_t revents);

    /// Is called when someone is writing to _wakeUpPipe
    static int wakeupCallback(sd_event_source* source, int fd, std::uint32_t revents, void* user_data) noexcept;

    /// Called when a flexray signal has been changed
    static int DESignalChanged(sd_bus_message* message, void* userdata, sd_bus_error* error) noexcept;

    static int VSMStarted(sd_bus_message* message, void* userdata, sd_bus_error* error) noexcept;

    static int eventLoopStartedHandler(sd_event_source* s, void* userdata);

    bool addMatch(autosar::Dir dir, const std::string& name) noexcept;
    bool addMatchVSMStartedSignal() noexcept;

    /// Reads the data for a new signal
    bool readSignalData(const std::string& signalName, autosar::Dir direction, std::string& signalDataOut) noexcept;

    void notifyCallback(const std::string& signalName, const std::string& signalData) noexcept;

    DataElementCommBusDBusReceiver(Pipe& wakeUpPipe) noexcept;

    static Pipe& createWakeupPipe();
};
