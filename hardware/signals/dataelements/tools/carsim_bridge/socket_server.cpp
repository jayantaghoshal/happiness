/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

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

SocketServer::SocketServer(int port) {
    int opt = 1;

    ALOGV("DEBUG-SocketServer::SocketServer-1");

    // Creating socket file descriptor
    if ((*server_socket_fd_Ptr_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw std::runtime_error("Error: SocketServer::SocketServer, Socket failed (internal ex: " +
                                 std::string(strerror(errno)) + ")");
    }

    ALOGV("DEBUG-SocketServer::SocketServer-2");

    // Forcefully attaching socket to the port
    if (setsockopt(*server_socket_fd_Ptr_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        throw std::runtime_error("Error, SocketServer::SocketServer, Setsockopt");
    }
    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = htons(port);

    ALOGV("DEBUG-SocketServer::SocketServer-3");

    // Forcefully attaching socket to the port
    if (bind(*server_socket_fd_Ptr_, (struct sockaddr*)&address_, sizeof(address_)) < 0) {
        throw std::runtime_error("Error: SocketServer::SocketServer, Bind failed (internal ex: " +
                                 std::string(strerror(errno)) + ")");
    }

    ALOGV("DEBUG-SocketServer::SocketServer-4");

    if (listen(*server_socket_fd_Ptr_, 3) < 0) {
        throw std::runtime_error("Error: SocketServer::SocketServer, Listen (internal ex: " +
                                 std::string(strerror(errno)) + ")");
    }
}

std::shared_ptr<CarSim::SocketConnection> SocketServer::Connect() {
    int new_socket{-1};  // the client socket instance
    int addrlen = sizeof(address_);

    ALOGV("DEBUG-SocketServer::Connect-1");

    if ((new_socket = accept(*server_socket_fd_Ptr_, (struct sockaddr*)&address_, (socklen_t*)&addrlen)) < 0) {
        throw std::runtime_error("Error: SocketServer::Connect, Accept (internal ex: " + std::string(strerror(errno)) +
                                 ")");
    }

    ALOGV("DEBUG-SocketServer::Connect-2");

    std::shared_ptr<CarSim::SocketConnection> connectionPtr(new CarSim::SocketConnection(new_socket));
    return connectionPtr;
}

}  // CarSim