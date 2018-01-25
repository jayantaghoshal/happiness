/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <AudioTable.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <vendor/delphi/audiomanager/1.0/IAudioManager.h>
#include "vendor/delphi/audiomanager/1.0/types.h"

#include <cutils/log.h>
#include <stdint.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <tuple>

#undef LOG_TAG
#define LOG_TAG "SountNotificationTest"

static std::atomic_bool exitApp(false);
static std::atomic_bool connected(false);
static std::atomic_bool disconnected(true);

namespace {

class AudioManagerCbImpl : public vendor::delphi::audiomanager::V1_0::IAudioManagerCallback {
  public:
    virtual ::android::hardware::Return<void> onDisconnected(uint32_t connectionID) override {
        printf("%s %d\n", __FUNCTION__, connectionID);
        fflush(stdout);
        // exitApp = true;
        connected = false;
        disconnected = true;
        return android::hardware::Return<void>();
    }

    virtual ::android::hardware::Return<void> onConnected(uint32_t connectionID) override {
        printf("%s %d\n", __FUNCTION__, connectionID);
        fflush(stdout);
        connected = true;
        disconnected = false;
        return android::hardware::Return<void>();
    }

    virtual ::android::hardware::Return<void> onWavFileFinished(uint32_t cId) override {
        printf("%s %d\n", __FUNCTION__, cId);
        fflush(stdout);
        return android::hardware::Return<void>();
    }
    virtual ::android::hardware::Return<void> onRampedIn(uint32_t connectionID) override {
        printf("%s %d\n", __FUNCTION__, connectionID);
        fflush(stdout);
        return android::hardware::Return<void>();
    }
    virtual ::android::hardware::Return<void> ackSetSinkVolumeChange(uint32_t sinkId, int16_t volume) override {
        printf("%s sink: %d, volume: %d\n", __FUNCTION__, sinkId, volume);
        fflush(stdout);
        return android::hardware::Return<void>();
    }
};
}

int main(int argc, char* argv[]) {
    android::sp<vendor::delphi::audiomanager::V1_0::IAudioManager> service_;
    android::sp<android::IBinder> binderInterface;

    printf("Sound notification test app.\n");

    if (argc < 2) {
        printf("No sound index provided\nUsage: %s <sound index>. Sound index from 0 to %i\n",
               argv[0],
               (int32_t)(AudioTable::getSources().size() - 1));
        return 1;
    }

    int32_t sound_index = atoi(argv[1]);
    if (sound_index < 0 || sound_index >= (int32_t)AudioTable::getSources().size()) {
        printf("Invalid sound index provided: %i\n", sound_index);
        return 1;
    }

    using namespace android;

    while (service_.get() == nullptr) {
        service_ = vendor::delphi::audiomanager::V1_0::IAudioManager::getService();

        if (service_ == nullptr) {
            printf("%s: getService(am_service) failed\n", __func__);
            fflush(stdout);
            sleep(1);
        }
    }
    using namespace vendor::delphi::audiomanager::V1_0;

    // subscribe
    sp<vendor::delphi::audiomanager::V1_0::IAudioManagerCallback> callback(new AudioManagerCbImpl);
    hardware::Return<void> ret = service_->subscribe(callback);
    if (!ret.isOk()) {
        printf("subscribe failed with status: %s\n", ret.description().c_str());
        return 1;
    }

    auto sound = AudioTable::getSources().at(sound_index);
    printf("Will play notification sound id: %i, name: %s\n", std::get<2>(sound), std::get<3>(sound));
    fflush(stdout);
    for (int count = 0; count < 3; count++) {
        ret = service_->playSound(static_cast<int32_t>(std::get<0>(sound)),
                                  static_cast<int32_t>(std::get<1>(sound)),
                                  [&](AMStatus s, int64_t cId) {
                                      printf("playSound status: %d connectionID: %ld\n", s, cId);
                                      fflush(stdout);
                                      if (s != AMStatus::OK) {
                                          exitApp = true;
                                      }
                                  });

        // wait for connection
        while (!connected.load() && !exitApp.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100LL));
        }
        // now wait for disconnection
        while (!disconnected.load() && !exitApp.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100LL));
        }
    }
    service_->unsubscribe(callback);
}
