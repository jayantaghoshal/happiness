/*
* Copyright 2017 Volvo Car Corporation
* This file is covered by LICENSE file in the root of this project
*/

#include "pthread_ext.h"

// NOTE: Android doesn't support robust pthread, and it is ok to return 0

int pthread_mutexattr_setrobust(pthread_mutexattr_t */*__mutex_t*/, int /*robust*/)
{
	return 0;
}

int pthread_mutex_consistent (pthread_mutex_t */*__mutex*/)
{
    return 0;
}

