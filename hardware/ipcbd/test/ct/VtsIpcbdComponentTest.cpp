#include <gtest/gtest.h>
#include <unistd.h>
#include <iostream>
#include <cutils/log.h>
#include <sched.h>
#include <thread>
#include <future>

#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IMessageCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IResponseCallback.h>

#include "ipcb_simulator.h"
#include "ipcommandbus/IpCmdTypes.h"
#include "ipcommandbus/vcc_pdu_header.h"

#define LOG_TAG "VtsIpcbdComponentTest"

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IResponseCallback;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Error;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::OperationType;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::CommandResult;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::SubscribeResult;
using ::vendor::volvocars::hardware::common::V1_0::Ecu;

using namespace ::testing;

#define SHELLSCRIPT "\
echo \"Setting env variable\" \n\
export VCC_LOCALCONFIG_PATH=/data/local/tmp/ipcbd/localconfig.json \n\
echo \"Starting IpcbD\" \n\
/vendor/bin/hw/ipcbd ipcb_test UDP & \n\
echo \"Sleeping for 2 sec\" \n\
sleep 2 \n\
"

// Class implementing Methods from ::vendor::volvocars::hardware::vehiclecom::V1_0::IResponseCallback follow.
class VehicleComClient : public IResponseCallback, public IMessageCallback
{
    Return<void> onMessageRcvd(const Msg &msg) override
    {
      if (onMessageRcvdCallback != NULL)
      {
        onMessageRcvdCallback(msg);
      }
      return Void();
    }

  Return<void> onResponseRcvd(const Msg &msg) override
  {
    if (onResponseRcvdCallback != NULL)
    {
        onResponseRcvdCallback(msg);
    }
    return Void();
  }

  Return<void> onErrorRcvd(const Error &error) override
  {
    if (onErrorRcvdCallback != NULL)
    {
        onErrorRcvdCallback(error);
    }
    return Void();
  }

public:
    std::function<void(const Msg &msg)> onMessageRcvdCallback;
    std::function<void(const Msg &msg)> onResponseRcvdCallback;
    std::function<void(const Error &error)> onErrorRcvdCallback;
};


class VtsIpcbdComponentTest : public ::Test
{
protected:
    static void SetUpTestCase(){
        //Setup services
        ALOGI("SetupTestcase");
        system(SHELLSCRIPT);
    }
};


/**

 **/
TEST_F(VtsIpcbdComponentTest, TestSubscribeUnsubscribeRequest){

    ALOGI("TestSubscribeUnsubscribeRequest, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    uint8_t sequence_id = 0;
    uint8_t message_received = 0;

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    // ** Test add subscriber (REQUEST)  ** //

    ALOGI("TestSubscribeUnsubscribeRequest, starting test");

    //Setup a request PDU to send from Ipcb simulator to IpcbD
    Pdu request;
    request.header.service_id = 0x1;
    request.header.operation_id = 0x1;
    request.header.operation_type = Connectivity::IpCmdTypes::OperationType::REQUEST;
    request.header.sender_handle_id = (request.header.service_id & 0xFF) << 24 |
                                      (request.header.operation_id & 0xFF) << 16 |
                                      (static_cast<uint8_t>(request.header.operation_type)) << 8 |
                                      sequence_id++;
    request.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    request.header.protocol_version = 2;

    vehicle_com_client->onMessageRcvdCallback =
    [&message_received, request](const Msg &msg){
        ALOGD("Message received in client (%d, %d, %d)", msg.pdu.header.serviceID, msg.pdu.header.operationID, msg.pdu.header.seqNbr);
        if (msg.pdu.header.serviceID == request.header.service_id &&
            msg.pdu.header.operationID == request.header.operation_id &&
            msg.pdu.header.seqNbr == 0)
        {
                ++message_received;
        }
    };

    SubscribeResult result;
    ipcb_daemon_->subscribe(0x1, 0x1, OperationType::REQUEST, vehicle_com_client, [&result](SubscribeResult sr) { result = sr; });

    EXPECT_TRUE(result.commandResult.success);
    uint64_t subscriber_id = result.subscriberId;

    EXPECT_NE(subscriber_id, 0);

    ALOGD("Send Request to IHU");
    tcam_sim.SendPdu(request);

    //TODO: Wait in a better way for callback handler to be called
    usleep(500000);

    //Read PDU in Ipcb Simulator, expect that we got an ACK on our request
    Pdu read_pdu;
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu));
    EXPECT_TRUE(read_pdu.header.operation_type == Connectivity::IpCmdTypes::OperationType::ACK);

    //Check that message has been received in "VehicleComClient"
    EXPECT_EQ(message_received, 1);

    //Unsubscribe message handler
    CommandResult cresult;
    ipcb_daemon_->unsubscribe(subscriber_id, [&cresult](CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    //Update sequence number (lowest byte of sender_handle_id, so just inc by one)
    request.header.sender_handle_id++;

    //Send another PDU to trigger error case
    tcam_sim.SendPdu(request);

    //TODO: Wait in a better way for callback handler to be called
    usleep(500000);

    //Check that message has NOT been received in "VehicleComClient"
    EXPECT_EQ(message_received, 1);

    //Check that we get an error since we are not handling the request
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu));
    EXPECT_TRUE(read_pdu.header.operation_type == Connectivity::IpCmdTypes::OperationType::ERROR);

    ALOGI("TestSubscribeUnsubscribeRequest, test complete");
}

