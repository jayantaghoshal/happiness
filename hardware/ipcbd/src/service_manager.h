#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H

#include <IDispatcher.h>
#include <ipcommandbus/MessageDispatcher.h>
#include <cutils/log.h>
#include <string>

#include <vendor/volvocars/hardware/ipcb/1.0/IIpcb.h>
#include <vendor/volvocars/hardware/ipcb/1.0/IMessageCallback.h>
#include <vendor/volvocars/hardware/ipcb/1.0/IResponseCallback.h>

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::vendor::volvocars::hardware::ipcb::V1_0::IIpcb;
using ::vendor::volvocars::hardware::ipcb::V1_0::IMessageCallback;
using ::vendor::volvocars::hardware::ipcb::V1_0::IResponseCallback;
using ::vendor::volvocars::hardware::ipcb::V1_0::Msg;
using ::vendor::volvocars::hardware::ipcb::V1_0::Status;
using ::vendor::volvocars::hardware::ipcb::V1_0::OperationType;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

namespace Connectivity
{
/**
 * Service manager for managing Common API service instances.
 *
 */
class ServiceManager : public IIpcb
{
public:
    ServiceManager(const ServiceManager&) = delete;

    ServiceManager(std::string service_name, ::Connectivity::MessageDispatcher& msgDispatcher);

    // Methods from ::vendor::volvocars::hardware::ipcb::V1_0::IIpcb follow.
    Return<Status> subscribeMessage(uint16_t serviceID, uint16_t operationID, const hidl_vec<OperationType>& operationTypes, const sp<IMessageCallback>& callbackHandler) override;
    Return<Status> subscribeResponse(uint16_t serviceID, uint16_t operationID, const sp<IResponseCallback>& callbackHandler) override;
    Return<Status> unsubscribe(uint16_t serviceID, uint16_t operationID, const sp<IMessageCallback>& callbackHandler) override;
    Return<void> sendMessage(const Msg& msg) override;

private:
    MessageDispatcher& messageDispatcher_;
};

}  // namespace Connectivity

#endif  // SERVICE_MANAGER_H
