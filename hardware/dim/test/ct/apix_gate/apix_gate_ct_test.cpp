#include <gtest/gtest.h>
#include <cutils/log.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <iostream>
#include "apix_gate_helpers.h"

#define PORT 50001
//simulator script use same IP but different port as real DIM
#define DIM_SIM_IP "198.18.24.1"
#define LOG_TAG "ApixGateTest:"

TEST(ApixGateTest, ApixGatePingTest){
    ALOGI("ApixGatePingTest starting...");
    //Arrange
    const std::string address = DIM_SIM_IP;
    const int attempts = 4;
    std::string output;
    //Act & Assert
    EXPECT_EQ(true, Ping(address, attempts, output));
}

TEST(ApixGateTest, ApixGateSetupSocketTest){
    ALOGI("ApixGateSetupSocketTest starting...");
    //Arrange
    int sock = 0;
    struct sockaddr_in dim_addr;
    memset(&dim_addr,'0', sizeof(dim_addr));
    dim_addr.sin_family = AF_INET;
    dim_addr.sin_port = htons(PORT);

     //Act & Assert
    ASSERT_LE(0, (sock=socket(AF_INET, SOCK_STREAM,0)));
    ASSERT_LT(0, inet_pton(AF_INET, DIM_SIM_IP, &dim_addr.sin_addr));
    EXPECT_LE(0, connect(sock, (struct sockaddr *)&dim_addr, sizeof(dim_addr)));
    close(sock);
}

TEST(ApixGateTest, ApixGateSocketRWTest){
    ALOGI("ApixGateSocketRWTest starting...");
     //Arrange
    int sock = 0, returnVal;
    struct sockaddr_in dim_addr;
    memset(&dim_addr,'0', sizeof(dim_addr));
    dim_addr.sin_family = AF_INET;
    dim_addr.sin_port = htons(PORT);
    //Send Integer over socket (\n used as separator)
    const char *msg = "42\n";
    char buffer[1024] = {0};

    //Act
    //If any of these fail, no point to continue
    ASSERT_LE(0, (sock=socket(AF_INET, SOCK_STREAM, 0)));
    ASSERT_LT(0, inet_pton(AF_INET, DIM_SIM_IP, &dim_addr.sin_addr));
    ASSERT_LE(0, connect(sock, (struct sockaddr *)&dim_addr, sizeof(dim_addr)));

    send(sock, msg, strlen(msg), 0);
    returnVal = read(sock, buffer, 1024);
    std::string s (buffer);
    //Assert
    //Expect integer to be increased by +2 in response
    EXPECT_EQ("44\n",s);
    close(sock);
}