/**

 **/
TEST_F(VtsIpcbdComponentTest, TestMultipleSubscribeSuccess){

    ALOGI("TestMultipleSubscribeSuccess, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    uint8_t sequence_id = 0;
    uint8_t message_received = 0;

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    vehicle_com_client->onMessageRcvdCallback =
    [&message_received](const Msg &msg){
        ALOGD("Message received in client (%d, %d, %d)", msg.pdu.header.serviceID, msg.pdu.header.operationID, msg.pdu.header.seqNbr);
        ++message_received;
    };

    // ** Test add subscriber (NOTIFICATION)  ** //
    ALOGI("TestMultipleSubscribeSuccess, starting test");

    //Register two subscribers
    SubscribeResult result;
    ipcb_daemon_->subscribe(0x1, 0x1, OperationType::NOTIFICATION, vehicle_com_client, [&result](SubscribeResult sr) { result = sr; });
    EXPECT_TRUE(result.commandResult.success);
    uint64_t subscriber_id_1 = result.subscriberId;

    ipcb_daemon_->subscribe(0x1, 0x1, OperationType::NOTIFICATION, vehicle_com_client, [&result](SubscribeResult sr) { result = sr; });
    EXPECT_TRUE(result.commandResult.success);
    uint64_t subscriber_id_2 = result.subscriberId;

    //Setup a notification PDU to send from Ipcb simulator to IpcbD
    Pdu notification;
    notification.header.service_id = 0x1;
    notification.header.operation_id = 0x1;
    notification.header.operation_type = Connectivity::IpCmdTypes::OperationType::NOTIFICATION;
    notification.header.sender_handle_id = (notification.header.service_id & 0xFF) << 24 |
                                           (notification.header.operation_id & 0xFF) << 16 |
                                           (static_cast<uint8_t>(notification.header.operation_type)) << 8 |
                                           sequence_id++;
    notification.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    notification.header.protocol_version = 2;

    tcam_sim.SendPdu(notification);

    //TODO: Wait in a better way for callback handler to be called
    usleep(500000);

    //Check that message has been received in "VehicleComClient" two times
    EXPECT_EQ(message_received, 2);

    //Unsubscribe message handler
    CommandResult cresult;
    ipcb_daemon_->unsubscribe(subscriber_id_1, [&cresult](CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    ipcb_daemon_->unsubscribe(subscriber_id_2, [&cresult](CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    ALOGI("TestMultipleSubscribeSuccess, test complete");
}

TEST_F(VtsIpcbdComponentTest, TestMultipleSubscribeFail){

    ALOGI("TestMultipleSubscribeFail, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    uint8_t sequence_id = 0;
    uint8_t message_received = 0;

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    vehicle_com_client->onMessageRcvdCallback =
    [&message_received](const Msg &msg){
        ALOGD("Message received in client (%d, %d, %d)", msg.pdu.header.serviceID, msg.pdu.header.operationID, msg.pdu.header.seqNbr);
        ++message_received;
    };

    // ** Test add subscriber (REQUEST)  ** //
    ALOGI("TestMultipleSubscribeFail, starting test");

    //Register two subscribers
    SubscribeResult result;
    ipcb_daemon_->subscribe(0x1, 0x1, OperationType::REQUEST, vehicle_com_client, [&result](SubscribeResult sr) { result = sr; });
    EXPECT_TRUE(result.commandResult.success);
    uint64_t subscriber_id = result.subscriberId;

    ipcb_daemon_->subscribe(0x1, 0x1, OperationType::REQUEST, vehicle_com_client, [&result](SubscribeResult sr) { result = sr; });
    EXPECT_FALSE(result.commandResult.success);

    //Setup a request PDU to send from Ipcb simulator to IpcbD
    Pdu request;
    request.header.service_id = 0x1;
    request.header.operation_id = 0x1;
    request.header.operation_type = Connectivity::IpCmdTypes::OperationType::REQUEST;
    request.header.sender_handle_id = (request.header.service_id & 0xFF) << 24 |
                                      (request.header.operation_id & 0xFF) << 16 |
                                      (static_cast<uint8_t>(request.header.operation_type)) << 8 |
                                      sequence_id++;
    request.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    request.header.protocol_version = 2;

    tcam_sim.SendPdu(request);

    //TODO: Wait in a better way for callback handler to be called
    usleep(500000);

    //Check that message has been received in "VehicleComClient" two times
    EXPECT_EQ(message_received, 1);

    //Unsubscribe message handler
    CommandResult cresult;
    ipcb_daemon_->unsubscribe(subscriber_id, [&cresult](CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    ALOGI("TestMultipleSubscribeFail, test complete");
}

/**

 **/
TEST_F(VtsIpcbdComponentTest, TestRequestResponse){

    ALOGI("TestRequestResponse, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    uint8_t sequence_id = 0;

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    ALOGI("TestRequestResponse, starting test");

    //Prepare a message to send
    Msg message;

    message.ecu = Ecu::VCM;

    message.pdu.header.serviceID = 0x1;
    message.pdu.header.operationID = 0x1;
    message.pdu.header.operationType = OperationType::SETREQUEST;

    message.pdu.header.seqNbr = sequence_id++;

    //Send message from IpcbD
    ALOGI("TestRequestResponse, Send message");
    CommandResult cresult;
    ipcb_daemon_->sendRequest(message, {false, 0, 0}, vehicle_com_client, [&cresult](CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    //Read PDU in Ipcb Simulator, expect that we got an ACK on our request
    Pdu read_pdu;
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu));
    EXPECT_TRUE(read_pdu.header.operation_type == Connectivity::IpCmdTypes::OperationType::SETREQUEST);

    //Setup a response PDU to send from Ipcb simulator to IpcbD
    Pdu response;
    response.header.service_id = 0x1;
    response.header.operation_id = 0x1;
    response.header.operation_type = Connectivity::IpCmdTypes::OperationType::RESPONSE;
    response.header.sender_handle_id = read_pdu.header.sender_handle_id;
    response.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    response.header.protocol_version = 2;

    uint8_t response_received = 0;
    vehicle_com_client->onResponseRcvdCallback =
    [&response_received, response](const Msg &msg){
        ALOGD("Response received in client (%d, %d, %d)", msg.pdu.header.serviceID, msg.pdu.header.operationID, msg.pdu.header.seqNbr);
        if (msg.pdu.header.serviceID == response.header.service_id &&
            msg.pdu.header.operationID == response.header.operation_id &&
            msg.pdu.header.seqNbr == 0)
        {
                ++response_received;
        }
    };

    ALOGD("Send Response to IHU");
    tcam_sim.SendPdu(response);

    //TODO: Wait in a better way for callback handler to be called
    usleep(500000);

    //Check that response has been received in "VehicleComClient"
    EXPECT_EQ(response_received, 1);

    ALOGI("TestSubscribeUnsubscribeRequest, test complete");
}

/**

 **/
/*
TEST_F(VtsIpcbdComponentTest, TestNoAckRetry){
    system(SHELLSCRIPT);

    ALOGI("TestNoAckRetry, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client;

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    uint8_t sequenceId_ = 0;

    //Done setting up, start testing!
    ALOGI("TestNoAckRetry, start test");

    //Prepare a message to send
    Msg message;

    message.ecu = Ecu::VCM;

    message.pdu.header.serviceID = 0x00AB;
    message.pdu.header.operationID = 0x0B05;
    message.pdu.header.operationType = OperationType::SETREQUEST;

    message.pdu.header.seqNbr = sequenceId_++;

    ipcb_daemon_->sendRequest(message, {true, 2, 500}, vehicle_com_client);

    Pdu read_pdu;
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu));

    //Measure time until resend is received
    auto start = std::chrono::steady_clock::now();

    std::future<bool> f_pdu_received = std::async([&tcam_sim, &read_pdu](){return tcam_sim.ReceivePdu(read_pdu);});

    std::chrono::milliseconds span(1000);

    if (f_pdu_received.wait_for(span) == std::future_status::timeout)
        ASSERT_TRUE(false);
    EXPECT_TRUE(f_pdu_received.get());
    std::chrono::milliseconds duration = std::chrono::duration_cast< std::chrono::milliseconds> (std::chrono::steady_clock::now() - start);
    EXPECT_TRUE(std::abs(duration.count() - 500) < 100); //Less than 100 ms diff in resend time

    ALOGI("TestNoAckRetry, finished test");
}
*/

/**

 **/
TEST_F(VtsIpcbdComponentTest, TestNoResponseRetry){

    ALOGI("TestNoResponseRetry, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    int error_received = 0;
    vehicle_com_client->onErrorRcvdCallback = [&error_received](const Error &error){(void)error; ++error_received;};

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    uint8_t sequenceId_ = 0;

    //Done setting up, start testing!
    ALOGI("TestNoResponseRetry, start test");

    //Prepare a message to send
    Msg message;

    message.ecu = Ecu::VCM;

    message.pdu.header.serviceID = 0x00AB;
    message.pdu.header.operationID = 0x0B05;
    message.pdu.header.operationType = OperationType::SETREQUEST;

    message.pdu.header.seqNbr = sequenceId_++;

    //Send message from IpcbD
    ALOGI("TestNoResponseRetry, Send message");
    CommandResult cresult;
    ipcb_daemon_->sendRequest(message, {true, 2, 500}, vehicle_com_client, [&cresult](CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    //Receive message from IpcbD
    Pdu read_pdu;
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu));
    ALOGI("TestNoResponseRetry, PDU #1 received in simulator");

    //Measure time until resend is received
    auto start = std::chrono::steady_clock::now();

    //Send ack, so that IpcbD expects response next
    Pdu ack;
    ack.header.service_id = 0x00AB;
    ack.header.operation_id = 0x0B05;
    ack.header.operation_type = Connectivity::IpCmdTypes::OperationType::ACK;
    ack.header.sender_handle_id = read_pdu.header.sender_handle_id;
    ack.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    ack.header.protocol_version = 2;
    ALOGI("TestNoResponseRetry, Sending ACK from simulator");
    tcam_sim.SendPdu(ack);

    {
        //Wait for resend 2s
        std::future<bool> f_pdu_received = std::async([&tcam_sim, &read_pdu](){return tcam_sim.ReceivePdu(read_pdu);});

        if (f_pdu_received.wait_for(std::chrono::milliseconds(2000)) == std::future_status::timeout)
            ASSERT_TRUE(false);
        EXPECT_TRUE(f_pdu_received.get());
        std::chrono::milliseconds duration = std::chrono::duration_cast< std::chrono::milliseconds> (std::chrono::steady_clock::now() - start);
        ALOGI("TestNoResponseRetry, duration = %d", duration.count());
        EXPECT_TRUE(std::abs(duration.count() - 500) < 100); //Less than 100 ms diff in resend time
    }

    ALOGI("TestNoResponseRetry, PDU #2 received in simulator");
    //Send ack, so that IpcbD expects response next
    ALOGI("TestNoResponseRetry, Sending ACK from simulator");

    start = std::chrono::steady_clock::now();
    tcam_sim.SendPdu(ack);

    {
        //Wait for resend 2s
        std::future<bool> f_pdu_received = std::async([&tcam_sim, &read_pdu](){return tcam_sim.ReceivePdu(read_pdu);});

        if (f_pdu_received.wait_for(std::chrono::milliseconds(2000)) == std::future_status::timeout)
            ASSERT_TRUE(false);
        EXPECT_TRUE(f_pdu_received.get());
        std::chrono::milliseconds duration = std::chrono::duration_cast< std::chrono::milliseconds> (std::chrono::steady_clock::now() - start);
        ALOGI("TestNoResponseRetry, duration = %d", duration.count());
        EXPECT_TRUE(std::abs(duration.count() - 750) < 100); //Less than 100 ms diff in resend time
    }

    ALOGI("TestNoResponseRetry, PDU #3 received in simulator");
    //Send ack, so that IpcbD expects response next
    ALOGI("TestNoResponseRetry, Sending ACK from simulator");
    tcam_sim.SendPdu(ack);

    ALOGI("TestNoResponseRetry, Sleep for 3 seconds");
    //Wait for timeout
    sleep(3);

    //Expect error to be recived
    EXPECT_EQ(error_received, 1);

    ALOGI("TestNoResponseRetry, finished test");
}