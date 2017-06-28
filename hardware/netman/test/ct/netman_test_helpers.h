#ifndef NETMAN_HELPERS_
#define NETMAN_HELPERS_

#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

std::string GetIpAddressForInterface(const std::string& ifname);

pid_t FindProcessId(const std::string& name);

#endif