/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <unistd.h>
#include <cstdint>
#include "carconfig_updater.h"

int32_t main(int argc, char *argv[]) { return CarConfigUpdater::runUpdater(); }
