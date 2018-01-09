/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

bool Ping(const std::string &ip, int attempts, std::string &result) {
    std::string command = "ip netns exec vcc ping -c " + std::to_string(attempts) + " " + ip;
    std::string mode = "r";
    std::stringstream result_details;
    FILE *in = nullptr;
    char buff[512];
    if (!(in = popen(command.c_str(), mode.c_str()))) {
        return false;
    }
    while (fgets(buff, sizeof(buff), in) != NULL) {
        result_details << buff;
    }
    result = result_details.str();
    int exitcode = pclose(in);
    return exitcode == 0;
}
