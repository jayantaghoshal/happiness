/**
 * Unit test of the file_monitor class.
 *
 * Production code under test:
 *   ihu_cedric/core/src/cedric_file_monitor.cpp
 *
 */

#include <gtest/gtest.h>
#include "cedric_file_monitor.h"
#include <sys/inotify.h>
#include <stdlib.h>


TEST(FileMonitorTest, Constructor_GivenInvalidFilePath_ThrowsException)
{
    int count = 0;
    ASSERT_THROW(new cedric::core::file_monitor( "./file/that/does/not/exist", IN_MODIFY, [&count] (uint32_t mask) { if (mask&IN_MODIFY) count++; }), std::invalid_argument);
}

TEST(FileMonitorTest, Constructor_GivenValidFilePath_CallbackIsTriggered)
{
    system("touch ./testfile"); // Create a sample file
    int count = 0;
    cedric::core::file_monitor fm ( "./testfile", IN_OPEN, [&count] (uint32_t mask) { if (mask&IN_OPEN) count++; });
    system("touch ./testfile"); // Touch the sample file
    ASSERT_EQ(count, 1);
    system("rm ./testfile"); // Remove since we don't need this anymore
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
