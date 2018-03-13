/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <sys/socket.h>
#include <map>
#include <memory>
#include <thread>
#include <unordered_set>

#include <utils/SystemClock.h>

#include <vhal_v2_0/RecurrentTimer.h>
#include <vhal_v2_0/VehicleHal.h>
#include "vhal_v2_0/VehiclePropertyStore.h"

#include "DefaultConfig.h"
#include "FakeValueGenerator.h"
#include "VehicleEmulator.h"

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

/** Implementation of VehicleHal that connected to emulator instead of real vehicle network. */
class EmulatedVehicleHal : public EmulatedVehicleHalIface {
  public:
    EmulatedVehicleHal(VehiclePropertyStore* propStore);
    ~EmulatedVehicleHal() = default;

    //  Methods from VehicleHal
    void onCreate() override;
    std::vector<VehiclePropConfig> listProperties() override;
    VehiclePropValuePtr get(const VehiclePropValue& requestedPropValue, StatusCode* outStatus) override;
    StatusCode set(const VehiclePropValue& propValue) override;
    StatusCode subscribe(int32_t property, int32_t areas, float sampleRate) override;
    StatusCode unsubscribe(int32_t property) override;

    //  Methods from EmulatedVehicleHalIface
    bool setPropertyFromVehicle(const VehiclePropValue& propValue) override;
    std::vector<VehiclePropValue> getAllProperties() const override;

  private:
    constexpr std::chrono::nanoseconds hertzToNanoseconds(float hz) const {
        return std::chrono::nanoseconds(static_cast<int64_t>(1000000000L / hz));
    }

    StatusCode handleGenerateFakeDataRequest(const VehiclePropValue& request);
    void onFakeValueGenerated(int32_t propId, float value);

    void onContinuousPropertyTimer(const std::vector<int32_t>& properties);
    bool isContinuousProperty(int32_t propId) const;
    void initStaticConfig();
    void initObd2LiveFrame(const VehiclePropConfig& propConfig);
    void initObd2FreezeFrame(const VehiclePropConfig& propConfig);
    StatusCode fillObd2FreezeFrame(const VehiclePropValue& requestedPropValue, VehiclePropValue* outValue);
    StatusCode fillObd2DtcInfo(VehiclePropValue* outValue);
    StatusCode clearObd2FreezeFrames(const VehiclePropValue& propValue);

    /* Private members */
    VehiclePropertyStore* mPropStore;
    std::unordered_set<int32_t> mHvacPowerProps;
    RecurrentTimer mRecurrentTimer;
    FakeValueGenerator mFakeValueGenerator;
};

}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
