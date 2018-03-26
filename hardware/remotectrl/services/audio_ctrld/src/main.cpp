/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <audio_ctrl_service.h>
#include <hidl/HidlTransportSupport.h>
#include <memory>
#include <new>
#include <stdexcept>
#include "convapi_signals_def.h"
#include "service_info.h"

#undef LOG_TAG
#define LOG_TAG "RemoteCtrl_AudioCtrl"
#include <cutils/log.h>

using namespace ::android::hardware;
using namespace vcc::remotectrl;

int main() {
    ALOGI("Convenience API audio control daemon 0.1 starting");

    try {
        ServiceInfo service_info{
                .service_name_ = "RemoteCtrl_AudioCtrl",
                .service_id_ = REMOTECTRL_AUDIOCTRL_SERVICE_ID,
                .instance_id_ = REMOTECTRL_AUDIOCTRL_SERVICE_INSTANCE_ID,
                .methods_ = {REMOTECTRL_AUDIOCTRL_METHOD_ID_GETVOLUME, REMOTECTRL_AUDIOCTRL_METHOD_ID_SETVOLUME},
                .events_ = {REMOTECTRL_AUDIOCTRL_EVENT_ID_VOLUMESTATUS}};

        ::android::sp<remoteaudioctrl::AudioCtrlService> audio_ctrl_service =
                new remoteaudioctrl::AudioCtrlService(service_info);

        audio_ctrl_service->StartService();

        configureRpcThreadpool(1, true);

        if (::android::OK != audio_ctrl_service->registerAsService()) {
            throw std::runtime_error("Failed to register Service...");
        }

        joinRpcThreadpool();
    } catch (const std::runtime_error& e) {
        ALOGE("%s", e.what());
        return EXIT_FAILURE;
    } catch (const std::bad_alloc& e) {
        ALOGE("%s", e.what());
        return EXIT_FAILURE;
    }

    ALOGD("ConvAPI AudioCtrl is exiting...");
    return EXIT_SUCCESS;
}
