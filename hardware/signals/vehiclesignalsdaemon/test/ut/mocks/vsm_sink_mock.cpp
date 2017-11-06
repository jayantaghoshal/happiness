/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include <gmock/gmock.h>
#include <vsm_sink_mock.h>

static VsmSinkMock* vsmSinkMock;

void vsm_sink_mock_init(VsmSinkMock* mockInstance) { vsmSinkMock = mockInstance; }

void vsm_sink_init(void) {
    ASSERT_TRUE(vsmSinkMock != nullptr);
    vsmSinkMock->vsm_sink_init();
}
