#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cutils/log.h>
#include <stdexcept>
#include "include/ipcb_simulator.h"

#define LOG_TAG "ipcb_simulator"

#define SERVER_BCAST_IP "198.18.255.255"
#define SERVER_ADDR "127.0.0.1"

using ::tarmac::eventloop::IDispatcher;



IpcbSimulator::IpcbSimulator(uint32_t dst_port, uint32_t src_port, int bcast_enable) :
DST_PORT(dst_port),SRC_PORT(src_port),broadcastEnable(bcast_enable),timer{IDispatcher::GetDefaultDispatcher()}
{
    if(broadcastEnable)
        SEND_ADDR = SERVER_BCAST_IP;
    else
        SEND_ADDR = SERVER_ADDR;

    ALOGD("Packet Injector started");
    IpcbSimulator::setup();
}

void IpcbSimulator::setup()
{
    //Setup local socket socket
    if ( (local_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ALOGD("Failed to create local socket...die now!");
        exit(1);
    }


    int ret=setsockopt(local_socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(DST_PORT);
    if (inet_aton(SEND_ADDR.c_str() , &sa.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    //Bind local socket to local port
    memset(&srcaddr, 0, sizeof(srcaddr));
    srcaddr.sin_family = AF_INET;
    srcaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srcaddr.sin_port = htons(SRC_PORT);


    if (bind(local_socket, (struct sockaddr *) &srcaddr, sizeof(srcaddr)) < 0) {
        ALOGD("bind error...die now!");
        perror("bind error");
        exit(1);
    }
}


void IpcbSimulator::CreateAndSendIpActivityMessage()
{
    Pdu pdu;
    ALOGD("Send Activity Message");
    pdu.createHeader(0xFFFF,0xFF01,
        IpCmdTypes::OperationType::NOTIFICATION_CYCLIC,IpCmdTypes::DataType::NOT_ENCODED,
        sequenceId_);

    pdu.header.protocol_version = 2;
    sequenceId_++;

    pdu.setPayload(std::vector<uint8_t>({0x01, (uint8_t)0x00, 0, 0}));

    buffer.clear();
    pdu.toData(buffer);
    if (sendto(local_socket, reinterpret_cast<const void *> (&buffer[0]), buffer.size(), 0 , reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)))==-1)
    {
        ALOGD("Failed to send UDP packet...Try again now!");
    }

    StartActivityMessageTimer();
}

void IpcbSimulator::StartActivityMessageTimer()
{
    activityPacketInjector = timer.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { CreateAndSendIpActivityMessage(); });
}

void IpcbSimulator::StopActivityMessageTimer()
{
    timer.Cancel(activityPacketInjector);
}

void IpcbSimulator::SendPdu(Pdu pdu)
{
    ALOGD("Send pdu");
    buffer.clear();
    pdu.toData(buffer);
    // buffer.push_back(1);
    if (sendto(local_socket, reinterpret_cast<const void *> (&buffer[0]), buffer.size(), 0 , reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)))==-1)
    {
        ALOGD("Failed to send UDP packet...Try again now!");
    }
}

Pdu IpcbSimulator::ReceivePdu()
{
    std::vector<uint8_t> rd_buffer;

    ssize_t packet_length = recv(local_socket, nullptr, 0, MSG_PEEK | MSG_TRUNC);
    if (packet_length <= 0)
    {
        throw std::runtime_error("Failed to get size of pending Datagram");
    }

    rd_buffer.resize(packet_length);


    if (0 > recvfrom(local_socket,
                        static_cast<void *>(rd_buffer.data()),
                        packet_length,
                        0,
                        reinterpret_cast<struct sockaddr *>(&sa_out),
                        &addrlen))
    {
        throw std::runtime_error("Failed to receive");
    }

    Pdu temp_pdu;
    temp_pdu.fromData(rd_buffer);
    return temp_pdu;
}