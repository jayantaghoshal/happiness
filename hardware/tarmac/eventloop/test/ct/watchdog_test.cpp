/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <IDispatcher.h>
#include "dispatcher_watchdog.h"

#include <gtest/gtest.h>
#include <chrono>
#include <future>

#define LOG_TAG "eventloop.Tests"
#include <cutils/log.h>
using namespace tarmac::eventloop;
using namespace std::chrono_literals;

class EventLoopWatchDogTest : public ::testing::Test {
  public:
    EventLoopWatchDogTest()
        : dispatcher{IDispatcher::CreateDispatcher()}, watchdog(WatchDog::Create([this](std::thread::id id) {
              (void)id;
              if (!watchdog_ever_set) {
                  watchdog_failed_promise.set_value();
              }
              watchdog_ever_set = true;
          })) {
        timer_completed_future = timer_completed_promise.get_future();
        watchdog_failed_future = watchdog_failed_promise.get_future();
        watchdog->Watch(dispatcher, 3s);
    }

    const std::shared_ptr<IDispatcher> dispatcher;
    const std::unique_ptr<WatchDog> watchdog;
    std::promise<void> timer_completed_promise;
    std::future<void> timer_completed_future;

    bool watchdog_ever_set = false;
    std::promise<void> watchdog_failed_promise;
    std::future<void> watchdog_failed_future;
};

TEST_F(EventLoopWatchDogTest, TestEnabledWatchDogNotTimeout) {
    auto timer_end = std::chrono::steady_clock::now() + 2s;
    auto watchdog_end = std::chrono::steady_clock::now() + 5s;
    dispatcher->Enqueue([&]() {
        std::this_thread::sleep_for(1s);  // Blocking dispatcher shorter than watchdog limit
        timer_completed_promise.set_value();
    });
    EXPECT_EQ(std::future_status::ready, timer_completed_future.wait_until(timer_end)) << "Blocking timer not finished";
    EXPECT_EQ(std::future_status::timeout, watchdog_failed_future.wait_until(watchdog_end))
            << "Watchdog should not be set";
}

TEST_F(EventLoopWatchDogTest, TestEnabledWatchDogTimeout) {
    auto timer_end = std::chrono::steady_clock::now() + 6s;
    auto watchdog_end = std::chrono::steady_clock::now() + 5s;
    dispatcher->Enqueue([&]() {
        // Block the dispatcher longer than watchdog limit 2s (+margin because watchdog internally has schedule)
        std::this_thread::sleep_for(4s);
        timer_completed_promise.set_value();
    });
    EXPECT_EQ(std::future_status::ready, watchdog_failed_future.wait_until(watchdog_end)) << "Watchdog should be set";
    EXPECT_EQ(std::future_status::ready, timer_completed_future.wait_until(timer_end)) << "Blocking timer not finished";
}