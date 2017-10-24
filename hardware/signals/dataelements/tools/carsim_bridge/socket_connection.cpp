#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "socket_connection.h"
#define LOG_TAG "CarSim_Bridge"

namespace CarSim {

SocketConnection::SocketConnection(int socket_fd) {
  socket_fd_ = socket_fd;
  ALOGV("SocketConnection constructor\n");
}

SocketConnection::~SocketConnection() {
  close(socket_fd_);
  ALOGV("SocketConnection deconstructor\n");
}

std::string SocketConnection::Read() {
  const int message_length = GetPayloadMessageLengthFromHeader();

  if (message_length > 0)
    return GetPayloadMessage(message_length);
  else
    return "";  // empty since its cant be null...
}

size_t SocketConnection::Send(std::string message) {
  char completeMessage[kHeaderMessageLength + kHeaderMessageLength + message.size()];
  sprintf(completeMessage, "%s%04d%s", kHeaderTag.c_str(), int(message.size()), message.c_str());

  return send(socket_fd_, completeMessage, strlen(completeMessage), 0);
}

int SocketConnection::GetPayloadMessageLengthFromHeader() {
  char headerBuffer[kHeaderTagLength + kHeaderMessageLength + 1] = {0};  //+1 for nullterminator

  int numberOfBytesRead = read(socket_fd_, headerBuffer, kHeaderTagLength + kHeaderMessageLength);  // Read header

  // IF CHANGING MY MIND: 0 bytes is a non-exception closing of the connection, trying to avoid use exception in this
  // case
  // if ( numberOfBytesRead == 0) {
  //  throw std::runtime_error("0 bytes read => Connection is closed");
  //}

  if (numberOfBytesRead <= -1) {
    throw std::runtime_error("Error: Socket Read " + std::string(strerror(errno)) + "\n");
  }

  // Check header message lenght
  if (numberOfBytesRead != kHeaderTagLength + kHeaderMessageLength) {
    printf("headerBuffer: %s\n", headerBuffer);
    printf("numberOfBytesRead: %zd, kHeaderTagLength: %zd, kHeaderMessageLength: %zd\n", numberOfBytesRead,
           kHeaderTagLength, kHeaderMessageLength);
    throw std::runtime_error("Error: read bytes not matching messgage-length.\n");
  }

  // Check Header Tag
  std::string bufferHeaderTag(headerBuffer, kHeaderTagLength);
  if (bufferHeaderTag != kHeaderTag) {
    std::string err = "Incorrect header tag, expected " + kHeaderTag + ", received " + bufferHeaderTag + "\n";
    throw std::runtime_error(err);
  }

  // Check message body length
  std::string messageLengthStr(headerBuffer + kHeaderTagLength, kHeaderMessageLength);

  ALOGV("DEBUG-Header:[%s][%s]\n", bufferHeaderTag.c_str(), messageLengthStr.c_str());

  return atoi(messageLengthStr.c_str());
}

std::string SocketConnection::GetPayloadMessage(int messageLength) {
  std::vector<char> messageBuffer(messageLength, 0);
  int numberOfBytesRead = read(socket_fd_, messageBuffer.data(), messageLength);

  if (numberOfBytesRead < 0) throw std::runtime_error("Error: GetPayloadMessage->numberOfBytesRead < 0\n.");

  if (numberOfBytesRead != messageLength) {
    ALOGV("DEBUG-Body-buffer:%.*s (number of bytes: %zd)\n", numberOfBytesRead, messageBuffer.data(),
          numberOfBytesRead);
    throw std::runtime_error("Error: GetPayloadMessage->Wrong message length or error in read");
  }

  std::string payload(messageBuffer.data(), numberOfBytesRead);

  ALOGV("DEBUG-GetPayloadMessage:[%s](bytes: %zd)\n", payload.c_str(), numberOfBytesRead);

  return payload;
}

}  // CarSim