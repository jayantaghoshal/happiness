/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstddef>
#include <cstdint>

#if defined(__COVERITY__) && !defined(__INCLUDE_LEVEL__)
/* Systemd's use of gcc's __INCLUDE_LEVEL__ extension macro appears to confuse
* Coverity. Here's a kludge to unconfuse it.
*/
#define __INCLUDE_LEVEL__ 2
#endif
#include <systemd/sd-bus.h>

class restartClient {
  public:
    restartClient();
    ~restartClient();
    bool restart();

  private:
    sd_bus_error error;
    sd_bus_message *m;
    sd_bus *bus;
};
