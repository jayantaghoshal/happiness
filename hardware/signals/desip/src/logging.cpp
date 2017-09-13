#ifndef ENABLE_IVI_LOGGING

#include "logging.h"

#include <chrono>
#include <iomanip>

namespace internal {
static void LogWriteTime(std::ostream& os) {
    auto milliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now())
                                .time_since_epoch()
                                .count();
    os << std::setfill(' ') << std::setw(16) << milliseconds;
}

LwClogWithNewline::LwClogWithNewline(const char* header) {
    LogWriteTime(ss_);
    ss_ << header;
}

LwClogWithNewline::~LwClogWithNewline(){
    std::clog << ss_.rdbuf() << std::endl;
}

}

#endif
