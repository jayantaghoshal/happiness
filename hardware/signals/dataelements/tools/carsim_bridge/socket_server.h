#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string>

#include "socket_connection.h"

namespace CarSim {

class SocketServer {
 public:
  void Init(int port);
  std::shared_ptr<CarSim::SocketConnection> Connect();

 private:
  int server_fd_{-1};    // server socket
  sockaddr_in address_;  // struct

  const std::string SOF{"CarSim"};
  static constexpr std::uint8_t SOF_length{6};
  static constexpr std::uint8_t messageLength_length{4};  // fixed 4 bytes describing an int32
};
}