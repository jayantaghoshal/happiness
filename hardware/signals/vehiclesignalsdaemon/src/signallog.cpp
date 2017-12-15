/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "signallog.h"
#include <cutils/log.h>
#undef LOG_TAG
#define LOG_TAG "VSD"

SignalLog::SignalLog(const std::string& logFile0, const std::string& logFile1)
    : _logFiles{logFile0, logFile1}, MAX_LOG_SIZE(200000) {
    setActiveLogFile(0);
}

void SignalLog::log(const char* signalName, const char* value, autosar::Dir direction) {
    const char* dirString;
    switch (direction) {
        case autosar::Dir::IN:
            dirString = "<-  ";
            break;

        case autosar::Dir::OUT:
            dirString = "->  ";
            break;

        case autosar::Dir::INTERNAL:
            dirString = "[internal]  ";
            break;
    }

    if (_activeLogFileStream.is_open()) {
        // TODO should we log a timestamp looking like the one in dlt??
        _activeLogFileStream << dirString << signalName << "\t" << value << std::endl;
        if (_activeLogFileStream.tellp() > MAX_LOG_SIZE) {  // time to switch file
            setActiveLogFile(_activeLogFileIndex == 0 ? 1 : 0);
        }
    }
}

void SignalLog::setActiveLogFile(int newActiveLogFileIndex) {
    _activeLogFileIndex = newActiveLogFileIndex;
    _activeLogFileStream.close();  // close() will silently fail if no file is open

    // open new file for writing and truncate it
    _activeLogFileStream.open(_logFiles[newActiveLogFileIndex].c_str(), std::ios::trunc);
    if (!_activeLogFileStream.is_open()) {
        // No further attempts are made to open log files
        ALOGE("Failed to open signal log file: %s", _logFiles[newActiveLogFileIndex].c_str());
    }
}
