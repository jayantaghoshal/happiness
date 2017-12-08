#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H

#include <IDispatcher.h>
#include <ipcommandbus/MessageDispatcher.h>
#include <string>

#include <vendor/volvocars/hardware/vehiclecom/1.0/IMessageCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IResponseCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IResponseCallback;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::OperationType;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::RetryInfo;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

namespace Connectivity {
/**
 * Service manager for managing Common API service instances.
 *
 */
class ServiceManager : public IVehicleCom {
  public:
    ServiceManager(const ServiceManager&) = delete;

    ServiceManager(std::string service_name, ::Connectivity::MessageDispatcher& msgDispatcher);

    // Methods from ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom follow.
    Return<void> subscribe(uint16_t serviceID, uint16_t operationID, OperationType operationType,
                           const sp<IMessageCallback>& callbackHandler, subscribe_cb _hidl_cb) override;
    Return<void> unsubscribe(uint64_t subscriberId, unsubscribe_cb _hidl_cb) override;
    Return<void> sendMessage(const Msg& msg, const RetryInfo& retryInfo, sendMessage_cb _hidl_cb) override;
    Return<void> sendRequest(const Msg& msg, const RetryInfo& retryInfo, const sp<IResponseCallback>& callbackHandler,
                             sendRequest_cb _hidl_cb) override;

  private:
    MessageDispatcher& messageDispatcher_;
};

}  // namespace Connectivity

#endif  // SERVICE_MANAGER_H
