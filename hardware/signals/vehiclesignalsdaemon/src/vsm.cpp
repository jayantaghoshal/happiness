/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <reuse.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vsm.h>
#include <vsm_inject.h>

bool avmpVersionCheckOk = false;

void vsm_init(void) { vsm_inject_init(); }
void vsm_deInit(void) {}
void vsm_start(void) { vsm_inject_start(); }
