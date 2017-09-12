/* Copyright 2016 Delphi Automotive */

#if ENABLE_IVI_LOGGING
#include <ivi-logging.h>

// Define a context for the IVI-logging library to use.
typedef logging::DefaultLogContext LogContext;

#else
#include <iostream>
#include <sstream>

namespace internal {
class LwLogIgnore {
public:
    template<typename T>
    inline LwLogIgnore& operator<< (const T & arg) {
        (void)arg;
        return *this;
    }
};

class LwClogWithNewline {
private:
    std::stringstream ss_;

public:
    LwClogWithNewline(const char* header);

    ~LwClogWithNewline();

    template<typename T>
    inline LwClogWithNewline& operator<< (const T & arg) {
        ss_ << arg;
        return *this;
    }

};

}

#if DESIP_CLOG_VERBOSE == 1
#define log_verbose() ::internal::LwClogWithNewline("[desip][VERBO] ")
#define log_debug() ::internal::LwClogWithNewline("[desip][DEBUG] ")
#else
#define log_verbose() ::internal::LwLogIgnore()
#define log_debug() ::internal::LwLogIgnore()
#endif

#define log_info() ::internal::LwClogWithNewline("[desip][ INFO] ")
#define log_warn() ::internal::LwClogWithNewline("[desip][ WARN] ")
#define log_error() ::internal::LwClogWithNewline("[desip][ERROR] ")

#define LOG_DECLARE_DEFAULT_CONTEXT(...)
#define LOG_IMPORT_CONTEXT(...)
#define LOG_SET_DEFAULT_CONTEXT(...)
#endif
