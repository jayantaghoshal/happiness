/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include <gmock/gmock.h>
#include <pthread_mock.h>

static PthreadMock *pthreadMock;

extern "C" {
extern int createReturnValue;
extern bool createCalledFromTest;
extern int createCalled;
extern int __real_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *),
                                 void *arg);
}

void pthread_mock_init(PthreadMock *mockInstance) { pthreadMock = mockInstance; }

int pthread_attr_init(pthread_attr_t *attr) {
    pthreadMock->pthread_attr_init(attr);
    return pthreadMock->attrInitReturnValue;
}

extern "C" {
int __wrap_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
    if (createCalledFromTest) {
        createCalled++;
        return createReturnValue;
    } else {
        return __real_pthread_create(thread, attr, start_routine, arg);
    }
}
}
