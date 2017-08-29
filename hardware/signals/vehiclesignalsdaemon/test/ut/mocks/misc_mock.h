/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once

#include <gmock/gmock.h>

class Misc
{
public:
   virtual int unlink(const char *pathName) = 0;
   virtual mode_t umask(mode_t uMask) = 0;
   virtual int mkfifo(const char *pathName, mode_t mode) = 0;
   virtual int open(const char *pathName, int flags) = 0;
   virtual int select(int nfds, fd_set *readfds, fd_set *writefds,
              fd_set *exceptfds, struct timeval *timeout) = 0;
   virtual ssize_t read(int fd, void *buf, size_t count) = 0;
   virtual char *strerror(int errnum) = 0;
};

class MiscMock: public Misc
{
public:
   int selectReturnValue;
   int unlinkReturnValue;
   int mkfifoReturnValue;

   MOCK_METHOD1(unlink, int(const char *pathName));
   MOCK_METHOD1(umask, mode_t(mode_t uMask));
   MOCK_METHOD2(mkfifo, int(const char *pathName, mode_t mode));
   MOCK_METHOD2(open, int(const char *pathName, int flags));
   MOCK_METHOD5(select, int(int nfds, fd_set *readfds, fd_set *writefds,
                            fd_set *exceptfds, struct timeval *timeout));
   MOCK_METHOD3(read, ssize_t(int fd, void *buf, size_t count));
   MOCK_METHOD1(strerror, char *(int errnum));
};

void misc_mock_init(MiscMock* mockInstance);
