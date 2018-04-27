/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gmock/gmock.h>
#include <vsm_inject_mock.h>

static VsmInjectMock* vsmInjectMock;

void vsm_inject_mock_init(VsmInjectMock* mockInstance) {
    vsmInjectMock = mockInstance;
}

void vsm_inject_init(void) {
    ASSERT_TRUE(vsmInjectMock != nullptr);
    vsmInjectMock->vsm_inject_init();
}

void vsm_inject_start(void) {
    ASSERT_TRUE(vsmInjectMock != nullptr);
    vsmInjectMock->vsm_inject_start();
}

void vsm_inject_inject(uint16_t signalId, void* message, bool injectError, const uint32_t length) {
    ASSERT_TRUE(vsmInjectMock != nullptr);
    vsmInjectMock->vsm_inject_inject(signalId, message, injectError, length);
}
