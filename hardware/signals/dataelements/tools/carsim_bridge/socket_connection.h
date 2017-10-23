#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <utils/Log.h>
#include <string>

// Message is sent/received as:
// [tag][number of chars in msg][message]
namespace CarSim {

class SocketConnection {
 public:
  // Created by and returned by when connection is established SocketServer.
  SocketConnection(int socket_fd);
  ~SocketConnection();
  std::string Read();
  size_t Send(std::string message);

 private:
  const std::string kHeaderTag{"CarSim"};
  static constexpr std::uint8_t kHeaderTagLength{6};
  static constexpr std::uint8_t kHeaderMessageLength{4};  // fixed 4 bytes describing an int32

  int socket_fd_{-1};

  // Header contains tag + the char/byte length of the payload.
  int GetPayloadMessageLengthFromHeader();
  std::string GetPayloadMessage(int messageLength);
};

}  // CarSim