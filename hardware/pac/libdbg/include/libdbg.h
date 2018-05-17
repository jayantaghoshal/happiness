/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstring>

#ifndef LOG_TAG
#error Please define LOG_TAG!
#endif

#include <log/log.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_LINE_MESSAGE_SIZE 512
#define LOG_LINE_PREFIX_SIZE 128
#define LOG_LINE_TAG_SIZE 128

#define LOG_LINE_SIZE LOG_LINE_PREFIX_SIZE + LOG_LINE_MESSAGE_SIZE

#ifdef LIBDBG_ENABLE_DEBUG
#define dbgF(message, ...) \
    pdgb(LOG_TAG, ANDROID_LOG_FATAL, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define dbgE(message, ...) \
    pdgb(LOG_TAG, ANDROID_LOG_ERROR, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define dbgW(message, ...) pdgb(LOG_TAG, ANDROID_LOG_WARN, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define dbgD(message, ...) \
    pdgb(LOG_TAG, ANDROID_LOG_DEBUG, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define dbgI(message, ...) pdgb(LOG_TAG, ANDROID_LOG_INFO, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#define dbgV(message, ...) \
    pdgb(LOG_TAG, ANDROID_LOG_VERBOSE, __FILENAME__, __FUNCTION__, __LINE__, message, ##__VA_ARGS__)
#else
#define dbgF(message, ...)
#define dbgW(message, ...)
#define dbgE(message, ...)
#define dbgD(message, ...)
#define dbgI(message, ...)
#define dbgV(message, ...)
#endif

// Tell the compiler that the function uses the printf format style and that the 6th argument is the
// format string and the 7th the first format argument to check. This makes it possible to validate
// the format string passed to the function in compile time with the -Wformat flag.
__attribute__((__format__(__printf__, 6, 7))) bool pdgb(const char* log_tag,
                                                        android_LogPriority log_prio,
                                                        const char* file_name,
                                                        const char* function_name,
                                                        int line,
                                                        const char* message_format,
                                                        ...);
