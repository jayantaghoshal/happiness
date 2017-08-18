/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "pipe_mock.h"
#include "pipe.h"
#include <gmock/gmock.h>

static PipeMock* _mock;

void PipeMock::setObj(PipeMock* mock)
{
    _mock = mock;
}

Pipe::~Pipe()
{
}

int Pipe::Open()
{
    return _mock->Open();
}

void Pipe::Close()
{
    return _mock->Close();
}

ssize_t Pipe::Read(void* data, std::size_t data_size) const
{
    return _mock->Read(data, data_size);
}

ssize_t Pipe::Write(const void* data, std::size_t data_size) const
{
    return _mock->Write(data, data_size);
}
