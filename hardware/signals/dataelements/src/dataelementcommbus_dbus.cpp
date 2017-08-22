/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelementcommbus_dbus.h"
#include "dataelementcommbus_dbus_common.h"

#include <thread>

#undef LOG_TAG
#define LOG_TAG "DataElementDBus"
#include <cutils/log.h>

SdBusWrapper::SdBusWrapper()
{
    int result = sd_bus_default_system(&_sdbus);

    if (result < 0)
    {
        log_error() << "SdBusWrapper: Failed to open system dbus: " << -result << " " << strerror(-result);
    }
}

SdBusWrapper::~SdBusWrapper()
{
    sd_bus_unref(_sdbus);
}

thread_local SdBusWrapper DataElementCommBusDBus::sdbus;

DataElementCommBusDBus& DataElementCommBusDBus::instance() noexcept
{
    static DataElementCommBusDBus theInstance(DataElementCommBusDBusReceiver::instance());
    return theInstance;
}

IDataElementCommBus* IDataElementCommBus::create()
{
    log_info() << "DataElementCommBus::create()";
    return &DataElementCommBusDBus::instance();
}

DataElementCommBusDBus::DataElementCommBusDBus(DataElementCommBusDBusReceiver& dBusReceiver, bool startAsynch)
    : dBusReceiver_(dBusReceiver)
{

    dBusReceiver_.setVSMStartedCB([this] { VSMStarted(); });

    // Start the dbus message loop into a seperate thread
    if (startAsynch)
    {
        loopthread_ = std::thread([this]() {
            bool result = dBusReceiver_.startDBUSEventLoop();
            if (!result)
            {
                log_fatal() << "DataElementCommBusDBus::DataElementCommBusDBus, Failed "
                               "to start DE Receiver event loop";
            }
            log_info() << "DataElementCommBusDBus loopthread exited";
        });
    }
    else
    {
        dBusReceiver_.startDBUSEventLoop();
    }
}

// Will never be called as it is a singelton
DataElementCommBusDBus::~DataElementCommBusDBus()
{
    dBusReceiver_.exit();
    if (loopthread_.joinable()) loopthread_.join();
    log_info() << "~DataElementCommBusDBus done";
}

void DataElementCommBusDBus::send(const std::string& name, const std::string& payload, autosar::Dir dir)
{
    sd_bus_error error = SD_BUS_ERROR_NULL;
#ifdef DEBUG
    log_verbose() << "Sending name: " << name << "payload:" << payload;
#endif

    auto retval = sd_bus_set_property(sdbus._sdbus,
                                      DECommonDBUS::dirToDBUSInterfaceName(dir).c_str(),
                                      DECommonDBUS::dirToDBUSObjectName(dir).c_str(),
                                      DECommonDBUS::dirToDBUSInterfaceName(dir).c_str(),
                                      name.c_str(),
                                      &error,
                                      "s",
                                      payload.c_str()); // "s" = string argument

    if (retval < 0)
    {
        log_warn() << "Error writeProperty property:" << name << " Error: " << strerror(-retval);

        // If we fail to write messages it might be that the service is not up yet
        // put it in the map to try later on
        {
            auto                        key = std::make_tuple(name, dir);
            std::lock_guard<std::mutex> lock(pendingMessageMutex_);
            pendingSendMessages_[key] = payload;
        }
    }
}

void DataElementCommBusDBus::setNewDataElementHandler(
    std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback)
{
    dBusReceiver_.setNewDataElementHandler(std::move(newDataElementCallback));
}

void DataElementCommBusDBus::addName(autosar::Dir dir, const std::string& name)
{
    log_verbose() << "addName()" << name;
    dBusReceiver_.subscribeToSignal(dir, name);
}

void DataElementCommBusDBus::VSMStarted()
{
    log_info() << "Resend total nr of " << pendingSendMessages_.size() << " messages";

    std::map<std::tuple<std::string, autosar::Dir>, std::string> pendingSendMessagesCopy;

    { // Lock scope
        std::lock_guard<std::mutex> lock(pendingMessageMutex_);
        // Take a copy of the list to decrease the scope of lock
        pendingSendMessagesCopy = pendingSendMessages_;
        pendingSendMessages_.clear();
    }

    for (const auto& msg : pendingSendMessagesCopy)
    {
        auto msgName = std::get<0>(msg.first);
        auto msgDir  = std::get<1>(msg.first);
        auto payload = msg.second;
        log_verbose() << "Resending: " << msgName;
        send(msgName, payload, msgDir);
    }
}
