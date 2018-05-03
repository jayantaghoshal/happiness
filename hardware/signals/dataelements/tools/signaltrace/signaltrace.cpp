/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <set>
#include <sstream>
#include <thread>

#include "vendor/volvocars/hardware/signals/1.0/ISignals.h"
#include "vendor/volvocars/hardware/signals/1.0/ISignalsChangedCallback.h"
#include "vendor/volvocars/hardware/signals/1.0/types.h"

using namespace vendor::volvocars::hardware::signals::V1_0;

const char* dirStr(Dir dir) {
    switch (dir) {
        case Dir::IN:
            return "IN";
        case Dir::OUT:
            return "OUT";
        case Dir::INTERNAL:
            return "INTERNAL";
        default:
            return "???";
    }
}

class SignalChangedCallback : public ISignalsChangedCallback {
  public:
    ::android::hardware::Return<void> signalChanged(const ::android::hardware::hidl_string& signalName,
                                                    Dir dir,
                                                    const ::android::hardware::hidl_string& data) override {
        printf("[%s] %s %s\n", dirStr(dir), signalName.c_str(), data.c_str());
        fflush(stdout);  // Important, when piping through grep (non interative terminal), \n does not flush the output.
        return ::android::hardware::Void();
    }
};

void printHelp() {
    printf("\n");
    printf("Usage: [in|out|internal] [SIGNALS] [-l, -list]\n");
    printf("Example: 'signaltrace' trace all signals\n");
    printf("Example: 'signaltrace in' trace all signals received by IHU\n");
    printf("Example: 'signaltrace Prof*' trace all signals that starts with \"Prof\"\n");
    printf("Example: 'signaltrace in *Mod*' trace all in signals that contains string \"Mod\"\n");
    printf("Example: 'signaltrace in -list' First lists all received in signals last received value then trace all in "
           "signals \n\n");
    printf("NOTE: Signal filters are case sensitive\n\n");
}

void printCurrentSignalsForDir(ISignals& service, Dir dir, const std::string& filter) {
    printf("Get all %s signals with filter %s\n", dirStr(dir), filter.c_str());

    ::android::hardware::hidl_vec<Result> result;
    auto _hidl_cb = [&](const ::android::hardware::hidl_vec<Result>& data) { result = data; };
    auto res = service.get_all(filter, dir, _hidl_cb);
    if (!res.isOk()) {
        printf("Failed to call get_all() to server. Description: %s\n", res.description().c_str());
        return;
    }
    for (const auto& r : result) {
        printf("[%s] %s %s\n", dirStr(dir), r.name.c_str(), r.value.c_str());
    }
    fflush(stdout);
}

class DeathRecipientToFunction : public android::hardware::hidl_death_recipient {
  public:
    DeathRecipientToFunction(const std::function<void()> onDeath) : onDeath{onDeath} {}
    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override {
        onDeath();
    }

  private:
    const std::function<void()> onDeath;
};

class HidlSubscriber {
  public:
    HidlSubscriber(::android::sp<ISignals> alreadyConnectedService,
                   const std::string& filter,
                   const std::set<Dir>& dirs);

  private:
    bool resubscribe();

    std::mutex serviceMutex;
    ::android::sp<ISignals> service;
    const ::android::sp<SignalChangedCallback> signalChangedListener;
    const ::android::sp<DeathRecipientToFunction> deathSubscriber;
    const std::string filter;
    const std::set<Dir> dirs;
};

bool HidlSubscriber::resubscribe() {
    auto result = service->linkToDeath(deathSubscriber, 1234);
    if (!result.isOk()) {
        printf("ERROR: Signaling service not available, link to death not possible: %s\n",
               result.description().c_str());
        return false;
    }
    for (const Dir& d : dirs) {
        auto subscribe = service->subscribe(filter, d, signalChangedListener, getpid());
        if (!subscribe.isOk()) {
            printf("ERROR: Failed to subscribe to server with dir=%s and filter=%s . Description: %s\n",
                   dirStr(d),
                   filter.c_str(),
                   subscribe.description().c_str());
            return false;
        }
    }
    return true;
}

HidlSubscriber::HidlSubscriber(::android::sp<ISignals> alreadyConnectedService,
                               const std::string& filter,
                               const std::set<Dir>& dirs)
    : service{alreadyConnectedService},
      signalChangedListener{new SignalChangedCallback()},
      deathSubscriber{new DeathRecipientToFunction([&] {
          printf("ERROR: Lost connection to HIDL server\n");
          fflush(stdout);
          {
              std::lock_guard<std::mutex> lock(serviceMutex);
              service = nullptr;
          }
      })},
      filter{filter},
      dirs{dirs} {
    if (service != nullptr) {
        resubscribe();
    }

    while (true) {
        {
            std::lock_guard<std::mutex> lock2(serviceMutex);
            while (service == nullptr) {
                service = ISignals::getService();
                if (service != nullptr) {
                    printf("Reconnected to HIDL server\n");
                    fflush(stdout);
                    if (!resubscribe()) {
                        service = nullptr;
                        printf("Failed to reconnect to HIDL server\n");
                        fflush(stdout);
                    }
                }
                std::this_thread::sleep_for(
                        std::chrono::seconds(2));  // TODO: Could use ISignals::registerForNotifications instead
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char* argv[]) {
    bool listSignals = false;
    std::string filter = "*";
    std::set<Dir> dirs{Dir::IN, Dir::OUT, Dir::INTERNAL};
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "in" || arg == "IN") {
            dirs = std::set<Dir>{Dir::IN};
        } else if (arg == "out" || arg == "OUT") {
            dirs = std::set<Dir>{Dir::OUT};
        } else if (arg == "internal" || arg == "INTERNAL") {
            dirs = std::set<Dir>{Dir::INTERNAL};
        } else if (arg == "-l" || arg == "--list" || arg == "-list") {
            listSignals = true;
        } else if (arg == "--help" || arg == "-help" || arg == "-h") {
            printHelp();
            return 0;
        } else {
            filter = arg;
        }
    }

    ::android::sp<ISignals> service = ISignals::getService();
    if (service == nullptr) {
        printf("Service is null, is dataelements-hidl-server running?\n");
        return 1;
    }

    if (listSignals) {
        for (const Dir& d : dirs) {
            printCurrentSignalsForDir(*service, d, filter);
        }
        return 0;
    }

    HidlSubscriber runsForever(service, filter, dirs);
}
