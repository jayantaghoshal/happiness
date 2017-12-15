/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>
#include <memory>
#include "VehicleHalImpl.h"
#include "vhal_v2_0/VehicleHalManager.h"

#include "AudioVehicleHalImpl.h"
#include "PowerModule.h"
#include "hvacmodule.h"
#include "keymanagermodule.h"

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include "carconfigmodule.h"

#undef LOG_TAG
#define LOG_TAG "automotive.vehicle2.0"

namespace vhal_20 = android::hardware::automotive::vehicle::V2_0;
namespace vccvhal_10 = vendor::volvocars::hardware::vehiclehal::V1_0;

int main(int /* argc */, char* /* argv */ []) {
    // auto store = std::make_unique<VehiclePropertyStore>();
    auto store = std::make_unique<vhal_20::VehiclePropertyStore>();
    auto hal = std::make_unique<vhal_20::impl::VehicleHalImpl>(store.get());

    // Create Modules
    auto powerModule = std::make_unique<vhal_20::impl::PowerModule>(hal.get());
    // TODO Commented out audioModule since it makes reading properties from vhal fail for other functions, e.g. climate
    // Keeps logging: AudioModVHAL: getCurrentContext: Unexpected currentSource [-1]
    // AudioModVHAL: getAllPropValues: invalid context, return a dummy vector, initial call?
    // auto audioModule = std::make_unique<vhal_20::impl::AudioModule>(hal.get());
    auto carConfigModule = std::make_unique<vccvhal_10::impl::CarConfigHal>(hal.get());
    auto hvacModule = std::make_unique<HvacModule>(hal.get());
    auto keyManagerModule = std::make_unique<KeyManagerModule>(hal.get());

    // Register modules
    powerModule->registerToVehicleHal();
    // audioModule->registerToVehicleHal();
    hvacModule->registerToVehicleHal();
    carConfigModule->registerToVehicleHal();
    keyManagerModule->registerToVehicleHal();

    auto service = std::make_unique<vhal_20::VehicleHalManager>(hal.get());

    // Start subscriptions to VIP, do this after HAL is setup
    keyManagerModule->StartSubscribe();

    // Configured to only use 1 thread as the VHAL Manager and Implementation is not threadsafe.
    android::hardware::configureRpcThreadpool(1, true /* callerWillJoin */);

    ALOGI("Registering as service...");
    android::status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Could not register service, %d", status);
    }

    ALOGI("Ready starting VCC VHAL Daemon");
    android::hardware::joinRpcThreadpool();

    return 0;
}
