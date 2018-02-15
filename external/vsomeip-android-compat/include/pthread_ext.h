/*
* Copyright 2017 Volvo Car Corporation
* This file is covered by LICENSE file in the root of this project
*/

#pragma once

#ifndef __USE_GNU
#define __USE_GNU 1
#endif

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PTHREAD_MUTEX_ROBUST 0
#define PTHREAD_MUTEX_STALLED 0

extern int pthread_mutexattr_setrobust(pthread_mutexattr_t *attrp, int robust);

extern int pthread_mutex_consistent(pthread_mutex_t *__mutex);


#ifdef __cplusplus
}
#endif
