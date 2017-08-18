/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "pipe.h"
#include <gmock/gmock.h>

class PipeMock : public Pipe
{
public:
    MOCK_METHOD0(Open, int());
    MOCK_METHOD0(Close, void());
    MOCK_METHOD2(Read, ssize_t(void* data, std::size_t data_size));
    MOCK_METHOD2(Write, ssize_t(const void* data, std::size_t data_size));

    static void setObj(PipeMock* mock);

    virtual ~PipeMock()
    {
    }
};
