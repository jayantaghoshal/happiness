/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "pipe.h"

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>

Pipe::Pipe(Pipe &&other) noexcept { fds.swap(other.fds); }

Pipe::~Pipe() { Close(); }

Pipe &Pipe::operator=(Pipe &&other) noexcept {
  Close();
  fds.swap(other.fds);
  return *this;
}

int Pipe::Open() {
  if (IsOpen()) return true;

  return pipe2(fds.data(), O_CLOEXEC | O_NONBLOCK);
}

void Pipe::Close() {
  for (int &fd : fds) {
    if (fd != -1) {
      close(fd);
      fd = -1;
    }
  }
}

ssize_t Pipe::Read(void *data, std::size_t data_size) const {
  ssize_t ret;

  while (true) {
    ret = read(ReadFD(), data, data_size);

    if (ret >= 0) break;
    if (errno != EINTR) break;
  }

  return ret;
}

ssize_t Pipe::Write(const void *data, std::size_t data_size) const {
  ssize_t ret;

  while (true) {
    ret = write(WriteFD(), data, data_size);

    if (ret >= 0) break;
    if (errno != EINTR) break;
  }

  return ret;
}
