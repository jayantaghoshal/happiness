/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelementcommbus_dbus_receiver.h"
#include "dataelementcommbus_dbus_common.h"

#include <mutex>
#include <set>
#include "logging.h"

LOG_SET_DEFAULT_CONTEXT(deleContext)

const std::string                            DataElementCommBusDBusReceiver::VSM_STARTED_SIGNAL_NAME = "VSMStarted";
std::atomic<DataElementCommBusDBusReceiver*> DataElementCommBusDBusReceiver::_theInstance{ nullptr };
std::mutex                                   DataElementCommBusDBusReceiver::_singeltonLock;

////////////////// Factory methods /////////////////////////////////
Pipe& DataElementCommBusDBusReceiver::createWakeupPipe()
{
    static Pipe theInstance;
    return theInstance;
}

DataElementCommBusDBusReceiver& DataElementCommBusDBusReceiver::instance() noexcept
{
    if (_theInstance == nullptr)
    {
        // Thread safe singelton using pointers
        std::lock_guard<std::mutex> lock(_singeltonLock);
        if (_theInstance == nullptr)
        {
            _theInstance = new DataElementCommBusDBusReceiver(createWakeupPipe());
        }
    }
    return *_theInstance;
}
////////////////// End Factory methods /////////////////////////////////

DataElementCommBusDBusReceiver::DataElementCommBusDBusReceiver(Pipe& wakeUpPipe) noexcept : _bus(nullptr),
                                                                                            _sdevent(nullptr),
                                                                                            _wakeUpPipe(wakeUpPipe),
                                                                                            _callback(nullptr),
                                                                                            _vsmStartedCallback(nullptr)
{
    log_debug() << "DataElementCommBus Open DBUS for receiving";
    // Add defer handler to be able to interrupt the mainloop and add filters on
    // signals

    auto result = _wakeUpPipe.Open();
    if (result != 0)
    {
        int err = errno;
        log_error() << "Failed to create  wakeup pipe." << err;
    }

    result = sd_bus_open_system(&_bus);
    if (result < 0)
    {
        log_error() << "Failed to open system dbus: " << -result << " " << strerror(-result);
    }
}
// Will never be called as it is a singelton
DataElementCommBusDBusReceiver::~DataElementCommBusDBusReceiver()
{
    if (_bus)
    {
        sd_bus_close(_bus);
    }
    _wakeUpPipe.Close();
    _bus = nullptr;
}

bool DataElementCommBusDBusReceiver::startDBUSEventLoop() noexcept
{
    sd_event_source* _eventSource;
    // Set up sd-event for message loop
    int result = sd_event_default(&_sdevent);
    if (result < 0)
    {
        log_error() << "Failed creating sd event: " << strerror(-result);
        return false;
    }
    result = sd_bus_attach_event(_bus, _sdevent, 0);
    if (result < 0)
    {
        log_error() << "Failed creating sd bus attach event: " << strerror(-result);
        return false;
    }
    sd_event_source* event_source = NULL;

    // Setup so that we are called once when the event loop is started
    result = sd_event_add_defer(_sdevent, &event_source, eventLoopStartedHandler, this);
    if (result < 0)
    {
        log_error() << "Failed creating sd bus attach event: " << strerror(-result);
        return false;
    }

    result = sd_event_source_set_enabled(event_source, SD_EVENT_ONESHOT);
    if (result < 0)
    {
        log_error() << "Failed event set source enabled:", strerror(-result);
        return false;
    }

    result = sd_event_add_io(_sdevent, &_eventSource, _wakeUpPipe.ReadFD(), EPOLLIN, wakeupCallback, NULL);
    if (result < 0)
    {
        log_error() << "Failed to create wakeup pipe event source.";
        return false;
    }

    log_info() << "Start event loop for receiving: ";
    // Start the message loop, blocks forever
    result = sd_event_loop(_sdevent);
    if (result < 0)
    {
        log_error() << "Failed to start event loop for receiving : " << strerror(-result);
        return false;
    }

    log_info() << "Event loop exited";

    return true;
}

void DataElementCommBusDBusReceiver::exit()
{
    log_info() << "DataElementCommBusDBusReceiver exiting";
    if (_sdevent==nullptr)
    {
        log_info() << "DataElementCommBusDBusReceiver waiting for _sdevent";
        sleep(1);
    }
    log_info() << "sd_event_exit:" << sd_event_exit(_sdevent,0);
    wakeUpEventLoop();
}

