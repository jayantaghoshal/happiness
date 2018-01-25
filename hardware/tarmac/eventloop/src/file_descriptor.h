/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

class FileDescriptor {
  public:
    FileDescriptor(int fd);
    ~FileDescriptor();

    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor(FileDescriptor&&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;

    operator int() const { return fd_; }

  private:
    int fd_;
};
