/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelemvalue.h"
#include <time.h>

static long long millisecTimestamp(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long long timestamp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;  // timestamp in millisec
    return timestamp;
}

DataElemValueBase::DataElemValueBase() : _timestamp(millisecTimestamp()), _state(State::ERROR), _errorCode(-1) {}

DataElemValueBase::DataElemValueBase(long long timestamp, State state, int errorCode)
    : _timestamp(timestamp == -1 ? millisecTimestamp() : timestamp), _state(state), _errorCode(errorCode) {}

DataElemValueBase::~DataElemValueBase() {}

// State check convenience
bool DataElemValueBase::isOk() const { return _state == State::OK; }
bool DataElemValueBase::isError() const { return _state == State::ERROR; }

long long DataElemValueBase::timestamp() const { return _timestamp; }
DataElemValueBase::State DataElemValueBase::state() const { return _state; }
int DataElemValueBase::errorCode() const { return _errorCode; }
