/**
 * Unit test of the CarconfigOneshot file.
 *
 * Production code under test:
 *   ihu_cedric/carconfig_oneshot/src/carconfig_oneshot.cpp
 *
 */

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <algorithm>
#include "carconfig_setup.cpp"

class CarConfigOneshotTestFixture : public testing::Test
{
  public:
    std::string ccDefaultFileName = std::tmpnam(nullptr);
    std::string ccFileName = ccDefaultFileName + "new";
    std::string ccFileEmptyName = ccDefaultFileName + "empty";
    std::string ccDefaultTestDir = std::tmpnam(nullptr);
    std::string ccDefaultFullTestDir = ccDefaultTestDir + std::string("/test/");

    virtual void SetUp()
    {
        std::ofstream myfile;
        myfile.open(ccDefaultFileName);
        myfile << "1,1,1\n";
        myfile << "2,2,2\n";
        myfile.close();

        std::ofstream myEmptyfile;
        myEmptyfile.open(ccFileEmptyName, std::ios::out);
        myEmptyfile.close();
    }

    virtual void TearDown()
    {
        std::remove(ccDefaultFileName.c_str());
        std::remove(ccFileName.c_str());
        std::remove(ccFileEmptyName.c_str());
    }
};

bool compareFiles(const std::string path1, const std::string path2)
{
    std::ifstream f1(path1);
    std::ifstream f2(path2);

    // Can't read of file size wrong
    if (f1.fail() || f2.fail() || f1.tellg() != f2.tellg())
    {
        return false;
    }

    // Rewind to beginning
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);

    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()), std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

TEST_F(CarConfigOneshotTestFixture, carconfig_setup_givenDefaultFile_ACopyShouldBeMade)
{
    carconfig_setup(ccFileName.c_str(), ccDefaultFileName.c_str());
    ASSERT_TRUE(compareFiles(ccFileName, ccDefaultFileName));
}

TEST_F(CarConfigOneshotTestFixture, carconfig_setup_FunctionShouldReturnTrue)
{
    ASSERT_TRUE(carconfig_setup(ccFileName.c_str(), ccDefaultFileName.c_str()));
}

TEST_F(CarConfigOneshotTestFixture, fileExists_givenEmptyFile_theFunctionShouldReturnFalse)
{
    ASSERT_FALSE(fileExists(ccFileEmptyName));
}

TEST_F(CarConfigOneshotTestFixture, fileExists_givenPopulatedFile_theFunctionShouldReturnTrue)
{
    ASSERT_FALSE(fileExists(ccFileName));
}

TEST_F(CarConfigOneshotTestFixture, createPath_givenPath_thePathShouldBeCreated)
{
    struct stat info;
    createPath(ccDefaultFullTestDir);
    ASSERT_FALSE(stat(ccDefaultFullTestDir.c_str(), &info));
    std::string remCmd = "/bin/rm -rf " + ccDefaultTestDir;
    system(remCmd.c_str());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
