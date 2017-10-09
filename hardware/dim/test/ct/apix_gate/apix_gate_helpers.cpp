#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

bool Ping(const std::string &ip, const int attempts, std::string& result){
    std::string command = "ping -c " + std::to_string(attempts) + " " + ip;
    std::string mode = "r";
    std::stringstream result_details;
    FILE *in;
    char buff[512];
    if(!(in = popen(command.c_str(), mode.c_str()))){
        return false;
    }
    while(fgets(buff, sizeof(buff), in)!=NULL){
        result_details << buff;
    }
    result = result_details.str();
    int exitcode = pclose(in);
    return exitcode==0;
}
