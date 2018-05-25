/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <gpio.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace evs {
namespace hardware {
namespace lvdslink {

class LvdslinkGpioBaseTest : public ::testing::Test {
  protected:
    LvdslinkGpioBaseTest(const std::string& name, int number, Gpio::Direction direction)
        : gpio_{Gpio::Create(name, number, direction)} {}

    void SetUp() override { ASSERT_NE(gpio_.get(), nullptr); }

    void TearDown() override {
        gpio_.reset();
        ASSERT_EQ(gpio_.get(), nullptr);
    }

    std::unique_ptr<Gpio> gpio_;
};

class LvdslinkGpioTest_PWDN : public LvdslinkGpioBaseTest {
  public:
    LvdslinkGpioTest_PWDN() : LvdslinkGpioBaseTest("PWDN", 462, Gpio::Direction::OUT) {}
};

class LvdslinkGpioTest_INTOUT : public LvdslinkGpioBaseTest {
  public:
    LvdslinkGpioTest_INTOUT() : LvdslinkGpioBaseTest("INTOUT", 463, Gpio::Direction::IN) {}
};

class LvdslinkGpioTest_ERR : public LvdslinkGpioBaseTest {
  public:
    LvdslinkGpioTest_ERR() : LvdslinkGpioBaseTest("ERR", 464, Gpio::Direction::IN) {}
};

class LvdslinkGpioTest_LOCK : public LvdslinkGpioBaseTest {
  public:
    LvdslinkGpioTest_LOCK() : LvdslinkGpioBaseTest("LOCK", 465, Gpio::Direction::IN) {}
};

class LvdslinkGpioTest_LFLT : public LvdslinkGpioBaseTest {
  public:
    LvdslinkGpioTest_LFLT() : LvdslinkGpioBaseTest("LFLT", 466, Gpio::Direction::IN) {}
};

TEST(LvdslinkGpioTest, NonexistentPinNumber) {
    // It is assumed that this gpio does not exist on the device:
    std::unique_ptr<Gpio> gpio = Gpio::Create("gpio-id", 999, Gpio::Direction::OUT);
    EXPECT_EQ(gpio.get(), nullptr);
}

TEST(LvdslinkGpioTest, MismatchedDirectionIn) {
    std::unique_ptr<Gpio> gpio = Gpio::Create("gpio-id", 462, Gpio::Direction::IN);
    EXPECT_EQ(gpio.get(), nullptr);
}

TEST(LvdslinkGpioTest, MismatchedDirectionOut) {
    std::unique_ptr<Gpio> gpio = Gpio::Create("gpio-id", 463, Gpio::Direction::OUT);
    EXPECT_EQ(gpio.get(), nullptr);
}

TEST_F(LvdslinkGpioTest_PWDN, SetGetHigh) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), true);
    Gpio::Level level;
    EXPECT_EQ(gpio_->Get(&level), true);
    EXPECT_EQ(level, Gpio::Level::HIGH);
}

TEST_F(LvdslinkGpioTest_PWDN, SetGetLow) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::LOW), true);
    Gpio::Level level;
    EXPECT_EQ(gpio_->Get(&level), true);
    EXPECT_EQ(level, Gpio::Level::LOW);
}

TEST_F(LvdslinkGpioTest_INTOUT, Get) {
    Gpio::Level level;
    EXPECT_EQ(gpio_->Get(&level), true);
}

TEST_F(LvdslinkGpioTest_INTOUT, SetHigh) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), false);
}

TEST_F(LvdslinkGpioTest_INTOUT, SetLow) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), false);
}

TEST_F(LvdslinkGpioTest_ERR, Get) {
    Gpio::Level level;
    EXPECT_EQ(gpio_->Get(&level), true);
}

TEST_F(LvdslinkGpioTest_ERR, SetHigh) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), false);
}

TEST_F(LvdslinkGpioTest_ERR, SetLow) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), false);
}

TEST_F(LvdslinkGpioTest_LOCK, Get) {
    Gpio::Level level;
    EXPECT_EQ(gpio_->Get(&level), true);
}

TEST_F(LvdslinkGpioTest_LOCK, SetHigh) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), false);
}

TEST_F(LvdslinkGpioTest_LOCK, SetLow) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), false);
}

TEST_F(LvdslinkGpioTest_LFLT, Get) {
    Gpio::Level level;
    EXPECT_EQ(gpio_->Get(&level), true);
}

TEST_F(LvdslinkGpioTest_LFLT, SetHigh) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), false);
}

TEST_F(LvdslinkGpioTest_LFLT, SetLow) {
    EXPECT_EQ(gpio_->Set(Gpio::Level::HIGH), false);
}

}  // namespace lvdslink
}  // namespace hardware
}  // namespace evs
