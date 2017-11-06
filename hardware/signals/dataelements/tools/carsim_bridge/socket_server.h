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
    SocketServer(int port);
    std::shared_ptr<CarSim::SocketConnection> Connect();

  private:
    // int server_fd_{-1};    // server socket
    std::unique_ptr<int, decltype(&close_wrapper)> server_socket_fd_Ptr_{new int(), &close_wrapper};

    sockaddr_in address_;  // struct
};
}