void DataElementCommBusDBusReceiver::subscribeToSignal(autosar::Dir dir, const std::string& name) noexcept
{
    {
        std::lock_guard<std::mutex> lock(_pendingSubscriptionMutex);
        _pendingSubscriptions.emplace(name, dir);
        wakeUpEventLoop();
    }
}

void DataElementCommBusDBusReceiver::setNewDataElementHandler(
    std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) noexcept
{
    _callback = std::move(newDataElementCallback);
}

void DataElementCommBusDBusReceiver::setVSMStartedCB(std::function<void(void)>&& vsmStartedCallback) noexcept
{
    _vsmStartedCallback = vsmStartedCallback;
}

void DataElementCommBusDBusReceiver::emptyWakeupPipe(std::uint32_t revents)
{
    if ((revents & EPOLLIN) != 0)
    {
        char buffer[100];
        if (DataElementCommBusDBusReceiver::instance()._wakeUpPipe.Read(buffer, sizeof buffer) < 0)
            log_error() << "Failed to read from wakeup pipe.";
    }
}

bool DataElementCommBusDBusReceiver::wakeUpEventLoop()
{
    char data = 0;
    if (_wakeUpPipe.Write(&data, sizeof data) < 0)
    {
        log_error() << "Failed to write to wakeup pipe.";
        return false;
    }
    return true;
}

int DataElementCommBusDBusReceiver::wakeupCallback(sd_event_source* source,
                                                   int              fd,
                                                   std::uint32_t    revents,
                                                   void*            user_data) noexcept
{
    (void)source;
    (void)fd;
    (void)user_data;

    DataElementCommBusDBusReceiver& thisInst = DataElementCommBusDBusReceiver::instance();

    log_debug() << "wakeupCallback sub size = " << thisInst._pendingSubscriptions.size();
    { // Lock scope
        std::lock_guard<std::mutex> lock(thisInst._pendingSubscriptionMutex);

        // Clear the data from pipe
        thisInst.emptyWakeupPipe(revents);

        auto i = std::begin(thisInst._pendingSubscriptions);

        // Loop through the list of pending subscriprions and
        // 1. Subscrbe to changes for that signal by doing addMatch
        // 2. Read the current value and execute the registered callback
        while (i != std::end(thisInst._pendingSubscriptions))
        {
            const auto subName = std::get<0>(*i);
            const auto subDir  = std::get<1>(*i);
            if (thisInst.addMatch(subDir, subName))
            {
                // Read and notify clients
                std::string signalDataOut;
                if (thisInst.readSignalData(subName, subDir, signalDataOut) && signalDataOut != "")
                {
                    thisInst.notifyCallback(subName, signalDataOut);
                }
                i = thisInst._pendingSubscriptions.erase(i);
            }
            else
            {
                ++i;
            }
        }
    }
    return 0;
}

bool DataElementCommBusDBusReceiver::addMatch(autosar::Dir dir, const std::string& name) noexcept
{
    // Building up the the match string could look something like this:
    // type='signal', sender='com.ihu.VehicleSignalsManager.In',
    // interface='com.ihu.VehicleSignalsManager.In',member='SignalChanged',arg0='ProfPenSts1',
    // sender='com.ihu.VehicleSignalsManager.In'

    std::string matchString = "sender='" + DECommonDBUS::dirToDBUSInterfaceName(dir) + "',";
    matchString += "interface='" + DECommonDBUS::dirToDBUSInterfaceName(dir) + "',";
    matchString += "member='" + DECommonDBUS::DBUS_SIGNALS_CHANGED_NAME + "',";
    matchString += "arg0='" + name + "'";

    int direction = static_cast<int>(dir);

    auto ret_val = sd_bus_add_match(_bus, NULL, matchString.c_str(), DESignalChanged, (void*)(intptr_t)direction);
    if (ret_val < 0)
    {
        log_error() << "Failed to add match for "
                       "SignalChanged: "
                    << strerror(-ret_val);
        return false;
    }

    return true;
}

