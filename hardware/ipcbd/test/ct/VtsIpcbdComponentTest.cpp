/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gtest/gtest.h>
#include <sched.h>
#include <unistd.h>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#include <vendor/volvocars/hardware/vehiclecom/1.0/IMessageCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IResponseCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>

#include "ipcommandbus/IpCmdTypes.h"
#include "ipcommandbus/vcc_pdu_header.h"
#include "src/packet_injector.h"

#undef LOG_TAG
#define LOG_TAG "VtsIpcbdComponentTest"
#include <cutils/log.h>

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::vendor::volvocars::hardware::common::V1_0::Ecu;

namespace vehiclecom = ::vendor::volvocars::hardware::vehiclecom::V1_0;
namespace ipcb_testing = vcc::ipcb::testing;

using namespace ::testing;

static pid_t pid = 0;
static bool setupSuccessful = false;

// Class implementing Methods from ::vendor::volvocars::hardware::vehiclecom::V1_0::IResponseCallback follow.
class VehicleComClient : public vehiclecom::IResponseCallback, public vehiclecom::IMessageCallback {
    Return<void> onMessageRcvd(const vehiclecom::Msg& msg) override {
        if (onMessageRcvdCallback != nullptr) {
            onMessageRcvdCallback(msg);
        }
        return Void();
    }

    Return<void> onResponseRcvd(const vehiclecom::Msg& msg) override {
        if (onResponseRcvdCallback != nullptr) {
            onResponseRcvdCallback(msg);
        }
        return Void();
    }

    Return<void> onErrorRcvd(const vehiclecom::Error& error) override {
        if (onErrorRcvdCallback != nullptr) {
            onErrorRcvdCallback(error);
        }
        return Void();
    }

  public:
    std::function<void(const vehiclecom::Msg& msg)> onMessageRcvdCallback;
    std::function<void(const vehiclecom::Msg& msg)> onResponseRcvdCallback;
    std::function<void(const vehiclecom::Error& error)> onErrorRcvdCallback;
};

class VtsIpcbdComponentTest : public ::Test {
  protected:
    static void SetUpTestCase() {
        ALOGD("+ SetUpTestCase ");

        // Expected that all tests would be aborted here, but that is not the case.
        // For now, abort all tests manually
        ASSERT_TRUE(fileExists("/data/local/tmp/localconfig.json"));

        system("ifconfig lo:0 127.0.0.2 up");

        setupSuccessful = true;

        ALOGD("- SetUpTestCase ");
    }

    static bool processExists(int pid) {
        // Calling kill with signal 0 will just return 0 if prcess is running
        return (0 == kill(pid, 0));
    }

    static bool fileExists(const std::string& name) {
        if (FILE* file = fopen(name.c_str(), "re")) {
            fclose(file);
            return true;
        } else {
            return false;
        }
    }

    static std::string getCmdOut(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            ALOGE("popen() FAILED");
        }
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
                result += buffer.data();
                break;  // break it since we just interested to get process id
            }
        }
        return result;
    }

    static void TearDownTestCase() {
        ALOGD("+ TearDownTestCase");

        system("ifconfig lo:0 down");

        ALOGD("- TearDownTestCase");
    }

    void SetUp() override {
        ALOGD("+ SetUp ");

        // run a process and create a streambuf that reads its stdout and stderr
        std::string pid_str = getCmdOut(
                "VCC_LOCALCONFIG_PATH=/data/local/tmp/localconfig.json "
                "/vendor/bin/ipcbd ipcb_test UDP & "
                " echo $!");
        std::string::size_type sz;  // alias of size_t
        pid = std::stoi(pid_str, &sz);

        uint8_t count = 0;
        while (nullptr == vehiclecom::IVehicleCom::tryGetService("ipcb_test").get()) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);

            if (!processExists(pid)) {
                ASSERT_TRUE(false) << "PID lost while waiting for service to be registered";
            }

            if (20 == ++count) {
                ASSERT_TRUE(false) << "Timed out while waiting for service to be registered";
            }
        }

        ALOGD("Process int id: %u", pid);

        // Make sure Test Case Setup was executed correctly
        ASSERT_TRUE(setupSuccessful) << "Setup Test Case failed, failing test";

        // Make sure IpcbD is still running
        ASSERT_TRUE(processExists(pid)) << "IpcbD is not running anymore, did it crash?";

        ALOGD("- SetUp ");
    }

    void TearDown() override {
        ALOGD("+ TearDown");
        int result = -1;
        if (pid != 0) {
            result = kill(pid, SIGTERM);
            ALOGD("Result of killing process: %d", result);
        }
        ALOGD("- TearDown");
    }
};

