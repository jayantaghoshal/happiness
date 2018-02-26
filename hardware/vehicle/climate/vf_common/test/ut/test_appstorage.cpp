/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "appstorage.h"
#include "src/appstorage_private.h"

#include <fcntl.h>
#include <gtest/gtest.h>
#include <list>

const std::string PATH_ROOT( AppStorage::getPlatformStoragePath() );
const std::string PATH_APP( PATH_ROOT + "test_common" );
const std::string PATH_DB( PATH_APP + "/db.json" );

bool fileExist(const std::string& file)
{
    int fd = open(file.c_str(), O_RDONLY);
    if (fd >= 0)
    {
        close(fd);
    }
    return fd >= 0;
}

class AppStorageTestFixture : public ::testing::Test
{
public:
    AppStorageTestFixture()
        : testing::Test()
    {
        // Guarantee we have the path /tmp/appstorage/test_appstorage and that it is empty
        //mkdir(PATH_ROOT.c_str(), 0775);
        //mkdir(PATH_APP.c_str(), 0775);
        unlink(PATH_DB.c_str());

        AppStorage::instance().init(); // re-read the database from file
    }
};

TEST_F(AppStorageTestFixture, initialStartSetValue_dbFileCreated)
{
    Dispatcher* disp = new Dispatcher();
    AppStorage  db(PATH_DB, disp);

    EXPECT_FALSE(fileExist(PATH_DB));

    db.setValue("key1", "value1");
    db.syncronize();

    EXPECT_TRUE(fileExist(PATH_DB));
}

TEST_F(AppStorageTestFixture, setValue_verifyWithGet)
{
    Dispatcher* disp = new Dispatcher();
    AppStorage  db(PATH_DB, disp);

    EXPECT_TRUE("myDefault" == db.getValue("key1", "myDefault"));

    db.setValue("key1", "value1");
    EXPECT_TRUE("value1" == db.getValue("key1", "myDefault"));

    db.setValue("key1", "value2");
    EXPECT_TRUE("value2" == db.getValue("key1", "myDefault"));
}

TEST_F(AppStorageTestFixture, setValue_verifyCorrectValueAtNextStart)
{
    {
        EXPECT_FALSE(fileExist(PATH_DB));
        Dispatcher* disp = new Dispatcher();
        AppStorage  db1(PATH_DB, disp);
        db1.setValue("key1", "shallSurviveToNextStart");
    }

    {
        EXPECT_TRUE(fileExist(PATH_DB));
        Dispatcher* disp = new Dispatcher();
        AppStorage  db2(PATH_DB, disp);
        EXPECT_TRUE("shallSurviveToNextStart" == db2.getValue("key1", ""));
    }
}

TEST_F(AppStorageTestFixture, corruptFile_behaveAsEmptyDatabase)
{
    {
        EXPECT_FALSE(fileExist(PATH_DB));
        Dispatcher* disp = new Dispatcher();
        AppStorage  db1(PATH_DB, disp);
        db1.setValue("key1", "shallSurviveToNextStart1");
        db1.setValue("key2", "shallSurviveToNextStart2");
        db1.setValue("key3", "shallSurviveToNextStart3");
        db1.setValue("key4", "shallSurviveToNextStart4");
        db1.setValue("key5", "shallSurviveToNextStart5");
        db1.setValue("key6", "shallSurviveToNextStart6");
    }

    // truncate file to about half its size (100 characters)
    truncate(PATH_DB.c_str(), 100);

    {
        EXPECT_TRUE(fileExist(PATH_DB));
        Dispatcher* disp = new Dispatcher();
        AppStorage  db2(PATH_DB, disp);
        EXPECT_TRUE("myDefault" == db2.getValue("key1", "myDefault"));
        db2.setValue("key1", "shallSurviveToNextStart1");
        db2.setValue("key2", "shallSurviveToNextStart2");
        db2.setValue("key3", "shallSurviveToNextStart3");
        db2.setValue("key4", "shallSurviveToNextStart4");
        db2.setValue("key5", "shallSurviveToNextStart5");
        db2.setValue("key6", "shallSurviveToNextStart6");
    }

    // truncate file to zero bytes
    truncate(PATH_DB.c_str(), 0);
    {
        EXPECT_TRUE(fileExist(PATH_DB));
        Dispatcher* disp = new Dispatcher();
        AppStorage  db3(PATH_DB, disp);
        EXPECT_TRUE("myDefault" == db3.getValue("key1", "myDefault"));
    }
}

