/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <AudioTable.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <com/delphi/BnAudioManagerCallback.h>
#include <com/delphi/BpAudioManager.h>
#include <cutils/log.h>
#include <tuple>

#undef LOG_TAG
#define LOG_TAG "SountNotificationTest"

namespace {

class AudioManagerCbImpl : public com::delphi::BnAudioManagerCallback {
  public:
    android::binder::Status soundStopped(int32_t connId) override {
        printf("%s connId:%d\n", __func__, connId);
        return android::binder::Status::ok();
    }
};
}

int main(int argc, char* argv[]) {
    android::sp<com::delphi::IAudioManager> service;
    android::sp<android::IBinder> binderInterface;
    int64_t currentConnection;

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

    sp<IServiceManager> sm = defaultServiceManager();

    if (sm.get() == nullptr) {
        printf("service manager is null\n");
        return 1;
    }

    printf("%s: Starting getService(am_service)\n", __FUNCTION__);
    int retries = 3;
    while (binderInterface.get() == nullptr && --retries >= 0) {
        binderInterface = sm->getService(String16("am_service"));

        if (binderInterface.get() == nullptr) {
            printf("%s: getService(am_service) failed\n", __FUNCTION__);
            fflush(stdout);
            usleep(500000);
        }
    }
    printf("%s: getService(am_service) %p\n", __FUNCTION__, binderInterface.get());
    if (binderInterface.get() == nullptr) {
        printf("%s: am_service is null, audio will be broken\n", __FUNCTION__);
        return 1;
    }

    service = interface_cast<com::delphi::IAudioManager>(binderInterface);

    if (service.get() == nullptr) {
        printf("am_service is null\n");
        return 1;
    }

    binder::Status status;

    // subscribe
    sp<com::delphi::IAudioManagerCallback> callback(new AudioManagerCbImpl);
    status = service->subscribe(callback);
    if (!status.isOk()) {
        printf("subscribe failed with status: %s\n", status.exceptionMessage().string());
    }

    currentConnection = -1;

    auto sound = AudioTable::getSources().at(sound_index);
    printf("Will play notification sound id: %i, name: %s\n", std::get<2>(sound), std::get<3>(sound));
    status = service->playSound(
            static_cast<int32_t>(std::get<0>(sound)), static_cast<int32_t>(std::get<1>(sound)), &currentConnection);
    printf("playSound status: %s connectionID: %ld\n", status.toString8().string(), currentConnection);
    fflush(stdout);
    usleep(1000000);
    service->stopSound(currentConnection);
}
