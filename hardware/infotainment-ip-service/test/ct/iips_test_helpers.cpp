#include "iips_test_helpers.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>


pid_t FindProcessId(const std::string& name)
{
    DIR* dir;
    struct dirent* ent;
    char buf[512];

    long  pid;
    char pname[100] = {0,};
    char state;
    FILE *fp=NULL;

    if (!(dir = opendir("/proc"))) {
        perror("can't open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL) {
        long lpid = atol(ent->d_name);
        if(lpid < 0)
            continue;
        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
        fp = fopen(buf, "r");

        if (fp) {
            if ( (fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3 ){
                printf("fscanf failed \n");
                fclose(fp);
                closedir(dir);
                return -1;
            }
            if (pname == name) {
                fclose(fp);
                closedir(dir);
                return (pid_t)lpid;
            }
            fclose(fp);
        }
    }

    closedir(dir);
    return -1;
}

std::vector<std::string> ExecuteCommand(const std::string& command) {
    FILE *fp;
    char buffer[1024];
    std::vector<std::string> result;

    // Open the command for reading
    fp = popen(command.c_str(), "r");
    if (fp == NULL) {
        return result;
    }

    // Read the output a line at a time
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        result.push_back(buffer);
    }

    pclose(fp);

    return result;
}
