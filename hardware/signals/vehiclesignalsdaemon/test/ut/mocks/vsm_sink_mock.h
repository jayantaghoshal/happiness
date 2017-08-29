/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include <vsm_sink.h>
#include <gmock/gmock.h>

class IVsmSink
{
public:
   virtual void vsm_sink_init(void) = 0;
};

class VsmSinkMock: public IVsmSink
{
public:
   MOCK_METHOD0(vsm_sink_init, void());
};

void vsm_sink_mock_init(VsmSinkMock* mockInstance);
