#include "dim_manager.h"
#include <IDispatcher.h>
#include <hidl/HidlTransportSupport.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include "VccIpCmdApi.h"
#include "dim_operation_data.h"

#define LOG_TAG "dim_manager"
#include <cutils/log.h>

using ::vendor::volvocars::hardware::vehiclecom::V1_0::CommandResult;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::OperationType;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;
using ::vendor::volvocars::hardware::common::V1_0::Ecu;
using ::android::sp;
using namespace Connectivity;
using namespace tarmac::eventloop;
using namespace android::hardware;

namespace vcc {
namespace dim {

bool DimManager::SetVehicleCom() {
    vehicle_com_server_ = IVehicleCom::getService(service_name_);
    if (vehicle_com_server_ != nullptr) {
        ALOGD("DimManager::SetVehicleCom: IpcbD found and successfully connected");
        return true;
    }
    ALOGE("DimManager::SetVehicleCom: Error: IpcbD returned null!");
    return false;
}

bool DimManager::SetDimVideoStreamMode(DimTbtArea::TypeActivationIdentifier id) {
    if (vehicle_com_server_ != nullptr) {
        DimTbtArea dimTbtArea(std::make_unique<DimOperationData>());
        Msg message;
        message.ecu = Ecu::DIM;
        message.pdu.header.serviceID = static_cast<uint16_t>(VccIpCmd::ServiceId::InfotainmentPlatform);
        message.pdu.header.operationID = static_cast<uint16_t>(VccIpCmd::OperationId::DIMTbTArea);
        message.pdu.header.operationType = OperationType::SETREQUEST_NORETURN;
        message.pdu.header.encoded = false;
        message.pdu.header.seqNbr = ++sequence_id_;

        dimTbtArea.SetTypeActivationIdentifier(id);
        std::vector<uint8_t> data_;
        dimTbtArea.GetData(data_);
        message.pdu.payload = data_;
        ALOGD("Attempting to send TbtArea(=1) message");
        CommandResult result;
        vehicle_com_server_->sendMessage(message, {false, 0, 0}, [&result](CommandResult sr) { result = sr; });
        if (!result.success) {
            ALOGE("Send Message failed with error: %s", result.errMsg.c_str());
            return false;
        }
        return true;
    }
    ALOGE("vehicle_com_server_ is NULL, cannot not send message");
    return false;
}

::android::hardware::Return<void> DimManager::onRegistration(const ::android::hardware::hidl_string& fqName,
                                                             const ::android::hardware::hidl_string& name,
                                                             bool preexisting) {
    ALOGD("IServiceNotification::OnRegistration called! %s, %s", fqName.c_str(), name.c_str());
    (void)preexisting;

    if (name != service_name_) {
        ALOGE("Received notification from an incorrect service");
        return ::android::hardware::Return<void>();
    }

    if (!SetVehicleCom()) {
        // Return if vehicle_com_server_ cant be retrieved
        return ::android::hardware::Return<void>();
    }

    hidl_death_recipient* this_as_recipient = this;
    vehicle_com_server_->linkToDeath(this_as_recipient, 0);

    if (!SetDimVideoStreamMode(DimTbtArea::TypeActivationIdentifier::ACTIVE)) {
        ExitDimManager();
    }

    return ::android::hardware::Return<void>();
}

void DimManager::serviceDied(uint64_t cookie, const android::wp<IBase>& who) {
    (void)cookie;  // TODO Use cookie to validate callback?
    (void)who;     // TODO what is the state of "who" after its death? perhaps use to validate callback?
    ALOGD("DimService got killed, forcing dimmanager to stop");
    ExitDimManager();
}

void DimManager::ExitDimManager() {
    ALOGD("ExitDimManager was called");
    // Delegate shutdown to SIGTERM handler.
    kill(getpid(), SIGTERM);
    // Halt execution and wait for TERM-handler to do its work.
    pause();
}

void DimManager::Start() {
    ALOGD("Dim daemon starting ... ");
    IVehicleCom::registerForNotifications(service_name_, this);
}

}  // namespace dim
}  // namespace vcc
