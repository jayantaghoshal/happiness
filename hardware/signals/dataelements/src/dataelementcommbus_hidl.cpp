#include "dataelementcommbus_hidl.h"
#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "DataElementCommbusHidl"

DataElementCommBusHIDL::DataElementCommBusHIDL()
    : pendingSendMessages_{}, vsd_proxy_{nullptr}, dataElementCallback_{nullptr}
{
}

IDataElementCommBus* IDataElementCommBus::create()
{
  ALOGI("DataElementCommBus::create()");
  return &DataElementCommBusHIDL::instance();
}

DataElementCommBusHIDL& DataElementCommBusHIDL::instance() noexcept
{
  static DataElementCommBusHIDL theInstance{};
  return theInstance;
}
void DataElementCommBusHIDL::send(const std::string& name, const std::string& payload, autosar::Dir dir)
{
  ::android::sp<dataElemHidl::ISignals> vsd_proxy_local;
  {  // Lock scope
    std::lock_guard<std::mutex> lock(vsdProxyMutex_);
    // Make a copy and release the lock
    vsd_proxy_local = vsd_proxy_;
  }
  sendWithoutProxyMutex(name, payload, dir, vsd_proxy_local);
}

// Param vsd_proxy_local should be a local copy
void DataElementCommBusHIDL::sendWithoutProxyMutex(const std::string& name, const std::string& payload,
                                                   autosar::Dir dir,
                                                   ::android::sp<dataElemHidl::ISignals> vsd_proxy_local)
{
  if (vsd_proxy_local != nullptr)
  {
    auto result = vsd_proxy_local->send(name, static_cast<dataElemHidl::Dir>(dir), payload);
    if (result.isOk())
    {
      return;
    }

    ALOGW("Error %s while writing property: %s. Message will be send if service becomes available",
          result.description().c_str(), name.c_str());

    // If we fail to write messages it might be that the service is not up yet
    // put it in the map to try later on
    {  // lock scope
      auto key = std::make_tuple(name, dir);
      std::lock_guard<std::mutex> lock(pendingMessageMutex_);
      pendingSendMessages_[key] = payload;
    }
  }
}

void DataElementCommBusHIDL::setNewDataElementHandler(
    std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback)
{
  bool doOnce = dataElementCallback_ == nullptr;
  dataElementCallback_ = std::move(newDataElementCallback);
  ALOGI("New dataElementHandler set");

  if (doOnce)
  {
    ALOGI("Register for notifications for signaling service to come up");
    dataElemHidl::ISignals::registerForNotifications("default", this);
    connectToVsdProxyAndResend();
  }
}

void DataElementCommBusHIDL::addName(autosar::Dir dir, const std::string& name)
{
  {  // lock scope
    std::lock_guard<std::mutex> lock(vsdProxyMutex_);
    if (vsd_proxy_ != nullptr)
    {
      if (vsd_proxy_->subscribe(name, static_cast<dataElemHidl::Dir>(dir), this).isOk())
      {
        return;
      }
    }
  }
  ALOGE("Signaling service not available, subscription of %s not registered", name.c_str());
}

void DataElementCommBusHIDL::resendMessages(::android::sp<dataElemHidl::ISignals> vsd_proxy_local)
{
  ALOGI("Resend total nr of %zu messages", pendingSendMessages_.size());

  std::map<std::tuple<std::string, autosar::Dir>, std::string> pendingSendMessagesCopy;

  {  // Lock scope
    std::lock_guard<std::mutex> lock(pendingMessageMutex_);
    // Take a copy of the list to decrease the scope of lock
    pendingSendMessagesCopy = pendingSendMessages_;
    pendingSendMessages_.clear();
  }

  for (const auto& msg : pendingSendMessagesCopy)
  {
    auto msgName = std::get<0>(msg.first);
    auto msgDir = std::get<1>(msg.first);
    auto payload = msg.second;
    ALOGD("Resending: %s", msgName.c_str());
    sendWithoutProxyMutex(msgName, payload, msgDir, vsd_proxy_local);
  }
}

void DataElementCommBusHIDL::connectToVsdProxyAndResend()
{
  ::android::sp<dataElemHidl::ISignals> vsd_proxy_local;
  {  // lock scope
    std::lock_guard<std::mutex> lock(vsdProxyMutex_);
    if (vsd_proxy_ != nullptr)
    {
      return;
    }
    vsd_proxy_ = dataElemHidl::ISignals::getService();
    if (vsd_proxy_ == nullptr)
    {
      ALOGE("Could not contact ISignals HAL");
      return;
    }
    auto result = vsd_proxy_->linkToDeath(this, ISIGNAL_HAL_DEATH_COOKIE);
    if (!result.isOk())
    {
      ALOGE("Signaling service not available, link to death not possible: %s", result.description().c_str());
      return;
    }
    ALOGI("Signaling service is up and registred as a proxy");
    vsd_proxy_local = vsd_proxy_;
  }

  resendMessages(vsd_proxy_local);
}

andrHw::Return<void> DataElementCommBusHIDL::onRegistration(const andrHw::hidl_string& fqName,
                                                            const andrHw::hidl_string& name, bool preexisting)
{
  ALOGI("Signal service started callback received");
  connectToVsdProxyAndResend();
  return andrHw::Void();
}

void DataElementCommBusHIDL::serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who)
{
  {  // lock scope
    std::lock_guard<std::mutex> lock(vsdProxyMutex_);
    if (cookie != ISIGNAL_HAL_DEATH_COOKIE || who != vsd_proxy_)
    {
      ALOGD(
          "Service died callback received but cookie and/or \"who\" did not match this clients server. Cookie "
          "received: %lu",
          cookie);
      return;
    }

    vsd_proxy_ = nullptr;
  }

  ALOGE("Signal service died callback received");
}

andrHw::Return<void> DataElementCommBusHIDL::signalChanged(const andrHw::hidl_string& signalName, dataElemHidl::Dir dir,
                                                           const andrHw::hidl_string& data)
{
  ALOGV("Signal %s changed to %s", signalName.c_str(), data.c_str());

  if (dataElementCallback_)
  {
    try
    {
      dataElementCallback_(signalName, data);
    }
    catch (std::exception& e)
    {
      ALOGE("Dataelement callback throw an exception: %s while receiving signal: %s", e.what(), signalName.c_str());
    }
  }
  return andrHw::Void();
}
