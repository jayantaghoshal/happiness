/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <memory>
#include <string>

namespace evs {
namespace hardware {
namespace lvdslink {

/**
 * Abstraction of a gpio pin exposed through two sysfs attributes
 * telling the pin's direction and setting/getting a pins logical
 * level.
 */
class Gpio final {
  public:
    enum class Level {
        LOW,
        HIGH,
    };

    enum class Direction {
        IN,
        OUT,
    };

    /**
     * Construct Gpio object and check that its sysfs attributes exist
     * and all constructer parameters matches sysfs.
     *
     * @param[in] name      friendly name
     * @param[in] number    expected pin number in /sys/class/gpio/
     * @param[in] direction expected pin direction in /sys/class/gpio/
     *
     * @returns _Gpio_ object if successful
     * @returns nullptr if any failure
     */
    static std::unique_ptr<Gpio> Create(const std::string& name, int number, Direction direction);

    /**
     * Set gpio pin to either logical high or low.
     *
     * @param[in] level the desired gpio logical level
     *
     * @returns true if successful
     * @returns false if Direction::IN, the sysfs path cannot be opened or be written to
     */
    bool Set(Level level);

    /**
     * Get the current logical level for the gpio pin.
     *
     * @param[out] level the current gpio logical level as read from sysfs path
     *
     * @returns true if successful
     * @returns false if the sysfs path cannot be opened or be read from
     */
    bool Get(Level* level);

    Gpio(const Gpio& other) = delete;
    Gpio& operator=(const Gpio& other) = delete;
    ~Gpio() = default;

  private:
    Gpio(const std::string& name, int number, Direction direction);

    FILE* OpenSysfs(const char* attribute, const char* mode);
    bool IsValid();

    const std::string name_;
    const int number_;
    const Direction direction_;
};

}  // namespace lvdslink
}  // namespace hardware
}  // namespace evs
