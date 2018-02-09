/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <misc_mock.h>

static MiscMock* miscMock;

extern "C" {
extern int openReturnValue;
extern bool openCalledFromTest;
extern bool readCalledFromTest;
extern int readCalled;
extern ssize_t readReturnValue;
extern int __real_open(const char* pathname, int flags, mode_t mode);
extern ssize_t __real_read(int fd, void* buf, size_t count);
}

void misc_mock_init(MiscMock* mockInstance) { miscMock = mockInstance; }

int unlink(const char* pathName) {
    miscMock->unlink(pathName);
    return miscMock->unlinkReturnValue;
}

mode_t umask(mode_t uMask) {
    miscMock->umask(uMask);
    return 0;
}

int mkfifo(const char* pathName, mode_t mode) {
    miscMock->mkfifo(pathName, mode);
    return miscMock->mkfifoReturnValue;
}

int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout) {
    miscMock->select(nfds, readfds, writefds, exceptfds, timeout);
    return miscMock->selectReturnValue--;
}

char* strerror(int errnum) {
    miscMock->strerror(errnum);
    return nullptr;
}

extern "C" {
int __wrap_open(const char* pathName, int flags, mode_t mode) {
    if (openCalledFromTest) {
        return openReturnValue;
    } else {
        return __real_open(pathName, flags, mode);
    }
}

ssize_t __wrap_read(int fd, void* buf, size_t count) {
    if (readCalledFromTest) {
        readCalled++;
        return readReturnValue--;
    } else {
        return __real_read(fd, buf, count);
    }
}
} /* close off extern "C" specification */
