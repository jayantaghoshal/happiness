#include <cutils/log.h>
#include <gtest/gtest.h>
#include <sched.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include "ipcb_simulator.h"

#undef LOG_TAG
#define LOG_TAG "IpcbSimulator_Test.main"

/**
 * Test that UPD packets can be sent and received by the IPCB simulator
 *
 * Note! Currently there seems to be some limitation in the network stack
 * preventing us from using the real network interface for UDP, once that is resolved
 * it should be possible for all tests to run in the network namespace
 **/
TEST(IpcbSimulatorTest, TestUDP) {
    ALOGI("Test UDP");

    IpcbSimulator Client("127.0.0.1", 55555, 66666, 0);
    IpcbSimulator Server("127.0.0.1", 66666, 55555, 0);

    ALOGI("TestUDP: 2");

    Pdu send_pdu;
    send_pdu.header.service_id = 1;
    send_pdu.header.operation_id = 1;
    send_pdu.header.protocol_version = 2;
    send_pdu.header.operation_type = Connectivity::IpCmdTypes::OperationType::NOTIFICATION;
    send_pdu.header.data_type = Connectivity::IpCmdTypes::DataType::NOT_ENCODED;
    send_pdu.header.length = VCCPDUHeader::DATA_SIZE - 8;

    ALOGI("TestUDP: 3");

    uint8_t arr[] = {10, 20, 30};
    send_pdu.payload.insert(send_pdu.payload.begin(), arr, arr + 3);
    send_pdu.header.length += send_pdu.payload.size();

    ALOGI("Sending PDU with length %d", send_pdu.header.length);

    EXPECT_TRUE(Client.SendPdu(send_pdu));

    ALOGI("PDU sent, waiting to receive");

    Pdu read_pdu;
    EXPECT_TRUE(Server.ReceivePdu(read_pdu));

    ALOGI("Received PDU with length %d", read_pdu.header.length);

    EXPECT_EQ(read_pdu.header.service_id, send_pdu.header.service_id);
    EXPECT_EQ(read_pdu.header.operation_id, send_pdu.header.operation_id);
    EXPECT_EQ(read_pdu.header.protocol_version, send_pdu.header.protocol_version);
    EXPECT_EQ(read_pdu.header.operation_type, send_pdu.header.operation_type);
    EXPECT_EQ(read_pdu.header.data_type, send_pdu.header.data_type);
    EXPECT_EQ(read_pdu.header.length, send_pdu.header.length);

    EXPECT_EQ(read_pdu.payload.size(), send_pdu.payload.size());
    EXPECT_EQ(read_pdu.payload[0], send_pdu.payload[0]);
    EXPECT_EQ(read_pdu.payload[1], send_pdu.payload[1]);
    EXPECT_EQ(read_pdu.payload[2], send_pdu.payload[2]);

    ALOGI("Test UDP finished");
}

/**
 * Test that UPD Broadcast packets can be sent and received by the IPCB simulator
 *
 * Note! Currently there seems to be some limitation in the network stack
 * preventing us from using the real network interface for UDP, therefore
 * this test is ran in a separate thread that is moved into the network
 * namespace. If that issue is resolved and the entire test can be executed
 * in the namespace, consider simplifying this test to not use an extra thread
 * and 'setns'
 **/
TEST(IpcbSimulatorTest, TestUDPB) {
    ALOGI("Test UDP Broadcast");

    // Execute test in separate thread to localize namespace switch
    std::thread test_thread([&]() {
        int fd = open("var/run/netns/vcc", O_RDONLY);
        if (fd > 0) {
            if (setns(fd, CLONE_NEWNET)) {
                ALOGE("Set NS failed!");
            }
        } else {
            ALOGE("Open NS filedescriptor failed!");
            ASSERT_TRUE(false);
        }

        IpcbSimulator Client("198.18.255.255", 99999, 88888, 1);
        IpcbSimulator Server("198.18.255.255", 88888, 99999, 1);

        Pdu send_pdu;
        send_pdu.header.service_id = 1;
        send_pdu.header.operation_id = 1;
        send_pdu.header.protocol_version = 2;
        send_pdu.header.operation_type = Connectivity::IpCmdTypes::OperationType::NOTIFICATION;
        send_pdu.header.data_type = Connectivity::IpCmdTypes::DataType::NOT_ENCODED;
        send_pdu.header.length = VCCPDUHeader::DATA_SIZE - 8;

        uint8_t arr[] = {10, 20, 30};
        send_pdu.payload.insert(send_pdu.payload.begin(), arr, arr + 3);
        send_pdu.header.length += send_pdu.payload.size();

        ALOGI("Sending PDU with length %d", send_pdu.header.length);

        EXPECT_TRUE(Client.SendPdu(send_pdu));

        Pdu read_pdu;
        EXPECT_TRUE(Server.ReceivePdu(read_pdu));

        ALOGI("Received PDU with length %d", read_pdu.header.length);

        EXPECT_EQ(read_pdu.header.service_id, send_pdu.header.service_id);
        EXPECT_EQ(read_pdu.header.operation_id, send_pdu.header.operation_id);
        EXPECT_EQ(read_pdu.header.protocol_version, send_pdu.header.protocol_version);
        EXPECT_EQ(read_pdu.header.operation_type, send_pdu.header.operation_type);
        EXPECT_EQ(read_pdu.header.data_type, send_pdu.header.data_type);
        EXPECT_EQ(read_pdu.header.length, send_pdu.header.length);

        EXPECT_EQ(read_pdu.payload.size(), send_pdu.payload.size());
        EXPECT_EQ(read_pdu.payload[0], send_pdu.payload[0]);
        EXPECT_EQ(read_pdu.payload[1], send_pdu.payload[1]);
        EXPECT_EQ(read_pdu.payload[2], send_pdu.payload[2]);
    });

    // Wait for test thread to finish
    test_thread.join();

    ALOGI("Test UDP Broadcast finished");
}