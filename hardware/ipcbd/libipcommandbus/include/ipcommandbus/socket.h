/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>

#include "isocket.h"
#include "local_config_parameters.h"

namespace Connectivity {
class Socket : public ISocket {
  public:
    using EcuIpMap = std::vector<std::pair<Message::Ecu, EcuAddress>>;

    Socket(tarmac::eventloop::IDispatcher& dispatcher, int domain, int type, int protocol, EcuIpMap ecu_ip_map);

    Socket(Socket&&) = delete;
    Socket& operator=(Socket&&) = delete;

    virtual ~Socket();
    virtual void setup(const Message::Ecu& ecu) = 0;
    virtual void read(std::vector<uint8_t>& buffer, Message::Ecu& ecu) = 0;
    virtual void writeTo(const std::vector<uint8_t>& buffer, const Message::Ecu& ecu) = 0;
    virtual void registerReadReadyCb(std::function<void(void)> readReadyCb) = 0;
    void endConnection() {}

    void set_option(int level, int option, int value);
    int get_option(int level, int option);
    uint32_t getTestSimPort();
    static std::vector<std::pair<Message::Ecu, ISocket::EcuAddress>> defaultEcuMap();

  protected:
    void setup(int domain, int type, int protocol);
    void teardown();
    void setHandler(std::function<void(void)> readEvenHandler);
    int getIpPrecedenceValue(IpPrecedence ipPrecedence);

    void backoffReset();  // set backoff timer to default/start value
    // get the current backoff timeout and then backoff with a factor 2 upto a max
    // value
    std::chrono::milliseconds backoffGet();

    int socket_fd_ = -1;
    tarmac::eventloop::IDispatcher& dispatcher_;

    // ToDo Abhi: When local_config object is created; everything is read. This
    // need to split
    LocalconfigParameters local_config;

    const EcuIpMap ecu_ip_map_;  ///< Maps between ECU and IP addresses.

  private:
    std::function<void(void)> read_ready_cb_ = nullptr;
    std::chrono::milliseconds backoff_timeout_;
};
}  // namespace Connectivity