bool DataElementCommBusDBusReceiver::addMatchVSMStartedSignal() noexcept
{
    // Building up the the match string could look something like this:
    // type='signal', sender='com.ihu.VehicleSignalsManager.In',
    // interface='com.ihu.VehicleSignalsManager.In',member='SignalChanged',arg0='ProfPenSts1',
    // sender='com.ihu.VehicleSignalsManager.In'

    std::string matchString = "sender='" + DECommonDBUS::dirToDBUSInterfaceName(autosar::Dir::IN) + "',";
    matchString += "interface='" + DECommonDBUS::dirToDBUSInterfaceName(autosar::Dir::IN) + "',";
    matchString += "member='" + VSM_STARTED_SIGNAL_NAME + "'";

    auto ret_val = sd_bus_add_match(_bus, NULL, matchString.c_str(), VSMStarted, NULL);
    if (ret_val < 0)
    {
        log_error() << "Failed to add match for " << VSM_STARTED_SIGNAL_NAME
                    << " SignalChanged: " << strerror(-ret_val);
        return false;
    }

    return true;
}

int DataElementCommBusDBusReceiver::DESignalChanged(sd_bus_message* message,
                                                    void*           userdata,
                                                    sd_bus_error*   error) noexcept
{
    (void)message;
    (void)error;

    const char* changedSignal = NULL;

    int          direction = (intptr_t)userdata;
    autosar::Dir dir       = static_cast<autosar::Dir>(direction);

    int result = sd_bus_message_read(message, "s", &changedSignal);

    if (result < 0)
    {
        log_error() << "DESignalChanged: failed to read DBUS message:" << strerror(-result);
        return -1;
    }
#ifdef DEBUG
    log_verbose() << "DESignalChanged " << direction << " " << changedSignal;
#endif
    // Read out the signal data that was changed and notify the client by calling
    // the callback
    std::string signalDataOut;

    if (DataElementCommBusDBusReceiver::instance().readSignalData(changedSignal, dir, signalDataOut)
        && signalDataOut != "")
    {
        DataElementCommBusDBusReceiver::instance().notifyCallback(changedSignal, signalDataOut);
    }

    return 0;
}

int DataElementCommBusDBusReceiver::VSMStarted(sd_bus_message* message, void* userdata, sd_bus_error* error) noexcept
{
    (void)message;
    (void)userdata;
    (void)error;

    if (DataElementCommBusDBusReceiver::instance()._vsmStartedCallback)
    {
        DataElementCommBusDBusReceiver::instance()._vsmStartedCallback();
    }
    return 0;
}

int DataElementCommBusDBusReceiver::eventLoopStartedHandler(sd_event_source* s, void* userdata)
{
    (void)s;

    DataElementCommBusDBusReceiver* thisObject = static_cast<DataElementCommBusDBusReceiver*>(userdata);

    thisObject->addMatchVSMStartedSignal();

    return 0;
}

bool DataElementCommBusDBusReceiver::readSignalData(const std::string& signalName,
                                                    autosar::Dir       direction,
                                                    std::string&       signalDataOut) noexcept
{
    char*        signalValue = nullptr;
    sd_bus_error error       = SD_BUS_ERROR_NULL;

    auto retval = sd_bus_get_property_string(_bus,
                                             DECommonDBUS::dirToDBUSInterfaceName(direction).c_str(),
                                             DECommonDBUS::dirToDBUSObjectName(direction).c_str(),
                                             DECommonDBUS::dirToDBUSInterfaceName(direction).c_str(),
                                             signalName.c_str(),
                                             &error,
                                             &signalValue);

    if (retval < 0 || (signalValue == nullptr))
    {
        log_warn() << "Error fetching property " << signalName << "Error:" << strerror(-retval);
        return false;
    }
    signalDataOut = signalValue;
    free(signalValue);

    return true;
}

void DataElementCommBusDBusReceiver::notifyCallback(const std::string& signalName,
                                                    const std::string& signalData) noexcept
{
    if (DataElementCommBusDBusReceiver::instance()._callback)
    {
        try
        {
            DataElementCommBusDBusReceiver::instance()._callback(signalName, signalData);
        }
        catch (const std::exception& e)
        {
            log_error() << "unhandled exception in DataElementCommBus::on_message callback,"
                        << " topic: " << signalName << " error: " << e.what();
        }
    }
}
