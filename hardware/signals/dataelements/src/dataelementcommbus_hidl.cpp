#include "dataelementcommbus_hidl.h"

DataElementCommBusHIDL::DataElementCommBusHIDL()
{
    proxy_ = ihu::signals::V1_0::ISignals::getService();
    // Set up fault handling by calling proxy_->linkToDeath() ??
}

IDataElementCommBus* IDataElementCommBus::create()
{
    //log_info() << "DataElementCommBus::create()";
    return &DataElementCommBusHIDL::instance();
}

DataElementCommBusHIDL& DataElementCommBusHIDL::instance() noexcept
{
    static DataElementCommBusHIDL theInstance{};
    return theInstance;
}

void DataElementCommBusHIDL::send(const std::string& name, const std::string& payload, autosar::Dir dir)
{
//#ifdef DEBUG
//    log_verbose() << "Sending name: " << name << "payload:" << payload;
//#endif

    proxy_->send(name, payload);

    // if (retval < 0)
    // {
    //     log_warn() << "Error writeProperty property:" << name << " Error: " << strerror(-retval);

    //     // If we fail to write messages it might be that the service is not up yet
    //     // put it in the map to try later on
    //     {
    //         auto                        key = std::make_tuple(name, dir);
    //         std::lock_guard<std::mutex> lock(pendingMessageMutex_);
    //         pendingSendMessages_[key] = payload;
    //     }
    // }
}

void DataElementCommBusHIDL::setNewDataElementHandler(
    std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback)
{
    //dBusReceiver_.setNewDataElementHandler(std::move(newDataElementCallback));
}

void DataElementCommBusHIDL::addName(autosar::Dir dir, const std::string& name)
{
    //dBusReceiver_.subscribeToSignal(dir, name);
    //proxy_->subscribe(name, cb)
}

void DataElementCommBusHIDL::VSMStarted()
{
    // log_info() << "Resend total nr of " << pendingSendMessages_.size() << " messages";

    // std::map<std::tuple<std::string, autosar::Dir>, std::string> pendingSendMessagesCopy;

    // { // Lock scope
    //     std::lock_guard<std::mutex> lock(pendingMessageMutex_);
    //     // Take a copy of the list to decrease the scope of lock
    //     pendingSendMessagesCopy = pendingSendMessages_;
    //     pendingSendMessages_.clear();
    // }

    // for (const auto& msg : pendingSendMessagesCopy)
    // {
    //     auto msgName = std::get<0>(msg.first);
    //     auto msgDir  = std::get<1>(msg.first);
    //     auto payload = msg.second;
    //     log_verbose() << "Resending: " << msgName;
    //     send(msgName, payload, msgDir);
    //}
}
