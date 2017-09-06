/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#ifndef SRC_PIPE_H__
#define SRC_PIPE_H__

#include <sys/types.h>

#include <array>
#include <cstddef>

class Pipe
{
public:
    Pipe()                  = default;
    Pipe(const Pipe& other) = delete;
    Pipe(Pipe&& other) noexcept;

    ~Pipe();

    Pipe& operator=(const Pipe& other) = delete;
    Pipe& operator                     =(Pipe&& other) noexcept;

    virtual int  Open();
    virtual void Close();

    bool IsOpen() const
    {
        return fds[0] != -1;
    }

    int ReadFD() const
    {
        return fds[0];
    }

    int WriteFD() const
    {
        return fds[1];
    }

    ssize_t Read(void* data, std::size_t data_size) const;
    ssize_t Write(const void* data, std::size_t data_size) const;

private:
    std::array<int, 2> fds = { { -1, -1 } };
};

#endif // SRC_PIPE_H__
