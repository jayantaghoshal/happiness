/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <unistd.h>
#include <cstdint>
#include "Application_dataelement.h"

int32_t main(int argc, char *argv[]) {
    int i;
    for (;;) {
        i = 5;
        sleep(5000);
    }  // dummy code for deamon to run
    return 1;
}
