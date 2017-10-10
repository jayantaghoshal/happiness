#include <cstdint>
#include <unistd.h>
#include "carconfig_updater.h"

int32_t main(int argc, char *argv[])
{
    return CarConfigUpdater::runUpdater();
}
