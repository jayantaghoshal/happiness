/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#include "ipcommandbus/TcpSocket.h"
#include <cassert>
#include <cutils/log.h>

#include "ipcommandbus/vcc_pdu_header.h"

#include <unistd.h>
#include <string>

#include <string.h>
#include <algorithm>

using namespace tarmac::eventloop;

namespace Connectivity
{
#define LOG_TAG "TCP_Socket"

TcpSocket::TcpSocket(IDispatcher & dispatcher, EcuIpMap ecu_ip_map) : Socket(dispatcher, AF_INET, SOCK_STREAM, IPPROTO_TCP, ecu_ip_map)
{
    setHandler(std::bind(&TcpSocket::readEventHandler, this));
}

TcpSocket::~TcpSocket()
{
    // flush buffers as well
}

void TcpSocket::registerReadReadyCb(std::function<void(void)> readReadyCb)
{
    ready_cb_ = std::move(readReadyCb);
}

void TcpSocket::setup(const Message::Ecu &ecu)
{
    auto it = std::find_if(ecu_ip_map_.begin(),
                           ecu_ip_map_.end(),
                           [ecu](const std::pair<Message::Ecu, EcuAddress> &pair) { return pair.first == ecu; });

    if (it == ecu_ip_map_.end())
        return;

    if (ecu == Message::Ecu::DIM)
    {
        set_option(SOL_SOCKET, SO_KEEPALIVE, 1);
        set_option(SOL_TCP, TCP_KEEPCNT, local_config.getDIMKeepAliveProbes());
        set_option(SOL_TCP, TCP_KEEPIDLE, static_cast<int>(local_config.getDIMKeepAliveTime().count()));
        set_option(SOL_TCP, TCP_KEEPINTVL, static_cast<int>(local_config.getDIMKeepAliveInterval().count()));
    }

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(it->second.port);

    if (1 != inet_pton(AF_INET, it->second.ip.c_str(), &sa.sin_addr))
    {
        throw SocketException(errno, "Address not in correct format");
    }

    // No need to bind. TCP random port will be selected for client
    // const int bind_status =
    //     bind(socket_fd_, reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)));
    // if (0 != bind_status)
    // {
    //     throw SocketException(errno, "Failed to bind to socket");
    // }

    const int connect_status =
        connect(socket_fd_, reinterpret_cast<struct sockaddr *>(&sa), static_cast<socklen_t>(sizeof(sa)));
    if (0 != connect_status)
    {
        throw SocketException(errno, "Failed to connect");
    }

    peer_ecu_ = ecu;
}

void TcpSocket::endConnection()
{
    resetConnection();
}

void TcpSocket::read(std::vector<uint8_t> &buffer, Message::Ecu &ecu)
{
    buffer.clear();
    if (!read_frame_buffer_.empty())
    {
        buffer = std::move(read_frame_buffer_.front());
        read_frame_buffer_.pop();
    }

    ecu = peer_ecu_;
}

void TcpSocket::writeTo(const std::vector<uint8_t> &buffer, const Message::Ecu & /*ecu*/)
{
    auto remaining_bytes = buffer.size();
    int sent_bytes = 0;

    while (remaining_bytes > 0)
    {
        int ret = -1;
        ret = send(socket_fd_, reinterpret_cast<const void *>(&buffer[sent_bytes]), remaining_bytes, 0);

        if (-1 == ret)
        {
            if (errno == EINTR)
            {
                // try to continue
                continue;
            }
            if (errno == EPIPE || errno == ECONNRESET || errno == ETIMEDOUT)
            {
                // connection is broken. reconnection will be triggered on read. What should happen with reqested
                // buffer? throw away? as WFR will be triggered and packet will be resent
                break;
            }

            throw SocketException(errno, "Failed to send packet");
        }
        sent_bytes += ret;
        remaining_bytes -= ret;
    }
}

int TcpSocket::reconnectionRetryHandler(std::uint64_t /*usec*/, void *userdata)
{
    // retry connection
    static_cast<TcpSocket *>(userdata)->reconnect();

    return 0;
}

void TcpSocket::readEventHandler()
{
    // Resize buffer to a value larger than header length
    std::vector<std::uint8_t> buffer(4000, 0);

    int packet_length = recv(socket_fd_, static_cast<void *>(buffer.data()), buffer.size(), 0);
    if (0 >= packet_length)
    {
        if (errno == ECONNRESET || errno == ETIMEDOUT || packet_length == 0)
        {
            // Connection is closed by peer/lost. Initiate reconnection
            resetConnection();
            reconnect();
            return;
        }
        throw SocketException(errno, "Failed to receive");
    }

    for (int cnt = 0; cnt < packet_length; ++cnt)
    {
        input_byte_stream_.push_back(buffer[cnt]);
    }

    packetizer();
}

void TcpSocket::packetizer()
{
    while (input_byte_stream_.size() >= VCCPDUHeader::DATA_SIZE)
    {
        std::vector<std::uint8_t> header_buffer(input_byte_stream_.begin(),
                                                input_byte_stream_.begin() + VCCPDUHeader::DATA_SIZE);

        VCCPDUHeader header = VCCPDUHeader::from_data(header_buffer);

        // create a sender_handle_id from extracted header and see if it matches with sender_handle_id in header
        // Doc: 31873255 IHU4.0 BT SWRS IP Proto: section 6.8.1.1.2.3.2.5 SenderHandleId
        VccIpCmd::SenderHandleId sender_handle_id = ((static_cast<uint16_t>(header.service_id) & 0xFF) << 24) |
                                                    ((static_cast<uint16_t>(header.operation_id) & 0xFF) << 16) |
                                                    ((static_cast<uint8_t>(header.operation_type) & 0xFF) << 8) | 0x00;

        if ((header.sender_handle_id & 0xFFFFFF00) == sender_handle_id)
        {
            if (input_byte_stream_.size() >= (header.length + VCCPDUHeader::BASE_LENGTH))
            {
                const std::deque<std::uint8_t>::iterator end_it =
                    input_byte_stream_.begin() + header.length + VCCPDUHeader::BASE_LENGTH;

                read_frame_buffer_.emplace(input_byte_stream_.begin(), end_it);
                input_byte_stream_.erase(input_byte_stream_.begin(), end_it);
                continue;
            }

            break;
        }
        else
        {
            // this is a fatal error. reset Connnection
            resetConnection();
            reconnect();
            return;
        }
    }

    if (ready_cb_ && !read_frame_buffer_.empty())
        ready_cb_();
}

void TcpSocket::resetConnection()
{
    input_byte_stream_.clear();

    std::queue<std::vector<std::uint8_t>> empty_queue;
    std::swap(read_frame_buffer_, empty_queue);

    close(socket_fd_);
    socket_fd_ = -1;
}

void TcpSocket::reconnect()
{
    try
    {
        if (socket_fd_ >= 0)
            close(socket_fd_);

        Socket::setup(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        setup(peer_ecu_);
    }
    catch (const SocketException &e)
    {
        /*std::uint64_t now;
        sd_event_now(&sd_event_, CLOCK_MONOTONIC, &now);
        sd_event_source *time_event_source;
        int ret = sd_event_add_time(
            &sd_event_, &time_event_source, CLOCK_MONOTONIC, now + 100, 1, reconnectionRetryHandler, this);
        assert_sd_throw(ret >= 0, "Failed to add timer event source to event loop");
        time_event_source_.reset(time_event_source);*/

        dispatcher_.EnqueueWithDelay(std::chrono::microseconds(100), [this]() {
            reconnectionRetryHandler(100,this);
        });
    }
}
}
