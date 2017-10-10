#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <chrono>
#include <random>
#include <cutils/log.h>
#include "signals_server.h"
#include <vendor/volvocars/hardware/signals/1.0/ISignals.h>
#include <vendor/volvocars/hardware/signals/1.0/ISignalsChangedCallback.h>

#undef LOG_TAG
#define LOG_TAG "DataelementsServerUT"


using namespace vendor::volvocars::hardware::signals::V1_0;
using ::testing::_;
using ::android::hardware::hidl_string;

class SignalsChangedMock : public ISignalsChangedCallback 
{
public:
    MOCK_METHOD3(signalChanged, ::android::hardware::Return<void>(
        const ::android::hardware::hidl_string& signalName, 
        Dir dir, 
        const ::android::hardware::hidl_string& data));    
};

class ServerTest : public ::testing::Test{
public:
    const ::android::hardware::hidl_string signalName_A = "hej";   
    const ::android::hardware::hidl_string signalName_B = "hejsfa";     

    const ::android::hardware::hidl_string value_A = "sdflkjfdslkmsdf";   
    const ::android::hardware::hidl_string value_B = "dslökfdlöfkdfölfdkdfgklms,ds";  

    vendor::volvocars::hardware::signals::V1_0::implementation::SignalsServer sut;
    ::android::sp<SignalsChangedMock> callbackA = ::android::sp<SignalsChangedMock>(new SignalsChangedMock());
    ::android::sp<SignalsChangedMock> callbackB = ::android::sp<SignalsChangedMock>(new SignalsChangedMock());
};

TEST_F (ServerTest, subscription_for_name_fired) {    
    sut.subscribe(signalName_A, Dir::IN, callbackA);
    EXPECT_CALL(*callbackA, signalChanged(signalName_A, Dir::IN, value_A)).Times(1);    
    sut.send(signalName_A, Dir::IN, value_A);
}

TEST_F (ServerTest, subscription_for_other_name_not_fired) {
    sut.subscribe(signalName_A, Dir::IN, callbackA);
    EXPECT_CALL(*callbackA, signalChanged(signalName_A, _, ::testing::_)).Times(0);    
    sut.send(signalName_B, Dir::IN, value_B);
}

TEST_F (ServerTest, multipleSubscriptions_differentSignals_onlyMatchingFired) {
    sut.subscribe(signalName_A, Dir::IN, callbackA);
    sut.subscribe(signalName_B, Dir::IN, callbackB);
    EXPECT_CALL(*callbackA, signalChanged(signalName_A, _, ::testing::_)).Times(0);    
    EXPECT_CALL(*callbackB, signalChanged(signalName_B, Dir::IN, value_B)).Times(1);  
    sut.send(signalName_B, Dir::IN, value_B);
}

TEST_F (ServerTest, multipleSubscriptions_sameSignals_bothFired) {
    sut.subscribe(signalName_A, Dir::IN, callbackA);
    sut.subscribe(signalName_A, Dir::IN, callbackB);
    EXPECT_CALL(*callbackA, signalChanged(signalName_A, Dir::IN, value_B)).Times(1);    
    EXPECT_CALL(*callbackB, signalChanged(signalName_A, Dir::IN, value_B)).Times(1);  
    sut.send(signalName_A, Dir::IN, value_B);
}

TEST_F (ServerTest, set_get) {
    ::android::hardware::hidl_string got_value = "";
    auto _hidl_cb = [&](const ::android::hardware::hidl_string& value) {
        got_value = value;
    };

    sut.send(signalName_B, Dir::OUT, value_B);
    EXPECT_TRUE(sut.get(signalName_B, Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(value_B, got_value);
}


TEST_F (ServerTest, get_wildcard_match) {
    ::android::hardware::hidl_vec<Result> got_results;
    auto _hidl_cb = [&](const ::android::hardware::hidl_vec<Result>& data) {
        got_results = data;
    };

    sut.send("abcdefghijklm", Dir::OUT, "abc_value");
    sut.send("aaaaaaaaa", Dir::OUT, "aaa_value");
    EXPECT_TRUE(sut.get_all("abcdefghijklm", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(1u, got_results.size());

    EXPECT_TRUE(sut.get_all("a*", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(2u, got_results.size());

    EXPECT_TRUE(sut.get_all("*a", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(1u, got_results.size());

    EXPECT_TRUE(sut.get_all("*a*", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(2u, got_results.size());

    EXPECT_TRUE(sut.get_all("a", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(0u, got_results.size());

    EXPECT_TRUE(sut.get_all("b", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(0u, got_results.size());

    EXPECT_TRUE(sut.get_all("b*", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(0u, got_results.size());

    EXPECT_TRUE(sut.get_all("*b", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(0u, got_results.size());

    EXPECT_TRUE(sut.get_all("*b*", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(1u, got_results.size());


    EXPECT_TRUE(sut.get_all("*b*d", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(0u, got_results.size());

    EXPECT_TRUE(sut.get_all("*b*d*", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(1u, got_results.size());

    EXPECT_TRUE(sut.get_all("*b*a*", Dir::OUT, _hidl_cb).isOk());
    EXPECT_EQ(0u, got_results.size());
}


TEST_F (ServerTest, subscription_wildcard_match1) {
    sut.subscribe("abc*", Dir::IN, callbackA);
    sut.subscribe("abc", Dir::IN, callbackB);

    EXPECT_CALL(*callbackA, signalChanged(hidl_string{"abcdefg"}, _, value_A)).Times(1);    
    EXPECT_CALL(*callbackB, signalChanged(_, _, _)).Times(0);    
    sut.send("abcdefg", Dir::IN, value_A);
}
TEST_F (ServerTest, subscription_wildcard_match2) {
    sut.subscribe("abc*", Dir::IN, callbackA);
    sut.subscribe("abc", Dir::IN, callbackB);

    EXPECT_CALL(*callbackA, signalChanged(hidl_string{"abc"}, Dir::IN, value_A)).Times(1);    
    EXPECT_CALL(*callbackB, signalChanged(hidl_string{"abc"}, Dir::IN, value_A)).Times(1);    
    sut.send("abc", Dir::IN, value_A);
}
TEST_F (ServerTest, subscription_wildcard_match3) {
    sut.subscribe("abc*", Dir::IN, callbackA);
    sut.subscribe("abc", Dir::IN, callbackB);

    EXPECT_CALL(*callbackA, signalChanged(_, _, _)).Times(0);    
    EXPECT_CALL(*callbackB, signalChanged(_, _, _)).Times(0);    
    sut.send("def", Dir::IN, value_A);
}