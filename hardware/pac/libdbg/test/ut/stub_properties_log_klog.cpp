/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/klog.h>
#include <cstring>

#include "stub_property_set.h"

#include <log/log.h>

namespace {
char property_log_level_val[PROPERTY_VALUE_MAX];
char property_logcat_val[PROPERTY_VALUE_MAX] = "running";
}  // namespace

void stub_property_reset() {
    strcpy(property_logcat_val, "running");
    strcpy(property_log_level_val, "");
}

int stub_property_set(const char* key, const char* value) {
    if (strcmp(key, PROPERTY_LOG_LEVEL) == 0) {
        snprintf(property_log_level_val, PROPERTY_VALUE_MAX, "%s", value);
    } else {
        snprintf(property_logcat_val, PROPERTY_VALUE_MAX, "%s", value);
    }

    return 0;
}

int property_get(const char* key, char* value, const char* /*default_value*/) {
    if (strcmp(key, PROPERTY_LOG_LEVEL) == 0) {
        strcpy(value, property_log_level_val);
    } else {
        strcpy(value, property_logcat_val);
    }

    return 0;
}

void klog_writev(int /* level */, const struct iovec* /* iov */, int /* iov_count */) {}
int __android_log_print(int /* prio */, const char* /* tag */, const char* /* fmt */, ...) {
    return 0;
}