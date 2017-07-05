/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
/**
 * @file
 *
 * @brief UdpSocket class
 *
 */
#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <cstdint>
#include <functional>
#include <limits>
#include <string>
#include <vector>

#include "ipcommandbus/UdpSocketIf.h"

#include "assert.h"

namespace Connectivity
{
/**
 * Stub udp socket class just to get the MessageDispatcher class to compile.
 * The intended use case is unit testing of service classes,
 * and we catch the send calls already at (mock) IpService level,
 * so this is never supposed to get called...
 *
 * (Q: Why not stub MessageDispatcher instead?
 *  A: Much easier to stub two 10 liner classes that will nearly never change than one of our biggest classes
 *     that will undoubtably be updated some times...
 * )
 */

/**
 * Wraps some socket functions as creation, read and write.
 */
class UdpSocket : public UdpSocketIf
{
public:
    UdpSocket()
    {
    }
    virtual ~UdpSocket()
    {
    }

    /**
     * Bind the UDP socket to local IP/port.
     *
     * @param[in] localIp                       Local ip addres to use. This will set which interface to use.
     * @param[in] localPort                     Local port the socket will be bound to.
     * @param[in] ipPrecedence                  Optional ToS IP Precedence value (default is Priority)
     * @return                                  True if all went well.
     *                                          False if an error occurred.
     */
    bool setup(const std::string &localIp, uint16_t localPort, IpPrecedence ipPrecedence = IpPrecedence::kPriority)
    {
        return true;
    }

    void registerReadReadyCb(std::function<void(void)> readReadyCb)
    {
    }

    /**
     * Read one UDP message from the socket.
     *
     * @param[inout] buffer                     Buffer to which read data will be appended.
     * @param[out] fromIp                       Peer IP from which the package was sent.
     * @param[out] fromPort                     Peer port from which the package was sent.
     * @return                                  Actual number of bytes read. (-1 on failure)
     */
    std::int64_t read(std::vector<uint8_t> &buffer, std::string &fromIp, uint16_t &fromPort)
    {
        fprintf(stderr, "Not supposed to reach this stage in service layer tests...");
        assert(0);
        return 0;
    }

    /**
     * Write data on the socket with a specified target IP/port.
     *
     * @param[in] buffer                        Buffer to send
     * @param[in] ip                            Destination IP address
     * @param[in] port                          Destination port
     * @return                                  Number of bytes written.
     */
    virtual std::size_t writeTo(const std::vector<uint8_t> &buffer, const std::string &ip, uint16_t port)
    {
        fprintf(stderr, "Not supposed to reach this stage in service layer tests...");
        assert(0);
        return buffer.size();
    }

    void havePendingDatagrams()
    {
        fprintf(stderr, "Not supposed to reach this stage in service layer tests...");
        assert(0);
    }
};

}  // Connectivity

#endif  // UDPSOCKET_H
