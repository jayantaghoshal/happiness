/**
 * Unit test of the CarConfigFileWriter class.
 *
 * Production code under test:
 *   ihu_cedric/carconfig/src/carconfig_writer.cpp
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "carconfig_writer.h"
#include "carconfig_reader.h"
#include <sstream>
#include <unistd.h>

class CarConfigWriterTestFixture : public ::testing::Test
{
   public:
    virtual void SetUp()
    {
        std::stringstream tmp;
        tmp << _CSVFOLDER;
        csvPath = tmp.str();

        CarConfigFileWriter writer = CarConfigFileWriter(csvPath + "carconfig_file_ok.csv");
        writer.commitToSharedMemory();

        shmFile = fopen("/dev/shm/carconfig", "rb");
        ASSERT_TRUE(shmFile);
    }

    virtual void TearDown() {}

   protected:
    FILE *shmFile = nullptr;
    std::string csvPath;
};

TEST_F(CarConfigWriterTestFixture,
       commitToSharedMemory_GivenGoodFileWithStatusFLagGood_SharedMemoryFilePositionShouldBeRealValue)
{
    char result[2];

    // Status flag is GOOD so the value should be 1 and the raw value the same.
    fread(result, 1, 2, shmFile);
    ASSERT_TRUE((result[0] & 0xFF) == 0x01);
    ASSERT_TRUE((result[1] & 0xFF) == 0x01);
}

TEST_F(CarConfigWriterTestFixture,
       commitToSharedMemory_GivenGoodFileWithStatusFLagBad_SharedMemoryFilePositionShouldBeRawValue)
{
    char result[2];

    // Status  flag is BAD so the value should be 255 and the raw value
    // the actual value.
    fseek(shmFile, 2, 0);
    fread(result, 1, 2, shmFile);
    ASSERT_TRUE((result[0] & 0xFF) == 0xFF);
    ASSERT_TRUE((result[1] & 0xFF) == 0x02);
}

TEST_F(CarConfigWriterTestFixture, commitToSharedMemory_GivenGoodFile_SharedMemoryFileShouldBeOk)
{
    char result[2];

    // Check the last position of the file. The file is 1778 long but since it's
    // two variable per position and we are now reading an arrays it 1778 x 2.
    fseek(shmFile, 3554, 0);
    fread(result, 1, 2, shmFile);
    ASSERT_TRUE((result[0] & 0xFF) == 0xFE);
    ASSERT_TRUE((result[1] & 0xFF) == 0xFE);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
