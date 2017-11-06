/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include <gmock/gmock.h>
#include <vsm_inject.h>

class IVsmInject {
  public:
    virtual void vsm_inject_init(void) = 0;

    virtual void vsm_inject_start(void) = 0;

    virtual void vsm_inject_inject(uint16_t signalId, void* message, bool injectError, const uint32_t length) = 0;
};

class VsmInjectMock : public IVsmInject {
  public:
    MOCK_METHOD0(vsm_inject_init, void());
    MOCK_METHOD0(vsm_inject_start, void());
    MOCK_METHOD4(vsm_inject_inject, void(uint16_t, void*, bool, const uint32_t));
};

void vsm_inject_mock_init(VsmInjectMock* mockInstance);
