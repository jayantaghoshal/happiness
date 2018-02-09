/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "dataelemvalue.h"
#include <ctime>

static long long millisecTimestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long long timestamp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;  // timestamp in millisec
    return timestamp;
}

DataElemValueBase::DataElemValueBase() : _timestamp(millisecTimestamp()), _state(State::ERROR), _errorCode(-1) {}

DataElemValueBase::DataElemValueBase(int64_t timestamp, State state, int errorCode)
    : _timestamp(timestamp == -1 ? millisecTimestamp() : timestamp), _state(state), _errorCode(errorCode) {}

DataElemValueBase::~DataElemValueBase() = default;

// State check convenience
bool DataElemValueBase::isOk() const { return _state == State::OK; }
bool DataElemValueBase::isError() const { return _state == State::ERROR; }

int64_t DataElemValueBase::timestamp() const { return _timestamp; }
DataElemValueBase::State DataElemValueBase::state() const { return _state; }
int DataElemValueBase::errorCode() const { return _errorCode; }
