/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include "dataelementcommbus.h"
#include "dataelementcommbus_dbus_receiver.h"
#include "pipe.h"
#include <map>
#include <set>
#include <systemd/sd-bus.h>
#include <thread>
#include "logging.h"

/// This class keeps track of that the correct sdbus object is used when
/// accessed from different threads.
class SdBusWrapper
{
public:
    SdBusWrapper();

    virtual ~SdBusWrapper();
    sd_bus* _sdbus = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////////////
/// DBus implementation of the DataElementCommBus
class DataElementCommBusDBus : public IDataElementCommBus
{
public:
    void setNewDataElementHandler(
        std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) override;
    void addName(autosar::Dir dir, const std::string& name) override;
    void send(const std::string& name, const std::string& payload, autosar::Dir dir) override;

    static DataElementCommBusDBus& instance() noexcept;

    // delete copy and move constructors and assign operators
    DataElementCommBusDBus(DataElementCommBusDBus const&) = delete;            // Copy construct
    DataElementCommBusDBus(DataElementCommBusDBus&&)      = delete;            // Move construct
    DataElementCommBusDBus& operator=(DataElementCommBusDBus const&) = delete; // Copy assign
    DataElementCommBusDBus& operator=(DataElementCommBusDBus&&) = delete;      // Move assign

#ifndef UNIT_TEST
private:
#endif
    DataElementCommBusDBus(DataElementCommBusDBusReceiver& dBusReceiver, bool startAsynch = true);
    virtual ~DataElementCommBusDBus();

    void startDBUSMessageLoop();
    void VSMStarted();

    static thread_local SdBusWrapper sdbus;
    std::mutex                       pendingMessageMutex_;
    std::map<std::tuple<std::string, autosar::Dir>, std::string> pendingSendMessages_;
    DataElementCommBusDBusReceiver& dBusReceiver_;
    std::thread loopthread_;
};
