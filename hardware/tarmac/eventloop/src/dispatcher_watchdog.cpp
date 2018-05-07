/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "dispatcher_watchdog.h"
#include <map>
#include <mutex>
#include <sstream>
#include <thread>
#include "IDispatcher.h"
#define LOG_TAG "tarmac.dispatcher"
#include <cutils/log.h>

using namespace std::chrono_literals;

namespace tarmac {
namespace eventloop {

struct WatchdogEntry {
    std::chrono::steady_clock::time_point lastKick;
    std::chrono::steady_clock::duration maxTime;
};
std::string threadIdToString(std::thread::id id) {
    std::ostringstream ss;
    ss << id;
    return ss.str();
}

class WatchDogImpl final : public WatchDog {
  public:
    explicit WatchDogImpl(std::function<void(std::thread::id)>&& assertion_function)
        : watchDogThread_{IDispatcher::CreateDispatcher()}, assertion_function_{std::move(assertion_function)} {
        watchDogThreadJobId = watchDogThread_->EnqueueWithDelayCyclic(1s, [this]() {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                const auto now = std::chrono::steady_clock::now();
                for (const auto& kick : threadsKickedWatchDog_) {
                    auto timeSinceLastKick = now - kick.second.lastKick;

                    if (timeSinceLastKick > kick.second.maxTime) {
                        ALOGD("Assertion fail, time since last kick: %lld ms",
                              std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceLastKick).count());
                        assertion_function_(kick.first);
                    }
                }
            }
        });
    }
    ~WatchDogImpl() override { watchDogThread_->Cancel(watchDogThreadJobId); }

    void Watch(std::shared_ptr<IDispatcher> dispatcher, std::chrono::steady_clock::duration maxTime) override {
        WatchdogEntry entry;
        entry.lastKick = std::chrono::steady_clock::now();
        entry.maxTime = maxTime;

        dispatcher->Enqueue([entry, dispatcher, this]() {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                const auto insertionResult = threadsKickedWatchDog_.emplace(std::this_thread::get_id(), entry);
                bool newInsertion = insertionResult.second;
                if (!newInsertion) {
                    ALOGE("Dispatcher is already being watched");
                    assert(false);
                }
            }
        });
        auto kickInterval = std::chrono::duration_cast<std::chrono::microseconds>(maxTime / 3);
        dispatcher->EnqueueWithDelayCyclic(kickInterval, [this]() { KickWatchDog(); });
    }

    void KickWatchDog() {
        const auto thread_id = std::this_thread::get_id();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            const auto now = std::chrono::steady_clock::now();
            auto entryIt = threadsKickedWatchDog_.find(thread_id);
            if (entryIt == threadsKickedWatchDog_.end()) {
                ALOGE("Kicking watchdog not registered before");
                return;
            }
            entryIt->second.lastKick = now;
        }
    }

  private:
    const std::shared_ptr<IDispatcher> watchDogThread_;
    std::function<void(std::thread::id)> assertion_function_;
    std::mutex mutex_;
    std::map<std::thread::id, WatchdogEntry> threadsKickedWatchDog_;
    IDispatcher::JobId watchDogThreadJobId;
};

std::unique_ptr<WatchDog> WatchDog::Create() {
    return Create([](std::thread::id failing_thread_id) {
        ALOGE("Watchdog fail! Thread id: %s, processing exceeded max time",
              threadIdToString(failing_thread_id).c_str());
        assert(false);
    });
}

std::unique_ptr<WatchDog> WatchDog::Create(std::function<void(std::thread::id)>&& assertion_function) {
    return std::make_unique<WatchDogImpl>(std::move(assertion_function));
}

}  // namespace eventloop
}  // namespace tarmac