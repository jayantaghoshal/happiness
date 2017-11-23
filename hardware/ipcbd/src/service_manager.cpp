#include <cutils/log.h>
#include <set>

#include "service_manager.h"

#undef LOG_TAG
#define LOG_TAG "IpcbD.manager"

using namespace tarmac::eventloop;

using ::vendor::volvocars::hardware::vehiclecom::V1_0::Error;

namespace Connectivity {
ServiceManager::ServiceManager(std::string service_name, ::Connectivity::MessageDispatcher& msgDispatcher)
    : messageDispatcher_{msgDispatcher} {
    android::status_t status = registerAsService(service_name);  // TODO: Need name from main args here!

    if (status != android::OK) {
        ALOGE("Failed to register Ipcb binder service: %d", status);
    } else {
        ALOGI("Ipcb binder service register ok");
    }
}

// Methods from ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom follow.
Return<void> ServiceManager::subscribe(uint16_t serviceID, uint16_t operationID, OperationType operationType,
                                       const sp<IMessageCallback>& callbackHandler, subscribe_cb _hidl_cb) {
    ALOGD("+ Ipcb::subscribeMessage");

    if (operationType == OperationType::RESPONSE || operationType == OperationType::ERROR) {
        ALOGE("- Ipcb::subscribeMessage failed, provided OperationType not allowed (%d)", uint8_t(operationType));
        _hidl_cb({{false, ("Provided OperationType not allowed ") + std::to_string(uint8_t(operationType))}, 0});
        return Void();
    }

    uint64_t subscriberId = 0;
    subscriberId = messageDispatcher_.registerMessageCallback(
            serviceID, operationID, (IpCmdTypes::OperationType)operationType,
            [this, callbackHandler](Message& message, uint64_t& registeredReceiverId) {
                ALOGD("+ Ipcb::registerMessageCallback called");
                Msg msg;
                msg.ecu = (vendor::volvocars::hardware::common::V1_0::Ecu)message.ecu;
                msg.pdu.header.serviceID = message.pdu.header.service_id;
                msg.pdu.header.operationID = message.pdu.header.operation_id;
                msg.pdu.header.operationType = (OperationType)message.pdu.header.operation_type;
                msg.pdu.header.encoded = (IpCmdTypes::DataType::ENCODED == message.pdu.header.data_type);
                msg.pdu.header.seqNbr = uint8_t(message.pdu.header.sender_handle_id & 0xFF);
                msg.pdu.payload = std::move(message.pdu.payload);
                auto result = callbackHandler->onMessageRcvd(msg);
                // We always have to call isOk() eventthough we don't need it
                // https://source.android.com/devices/architecture/hidl-cpp/functions
                result.isOk();
                if (result.isDeadObject()) {
                    ALOGE("Callback function does not exist!");
                    messageDispatcher_.unregisterCallback(registeredReceiverId);
                    return false;
                }
                return true;
            });

    if (0 == subscriberId) {
        ALOGD("- Ipcb::subscribeMessage (FAILED)");
        _hidl_cb({{false, "Could not subscribe, probably a subscriber already exists!"}, subscriberId});
        return Void();
    } else {
        ALOGD("- Ipcb::subscribeMessage (SUCCESS)");
        _hidl_cb({{true, ""}, subscriberId});
        return Void();
    }
}

Return<void> ServiceManager::unsubscribe(uint64_t subscriberId, unsubscribe_cb _hidl_cb) {
    ALOGD("Ipcb::unsubscribe");

    if (!messageDispatcher_.unregisterCallback(subscriberId)) {
        _hidl_cb({false, "Failed to unsubscribe"});
        return Void();
    }

    _hidl_cb({true, ""});
    return Void();
}

Return<void> ServiceManager::sendMessage(const Msg& msg, const RetryInfo& retryInfo, sendMessage_cb _hidl_cb) {
    ALOGD("+ Ipcb::sendMessage");

    if ((OperationType)msg.pdu.header.operationType == OperationType::REQUEST ||
        (OperationType)msg.pdu.header.operationType == OperationType::SETREQUEST ||
        (OperationType)msg.pdu.header.operationType == OperationType::NOTIFICATION_REQUEST) {
        ALOGE("- Ipcb::sendMessage failed, message with OperationType %d cannot be sent, use sendRequest()",
              uint8_t(msg.pdu.header.operationType));
        _hidl_cb({false,
                  std::string("Message with OperationType " + std::to_string(uint8_t(msg.pdu.header.operationType)) +
                              std::string(" cannot be sent, use sendRequest()"))});
        return Void();
    }

    // Prepare header
    Pdu pdu;
    pdu.createHeader(msg.pdu.header.serviceID, msg.pdu.header.operationID,
                     (IpCmdTypes::OperationType)msg.pdu.header.operationType,
                     msg.pdu.header.encoded ? IpCmdTypes::DataType::ENCODED : IpCmdTypes::DataType::NOT_ENCODED,
                     msg.pdu.header.seqNbr);

    // .. and move it to pdu
    pdu.setPayload(std::move(msg.pdu.payload));

    // Create message and set pdu.
    Message message(std::move(pdu));
    message.ecu = (Message::Ecu)msg.ecu;

    message.retry_info = {retryInfo.overrideDefault, retryInfo.maxRetries, retryInfo.retryTimeoutMs};

    ALOGI(" Sending message (%04X.%04X) to %s", message.pdu.header.service_id, message.pdu.header.operation_id,
          Message::EcuStr(message.ecu));

    // Send the request
    std::shared_ptr<MessageDispatcher::CallerData> caller_data;
    caller_data = std::make_shared<MessageDispatcher::CallerData>();

    messageDispatcher_.sendMessage(std::move(message), caller_data);

    ALOGD("- Ipcb::sendMessage");
    _hidl_cb({true, ""});
    return Void();
}

Return<void> ServiceManager::sendRequest(const Msg& msg, const RetryInfo& retryInfo,
                                         const sp<IResponseCallback>& callbackHandler, sendRequest_cb _hidl_cb) {
    ALOGD("+ Ipcb::sendRequest");

    if ((OperationType)msg.pdu.header.operationType != OperationType::REQUEST &&
        (OperationType)msg.pdu.header.operationType != OperationType::SETREQUEST &&
        (OperationType)msg.pdu.header.operationType != OperationType::NOTIFICATION_REQUEST) {
        ALOGE("- Ipcb::sendRequest failed, request with OperationType %d cannot be sent, use sendMessage()",
              uint8_t(msg.pdu.header.operationType));
        _hidl_cb({false,
                  std::string("Request with OperationType " + std::to_string(uint8_t(msg.pdu.header.operationType)) +
                              std::string(" cannot be sent, use sendMessage()"))});
        return Void();
    }

    // Prepare caller data
    std::shared_ptr<MessageDispatcher::CallerData> caller_data = std::make_shared<MessageDispatcher::CallerData>();

    caller_data->responseCallback = [this, callbackHandler, caller_data](Message& message) {
        if (caller_data->errorType == ITransportServices::ErrorType::OK) {
            Msg msg;
            msg.ecu = (vendor::volvocars::hardware::common::V1_0::Ecu)message.ecu;
            msg.pdu.header.serviceID = message.pdu.header.service_id;
            msg.pdu.header.operationID = message.pdu.header.operation_id;
            msg.pdu.header.operationType = (OperationType)message.pdu.header.operation_type;
            msg.pdu.header.encoded = (IpCmdTypes::DataType::ENCODED == message.pdu.header.data_type);
            msg.pdu.header.seqNbr = uint8_t(message.pdu.header.sender_handle_id & 0xFF);
            msg.pdu.payload = std::move(message.pdu.payload);
            auto result = callbackHandler->onResponseRcvd(msg);
            // We always have to call isOk() eventthough we don't need it
            // https://source.android.com/devices/architecture/hidl-cpp/functions
            result.isOk();
            if (result.isDeadObject()) {
                ALOGE("Callback function does not exist!");
                return false;
            }
        } else {
            Error err;
            err.errorCode = caller_data->errorInfo.errorCode;
            err.errorInfo = caller_data->errorInfo.errorInfo;
            auto result = callbackHandler->onErrorRcvd(err);
            // We always have to call isOk() eventthough we don't need it
            // https://source.android.com/devices/architecture/hidl-cpp/functions
            result.isOk();
            if (result.isDeadObject()) {  // TODO: make a better error message here, maube unsubscribe?
                ALOGE("Callback function does not exist!");
                return false;
            }
        }
        return true;
    };

    // Prepare header
    Pdu pdu;
    pdu.createHeader(msg.pdu.header.serviceID, msg.pdu.header.operationID,
                     (IpCmdTypes::OperationType)msg.pdu.header.operationType,
                     msg.pdu.header.encoded ? IpCmdTypes::DataType::ENCODED : IpCmdTypes::DataType::NOT_ENCODED,
                     msg.pdu.header.seqNbr);

    // .. and move it to pdu
    pdu.setPayload(std::move(msg.pdu.payload));

    // Create message and set pdu.
    Message message(std::move(pdu));
    message.ecu = (Message::Ecu)msg.ecu;

    message.retry_info = {retryInfo.overrideDefault, retryInfo.maxRetries, retryInfo.retryTimeoutMs};

    ALOGI(" Sending message (%04X.%04X) to %s", message.pdu.header.service_id, message.pdu.header.operation_id,
          Message::EcuStr(message.ecu));

    // Send the request
    messageDispatcher_.sendMessage(std::move(message), caller_data);

    ALOGD("- Ipcb::sendRequest");
    _hidl_cb({true, ""});
    return Void();
}

}  // namespace Connectivity
