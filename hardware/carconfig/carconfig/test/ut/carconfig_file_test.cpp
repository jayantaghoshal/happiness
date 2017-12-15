/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/**
 * Unit test of the CarconfigFile class.
 *
 * Production code under test:
 *   ihu_cedric/carconfig/src/carconfig_reader.cpp
 *
 */

#include "carconfig_file.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

class CarConfigFileTestFixture : public ::testing::Test {
  public:
    virtual void SetUp() {
        std::stringstream tmp;
        tmp << _CSVFOLDER;
        csvPath = tmp.str();

        // Copy a CSV file for use in setValue tests
        std::ifstream source(csvPath + "carconfig_file_ok.csv", std::ios::binary);
        std::ofstream dest(csvPath + "carconfig_file_out.csv", std::ios::binary);
        dest << source.rdbuf();
        source.close();
        dest.close();
    }

    virtual void TearDown() {}

  protected:
    CarconfigFile cc;
    ccValue value;
    std::string csvPath;
};

TEST_F(CarConfigFileTestFixture, getValue_givenOkFileParameter_StatusShouldBeGood) {
    cc.open(csvPath + "carconfig_file_ok.csv");
    value = cc.getValue(1);
    ASSERT_TRUE(value.raw == 1);
    ASSERT_TRUE(value.status == ccStatus::GOOD);
}

TEST_F(CarConfigFileTestFixture, getValue_givenOkFileParameter_StatusShouldBeInvalid) {
    cc.open(csvPath + "carconfig_file_ok.csv");
    value = cc.getValue(2);
    ASSERT_TRUE(value.raw == 2);
    ASSERT_TRUE(value.status == ccStatus::INVALID);
}

TEST_F(CarConfigFileTestFixture, getValue_givenValidFileButOutOfRangeParameter_StatusShouldBeOutOfRange) {
    cc.open(csvPath + "carconfig_file_ok.csv");
    ASSERT_THROW(cc.getValue(1779), std::runtime_error);
}

TEST_F(CarConfigFileTestFixture, getValue_givenBadFileParameter_RawValueShouldBeOutOfRange) {
    cc.open(csvPath + "carconfig_file_out_of_range.csv");
    ASSERT_THROW(cc.getValue(1), std::runtime_error);
}

TEST_F(CarConfigFileTestFixture, getValue_givenBadFileParameter_SubstituteValueShouldBeOutOfRange) {
    cc.open(csvPath + "carconfig_file_out_of_range.csv");
    ASSERT_THROW(cc.getValue(2), std::runtime_error);
}

TEST_F(CarConfigFileTestFixture, getValue_givenBadFileParameter_StatusValueShouldBeOutOfRange) {
    cc.open(csvPath + "carconfig_file_out_of_range.csv");
    ASSERT_THROW(cc.getValue(3), std::runtime_error);
}

TEST_F(CarConfigFileTestFixture, getValue_givenBadFileParameter_StatusValueShouldBeUndefined) {
    cc.open(csvPath + "carconfig_file_out_of_range.csv");
    ASSERT_THROW(cc.getValue(4), std::runtime_error);
}

TEST_F(CarConfigFileTestFixture, setValue_givenGoodValue_OutPutShouldBeWrittenCorrectly) {
    cc.open(csvPath + "carconfig_file_out.csv");
    value = cc.getValue(1);
    ASSERT_TRUE(value.raw == 1);

    ccValue tmpValue;
    tmpValue.raw = 10;
    tmpValue.subs = 3;
    tmpValue.status = ccStatus::GOOD;
    cc.setValue(1, tmpValue);

    tmpValue.raw = 11;
    cc.setValue(4, tmpValue);

    value = cc.getValue(1);
    ASSERT_TRUE(value.raw == 10);
    ASSERT_TRUE(value.subs == 3);
    ASSERT_TRUE(value.status == ccStatus::GOOD);

    value = cc.getValue(2);
    ASSERT_TRUE(value.raw == 2);

    value = cc.getValue(4);
    ASSERT_TRUE(value.raw == 11);

    value = cc.getValue(1778);
    ASSERT_TRUE(value.raw == 254);

    // Close the file to make sure the changes aren't caught in a buffer.
    cc.close();
    value = cc.getValue(1);
    ASSERT_TRUE(value.raw == 10);

    value = cc.getValue(2);
    ASSERT_TRUE(value.raw == 2);

    value = cc.getValue(1778);
    ASSERT_TRUE(value.raw == 254);
}

TEST_F(CarConfigFileTestFixture, open_givenFile_isMissing) {
    ASSERT_THROW(cc.open(csvPath + "carconfig_file_xxx.csv"), std::runtime_error);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
