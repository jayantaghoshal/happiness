/**
 * Unit test of the diagnostics_client file.
 *
 * Production code under test:
 *   ihu_cedric/carconfig_updater/src/diagnostics_client.cpp
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include "diagnostics_client.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Field;

class diagnosticsClientTestFixture : public ::testing::Test
{
   public:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(diagnosticsClientTestFixture, registerDID_givenDID_theDIDShouldBeStoredOk)
{
    std::map<uint16_t, std::vector<uint8_t>> DIDList;
    diagnosticsClient diagClient;
    diagClient.registerDID(0x01, {0xFF});

    std::vector<uint8_t> testDID;
    DIDList = diagClient.testGetDIDList();
    testDID = DIDList.at(0x01);
    ASSERT_TRUE(testDID[0] == 0xFF);
}

TEST_F(diagnosticsClientTestFixture, updateDID_givenUpdatedDID_theDIDShouldBeUpdated)
{
    std::map<uint16_t, std::vector<uint8_t>> DIDList;
    diagnosticsClient diagClient;
    diagClient.registerDID(0x01, {0xFF});
    diagClient.updateDID(0x01, {0x22});

    std::vector<uint8_t> testDID;
    DIDList = diagClient.testGetDIDList();
    testDID = DIDList.at(0x01);
    ASSERT_TRUE(testDID[0] == 0x22);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