/**
 Test that subscribe and unsubscribe of a request works
 **/
TEST_F(VtsIpcbdComponentTest, TestSubscribeUnsubscribeRequest) {
    ALOGI("TestSubscribeUnsubscribeRequest, setting up");

    ipcb_testing::PacketInjector tcam_sim("127.0.0.2", 60000, "127.0.0.1", 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    uint8_t sequence_id = 0;
    uint8_t message_received = 0;

    // Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<vehiclecom::IVehicleCom> ipcb_daemon_ = vehiclecom::IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != nullptr);

    // ** Test add subscriber (REQUEST)  ** //

    ALOGI("TestSubscribeUnsubscribeRequest, starting test");

    // Setup a request PDU to send from Ipcb simulator to IpcbD
    Connectivity::Pdu request;
    request.header.service_id = 0x1;
    request.header.operation_id = 0x1;
    request.header.operation_type = Connectivity::IpCmdTypes::OperationType::REQUEST;
    request.header.sender_handle_id = (request.header.service_id & 0xFF) << 24 |
                                      (request.header.operation_id & 0xFF) << 16 |
                                      (static_cast<uint8_t>(request.header.operation_type)) << 8 | sequence_id++;
    request.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    request.header.protocol_version = 3;

    std::promise<void> p_msg_cb_triggered;
    std::future<void> f_msg_cb_triggered = p_msg_cb_triggered.get_future();
    vehicle_com_client->onMessageRcvdCallback =
            [&message_received, request, &p_msg_cb_triggered](const vehiclecom::Msg& msg) {
                ALOGD("Message received in client (%d, %d, %d)",
                      msg.pdu.header.serviceID,
                      msg.pdu.header.operationID,
                      msg.pdu.header.seqNbr);
                if (msg.pdu.header.serviceID == request.header.service_id &&
                    msg.pdu.header.operationID == request.header.operation_id && msg.pdu.header.seqNbr == 0) {
                    ++message_received;
                    p_msg_cb_triggered.set_value();
                }
            };

    vehiclecom::SubscribeResult result;
    ipcb_daemon_->subscribe(0x1,
                            0x1,
                            vehiclecom::OperationType::REQUEST,
                            vehicle_com_client,
                            [&result](vehiclecom::SubscribeResult sr) { result = sr; });

    EXPECT_TRUE(result.commandResult.success);
    uint64_t subscriber_id = result.subscriberId;

    EXPECT_NE(subscriber_id, 0u);

    ALOGD("Send Request to IHU");
    ASSERT_TRUE(tcam_sim.SendPdu(request));

    std::future_status status = f_msg_cb_triggered.wait_for(std::chrono::seconds(3));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ASSERT_TRUE(false) << "Message callback not triggered! (Timeout or deferred)";
    }

    // Check that message has been received in "VehicleComClient"
    EXPECT_EQ(message_received, 1);

    // Read PDU in Ipcb Simulator, expect that we got an ACK on our request
    Connectivity::Pdu read_pdu;
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu, 10));
    EXPECT_TRUE(read_pdu.header.operation_type == Connectivity::IpCmdTypes::OperationType::ACK);

    // Unsubscribe message handler
    vehiclecom::CommandResult cresult;
    ipcb_daemon_->unsubscribe(subscriber_id, [&cresult](vehiclecom::CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    // Update sequence number (lowest byte of sender_handle_id, so just inc by one)
    request.header.sender_handle_id++;

    // Send another PDU to trigger error case
    tcam_sim.SendPdu(request);

    // Wait a little while for the Pdu to be sent, even though we do not expect to get it...
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);

    // Check that message has NOT been received in "VehicleComClient"
    EXPECT_EQ(message_received, 1);

    // Check that we get an error since we are not handling the request
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu, 10));
    EXPECT_TRUE(read_pdu.header.operation_type == Connectivity::IpCmdTypes::OperationType::ERROR);

    ALOGI("TestSubscribeUnsubscribeRequest, test complete");
}

/**
 Test that multiple subscribers to a notification is ok
 **/
