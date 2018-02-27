/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ivi-logging.h"

#include <cutils/log.h>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#undef LOG_TAG
#define LOG_TAG "IHULOG"

#define VERBOSE_LOG(severity, file, func, line, message) "[%s] %s:%d:%s() %s\n", severity, file, line, func, message
#define COMPACT_LOG(file, func, line, message) "[%s:%s():%d] %s\n", file, func, line, message
#define COMPACT_SPACED_LOG(file, func, line, message) "\n[%s:%s():%d] %s\n", file, func, line, message

namespace logging {

static LogLevel g_log_level = LogLevel::Debug;
static LogDetailLevel g_log_detail_level = LogDetailLevel::LogCompact;

std::string LogClass::getFileName(std::string str) {
    std::string delimiter = "/";
    size_t pos = 0;

    while ((pos = str.find(delimiter)) != std::string::npos) {
        str.erase(0, pos + delimiter.length());
    }

    return str;
}

std::string LogClass::severityToStr(LogLevel logLevel) {
    std::string strLogLevel;

    switch (logLevel) {
        case LogLevel::Fatal:
            strLogLevel = "Fatal";
            break;
        case LogLevel::Error:
            strLogLevel = "Error";
            break;
        case LogLevel::Warning:
            strLogLevel = "Warning";
            break;
        case LogLevel::Debug:
            strLogLevel = "Debug";
            break;
        case LogLevel::Info:
            strLogLevel = "Info";
            break;
        case LogLevel::Verbose:
            strLogLevel = "verbose";
            break;
        default:
            strLogLevel = "None";
    }

    return strLogLevel;
}

LogClass::LogClass(const char* file, const char* func, int line, LogLevel severity) {
    severity_ = severity;
    line_ = line;
    func_ = func;

    if (g_log_detail_level == LogDetailLevel::LogVerbose) {
        file_ = file;
    } else {
        file_ = getFileName(file);
    }
}

LogClass::~LogClass() {
    if (!logstring_.empty())  // if empty don't log a thing
    {
        switch (severity_) {
            case LogLevel::Fatal:
            case LogLevel::Error:
                logE();
                break;
            case LogLevel::Warning:
                logW();
                break;
            case LogLevel::Verbose:
            case LogLevel::Info:
                logI();
                break;
            case LogLevel::Debug:
                logD();
                break;
            default:
                break;
        }
    }
}

LogClass& LogClass::writeFormatted() { return *this; }

LogClass& LogClass::writeFormatted(const char* fmt, ...) {
    if ((LogLevel::None == g_log_level) || (severity_ > g_log_level)) {
        return *this;
    }
    va_list argList;
    int result = -1, length = 256;
    char* message = nullptr;

    va_start(argList, fmt);

    while (result == -1) {
        delete[] message;
        message = new char[length + 1];
        memset(message, 0, length + 1);
        result = vsnprintf(message, length, fmt, argList);
        length *= 2;
    }
    va_end(argList);

    logstring_ += message;

    delete[] message;

    return *this;
}

void LogClass::logE() {
    switch (g_log_detail_level) {
        case LogDetailLevel::LogCompact:
            ALOGE(COMPACT_LOG(file_.c_str(), func_.c_str(), line_, logstring_.c_str()));
            break;
        case LogDetailLevel::LogCompactSpaced:
            ALOGE(COMPACT_SPACED_LOG(file_.c_str(), func_.c_str(), line_, logstring_.c_str()));
            break;
        case LogDetailLevel::LogVerbose:
            ALOGE(VERBOSE_LOG(severityToStr(severity_).c_str(), file_.c_str(), func_.c_str(), line_,
                              logstring_.c_str()));
            break;
    }
}

void LogClass::logW() {
    switch (g_log_detail_level) {
        case LogDetailLevel::LogCompact:
            ALOGW(COMPACT_LOG(file_.c_str(), func_.c_str(), line_, logstring_.c_str()));
            break;
        case LogDetailLevel::LogCompactSpaced:
            ALOGW(COMPACT_SPACED_LOG(file_.c_str(), func_.c_str(), line_, logstring_.c_str()));
            break;
        case LogDetailLevel::LogVerbose:
            ALOGW(VERBOSE_LOG(severityToStr(severity_).c_str(), file_.c_str(), func_.c_str(), line_,
                              logstring_.c_str()));
            break;
    }
}

void LogClass::logD() {
    switch (g_log_detail_level) {
        case LogDetailLevel::LogCompact:
            ALOGD(COMPACT_LOG(file_.c_str(), func_.c_str(), line_, logstring_.c_str()));
            break;
        case LogDetailLevel::LogCompactSpaced:
            ALOGD(COMPACT_SPACED_LOG(file_.c_str(), func_.c_str(), line_, logstring_.c_str()));
            break;
        case LogDetailLevel::LogVerbose:
            ALOGD(VERBOSE_LOG(severityToStr(severity_).c_str(), file_.c_str(), func_.c_str(), line_,
                              logstring_.c_str()));
            break;
    }
}

void LogClass::logI() {
    switch (g_log_detail_level) {
        case LogDetailLevel::LogCompact:
            ALOGI(COMPACT_LOG(file_.c_str(), func_.c_str(), line_, logstring_.c_str()));
            break;
        case LogDetailLevel::LogCompactSpaced:
            ALOGI(COMPACT_SPACED_LOG(file_.c_str(), func_.c_str(), line_, logstring_.c_str()));
            break;
        case LogDetailLevel::LogVerbose:
            ALOGI(VERBOSE_LOG(severityToStr(severity_).c_str(), file_.c_str(), func_.c_str(), line_,
                              logstring_.c_str()));
            break;
    }
}

namespace ConsoleLogContext {

void setupLogger(logging::LogLevel globalLoglevel, logging::LogDetailLevel globalDetailsLevel) {
    g_log_level = globalLoglevel;
    g_log_detail_level = globalDetailsLevel;
}
}
}  // namespace logging