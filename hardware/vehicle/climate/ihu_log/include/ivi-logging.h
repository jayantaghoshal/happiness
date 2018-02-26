/*
 * Copyright 2017, Volvo Car Corporation
*/
#ifndef IVI_LOGGING_H
#define IVI_LOGGING_H

#include <string>

namespace fsapi {

auto getDefaultContext = [](){};

}

namespace logging {

enum class LogLevel {
  None, Fatal, Error, Warning, Info, Debug, Verbose
};

enum class LogDetailLevel {
  LogCompactSpaced, LogCompact, LogVerbose
};


class LogClass
{
  private:
    void logE();
    void logW();
    void logD();
    void logI();
    void logV();
    std::string getFileName(std::string str);
    std::string severityToStr(LogLevel logLevel);

  public:
    LogClass(const char* file, const char* func, int line, LogLevel severity);
    ~LogClass();

    LogClass& writeFormatted();
    LogClass& writeFormatted(const char* fmt, ...);

    LogClass& operator<<(const char *str) {
        this->writeFormatted("%s", str);
        return *this;
    }

    LogClass& operator<<(const std::string& str) {
        this->writeFormatted("%s", str.c_str());
        return *this;
    }

    LogClass& operator<<(const int& i) {
        this->writeFormatted("%d", i);
        return *this;
    }

    LogClass& operator<<(const volatile int& i) {
        this->writeFormatted("%d", i);
        return *this;
    }

    LogClass& operator<<(void *p) {
        this->writeFormatted("%p", p);
        return *this;
    }

  protected:
    LogLevel severity_;
    std::string logstring_;
    int line_;
    std::string file_;
    std::string func_;
};

class DefaultLogContext {
    public:
    using LogData=logging::LogClass;
};

#define log_with_severity(severity, ...) logging::LogClass(__FILE__, __FUNCTION__, __LINE__, severity).writeFormatted(__VA_ARGS__)

#define log_fatal(...) log_with_severity(logging::LogLevel::Fatal, ## __VA_ARGS__)

#define log_error(...) log_with_severity(logging::LogLevel::Error, ## __VA_ARGS__)

#define log_verbose(...) log_with_severity(logging::LogLevel::Verbose, ## __VA_ARGS__)

#define log_info(...) log_with_severity(logging::LogLevel::Info, ## __VA_ARGS__)

#define log_warn(...) log_with_severity(logging::LogLevel::Warning, ## __VA_ARGS__)
#define log_warning(...) log_warn(__VA_ARGS__)

#define log_debug(...) log_with_severity(logging::LogLevel::Debug, ## __VA_ARGS__)

#define LOG_DECLARE_CONTEXT(contextName, contextShortID, contextDescription)
#define LOG_DECLARE_DEFAULT_CONTEXT(contextName, contextShortID, contextDescription)
#define LOG_IMPORT_DEFAULT_CONTEXT(context)
#define LOG_DECLARE_DEFAULT_LOCAL_CONTEXT(contextShortID, contextDescription)
#define LOG_IMPORT_CONTEXT(context)
#define LOG_SET_DEFAULT_CONTEXT(context)

namespace ConsoleLogContext
{
  void setupLogger(logging::LogLevel globalLoglevel, logging::LogDetailLevel globalDetailsLevel);
}


}

#endif // IVI-LOGGING_H
