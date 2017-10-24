#pragma once

#include <vendor/volvocars/hardware/signals/1.0/ISignals.h>
#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include "dataelementcommbus.h"

namespace andrHw = ::android::hardware;
namespace dataElemHidl = vendor::volvocars::hardware::signals::V1_0;

///////////////////////////////////////////////////////////////////////////////////////////
/// Hidl implementation of the DataElementCommBus
class DataElementCommBusHIDL final : public IDataElementCommBus,
                                     public ::android::hidl::manager::V1_0::IServiceNotification,
                                     public andrHw::hidl_death_recipient,
                                     public dataElemHidl::ISignalsChangedCallback {
 public:
  void setNewDataElementHandler(
      std::function<void(const std::string& name, const std::string& payload)>&& newDataElementCallback) override;
  void addName(autosar::Dir dir, const std::string& name) override;
  void send(const std::string& name, const std::string& payload, autosar::Dir dir) override;
  andrHw::Return<void> onRegistration(const andrHw::hidl_string& fqName, const andrHw::hidl_string& name,
                                      bool preexisting) override;
  void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override;

  andrHw::Return<void> signalChanged(const andrHw::hidl_string& signalName, dataElemHidl::Dir dir,
                                     const andrHw::hidl_string& data) override;

  static DataElementCommBusHIDL& instance() noexcept;

 private:
  DataElementCommBusHIDL();

  void resendMessages(::android::sp<dataElemHidl::ISignals>& vsd_proxy);
  void resendSubscriptions(::android::sp<dataElemHidl::ISignals>& vsd_proxy_local);
  void connectToVsdProxyAndResend();
  void sendWithoutProxyMutex(const std::string& name, const std::string& payload, const autosar::Dir dir,
                             ::android::sp<dataElemHidl::ISignals>& vsd_proxy);
  void addNameWithoutProxyMutex(const autosar::Dir dir, const std::string& name,
                                ::android::sp<dataElemHidl::ISignals>& vsd_proxy_local);

  std::mutex pendingMessageMutex_;
  std::mutex pendingSubscriptionsMutex_;
  std::mutex vsdProxyMutex_;
  std::map<std::tuple<std::string, autosar::Dir>, std::string> pendingSendMessages_;
  std::set<std::tuple<std::string, autosar::Dir>> pendingSubscriptions_;
  ::android::sp<dataElemHidl::ISignals> vsd_proxy_;
  std::function<void(const std::string& name, const std::string& payload)> dataElementCallback_;

  static const uint64_t ISIGNAL_HAL_DEATH_COOKIE = 0xDEAD;  // Cookie sent for ISignal hal death notification.
};
