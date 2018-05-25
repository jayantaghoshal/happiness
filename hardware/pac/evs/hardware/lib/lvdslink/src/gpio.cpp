/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "gpio.h"

#include <libdbg.h>

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstring>

namespace evs {
namespace hardware {
namespace lvdslink {

#define ARRAY_SIZE(__arr) (sizeof(__arr) / sizeof(__arr[0]))

std::unique_ptr<Gpio> Gpio::Create(const std::string& name, int number, Direction direction) {
    auto gpio = std::unique_ptr<Gpio>(new Gpio(name, number, direction));

    if (gpio && !gpio->IsValid()) {
        gpio.reset();
    }

    return gpio;
}

Gpio::Gpio(const std::string& name, int number, Direction direction)
    : name_(std::move(name)), number_(number), direction_(direction) {}

bool Gpio::Set(Level level) {
    if (direction_ == Direction::IN) {
        dbgE("%s:%d cannot set pin, is readonly", name_.c_str(), number_);
        return false;
    }

    FILE* fd = OpenSysfs("value", "w");
    bool ret = fd != nullptr;

    if (ret) {
        ret = fprintf(fd, "%c\n", level == Level::HIGH ? '1' : '0') == 2;
        if (ret) {
            dbgD("%s:%d=%s success", name_.c_str(), number_, level == Level::HIGH ? "high" : "low");
        }
        fclose(fd);
    }

    return ret;
}

bool Gpio::Get(Level* level) {
    FILE* fd = OpenSysfs("value", "r");
    bool ret = fd != nullptr;

    if (ret) {
        char value[2];
        if (fgets(value, ARRAY_SIZE(value), fd) == nullptr) {
            dbgE("%s:%d failed getting value", name_.c_str(), number_);
        } else {
            *level = value[0] == '1' ? Level::HIGH : Level::LOW;
            dbgD("%s:%d='%s' success", name_.c_str(), number_, value);
        }
        fclose(fd);
    }

    return ret;
}

/**
 * Open gpio syfs attribute.
 *
 * @param[in] attribute name of attribute
 * @param[in] mode      file mode
 *
 * @returns FILE pointer if successful else nullptr
 */
FILE* Gpio::OpenSysfs(const char* attribute, const char* mode) {
    char sysfs_path[64];
    snprintf(sysfs_path, ARRAY_SIZE(sysfs_path), "/sys/class/gpio/gpio%d/%s", number_, attribute);

    FILE* fd = fopen(sysfs_path, mode);
    if (fd == nullptr) {
        dbgE("%s:%d unable to open %s in mode '%s' (%s)", name_.c_str(), number_, sysfs_path, mode, strerror(errno));
    }
    return fd;
}

/**
 * Check that the c-tor parameters are correct and that the object is valid.
 *
 * @returns true if successful
 * @returns false if the sysfs path cannot be opened or be read from or if there is a mismatch in the i/o direction
 */
bool Gpio::IsValid() {
    FILE* fd = OpenSysfs("direction", "r");
    bool ret = fd != nullptr;

    if (ret) {
        char direction[4];
        if (fgets(direction, ARRAY_SIZE(direction), fd) == nullptr) {
            dbgE("%s:%d failed reading direction", name_.c_str(), number_);
        } else {
            ret = (strncmp(direction, "in", 2) == 0 && direction_ == Direction::IN) ||
                  (strncmp(direction, "out", 3) == 0 && direction_ == Direction::OUT);

            if (ret) {
                Level level;
                ret = Get(&level);
            } else {
                dbgE("%s:%d mismatch in pin direction expected: '%s' actual: '%s'",
                     name_.c_str(),
                     number_,
                     direction_ == Direction::IN ? "in" : "out",
                     direction);
            }
        }
        fclose(fd);
    }

    return ret;
}

}  // namespace lvdslink
}  // namespace hardware
}  // namespace evs
