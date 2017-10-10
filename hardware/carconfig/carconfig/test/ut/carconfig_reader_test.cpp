/**
 * Unit test of the CarConfigFileReader class.
 *
 * Production code under test:
 *   ihu_cedric/carconfig/src/carconfig_reader.cpp
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "carconfig_reader.h"
#include "carconfig_writer.h"
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>

namespace
{
bool fileExists(std::string const& filename)
{
    struct stat buffer;

    return (stat(filename.c_str(), &buffer) == 0);
}
}

class CarConfigReaderTestFixture : public ::testing::Test
{
   public:
    virtual void SetUp()
    {
        std::stringstream tmp;
        tmp << _CSVFOLDER;
        csvPath = tmp.str();

        CarConfigFileWriter writer = CarConfigFileWriter(csvPath + "carconfig_file_ok.csv");
        writer.commitToBinaryMemory();
    }

    virtual void TearDown() {}

   protected:
    std::string csvPath;
    CarConfigFileReader rd;
};

TEST_F(CarConfigReaderTestFixture, read_GivenNonExistingRamFile_errorShouldBeReturned)
{
    auto const shmFile = "/dev/shm/carconfig";
    if (fileExists(shmFile))
    {
        std::remove(shmFile);
    }

    EXPECT_THROW(rd.read(), std::runtime_error);
}

TEST_F(CarConfigReaderTestFixture, read_GivenExistingRamFile_OkShouldBeReturned)
{
    rd.read();
    ASSERT_TRUE(rd.isRead() == 1);
}

TEST_F(CarConfigReaderTestFixture, getValue_GivenValidParameter_ValueShouldBeReturned)
{
    rd.read();
    ASSERT_TRUE(rd.getValue(1) == 1);
}

TEST_F(CarConfigReaderTestFixture, getRawValue_GivenValidParameter_ValueShouldBeReturned)
{
    rd.read();
    ASSERT_TRUE(rd.getRawValue(1) == 1);
}

TEST_F(CarConfigReaderTestFixture, getValue_GivenParameterOutOfMaxRange_ExceptionShouldBeThown)
{
    rd.read();
    ASSERT_THROW(rd.getValue(0), std::out_of_range);
    ASSERT_THROW(rd.getValue(1779), std::out_of_range);
}

TEST_F(CarConfigReaderTestFixture, getRawValue_GivenParameterOutOfMaxRange_ExceptionShouldBeThown)
{
    rd.read();
    ASSERT_THROW(rd.getRawValue(0), std::out_of_range);
    ASSERT_THROW(rd.getRawValue(1779), std::out_of_range);
}

TEST_F(CarConfigReaderTestFixture, getValue_GivenValidParameterWithoutReadingFirst_ExceptionShouldBeThown)
{
    ASSERT_THROW(rd.getValue(1), std::runtime_error);
}

TEST_F(CarConfigReaderTestFixture, getRawValue_GivenValidParameterWithoutReadingFirst_ExceptionShouldBeThown)
{
    ASSERT_THROW(rd.getRawValue(1), std::runtime_error);
}

TEST_F(CarConfigReaderTestFixture, isRead_BeforeAndAfterReading)
{
    ASSERT_FALSE(rd.isRead());
    rd.read();
    ASSERT_TRUE(rd.isRead());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
