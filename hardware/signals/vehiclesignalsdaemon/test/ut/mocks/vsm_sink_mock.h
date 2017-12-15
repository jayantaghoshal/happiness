/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <vsm_sink.h>

class IVsmSink {
  public:
    virtual void vsm_sink_init(void) = 0;
};

class VsmSinkMock : public IVsmSink {
  public:
    MOCK_METHOD0(vsm_sink_init, void());
};

void vsm_sink_mock_init(VsmSinkMock* mockInstance);
