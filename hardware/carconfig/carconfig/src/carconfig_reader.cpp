#include <string.h>
#include <fstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdexcept>
#include "carconfig_reader.h"

uint8_t CarConfigFileReader::getValue(uint32_t position)
{
    if (!isRead())
    {
        throw std::runtime_error{"Trying to get value without calling cedric::core::carconfig::init() first!"};
    }
    if ((position < 1) || (position > cc_no_of_variables))
    {
        throw std::out_of_range{"Trying to access carconfig value " + std::to_string(position) +
                                 ", which is out of range"};
    }
    return addr[(position - 1) * sizeof(carConfigObject)];
}

uint8_t CarConfigFileReader::getRawValue(uint32_t position)
{
    if (!isRead())
    {
        throw std::runtime_error{"Trying to get RAW value without calling cedric::core::carconfig::init() first!"};
    }
    if ((position < 1) || (position > cc_no_of_variables))
    {
        throw std::out_of_range{"Trying to access carconfig raw value " + std::to_string(position) +
                                 ", which is out of range"};
    }
    return addr[(position - 1) * sizeof(carConfigObject) + 1];
}

void CarConfigFileReader::read()
{
    int const fd = shm_open(shm_file_name.c_str(), O_RDONLY, 0777);
    if (fd == -1)
    {
        throw std::runtime_error{ "shm_open: " + std::string{ strerror(errno) } };
    }

    addr = (char *)mmap(0, bufferSize(), PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    if (addr == (void *)-1)
    {
        addr = nullptr;
        throw std::runtime_error{ "mmap failed: " + std::string{ strerror(errno) }};
    }
}

uint8_t CarConfigFileReader::isRead() { return addr != nullptr; }