TEST_F(AppStorageTestFixture, setValue_verifyWrittenAfter10sec)
{
    EXPECT_FALSE(fileExist(PATH_DB));
    Dispatcher* disp = new Dispatcher();
    AppStorage  db1(PATH_DB, disp);

    db1.setValue("key1", "shallSurviveToNextStart");
    EXPECT_FALSE(fileExist(PATH_DB));

    disp->Dispatch();
    EXPECT_TRUE(fileExist(PATH_DB));
}

TEST_F(AppStorageTestFixture, setValue_verifyWrittenAtShutdown)
{
    EXPECT_FALSE(fileExist(PATH_DB));
    Dispatcher* disp = new Dispatcher();
    AppStorage  db1(PATH_DB, disp);

    db1.setValue("key1", "shallSurviveToNextStart");
    EXPECT_FALSE(fileExist(PATH_DB));

    db1.onLifeCycleRequest(1, 1234);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_TRUE(fileExist(PATH_DB));
}


// Test of the public interface
TEST_F(AppStorageTestFixture, setValueGetValue_verifyValue)
{
    appstorage::setValue("A", "valueA");
    EXPECT_TRUE("valueA" == appstorage::getValue("A", "myDefault"));
    EXPECT_TRUE("myDefault" == appstorage::getValue("B", "myDefault"));

    EXPECT_FALSE(fileExist(PATH_DB));
    appstorage::syncronize();
    EXPECT_TRUE(fileExist(PATH_DB));
}

TEST_F(AppStorageTestFixture, templateIntSetGet_verifyResult)
{
    appstorage::AppKeyValuePair<int> s1("myKey1",7);
    EXPECT_EQ(7,s1.get()); // verify the default value

    s1.set(9988);
    EXPECT_EQ(9988,s1.get()); // verify the set value

    EXPECT_FALSE(fileExist(PATH_DB));

    appstorage::syncronize();
    EXPECT_TRUE(fileExist(PATH_DB));
}

TEST_F(AppStorageTestFixture, templateDoubleSetGet_verifyResult)
{
    appstorage::AppKeyValuePair<double> s1("myKey1",3.14);
    EXPECT_EQ(3.14,s1.get()); // verify the default value

    s1.set(0.123);
    EXPECT_EQ(0.123,s1.get()); // verify the set value

    EXPECT_FALSE(fileExist(PATH_DB));

    appstorage::syncronize();
    EXPECT_TRUE(fileExist(PATH_DB));
}

TEST_F(AppStorageTestFixture, templateBoolSetGet_verifyResult)
{
    appstorage::AppKeyValuePair<bool> s1("myKey1",true);
    EXPECT_TRUE(s1.get()); // verify the default value

    s1.set(false);
    EXPECT_FALSE(s1.get()); // verify the set value

    EXPECT_FALSE(fileExist(PATH_DB));

    appstorage::syncronize();
    EXPECT_TRUE(fileExist(PATH_DB));
}

TEST_F(AppStorageTestFixture, templateListSetGet_verifyResult)
{
    std::list<double> l;
    l.push_back(7);

    appstorage::AppKeyValuePair< std::list<double> > s1("myKey1", l);
    EXPECT_EQ(1, s1.get().size()); // verify the default value
    EXPECT_EQ(7, s1.get().front()); // verify the default value

    l.push_back(6);
    l.push_back(5);
    s1.set(l);

    std::list<double> l2 = s1.get();
    EXPECT_EQ(l, l2); // verify the set value

    EXPECT_FALSE(fileExist(PATH_DB));

    appstorage::syncronize();
    EXPECT_TRUE(fileExist(PATH_DB));
}

TEST_F(AppStorageTestFixture, templateListJsonSetGet_verifyResult)
{
    std::list<json> l;
    l.push_back(7);

    appstorage::AppKeyValuePair< std::list<json> > s1("myKey1", l);
    EXPECT_EQ(1, s1.get().size()); // verify the default value
    EXPECT_EQ(7, s1.get().front()); // verify the default value

    l.push_back(6);
    l.push_back(5);
    s1.set(l);

    std::list<json> l2 = s1.get();
    EXPECT_EQ(l, l2); // verify the set value

    EXPECT_FALSE(fileExist(PATH_DB));

    appstorage::syncronize();
    EXPECT_TRUE(fileExist(PATH_DB));
}
