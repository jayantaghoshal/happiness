#include <gtest/gtest.h>
#include <unistd.h>
#include <iostream>
#include <cutils/log.h>
#include <sched.h>
#include <thread>
#include <future>

#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IResponseCallback.h>

#include "ipcb_simulator.h"
#include "ipcommandbus/IpCmdTypes.h"
#include "ipcommandbus/vcc_pdu_header.h"

#define LOG_TAG "VtsIpcbdRetryTest"

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
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IResponseCallback;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::CommandResult;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Error;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::OperationType;
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
class VehicleComClient : public IResponseCallback
{
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
  std::function<void(const Msg &msg)> onResponseRcvdCallback;
  std::function<void(const Error &error)> onErrorRcvdCallback;
};


class VtsIpcbdRetryTest : public ::Test
{
    virtual void SetupTestcase(){
        //Setup services
        ALOGI("SetupTestcase");
        //system(SHELLSCRIPT);
    }

    virtual void SetUp()
    {
        //Setup services
        ALOGI("SetUp");
//        system(SHELLSCRIPT);
    }

    virtual void TearDown()
    {
    }
};

/**

 **/
/*  TODO: Implement once possibility ro remove subscribers exist, until then we can only run one test case...
TEST_F(VtsIpcbdRetryTest, TestNormalFlow){
    system(SHELLSCRIPT);

    ALOGI("TestNormalFlow, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client;

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    //Add subscriber
    CommandResult result;
    ipcb_daemon_->subscribeResponse(0x00AB, 0x0B05, vehicle_com_client,
        [&result](CommandResult cr) { result = cr; });
    ASSERT_TRUE(result.success);

    uint8_t sequenceId_ = 0;

    //Done setting up, start testing!
    ALOGI("TestRetry1, start test");

    //Prepare a message to send
    Msg message;

    message.ecu = Ecu::VCM;

    message.pdu.header.serviceID = 0x00AB;
    message.pdu.header.operationID = 0x0B05;
    message.pdu.header.operationType = OperationType::SETREQUEST;

    message.pdu.header.seqNbr = sequenceId_++;

    ipcb_daemon_->sendMessage(message, {true, 2, 500});

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

    ALOGI("TestRetry1, finished test");
}
*/

/**

 **/
/*  TODO: Implement once possibility ro remove subscribers exist, until then we can only run one test case...
TEST_F(VtsIpcbdRetryTest, TestNoAckRetry){
    system(SHELLSCRIPT);

    ALOGI("TestRetry1, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client;

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    //Add subscriber
    CommandResult result;
    ipcb_daemon_->subscribeResponse(0x00AB, 0x0B05, vehicle_com_client,
        [&result](CommandResult cr) { result = cr; });
    ASSERT_TRUE(result.success);

    uint8_t sequenceId_ = 0;

    //Done setting up, start testing!
    ALOGI("TestRetry1, start test");

    //Prepare a message to send
    Msg message;

    message.ecu = Ecu::VCM;

    message.pdu.header.serviceID = 0x00AB;
    message.pdu.header.operationID = 0x0B05;
    message.pdu.header.operationType = OperationType::SETREQUEST;

    message.pdu.header.seqNbr = sequenceId_++;

    ipcb_daemon_->sendMessage(message, {true, 2, 500});

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

    ALOGI("TestRetry1, finished test");
}
*/

/**

 **/

TEST_F(VtsIpcbdRetryTest, TestNoResponseRetry){
    system(SHELLSCRIPT);

    ALOGI("TestNoResponseRetry, setting up");

    IpcbSimulator tcam_sim("127.0.0.1", 70000, 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    int error_received = 0;
    vehicle_com_client->onErrorRcvdCallback = [&error_received](const Error &error){(void)error; ++error_received;};

    //Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<IVehicleCom> ipcb_daemon_ = IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != NULL);

    //Add subscriber
    CommandResult result;
    ipcb_daemon_->subscribeResponse(0x00AB, 0x0B05, vehicle_com_client,
        [&result](CommandResult cr) { result = cr; });
    ASSERT_TRUE(result.success);

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
    ipcb_daemon_->sendMessage(message, {true, 2, 500});

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