/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>

#define LOG_TAG "ApixSetupSockUtil"
#include <cutils/log.h>

int main() {
    const int port = 50000;
    const char* dim_ip = "198.18.24.1";
    int sock = 0;
    fd_set file_desc;
    struct sockaddr_in dim_addr;
    struct timeval connect_timeout;

    // Setup non-blocking socket,
    sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sock < 0) {
        ALOGE("Error creating socket: %s", strerror(errno));
        close(sock);
        exit(EXIT_FAILURE);
    }
    dim_addr.sin_family = AF_INET;
    dim_addr.sin_port = htons(port);
    dim_addr.sin_addr.s_addr = inet_addr(dim_ip);

    int conn_result = connect(sock, reinterpret_cast<struct sockaddr*>(&dim_addr), sizeof(dim_addr));
    if (conn_result < 0) {
        if (errno == EINPROGRESS) {
            ALOGV("Connect in progress");
            // If TCP setup not successful in 5s -> Failure
            connect_timeout.tv_sec = 5;
            connect_timeout.tv_usec = 0;
            FD_ZERO(&file_desc);
            FD_SET(sock, &file_desc);
            int select_res = select(sock + 1, nullptr, &file_desc, nullptr, &connect_timeout);
            if (select_res < 0 && errno != EINTR) {
                ALOGE("Error: SELECT  call failed: %s", strerror(errno));
                close(sock);
                exit(EXIT_FAILURE);
            } else if (select_res > 0) {
                int optval;
                socklen_t optlen = sizeof(int);
                if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
                    ALOGE("ERROR: failed to execute sockopt:  %s", strerror(errno));
                    close(sock);
                    exit(EXIT_FAILURE);
                }
                if (optval != 0) {
                    ALOGE("ERROR: getsockopt returned error");
                    close(sock);
                    exit(EXIT_FAILURE);
                }
                // Success, connection established!
                ALOGV("Socket setup successfull");
                close(sock);
                exit(EXIT_SUCCESS);
            }
            // This means select returned 0 -> connection timeout!
            ALOGE("ERROR: SELECT TIMED OUT");
            close(sock);
            exit(EXIT_FAILURE);
        } else {
            ALOGE("ERROR: Error connecting");
            close(sock);
            exit(EXIT_FAILURE);
        }
    }
    ALOGV("Socket setup successfull");
    close(sock);
    exit(EXIT_SUCCESS);
}
