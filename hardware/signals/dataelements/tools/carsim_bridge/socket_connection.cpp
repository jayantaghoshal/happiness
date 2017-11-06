#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "socket_connection.h"
#define LOG_TAG "CarSim_Bridge"

namespace CarSim {

void close_wrapper(int* socket_fd_Ptr) { close(*socket_fd_Ptr); }

SocketConnection::SocketConnection(int socket_fd) : socket_fd_Ptr_{new int(socket_fd), &close_wrapper} {
    ALOGV("SocketConnection::SocketConnection");
}

std::string SocketConnection::Read() {
    // Read MessageHeader
    std::string headerMessage = ReadMessage(kHeaderTag.size() + kHeaderMessageLength);

    if (headerMessage.substr(0, kHeaderTag.size()) != kHeaderTag) {
        ALOGD("ERROR: SocketConnection::Read, Incorrect Message header tag, expected: %s, got %s", kHeaderTag.c_str(),
              headerMessage.c_str());
        return "";  // Invalid message
    }

    // Check message body length
    std::string messageLengthStr(headerMessage.substr(kHeaderTag.size()));
    const int message_length = atoi(messageLengthStr.c_str());

    if (message_length <= 0) {
        ALOGD("ERROR: SocketConnection::Read, Incorrect message length: %zd", message_length);
        return "";  // Empty since its cant be null...
    }

    return ReadMessage(message_length);
}

void SocketConnection::Send(std::string message) {
    char completeMessage[kHeaderMessageLength + kHeaderMessageLength + message.size()];
    sprintf(completeMessage, "%s%04d%s", kHeaderTag.c_str(), int(message.size()), message.c_str());

    size_t lengthOfCompleteMessage = strlen(completeMessage);
    size_t totalBytesSend{0};

    while (lengthOfCompleteMessage > totalBytesSend) {
        size_t bytesSend =
                send(*socket_fd_Ptr_, completeMessage + totalBytesSend, lengthOfCompleteMessage - totalBytesSend, 0);
        if (bytesSend <= 0) {
            throw std::runtime_error("Error: SocketConnection::Send, bytesSend < 0 (internal ex: " +
                                     std::string(strerror(errno)) + ")");
        }
        totalBytesSend += bytesSend;
    }
}

// Private functions below
std::string SocketConnection::ReadMessage(int messageLength) {
    std::vector<char> messageBuffer(messageLength, 0);

    ssize_t totalNumberOfBytesRead{0};
    while (totalNumberOfBytesRead < messageLength) {
        ssize_t numberOfBytesRead = read(*socket_fd_Ptr_, messageBuffer.data() + totalNumberOfBytesRead,
                                         messageLength - totalNumberOfBytesRead);

        if (numberOfBytesRead <= 0) {
            throw std::runtime_error("Error: SocketConnection::ReadMessage->numberOfBytesRead <= 0 (internal ex: " +
                                     std::string(strerror(errno)) + ").");
        }

        totalNumberOfBytesRead += numberOfBytesRead;
    }

    std::string message(messageBuffer.data(), totalNumberOfBytesRead);
    ALOGV("DEBUG-SocketConnection::ReadMessage:[%s](bytes: %zd)", message.c_str(), totalNumberOfBytesRead);
    return message;
}

}  // CarSim