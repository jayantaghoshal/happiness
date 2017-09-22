#include <cstdint>
#include <unistd.h>
#include "carconfig_updater.h"

int32_t main(int argc, char *argv[])
{
    int opt;

    while((opt = getopt(argc, argv, "d")) != -1)
    {
        switch(opt)
        {
        case 'd':
            debugMode = true;
            break;
        default:
            fprintf(stderr, "Unknown option: -%c\n", opt);
            exit(1);
            break;
        }
    }

    return runUpdater();
}
