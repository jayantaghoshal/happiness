/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>
#include <memory>
#include "VehicleHalImpl.h"
#include "vhal_v2_0/VehicleHalManager.h"

#include "LifecycleControl.h"
#include "PowerModule.h"
#include "activesafetymodule.h"
#include "activeuserprofilemodule.h"
#include "cartimemodule.h"
#include "connectedsafety.h"
#include "hvacmodule.h"
#include "illuminationmodule.h"
#include "keymanagermodule.h"
#include "libsettings/setting.h"
#include "libsettings/settingsmanagerhidl.h"
#include "sensormodule.h"
#include "vhal_modules/curve_speed_adaption_module.h"
#include "vhal_modules/e_lane_keeping_aid_module.h"
#include "vhal_modules/lane_keeping_aid_module.h"
#include "vhal_modules/speed_limit_adaptation_module.h"

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <future>
#include "carconfigmodule.h"
#include "climate_main.h"
#include "systeminformationmodule.h"

#undef LOG_TAG
#define LOG_TAG "automotive.vehicle2.0"

namespace vhal_20 = android::hardware::automotive::vehicle::V2_0;
namespace vccvhal_10 = vendor::volvocars::hardware::vehiclehal::V1_0;

int main(int /* argc */, char* /* argv */ []) {
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher = tarmac::eventloop::IDispatcher::CreateDispatcher();
    android::sp<SettingsFramework::SettingsManagerHidl> settings_manager =
            new SettingsFramework::SettingsManagerHidl(*dispatcher);

    auto store = std::make_unique<vhal_20::VehiclePropertyStore>();
    auto hal = std::make_unique<vhal_20::impl::VehicleHalImpl>(store.get());

    ////////////////////////////////////////////////////////////////
    // CLIMATE

    std::promise<void> p;
    std::future<void> f = p.get_future();
    std::unique_ptr<ClimateMain> m;
    // std::unique_ptr<v0::org::volvocars::climate::FirstRowStubImpl> climateService;
    dispatcher->EnqueueTask([&]() {
        m.reset(new ClimateMain(dispatcher));
        p.set_value();
    });
    f.wait();

    // tarmac::timeprovider::TimeProvider time_provider{dispatcher};
    // LegacyDispatcher::setGlobalInstanceHackTimeProvider(time_provider);
    // signal_proxy::Proxies signal_proxies;
    // common::daemon::Factory commonFactory_;
    // UserSelectionFactory user_selection{settings_manager};
    // FirstRowFactory first_row{settings_manager, time_provider, signal_proxies, user_selection, commonFactory_};

    ////////////////////////////////////////////////////////////////
    // CLIMATE

    // Create Modules
    auto powerModule = std::make_unique<vhal_20::impl::PowerModule>(hal.get());
    auto carConfigModule = std::make_unique<vccvhal_10::impl::CarConfigHal>(hal.get());
    auto activeUserProfileModule = std::make_unique<vccvhal_10::impl::ActiveUserProfileHal>(hal.get());
    auto hvacModule = std::make_unique<HvacModule>(hal.get(), m->first_row, m->commonFactory_, dispatcher);
    auto keyManagerModule = std::make_unique<KeyManagerModule>(hal.get());
    auto systemInformationModule = std::make_unique<SystemInformationModule>(hal.get());
    auto illuminationModule = std::make_unique<vccvhal_10::impl::IlluminationHal>(hal.get());
    auto carTimeModule = std::make_unique<CarTimeHal>(hal.get());
    auto sensorModule = std::make_unique<SensorModule>(hal.get());
    auto connectedSafetyModule = std::make_unique<vccvhal_10::impl::ConnectedSafety>(hal.get(), settings_manager);
    auto activeSafetyModule = std::make_unique<ActiveSafetyModule>(hal.get(), settings_manager);
    auto curve_speed_adaption_module =
            std::make_unique<CurveSpeedAdaptionModule>(hal.get(), dispatcher, settings_manager);
    auto speed_limit_adaptation_module = std::make_unique<SpeedLimitAdaptationModule>();
    auto lane_keeping_aid_module = std::make_unique<LaneKeepingAidModule>(hal.get(), dispatcher, settings_manager);
    auto e_lane_keeping_aid_module = std::make_unique<ELaneKeepingAidModule>(hal.get(), dispatcher, settings_manager);
    auto lifecycleControl =
            std::make_unique<vendor::delphi::lifecyclecontrol::V1_0::LifecycleControl>(powerModule.get());

    // Register modules
    powerModule->registerToVehicleHal();
    carConfigModule->registerToVehicleHal();
    keyManagerModule->registerToVehicleHal();
    activeUserProfileModule->registerToVehicleHal();
    systemInformationModule->registerToVehicleHal();
    illuminationModule->registerToVehicleHal();
    carTimeModule->registerToVehicleHal();
    sensorModule->registerToVehicleHal();
    connectedSafetyModule->registerToVehicleHal();
    activeSafetyModule->registerToVehicleHal();

    ::android::sp<vhal_20::VehicleHalManager> service = new vhal_20::VehicleHalManager{hal.get()};

    // Configured to only use 1 thread as the VHAL Manager and Implementation is not threadsafe.
    android::hardware::configureRpcThreadpool(1, true /* callerWillJoin */);

    ALOGI("Registering as service...");
    android::status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Could not register service, %d", status);
    }

    ALOGI("Ready starting VCC VHAL Daemon");

    auto lifecycleControl_service_status = lifecycleControl->registerAsService();
    if (android::OK != lifecycleControl_service_status) {
        ALOGE("Unable to register lifecycleControl HAL service (%d)", lifecycleControl_service_status);
    }
    ALOGI("waiting to join thread ...");
    android::hardware::joinRpcThreadpool();

    return 0;
}
