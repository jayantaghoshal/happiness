// Service which installs the default carconfig if it isn't in /var yet
// and loads it into shared memory for everyone to use.

// Change /var/ to DATA partition when path has been decided
#define CARCONFIG_FILE_NAME "/var/lib/carconfig/carconfig.csv"

// Default carconfig for first time execution in a vehicle
#define CARCONFIG_DEFAULT_FILE_NAME "/etc/carconfig-default.csv"

#include "carconfig_setup.h"

int main(int argc, const char* argv[]) { return carconfig_setup(CARCONFIG_FILE_NAME, CARCONFIG_DEFAULT_FILE_NAME); }
