#pragma once

#include <string>
#include <array>
#include <sys/stat.h>

namespace Carconfig_base
{

constexpr int cc_no_of_parameters = 1778;

// The internal type that represent 504 received carconfig frames
typedef struct
{
    bool received = false;
    bool ok = false;
    uint8_t value = 0;
    bool subscribed = false;
} ccFlexrayValue;

using CarConfigList = std::array<ccFlexrayValue, cc_no_of_parameters>;

/*
 * The carconfig backend file is based on the CSV "standard"
 * The file should have 1778 parameters according to the
 * following format:
 *  Raw Value,Substitute Value,Status flag
 * Example:
 *  255,255,0
 *
 * Each parameter can have a value of 0 to 255.
 * */
enum class ccStatus : uint8_t
{
    GOOD = 0,
    INVALID = 1
};

typedef struct
{
    uint8_t raw = 0x00;
    uint8_t subs = 0x00;
    ccStatus status;
} ccValue;

const std::string carconfig_file_name{"/oem_config/carconfig.csv"};

// Default carconfig for first time execution in a vehicle
const std::string carconfig_default_file_name{"/vendor/etc/config/carconfig-default.csv"};

const std::string carconfig_configured_filename {"/oem_config/CARCONFIG_CONFIGURED"};

static bool fileExists(const std::string &filePath)
{
    struct stat st;
    bool status = false;
    if (stat(filePath.c_str(), &st) == 0)
    {
        if (st.st_size > 0)
        {
            status = true;
        }
    }
    return status;
}

static bool emptyFileExists(const std::string &filePath)
{
    struct stat st;
    return stat(filePath.c_str(), &st) == 0;
}

}