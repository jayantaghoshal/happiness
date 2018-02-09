/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <gmock/gmock.h>
#include <pthread.h>

class Pthread {
  public:
    virtual int pthread_attr_init(pthread_attr_t* attr) = 0;
};

class PthreadMock : public Pthread {
  public:
    int attrInitReturnValue;
    MOCK_METHOD1(pthread_attr_init, int(pthread_attr_t* attr));
};

void pthread_mock_init(PthreadMock* mockInstance);
