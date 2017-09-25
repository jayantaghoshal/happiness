#define LOG_TAG "IpcbD.manager"

#include <set>
#include <cutils/log.h>

#include "service_manager.h"

using namespace tarmac::eventloop;

using ::vendor::volvocars::hardware::ipcb::V1_0::Error;

namespace Connectivity
{
ServiceManager::ServiceManager(std::string service_name,::Connectivity::MessageDispatcher& msgDispatcher)
    : messageDispatcher_{msgDispatcher}
{
    android::status_t status = registerAsService(service_name); //TODO: Need name from main args here!

     if (status != android::OK) {
        ALOGE("Failed to register Ipcb binder service: %d",status);
    } else {
        ALOGI("Ipcb binder service register ok");
    }
}

// Methods from ::vendor::volvocars::hardware::ipcb::V1_0::IIpcb follow.
Return<Status> ServiceManager::subscribeMessage(uint16_t serviceID, uint16_t operationID, const hidl_vec<OperationType>& operationTypes, const sp<IMessageCallback>& callbackHandler) {
    ALOGD("+ Ipcb::subscribeMessage");

    std::vector<OperationType> opTypes = (std::vector<OperationType>) operationTypes;

    //Fail is Operation Type vector is empty
    if (0 == operationTypes.size())
    {
        ALOGD("- Ipcb::subscribeMessage (FAIL)");
        return ::vendor::volvocars::hardware::ipcb::V1_0::Status::FAIL;
    }

    std::set<OperationType> subscribedTypes = {OperationType::RESPONSE, OperationType::ERROR};  //Do not allow RESPONSE or ERRORS to be added!
    for (auto const& opType: opTypes)
    {
        if (0 == subscribedTypes.count(opType))
        {
            messageDispatcher_.registerMessageCallback(serviceID, operationID, (IpCmdTypes::OperationType) opType,
                [callbackHandler](Message& message){
                    Msg msg;
                    msg.pdu.header.serviceID =  message.pdu.header.service_id;
                    msg.pdu.header.operationID =  message.pdu.header.operation_id;
                    msg.pdu.header.operationType =  (OperationType)message.pdu.header.operation_type;
                    msg.pdu.header.encoded = (IpCmdTypes::DataType::ENCODED == message.pdu.header.data_type);
                    msg.pdu.header.seqNbr = uint8_t(message.pdu.header.sender_handle_id & 0xFF);
                    msg.pdu.payload = std::move(message.pdu.payload);
                    if (callbackHandler->onMessageRcvd(msg).isDeadObject())
                    {  //TODO: make a better error message here, maube unsubscribe?
                        ALOGE("Callback function does not exist!");
                    }
                });
        }
        subscribedTypes.insert(opType);
    }

    ALOGD("- Ipcb::subscribeMessage (SUCCESS)");
    return ::vendor::volvocars::hardware::ipcb::V1_0::Status::SUCCESS;
}

Return<Status> ServiceManager::subscribeResponse(uint16_t serviceID, uint16_t operationID, const sp<IResponseCallback>& callbackHandler) {
    ALOGD("+ Ipcb::subscribeResponse");

    messageDispatcher_.registerResponseCallback(serviceID, operationID,
        [callbackHandler](Message& message, std::shared_ptr<::Connectivity::MessageDispatcher::CallerData> callerData){
            if (callerData->errorType == ITransportServices::ErrorType::OK)
            {
                Msg msg;
                msg.pdu.header.serviceID =  message.pdu.header.service_id;
                msg.pdu.header.operationID =  message.pdu.header.operation_id;
                msg.pdu.header.operationType =  (OperationType)message.pdu.header.operation_type;
                msg.pdu.header.encoded = (IpCmdTypes::DataType::ENCODED == message.pdu.header.data_type);
                msg.pdu.header.seqNbr = uint8_t(message.pdu.header.sender_handle_id & 0xFF);
                msg.pdu.payload = std::move(message.pdu.payload);
                if (callbackHandler->onResponseRcvd(msg).isDeadObject())
                {  //TODO: make a better error message here, maube unsubscribe?
                    ALOGE("Callback function does not exist!");
                }
            }
            else
            {
                Error err;
                err.errorCode = callerData->errorInfo.errorCode;
                err.errorInfo = callerData->errorInfo.errorInfo;
                if (callbackHandler->onErrorRcvd(err).isDeadObject())
                {  //TODO: make a better error message here, maube unsubscribe?
                    ALOGE("Callback function does not exist!");
                }
            }
        });

    ALOGD("- Ipcb::subscribeResponse (SUCCESS)");
    return ::vendor::volvocars::hardware::ipcb::V1_0::Status::SUCCESS;
}

Return<Status> ServiceManager::unsubscribe(uint16_t serviceID, uint16_t operationID, const sp<IMessageCallback>& callbackHandler) {

    ALOGD("+ Ipcb::unsubscribe (NOT IMPLEMENTED!)");

    (void) serviceID;
    (void) operationID;
    (void) callbackHandler;

    // TODO There is no unsubscribe currently, implement it!!

    return ::vendor::volvocars::hardware::ipcb::V1_0::Status {};
}

Return<void> ServiceManager::sendMessage(const Msg& msg) {

    ALOGD("+ Ipcb::sendMessage");
    // Prepare header
    Pdu pdu;
    pdu.createHeader(msg.pdu.header.serviceID,
                     msg.pdu.header.operationID,
                     (IpCmdTypes::OperationType) msg.pdu.header.operationType,
                     msg.pdu.header.encoded ? IpCmdTypes::DataType::ENCODED : IpCmdTypes::DataType::NOT_ENCODED,
                     msg.pdu.header.seqNbr);

    // .. and move it to pdu
    pdu.setPayload(std::move(msg.pdu.payload));

    // Create message and set pdu.
    Message message(std::move(pdu));
    message.ecu = (Message::Ecu)((int)msg.ecu + 1);

    ALOGI(" Sending message (%04X.%04X) to %s",
        message.pdu.header.service_id,
        message.pdu.header.operation_id,
        Message::EcuStr(message.ecu));

    // Send the request
    std::shared_ptr<MessageDispatcher::CallerData> caller_data;
    caller_data = std::make_shared<MessageDispatcher::CallerData>();

    messageDispatcher_.sendMessage(std::move(message), caller_data);

    ALOGD("- Ipcb::sendMessage");
    return Void();
}


}  // namespace Connectivity
