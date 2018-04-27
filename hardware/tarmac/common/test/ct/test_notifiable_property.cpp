/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "notifiable_property.h"

#include <gtest/gtest.h>

using namespace testing;

namespace TestNotifiableProperty {

class NotifiablePropertyTest : public Test {
  protected:
    NotifiableProperty<int> property_{0};
};

TEST_F(NotifiablePropertyTest, WhenInitiliased_ContainsValue) {
    EXPECT_EQ(0, property_.get());
}

TEST_F(NotifiablePropertyTest, WhenPropertySet_WillSet) {
    property_.set(10);
    EXPECT_EQ(10, property_.get());
}

TEST_F(NotifiablePropertyTest, LocalValue_WillUpdate_WhileSubscriptionIsInScope) {
    int value = 10;
    auto handle = property_.subscribe([&value](auto x) { value = x; });
    property_.set(30);
    EXPECT_EQ(30, value);
}

TEST_F(NotifiablePropertyTest, SubscriptionHandle_WillUnSubscribe_WhenScopeEnds) {
    int value = 10;
    {
        auto handle = property_.subscribe([&value](auto x) { value = x; });
    }
    property_.set(30);
    EXPECT_EQ(10, value);
}

TEST_F(NotifiablePropertyTest, SubscriptionHandle_NPOutOfScopeBeforeSubHandle_DoesNotCrashOrDeadlock) {
    for (int i = 0; i < 10000; i++) {
        std::unique_ptr<NotifiableProperty<int>> p = std::make_unique<NotifiableProperty<int>>(12345);
        {
            int value = 1;
            auto handle = p->subscribe([&value](auto x) { value = x; });
            p->set(1000);
            EXPECT_EQ(1000, value);
            p = nullptr;
            // Expect no crash/deadlock
        }
    }
}

TEST_F(NotifiablePropertyTest, NoDeadlockWhenSubscribingInCallback) {
    SubscriptionHandle sub2 = nullptr;
    auto sub1 = property_.subscribe([&](auto x) {
        (void)x;
        sub2 = property_.subscribe([&](auto y) { (void)y; });
    });

    property_.set(123294);
}

TEST_F(NotifiablePropertyTest, RecursiveSet) {
    int currentValue = 10;

    auto sub1 = property_.subscribe([&](auto x) {
        EXPECT_EQ(currentValue, x);
        if (currentValue > 0) {
            currentValue--;
            property_.set(currentValue);
        }

    });

    property_.set(currentValue);
    EXPECT_EQ(0, currentValue);
    EXPECT_EQ(0, property_.get());
}

TEST_F(NotifiablePropertyTest, MultipleSubscribersAndUnsubscribers) {
    int value1 = 0;
    int value2 = 0;
    int value3 = 0;
    int value4 = 0;

    auto handle1 = property_.subscribe([&](auto x) { value1 = x; });
    auto handle2 = property_.subscribe([&](auto x) { value2 = x; });
    auto handle3 = property_.subscribe([&](auto x) { value3 = x; });
    auto handle4 = property_.subscribe([&](auto x) { value4 = x; });
    handle2 = nullptr;
    property_.set(9);

    EXPECT_EQ(9, value1);
    EXPECT_EQ(0, value2);
    EXPECT_EQ(9, value3);
    EXPECT_EQ(9, value4);

    handle3 = nullptr;
    property_.set(2);
    EXPECT_EQ(2, value1);
    EXPECT_EQ(0, value2);
    EXPECT_EQ(9, value3);
    EXPECT_EQ(2, value4);
}

}  // namespace TestNotifiableProperty
