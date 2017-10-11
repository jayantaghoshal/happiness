#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cutils/log.h>
#include <stdexcept>
#include "tiny-ipcommandbus/include/udp_packet_injector.h"

#define SERVER_BCAST_IP "198.18.255.255"
#define SRC_PORT 60000   //The default port from which to send data

using ::tarmac::eventloop::IDispatcher;



UdpPacketInjector::UdpPacketInjector(uint16_t port) :
DST_PORT(port), timer{IDispatcher::GetDefaultDispatcher()}
{
    std::cout<<"Packet Injector started"<<std::endl;
    UdpPacketInjector::setup();
}

void UdpPacketInjector::setup()
{
    if ( (src_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        std::cout<<"Failed to create broadcast socket...die now!"<<std::endl;
        exit(1);
    }

    // int broadcastEnable=1;
    // int ret=setsockopt(src_socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(DST_PORT);

    if (inet_aton(SERVER_BCAST_IP , &sa.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    memset(&srcaddr, 0, sizeof(srcaddr));
    srcaddr.sin_family = AF_INET;
    srcaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srcaddr.sin_port = htons(SRC_PORT);

    if (bind(src_socket, (struct sockaddr *) &srcaddr, sizeof(srcaddr)) < 0) {
        std::cout<<"bind error...die now!"<<std::endl;
        exit(1);
    }
}


void UdpPacketInjector::CreateAndSendIpActivityMessage()
{
    Pdu pdu;

    pdu.createHeader(0xFFFF,0xFF01,
        IpCmdTypes::OperationType::NOTIFICATION_CYCLIC,IpCmdTypes::DataType::NOT_ENCODED,
        sequenceId_);

    sequenceId_++;

    pdu.setPayload(std::vector<uint8_t>({0x01, (uint8_t)0x00, 0, 0}));

    buffer.clear();
    pdu.toData(buffer);
    if (sendto(src_socket, reinterpret_cast<const void *> (&buffer[0]), buffer.size(), 0 , reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)))==-1)
    {
        std::cout<<"Failed to send UDP broadcast...Try again now!"<<std::endl;
    }

    StartActivityMessageTimer();
}

void UdpPacketInjector::StartActivityMessageTimer()
{
    activityPacketInjector = timer.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { CreateAndSendIpActivityMessage(); });
}

void UdpPacketInjector::StopActivityMessageTimer()
{
    timer.Cancel(activityPacketInjector);
}

void UdpPacketInjector::SendPdu(Pdu pdu)
{
    buffer.clear();
    pdu.toData(buffer);
    if (sendto(src_socket, reinterpret_cast<const void *> (&buffer[0]), buffer.size(), 0 , reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)))==-1)
    {
        std::cout<<"Failed to send UDP broadcast...Try again now!"<<std::endl;
    }
}

void UdpPacketInjector::ReceivePdu(Pdu &pdu)
{
    std::vector<uint8_t> rd_buffer;
    ssize_t packet_length = recv(src_socket, nullptr, 0, MSG_PEEK | MSG_TRUNC);
    if (packet_length <= 0)
    {
        throw std::runtime_error("Failed to get size of pending Datagram");
    }

    rd_buffer.resize(packet_length);

    struct sockaddr_in sa_out;

    socklen_t addrlen = static_cast<socklen_t>(sizeof(sa_out));
    memset(static_cast<void *>(&sa_out), 0, addrlen);
    sa_out.sin_family = AF_INET;

    if (0 > recvfrom(src_socket,
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
    pdu = temp_pdu;
}