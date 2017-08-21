#ifndef IIPS_HELPERS_
#define IIPS_HELPERS_

#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>

pid_t FindProcessId(const std::string& name);

std::vector<std::string> ExecuteCommand(const std::string& command);
#endif
