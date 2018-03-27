/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "libdbg.h"

#include <cutils/properties.h>
#include <cstdarg>
#include <cstdio>

#define ENUM_ANDROID_LOG_LEVEL_LAST_ITEM 8
#define PROPERTY_LOG_LEVEL "debug.pac.loglevel"
#define WRONG_LOG_LEVEL_MESSAGE " [WRONG LOG_LEVEL] "

namespace {
constexpr char const* kPossibleLogLevels[] = {"verbose", "debug", "info", "warn", "error", "fatal"};
constexpr int length = sizeof(kPossibleLogLevels) / sizeof(*kPossibleLogLevels);
}  // namespace

bool pdgb(const char* log_tag,
          android_LogPriority log_prio,
          const char* file_name,
          const char* function_name,
          int line,
          const char* message_format,
          ...) {
    char property_log_level[PROPERTY_VALUE_MAX];
    int log_level = 0;

    property_get(PROPERTY_LOG_LEVEL, property_log_level, nullptr);

    for (int i = 0; i < length; i++) {
        if (strcmp(property_log_level, kPossibleLogLevels[i]) == 0) {
            log_level = i + ANDROID_LOG_VERBOSE;
            break;
        }
    }

    if (log_prio < log_level) {
        return false;
    }

    va_list valist;
    char message[LOG_LINE_MESSAGE_SIZE];
    char log_line_number[LOG_LINE_SIZE];

    va_start(valist, message_format);
    vsnprintf(message, LOG_LINE_MESSAGE_SIZE, message_format, valist);
    va_end(valist);

    sprintf(log_line_number,
            "%s%s:%s():%d: %s",
            (ANDROID_LOG_SILENT != ENUM_ANDROID_LOG_LEVEL_LAST_ITEM) ? WRONG_LOG_LEVEL_MESSAGE : "",
            file_name,
            function_name,
            line,
            message);
    LOG_PRI(log_prio, log_tag, "%s", log_line_number);

    return true;
}