TEST_F(VtsIpcbdComponentTest, TestMultipleSubscribeSuccess) {
    ALOGI("TestMultipleSubscribeSuccess, setting up");

    ipcb_testing::PacketInjector tcam_sim("127.0.0.2", 60000, "127.0.0.1", 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    uint8_t sequence_id = 0;
    uint8_t message_received = 0;

    // Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<vehiclecom::IVehicleCom> ipcb_daemon_ = vehiclecom::IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != nullptr);

    std::promise<void> p_msg_cb_triggered;
    std::future<void> f_msg_cb_triggered = p_msg_cb_triggered.get_future();
    vehicle_com_client->onMessageRcvdCallback = [&message_received, &p_msg_cb_triggered](const vehiclecom::Msg& msg) {
        ALOGD("Message received in client (%d, %d, %d)",
              msg.pdu.header.serviceID,
              msg.pdu.header.operationID,
              msg.pdu.header.seqNbr);
        ++message_received;
        if (2 == message_received) {
            p_msg_cb_triggered.set_value();
        }
    };

    // ** Test add subscriber (NOTIFICATION)  ** //
    ALOGI("TestMultipleSubscribeSuccess, starting test");

    // Register two subscribers
    vehiclecom::SubscribeResult result;
    ipcb_daemon_->subscribe(0x1,
                            0x1,
                            vehiclecom::OperationType::NOTIFICATION,
                            vehicle_com_client,
                            [&result](vehiclecom::SubscribeResult sr) { result = sr; });
    EXPECT_TRUE(result.commandResult.success);
    uint64_t subscriber_id_1 = result.subscriberId;

    ipcb_daemon_->subscribe(0x1,
                            0x1,
                            vehiclecom::OperationType::NOTIFICATION,
                            vehicle_com_client,
                            [&result](vehiclecom::SubscribeResult sr) { result = sr; });
    EXPECT_TRUE(result.commandResult.success);
    uint64_t subscriber_id_2 = result.subscriberId;

    // Setup a notification PDU to send from Ipcb simulator to IpcbD
    Connectivity::Pdu notification;
    notification.header.service_id = 0x1;
    notification.header.operation_id = 0x1;
    notification.header.operation_type = Connectivity::IpCmdTypes::OperationType::NOTIFICATION;
    notification.header.sender_handle_id =
            (notification.header.service_id & 0xFF) << 24 | (notification.header.operation_id & 0xFF) << 16 |
            (static_cast<uint8_t>(notification.header.operation_type)) << 8 | sequence_id++;
    notification.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    notification.header.protocol_version = 3;

    tcam_sim.SendPdu(notification);

    std::future_status status = f_msg_cb_triggered.wait_for(std::chrono::seconds(1));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ASSERT_TRUE(false) << "Message callback not triggered twice! (Timeout or deferred)";
    }

    // Check that message has been received in "VehicleComClient" two times
    EXPECT_EQ(message_received, 2);

    // Unsubscribe message handler
    vehiclecom::CommandResult cresult;
    ipcb_daemon_->unsubscribe(subscriber_id_1, [&cresult](vehiclecom::CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    ipcb_daemon_->unsubscribe(subscriber_id_2, [&cresult](vehiclecom::CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    ALOGI("TestMultipleSubscribeSuccess, test complete");
}

/**
 Test that multiple subscribers to a request fails
 **/
TEST_F(VtsIpcbdComponentTest, TestMultipleSubscribeFail) {
    ALOGI("TestMultipleSubscribeFail, setting up");

    ipcb_testing::PacketInjector tcam_sim("127.0.0.2", 60000, "127.0.0.1", 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    uint8_t sequence_id = 0;
    uint8_t message_received = 0;

    // Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<vehiclecom::IVehicleCom> ipcb_daemon_ = vehiclecom::IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != nullptr);

    std::promise<void> p_msg_cb_triggered;
    std::future<void> f_msg_cb_triggered = p_msg_cb_triggered.get_future();
    vehicle_com_client->onMessageRcvdCallback = [&message_received, &p_msg_cb_triggered](const vehiclecom::Msg& msg) {
        ALOGD("Message received in client (%d, %d, %d)",
              msg.pdu.header.serviceID,
              msg.pdu.header.operationID,
              msg.pdu.header.seqNbr);
        ++message_received;
        p_msg_cb_triggered.set_value();
    };

    // ** Test add subscriber (REQUEST)  ** //
    ALOGI("TestMultipleSubscribeFail, starting test");

    // Register two subscribers
    vehiclecom::SubscribeResult result;
    ipcb_daemon_->subscribe(0x1,
                            0x1,
                            vehiclecom::OperationType::REQUEST,
                            vehicle_com_client,
                            [&result](vehiclecom::SubscribeResult sr) { result = sr; });
    ASSERT_TRUE(result.commandResult.success);
    uint64_t subscriber_id = result.subscriberId;

    ipcb_daemon_->subscribe(0x1,
                            0x1,
                            vehiclecom::OperationType::REQUEST,
                            vehicle_com_client,
                            [&result](vehiclecom::SubscribeResult sr) { result = sr; });
    ASSERT_FALSE(result.commandResult.success);

    // Setup a request PDU to send from Ipcb simulator to IpcbD
    Connectivity::Pdu request;
    request.header.service_id = 0x1;
    request.header.operation_id = 0x1;
    request.header.operation_type = Connectivity::IpCmdTypes::OperationType::REQUEST;
    request.header.sender_handle_id = (request.header.service_id & 0xFF) << 24 |
                                      (request.header.operation_id & 0xFF) << 16 |
                                      (static_cast<uint8_t>(request.header.operation_type)) << 8 | sequence_id++;
    request.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    request.header.protocol_version = 3;

    tcam_sim.SendPdu(request);

    std::future_status status = f_msg_cb_triggered.wait_for(std::chrono::seconds(1));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ASSERT_TRUE(false) << "Message callback not triggered! (Timeout or deferred)";
    }

    // Check that message has been received in "VehicleComClient" two times
    EXPECT_EQ(message_received, 1);

    // Unsubscribe message handler
    vehiclecom::CommandResult cresult;
    ipcb_daemon_->unsubscribe(subscriber_id, [&cresult](vehiclecom::CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    ALOGI("TestMultipleSubscribeFail, test complete");
}

/**
 Test that we can receive a response to a request
 **/
TEST_F(VtsIpcbdComponentTest, TestRequestResponse) {
    ALOGI("TestRequestResponse, setting up");

    ipcb_testing::PacketInjector tcam_sim("127.0.0.2", 60000, "127.0.0.1", 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    uint8_t sequence_id = 0;

    // Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<vehiclecom::IVehicleCom> ipcb_daemon_ = vehiclecom::IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != nullptr);

    ALOGI("TestRequestResponse, starting test");

    // Prepare a message to send
    vehiclecom::Msg message;

    message.ecu = Ecu::TCAM;

    message.pdu.header.serviceID = 0x1;
    message.pdu.header.operationID = 0x1;
    message.pdu.header.operationType = vehiclecom::OperationType::SETREQUEST;

    message.pdu.header.seqNbr = sequence_id++;

    // Send message from IpcbD
    ALOGI("TestRequestResponse, Send message");
    vehiclecom::CommandResult cresult;
    ipcb_daemon_->sendRequest(
            message, {false, 0, 0}, vehicle_com_client, [&cresult](vehiclecom::CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    // Read PDU in Ipcb Simulator, expect that we got an ACK on our request
    Connectivity::Pdu read_pdu;
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu, 10));
    EXPECT_TRUE(read_pdu.header.operation_type == Connectivity::IpCmdTypes::OperationType::SETREQUEST);

    // Setup a response PDU to send from Ipcb simulator to IpcbD
    Connectivity::Pdu response;
    response.header.service_id = 0x1;
    response.header.operation_id = 0x1;
    response.header.operation_type = Connectivity::IpCmdTypes::OperationType::RESPONSE;
    response.header.sender_handle_id = read_pdu.header.sender_handle_id;
    response.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    response.header.protocol_version = 3;

    uint8_t response_received = 0;
    std::promise<void> p_msg_cb_triggered;
    std::future<void> f_msg_cb_triggered = p_msg_cb_triggered.get_future();
    vehicle_com_client->onResponseRcvdCallback =
            [&response_received, response, &p_msg_cb_triggered](const vehiclecom::Msg& msg) {
                ALOGD("Response received in client (%d, %d, %d)",
                      msg.pdu.header.serviceID,
                      msg.pdu.header.operationID,
                      msg.pdu.header.seqNbr);
                if (msg.pdu.header.serviceID == response.header.service_id &&
                    msg.pdu.header.operationID == response.header.operation_id && msg.pdu.header.seqNbr == 0) {
                    ++response_received;
                    p_msg_cb_triggered.set_value();
                }
            };

    ALOGD("Send Response to IHU");
    tcam_sim.SendPdu(response);

    std::future_status status = f_msg_cb_triggered.wait_for(std::chrono::seconds(1));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ASSERT_TRUE(false) << "Message callback not triggered! (Timeout or deferred)";
    }

    // Check that response has been received in "VehicleComClient"
    EXPECT_EQ(response_received, 1);

    ALOGI("TestRequestResponse, test complete");
}

/**
  Test retry handling when we don't receive any ACK
 **/
TEST_F(VtsIpcbdComponentTest, TestNoAckRetry) {
    ALOGI("TestNoAckRetry, setting up");

    ipcb_testing::PacketInjector tcam_sim("127.0.0.2", 60000, "127.0.0.1", 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    std::promise<void> p_error_cb_triggered;
    std::future<void> f_error_cb_triggered = p_error_cb_triggered.get_future();
    int error_received = 0;
    vehicle_com_client->onErrorRcvdCallback = [&error_received, &p_error_cb_triggered](const vehiclecom::Error& error) {
        (void)error;
        ++error_received;
        p_error_cb_triggered.set_value();
    };

    // Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<vehiclecom::IVehicleCom> ipcb_daemon_ = vehiclecom::IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != nullptr);

    uint8_t sequenceId_ = 0;

    // Done setting up, start testing!
    ALOGI("TestNoAckRetry, start test");

    // Prepare a message to send
    vehiclecom::Msg message;

    message.ecu = Ecu::TCAM;

    message.pdu.header.serviceID = 0x00AB;
    message.pdu.header.operationID = 0x0B05;
    message.pdu.header.operationType = vehiclecom::OperationType::SETREQUEST;

    message.pdu.header.seqNbr = sequenceId_++;

    vehiclecom::CommandResult cresult;
    ipcb_daemon_->sendRequest(
            message, {false, 0, 0}, vehicle_com_client, [&cresult](vehiclecom::CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    Connectivity::Pdu read_pdu;
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu, 10));

    // Measure time until resend #1 is received
    {
        auto start = std::chrono::steady_clock::now();

        std::future<bool> f_pdu_received =
                std::async([&tcam_sim, &read_pdu]() { return tcam_sim.ReceivePdu(read_pdu); });

        std::chrono::milliseconds span(1000);

        if (f_pdu_received.wait_for(span) == std::future_status::timeout) ASSERT_TRUE(false);
        EXPECT_TRUE(f_pdu_received.get());
        std::chrono::milliseconds duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        EXPECT_TRUE(std::abs(duration.count() - 500) < 100);  // Less than 100 ms diff in resend time
    }

    // Measure time until resend #2 is received
    {
        auto start = std::chrono::steady_clock::now();

        std::future<bool> f_pdu_received =
                std::async([&tcam_sim, &read_pdu]() { return tcam_sim.ReceivePdu(read_pdu); });

        std::chrono::milliseconds span(1000);

        if (f_pdu_received.wait_for(span) == std::future_status::timeout) ASSERT_TRUE(false);
        EXPECT_TRUE(f_pdu_received.get());
        std::chrono::milliseconds duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        EXPECT_TRUE(std::abs(duration.count() - 750) < 100);  // Less than 100 ms diff in resend time
    }

    // Measure time until resend #3 is received
    {
        auto start = std::chrono::steady_clock::now();

        std::future<bool> f_pdu_received =
                std::async([&tcam_sim, &read_pdu]() { return tcam_sim.ReceivePdu(read_pdu); });

        std::chrono::milliseconds span(1500);

        if (f_pdu_received.wait_for(span) == std::future_status::timeout) ASSERT_TRUE(false);
        EXPECT_TRUE(f_pdu_received.get());
        std::chrono::milliseconds duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        EXPECT_TRUE(std::abs(duration.count() - 1125) < 100);  // Less than 100 ms diff in resend time
    }

    // Wait for error callback for a maximum of 3 seconds
    std::future_status status = f_error_cb_triggered.wait_for(std::chrono::seconds(3));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ASSERT_TRUE(false) << "Error callback not triggered! (Timeout or deferred)";
    }

    // Expect error to be recived
    EXPECT_EQ(error_received, 1);

    ALOGI("TestNoAckRetry, finished test");
}

/**
 Test retry handling when we don't receive any response, with custom retry info
 **/
TEST_F(VtsIpcbdComponentTest, TestNoResponseRetry) {
    ALOGI("TestNoResponseRetry, setting up");

    ipcb_testing::PacketInjector tcam_sim("127.0.0.2", 60000, "127.0.0.1", 50010, 0);

    sp<VehicleComClient> vehicle_com_client = new VehicleComClient();

    std::promise<void> p_error_cb_triggered;
    std::future<void> f_error_cb_triggered = p_error_cb_triggered.get_future();
    int error_received = 0;
    vehicle_com_client->onErrorRcvdCallback = [&error_received, &p_error_cb_triggered](const vehiclecom::Error& error) {
        (void)error;
        ++error_received;
        p_error_cb_triggered.set_value();
    };

    // Connect to IpcbD service
    ALOGD("Connect to service!");
    sp<vehiclecom::IVehicleCom> ipcb_daemon_ = vehiclecom::IVehicleCom::getService("ipcb_test");
    ASSERT_TRUE(ipcb_daemon_ != nullptr);

    uint8_t sequenceId_ = 0;

    // Done setting up, start testing!
    ALOGI("TestNoResponseRetry, start test");

    // Prepare a message to send
    vehiclecom::Msg message;

    message.ecu = Ecu::TCAM;

    message.pdu.header.serviceID = 0x00AB;
    message.pdu.header.operationID = 0x0B05;
    message.pdu.header.operationType = vehiclecom::OperationType::SETREQUEST;

    message.pdu.header.seqNbr = sequenceId_++;

    // Send message from IpcbD
    ALOGI("TestNoResponseRetry, Send message");
    vehiclecom::CommandResult cresult;
    ipcb_daemon_->sendRequest(
            message, {true, 2, 500}, vehicle_com_client, [&cresult](vehiclecom::CommandResult cr) { cresult = cr; });
    EXPECT_TRUE(cresult.success);

    // Receive message from IpcbD
    Connectivity::Pdu read_pdu;
    EXPECT_TRUE(tcam_sim.ReceivePdu(read_pdu, 10));
    ALOGI("TestNoResponseRetry, PDU #1 received in simulator");

    // Measure time until resend is received
    auto start = std::chrono::steady_clock::now();

    // Send ack, so that IpcbD expects response next
    Connectivity::Pdu ack;
    ack.header.service_id = 0x00AB;
    ack.header.operation_id = 0x0B05;
    ack.header.operation_type = Connectivity::IpCmdTypes::OperationType::ACK;
    ack.header.sender_handle_id = read_pdu.header.sender_handle_id;
    ack.header.length = Connectivity::VCCPDUHeader::DATA_SIZE - 8;
    ack.header.protocol_version = 3;
    ALOGI("TestNoResponseRetry, Sending ACK from simulator");
    tcam_sim.SendPdu(ack);

    {
        // Wait for resend 2s
        std::future<bool> f_pdu_received =
                std::async([&tcam_sim, &read_pdu]() { return tcam_sim.ReceivePdu(read_pdu); });

        if (f_pdu_received.wait_for(std::chrono::milliseconds(2000)) == std::future_status::timeout) ASSERT_TRUE(false);
        EXPECT_TRUE(f_pdu_received.get());
        std::chrono::milliseconds duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        ALOGI("TestNoResponseRetry, duration = %d", (int)duration.count());
        EXPECT_TRUE(std::abs(duration.count() - 500) < 100);  // Less than 100 ms diff in resend time
    }

    ALOGI("TestNoResponseRetry, PDU #2 received in simulator");
    // Send ack, so that IpcbD expects response next
    ALOGI("TestNoResponseRetry, Sending ACK from simulator");

    start = std::chrono::steady_clock::now();
    tcam_sim.SendPdu(ack);

    {
        // Wait for resend 2s
        std::future<bool> f_pdu_received =
                std::async([&tcam_sim, &read_pdu]() { return tcam_sim.ReceivePdu(read_pdu); });

        if (f_pdu_received.wait_for(std::chrono::milliseconds(2000)) == std::future_status::timeout) ASSERT_TRUE(false);
        EXPECT_TRUE(f_pdu_received.get());
        std::chrono::milliseconds duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        ALOGI("TestNoResponseRetry, duration = %d", (int)duration.count());
        EXPECT_TRUE(std::abs(duration.count() - 750) < 100);  // Less than 100 ms diff in resend time
    }

    ALOGI("TestNoResponseRetry, PDU #3 received in simulator");
    // Send ack, so that IpcbD expects response next
    ALOGI("TestNoResponseRetry, Sending ACK from simulator");
    tcam_sim.SendPdu(ack);

    // Wait for error callback for a maximum of 3 seconds
    std::future_status status = f_error_cb_triggered.wait_for(std::chrono::seconds(3));
    if (status == std::future_status::deferred || status == std::future_status::timeout) {
        ASSERT_TRUE(false) << "Error callback not triggered! (Timeout or deferred)";
    }

    // Expect error to be recived
    EXPECT_EQ(error_received, 1);

    ALOGI("TestNoResponseRetry, finished test");
}
