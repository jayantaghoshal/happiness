/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "file_descriptor.h"

#include <memory>
#include <sstream>
#include <stdexcept>

#include <unistd.h>

FileDescriptor::FileDescriptor(int fd) : fd_(fd) {
    if (fd < 0) {
        std::stringstream err_str;
        err_str << "File descriptor is invalid. errno=" << errno << ", " << strerror(errno);
        throw std::runtime_error(err_str.str());
    }
}

FileDescriptor::~FileDescriptor() {
    if (fd_ > -1) {
        close(fd_);
    }
}
