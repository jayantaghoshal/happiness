/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <vsm_sink_mock.h>

static VsmSinkMock* vsmSinkMock;

void vsm_sink_mock_init(VsmSinkMock* mockInstance) { vsmSinkMock = mockInstance; }

void vsm_sink_init(void) {
    ASSERT_TRUE(vsmSinkMock != nullptr);
    vsmSinkMock->vsm_sink_init();
}
