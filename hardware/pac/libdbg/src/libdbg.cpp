/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "libdbg.h"

#include <cutils/klog.h>
#include <cutils/properties.h>

#include "property_names.h"

#define ENUM_ANDROID_LOG_LEVEL_LAST_ITEM 8
#define WRONG_LOG_LEVEL_MESSAGE " [WRONG LOG_LEVEL] "
// log tag has to be added since klog uses no log tag argument
#define KLOG_LINE_SIZE LOG_LINE_SIZE + LOG_LINE_TAG_SIZE

namespace {

constexpr char const* kPossibleLogLevels[] = {"verbose", "debug", "info", "warn", "error", "fatal"};

constexpr int kNumLogLevels = sizeof(kPossibleLogLevels) / sizeof(*kPossibleLogLevels);

constexpr char const* kLogcatStatusRunning = "running";

bool IsLogcatRunning() {
    char property_logcat_status[PROPERTY_VALUE_MAX];

    property_get(PROPERTY_LOGCAT_STATUS, property_logcat_status, nullptr);

    return strcmp(property_logcat_status, kLogcatStatusRunning) == 0;
}

int ClampToKlogLevel(const int android_log_prio) {
    return android_log_prio > ANDROID_LOG_WARN ? KLOG_ERROR_LEVEL : KLOG_WARNING_LEVEL;
}

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

    for (int i = 0; i < kNumLogLevels; i++) {
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
    char log_line_msg[LOG_LINE_SIZE];

    va_start(valist, message_format);
    vsnprintf(message, LOG_LINE_MESSAGE_SIZE, message_format, valist);
    va_end(valist);

    snprintf(log_line_msg,
             LOG_LINE_SIZE,
             "%s%s:%s():%d: %s",
             (ANDROID_LOG_SILENT != ENUM_ANDROID_LOG_LEVEL_LAST_ITEM) ? WRONG_LOG_LEVEL_MESSAGE : "",
             file_name,
             function_name,
             line,
             message);

    // Make sure log tag is within size limit
    char log_tag_truncated[LOG_LINE_TAG_SIZE];
    snprintf(log_tag_truncated, LOG_LINE_TAG_SIZE, "%s", log_tag);

    // If logcat is not running klog has to be used
    if (!IsLogcatRunning()) {
        // klog only accounts for warn, error and fatal messages
        if (log_prio < ANDROID_LOG_WARN || log_prio > ANDROID_LOG_FATAL) {
            return false;
        }

        // log tag has to be added since klog uses no log tag argument
        char klog_line_msg[KLOG_LINE_SIZE];
        snprintf(klog_line_msg, KLOG_LINE_SIZE, "%s: %s", log_tag_truncated, log_line_msg);

        struct iovec iov[1];
        iov[0].iov_base = klog_line_msg;
        iov[0].iov_len = strlen(klog_line_msg);

        klog_writev(ClampToKlogLevel(log_prio), iov, 1);

    } else /*if logcat is running*/ {
        LOG_PRI(log_prio, log_tag_truncated, "%s", log_line_msg);
    }

    return true;
}
