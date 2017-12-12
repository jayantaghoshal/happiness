/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <utils/Log.h>
#include <memory>
#include <string>

// Message is sent/received as:
// [tag][number of chars in msg][message]

namespace CarSim {

void close_wrapper(int* socket_fd_ptr);

class SocketConnection {
  public:
    // Created by and returned by when connection is established SocketServer.
    SocketConnection(int socket_fd);
    std::string Read();
    void Send(std::string message);

  private:
    const std::string kHeaderTag{"CarSim"};
    static constexpr std::uint8_t kHeaderMessageLength{4};  // fixed 4 bytes describing an int32

    std::unique_ptr<int, decltype(&close_wrapper)> socket_fd_Ptr_;

    std::string ReadMessage(int messageLength);
};

}  // CarSim