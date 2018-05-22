/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cutils/log.h>
#include <hidl/HidlTransportSupport.h>
#include <memory>
#include "VehicleHalImpl.h"
#include "vhal_v2_0/VehicleHalManager.h"

#include "PowerModule.h"
#include "activesafetymodule.h"
#include "activeuserprofilemodule.h"
#include "cartimemodule.h"
#include "hvacmodule.h"
#include "illuminationmodule.h"
#include "keymanagermodule.h"
#include "libsettings/setting.h"
#include "libsettings/settingsmanagerhidl.h"
#include "sensormodule.h"
#include "vhal_modules/connected_safety_module.h"
#include "vhal_modules/curve_speed_adaption_module.h"
#include "vhal_modules/driver_support_function_module.h"
#include "vhal_modules/e_lane_keeping_aid_module.h"
#include "vhal_modules/lane_departure_warning_module.h"
#include "vhal_modules/lane_keeping_aid_module.h"
#include "vhal_modules/speed_limit_adaptation_module.h"
#include "vhal_modules/traffic_sign_information_module.h"
#include "vmsmodule.h"

#include <IDispatcher.h>
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <dispatcher_watchdog.h>
#include <future>
#include "Application_dataelement_synchronous.h"
#include "carconfigmodule.h"
#include "climate_main.h"
#include "systeminformationmodule.h"
#include "utils/vf_context.h"

#undef LOG_TAG
#define LOG_TAG "automotive.vehicle2.0"

namespace vhal_20 = android::hardware::automotive::vehicle::V2_0;
namespace vccvhal_10 = vendor::volvocars::hardware::vehiclehal::V1_0;

int main(int /* argc */, char* /* argv */ []) {
    // NOTE: This dispatcher is only started AFTER VehicleHalManager service is running
    //      Otherwise callbacks might accidentally call pushProp before VehicleHalManager started which is invalid.
    //      See ARTINFO-2533 for more info.
    //      It's also a way to avoid race conditions due to construction being
    //      on main thread while all callbacks come from the dispatcher thread.
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher =
            tarmac::eventloop::IDispatcher::CreateDispatcher(false);
    std::unique_ptr<tarmac::eventloop::WatchDog> watchdog = tarmac::eventloop::WatchDog::Create();
    watchdog->Watch(dispatcher, std::chrono::seconds(30));
    android::sp<SettingsFramework::SettingsManagerHidl> settings_manager =
            new SettingsFramework::SettingsManagerHidl(*dispatcher);
    DEDispatcher dataelements{dispatcher};

    auto store = std::make_unique<vhal_20::VehiclePropertyStore>();
    auto hal = std::make_unique<vhal_20::impl::VehicleHalImpl>(store.get());

    VFContext ctx{dispatcher, settings_manager, dataelements, *(hal.get()), vcc::LocalConfigDefault()};

    ClimateMain climateMain(dispatcher);

    // Create Modules
    auto powerModule = std::make_unique<vhal_20::impl::PowerModule>(hal.get());
    auto carConfigModule = std::make_unique<vccvhal_10::impl::CarConfigHal>(hal.get());
    auto activeUserProfileModule = std::make_unique<vccvhal_10::impl::ActiveUserProfileHal>(hal.get());
    auto hvacModule =
            std::make_unique<HvacModule>(hal.get(), climateMain.first_row, climateMain.commonFactory_, dispatcher);
    auto keyManagerModule = std::make_unique<KeyManagerModule>(hal.get());
    auto systemInformationModule = std::make_unique<SystemInformationModule>(hal.get());
    auto illuminationModule = std::make_unique<vccvhal_10::impl::IlluminationHal>(hal.get());
    auto carTimeModule = std::make_unique<CarTimeHal>(hal.get());
    auto sensorModule = std::make_unique<SensorModule>(hal.get());
    auto activeSafetyModule = std::make_unique<ActiveSafetyModule>(hal.get(), settings_manager);
    auto e_lane_keeping_aid_module = std::make_unique<ELaneKeepingAidModule>(hal.get(), dispatcher, settings_manager);
    auto traffic_sign_information_module =
            std::make_unique<TrafficSignInformationModule>(hal.get(), dispatcher, settings_manager);

    CurveSpeedAdaptionModule curveSpeedAdaption{&ctx};
    DriverSupportFunctionModule driver_support_function_module{&ctx};
    LaneDepartureWarningModule lane_departure_warning{&ctx};
    ConnectedSafetyModule connectedSafety{&ctx};
    LaneKeepingAidModule laneKeepingAid{&ctx};
    SpeedLimitAdaptationModule speedLimitAdaptation{&ctx};

    auto vmsModule = std::make_unique<vmsHal>(hal.get());
    // Register modules
    powerModule->registerToVehicleHal();
    carConfigModule->registerToVehicleHal();
    keyManagerModule->registerToVehicleHal();
    activeUserProfileModule->registerToVehicleHal();
    systemInformationModule->registerToVehicleHal();
    illuminationModule->registerToVehicleHal();
    carTimeModule->registerToVehicleHal();
    sensorModule->registerToVehicleHal();
    activeSafetyModule->registerToVehicleHal();
    vmsModule->registerToVehicleHal();
    ::android::sp<vhal_20::VehicleHalManager> service = new vhal_20::VehicleHalManager{hal.get()};

    // Configured to only use 1 thread as the VHAL Manager and Implementation is not threadsafe.
    android::hardware::configureRpcThreadpool(1, true /* callerWillJoin */);

    ALOGI("Registering as service...");
    android::status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("Could not register service, %d", status);
    }

    ALOGI("Ready starting VCC VHAL Daemon");

    ALOGI("waiting to join thread ...");

    std::thread dispatcherThread([&]() { dispatcher->RunUntil([]() { return false; }); });
    android::hardware::joinRpcThreadpool();
    dispatcherThread.join();

    return 0;
}
