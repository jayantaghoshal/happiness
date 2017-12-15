/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <gen_dataelements.h>
#include <fstream>
#include <iostream>
#include <string>

/*
 * Log signals to a ring-like buffer.
 *
 */
class SignalLog {
  public:
    SignalLog(const std::string& logFile0, const std::string& logFile1);
    void log(const char* signalName, const char* value, autosar::Dir direction);

  private:
    SignalLog();

    void setActiveLogFile(int newActiveLogFileIndex);

    const std::string _logFiles[2];  // The two files used for logging
    int _activeLogFileIndex;
    std::ofstream _activeLogFileStream;
    const int MAX_LOG_SIZE;
};
