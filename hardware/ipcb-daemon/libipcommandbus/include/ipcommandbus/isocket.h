/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cassert>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <system_error>
#include <vector>
#include "ipcommandbus/Message.h"

namespace Connectivity
{
class SocketException : public std::system_error
{
public:
    SocketException(int ev, const std::string &what_arg) : std::system_error(ev, std::system_category(), what_arg)
    {
    }
};

class ISocket
{
public:
    /**
     * Enumeration for IP Precedence, used for Type of Service.
     *
     * ==============================================================
     * DSCP <=> IP Precedence Conversion Table
     * ==============================================================
     * DSCP Name        DS Field Value (Dec)    IP Precedence (Description)
     * CS0              0                       0 : Best Effort
     * CS1,AF11-13      8,10,12,14              1 : Priority
     * CS2,AF21-23      16,18,20,22             2 : Immediate
     * CS3,AF31-33      24,26,28,30             3 : Flash - mainly used for voice signaling
     * CS4,AF41-43      32,34,36,38             4 : Flash Override
     * CS5,EF           40,46                   5 : Critical - mainly used for voice RTP
     * CS6              48                      6 : Internetwork Control
     * CS7              56                      7 : Network Control
     *
     * ==============================================================
     * "DSCP <=> TOS <=>IP Precedence" value Conversion Table
     * ==============================================================
     *
     * DSCP Dec	 TOS value   IP Prec
     * --------  ---------   -------
     * 0         0           0
     * 8         32          1
     * 10        40          1
     * 14        56          1
     * 18        72          2
     * 22        88          2
     * 24        96          3
     * 28        112         3
     * 34        136         4
     * 36        144         4
     * 38        152         4
     * 40        160         5
     * 46        184         5
     * 48        192         6
     * 56        224         7
     *
     * (Table from https://en.wikipedia.org/wiki/Type_of_service)
     *
     */
    enum class IpPrecedence : std::uint8_t
    {
        kRoutine,
        kPriority,
        kImmediate,
        kFlash,
        kFlashOverride,
        kCriticEcp,
        kInternetworkControl,
        kNetworkControl
    };

    struct EcuAddress
    {
        std::string ip;
        uint16_t port;
    };

    virtual ~ISocket() = default;

    virtual void endConnection() = 0;
    virtual void registerReadReadyCb(std::function<void(void)> readReadyCb) = 0;
    virtual void read(std::vector<uint8_t> &buffer, Message::Ecu &ecu) = 0;
    virtual void writeTo(const std::vector<uint8_t> &buffer, const Message::Ecu &ecu) = 0;
};
}
