#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "socket_server.h"
#define LOG_TAG "CarSim_Bridge"

namespace CarSim {

void SocketServer::Init(int port) {
  int opt = 1;
  int addrlen = sizeof(address_);

  ALOGV("DEBUG-SocketServer::Init-1\n");

  // Creating socket file descriptor
  if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    throw std::runtime_error("socket failed");
  }

  ALOGV("DEBUG-SocketServer::Init-2\n");

  // Forcefully attaching socket to the port
  if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    throw std::runtime_error("setsockopt");
  }
  address_.sin_family = AF_INET;
  address_.sin_addr.s_addr = INADDR_ANY;
  address_.sin_port = htons(port);

  ALOGV("DEBUG-SocketServer::Init-3\n");

  // Forcefully attaching socket to the port
  if (bind(server_fd_, (struct sockaddr *)&address_, sizeof(address_)) < 0) {
    throw std::runtime_error("bind failed");
  }

  ALOGV("DEBUG-SocketServer::Init-4\n");

  if (listen(server_fd_, 3) < 0) {
    throw std::runtime_error("listen");
  }

  printf("Initiating ServerSocket done.\n");
}

std::shared_ptr<CarSim::SocketConnection> SocketServer::Connect() {
  int new_socket{-1};  // the client socket instance
  int addrlen = sizeof(address_);

  ALOGV("DEBUG-SocketServer::connect-1\n");

  if ((new_socket = accept(server_fd_, (struct sockaddr *)&address_, (socklen_t *)&addrlen)) < 0) {
    throw std::runtime_error("accept");
  }

  ALOGV("DEBUG-SocketServer::connect-2\n");

  std::shared_ptr<CarSim::SocketConnection> connectionPtr(new CarSim::SocketConnection(new_socket));
  return connectionPtr;
}

}  // CarSim