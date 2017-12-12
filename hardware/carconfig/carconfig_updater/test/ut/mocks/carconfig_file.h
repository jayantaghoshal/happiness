/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef ICARCONFIG_FILE_H
#define ICARCONFIG_FILE_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

/*
 * The carconfig backend file is based on the CSV "standard"
 * The file should have 504 parameters according to the
 * following format:
 *  Raw Value,Substitute Value,Status flag
 * Example:
 *  255,255,0
 *
 * Each parameter can have a value of 0 to 255.
 * */

enum class ccStatus { GOOD = 0, INVALID = 1 };

typedef struct {
    unsigned char raw;
    unsigned char subs;
    ccStatus status;
} ccValue;

class ICarconfigFile {
  public:
    virtual ~ICarconfigFile();
    virtual void open(const std::string path) = 0;
    virtual void close() = 0;
    virtual ccValue getValue(int id) = 0;
    virtual bool setValue(uint32_t id, ccValue value) = 0;
};

class CarconfigFile : public ICarconfigFile {
  public:
    MOCK_METHOD1(open, void(const std::string));
    MOCK_METHOD0(close, void());
    MOCK_METHOD1(getValue, ccValue(int));
    MOCK_METHOD2(setValue, bool(uint32_t, ccValue));
};

#endif  // CARCONFIG_FILE_H
